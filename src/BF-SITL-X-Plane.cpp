#include <stdio.h>
#include <string.h>
#include <XPLMUtilities.h>
#include <XPLMMenus.h>
#include <XPLMProcessing.h>
#include "BetaflightData.h"
#include "XPlaneData.h"
#include "XmlSetup.h"

CBetaflightData *betaFlightNet;
TXPlaneData *simData;
XPLMFlightLoopID loopId;
CXmlSetup *pSetupData;

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
    betaFlightNet->Terminate();
    XPLMDestroyFlightLoop(loopId);

    if (betaFlightNet != NULL) {
        delete betaFlightNet;
        betaFlightNet = NULL;
    }
    if (simData != NULL) {
        delete simData;
        simData = NULL;
    }
    if (pSetupData != NULL) {
        delete pSetupData;
        pSetupData = NULL;
    }
}

PLUGIN_API void XPluginDisable(void)
{
}

float floop_cb(float elapsed1, float elapsed2, int ctr, void* refcon)
{
    betaFlightNet->RecvData();
    simData->sendServoControlToXPlane();

    simData->updateBetaflightControlFromXPlane();
    simData->updateBetaflightStateFromXPlane();
    betaFlightNet->SendData();

    return -1;
}

void menu_handler(void * in_menu_ref, void * in_item_ref)
{
    if (!strcmp((char*)in_item_ref, "Restart SITL")) {
        betaFlightNet->Restart();
    } else {
        betaFlightNet->setMode((char*)in_item_ref);
    }
}

void InitFlightLoop()
{
    XPLMCreateFlightLoop_t params;
    params.structSize = sizeof(XPLMCreateFlightLoop_t);
    params.callbackFunc = &floop_cb;
    params.phase = xplm_FlightLoop_Phase_AfterFlightModel;
    params.refcon = NULL;
    loopId = XPLMCreateFlightLoop(&params);
    XPLMScheduleFlightLoop(loopId, -1, true);
}

void CreateBetaflightMenu()
{
    int menu_container_idx = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "Betaflight", 0, 0);
    XPLMMenuID menu_id = XPLMCreateMenu("SITL", XPLMFindPluginsMenu(), menu_container_idx, menu_handler, NULL);
    for (std::size_t i = 0; i < pSetupData->SetupRC.ModeNames.size(); ++i) {
        const char *name = pSetupData->SetupRC.ModeNames[i].c_str();
        XPLMAppendMenuItem(menu_id, name, (void*)name, 1);
    }

    XPLMAppendMenuSeparator(menu_id);
    XPLMAppendMenuItem(menu_id, "Restart SITL", (void *)"Restart SITL", 1);
}

PLUGIN_API int XPluginEnable(void)
{
    pSetupData = new CXmlSetup;
    betaFlightNet = new CBetaflightData(pSetupData);
    simData = new TXPlaneData(betaFlightNet, pSetupData);

    betaFlightNet->Initialize();
    simData->Initialize();

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
