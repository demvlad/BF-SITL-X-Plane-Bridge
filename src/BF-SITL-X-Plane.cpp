#include <stdio.h>
#include <string.h>
#include <XPLMUtilities.h>
#include <XPLMMenus.h>
#include <XPLMProcessing.h>
#include "BetaflightLink.h"
#include "XPlaneAdapter.h"
#include "XmlSetup.h"

static CBetaflightLink *betaFlightLink;
static CXPlaneAdapter *xplaneAdapter;
static XPLMFlightLoopID loopId;
static CXmlSetup *setupData;

PLUGIN_API int XPluginStart(char *        outName,
                            char *        outSig,
                            char *        outDesc)
{
    strcpy(outName, "BetaflightXPlanePlugin");
    strcpy(outSig, "SITL.Betaflight.XPlanePlugin");
    strcpy(outDesc, "Betaflight SITL <-> X-Plane plugin");
    return true;
}

PLUGIN_API void XPluginStop(void)
{
    betaFlightLink->Terminate();
    XPLMDestroyFlightLoop(loopId);

    if (betaFlightLink != NULL) {
        delete betaFlightLink;
        betaFlightLink = NULL;
    }
    if (xplaneAdapter != NULL) {
        delete xplaneAdapter;
        xplaneAdapter = NULL;
    }
    if (setupData != NULL) {
        delete setupData;
        setupData = NULL;
    }
}

PLUGIN_API void XPluginDisable(void)
{
}

float floop_cb(float elapsed1, float elapsed2, int ctr, void* refcon)
{
    if (betaFlightLink == NULL || xplaneAdapter == NULL) {
        return 0.0f;
    }

    betaFlightLink->receiveMotorCommands();
    xplaneAdapter->Update(elapsed1);
    betaFlightLink->sendFlightState();

    return -1.0f;
}

void InitFlightLoop(void)
{
    XPLMCreateFlightLoop_t params;
    params.structSize = sizeof(XPLMCreateFlightLoop_t);
    params.callbackFunc = &floop_cb;
    params.phase = xplm_FlightLoop_Phase_AfterFlightModel;
    params.refcon = NULL;
    loopId = XPLMCreateFlightLoop(&params);
    XPLMScheduleFlightLoop(loopId, -1, true);
}

void menu_handler(void *in_menu_ref, void *in_item_ref)
{
    if (!strcmp((char*)in_item_ref, "Restart SITL")) {
        betaFlightLink->Restart();
    } else if (!strcmp((char*)in_item_ref, "Launch plane")) {
        xplaneAdapter->planeLaunch();
    } else {
        betaFlightLink->setMode((char*)in_item_ref);
    }
}

void CreateBetaflightMenu()
{
    int menu_container_idx = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "Betaflight", 0, 0);
    XPLMMenuID menu_id = XPLMCreateMenu("SITL", XPLMFindPluginsMenu(), menu_container_idx, menu_handler, NULL);
    XPLMAppendMenuItem(menu_id, "Launch plane", (void *)"Launch plane", 1);
    XPLMAppendMenuSeparator(menu_id);
    for (std::size_t i = 0; i < setupData->SetupRC.ModeNames.size(); ++i) {
        const char *name = setupData->SetupRC.ModeNames[i].c_str();
        XPLMAppendMenuItem(menu_id, name, (void*)name, 1);
    }

    XPLMAppendMenuSeparator(menu_id);
    XPLMAppendMenuItem(menu_id, "Restart SITL", (void *)"Restart SITL", 1);
}

PLUGIN_API int XPluginEnable(void)
{
    setupData = new CXmlSetup;
    betaFlightLink = new CBetaflightLink(setupData);
    xplaneAdapter = new CXPlaneAdapter(betaFlightLink, setupData);

    betaFlightLink->Initialize();
    xplaneAdapter->Initialize();

    InitFlightLoop();
    CreateBetaflightMenu();

    return 1;
}

PLUGIN_API void XPluginReceiveMessage(
                    XPLMPluginID    inFromWho,
                    int                inMessage,
                    void *            inParam)
{

}
