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

PLUGIN_API int XPluginStart(char *		outName,
							char *		outSig,
							char *		outDesc)
{
	strcpy(outName, "BetaflightXPlanePlugin");
	strcpy(outSig, "SITL.Betaflight.XPlanePlugin");
	strcpy(outDesc, "Betaflight SITL <-> X-Plane plugin");

	return true;
}

PLUGIN_API void	XPluginStop(void)
{
	betaFlightNet->Terminate();
	XPLMDestroyFlightLoop(loopId);
	delete betaFlightNet;
	delete simData;
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
	if (!strcmp((char*)in_item_ref, "Menu Arm"))
		betaFlightNet->setArm(true);
	else if (!strcmp((char*)in_item_ref, "Menu DisArm"))
		betaFlightNet->setArm(false);
	else if (!strcmp((char*)in_item_ref, "Menu Acro"))
		betaFlightNet->setAcroMode();
	else if (!strcmp((char*)in_item_ref, "Menu Manual"))
		betaFlightNet->setMode("manual");
	else if (!strcmp((char*)in_item_ref, "Menu Angle"))
		betaFlightNet->setMode("angle");
	else if (!strcmp((char*)in_item_ref, "Menu Airplane"))
		betaFlightNet->setMode("airplane");
	else if (!strcmp((char*)in_item_ref, "Restart SITL"))
		betaFlightNet->Restart();	
}

PLUGIN_API int XPluginEnable(void)
{
	CXmlSetup setupData;
	betaFlightNet = new CBetaflightData(setupData);
	simData = new TXPlaneData(betaFlightNet, setupData);

	betaFlightNet->Initialize();
	simData->Initialize();

	XPLMCreateFlightLoop_t params;
	params.structSize = sizeof(XPLMCreateFlightLoop_t);
	params.callbackFunc = &floop_cb;
	params.phase = xplm_FlightLoop_Phase_AfterFlightModel;
	params.refcon = NULL;
	loopId = XPLMCreateFlightLoop(&params);
	XPLMScheduleFlightLoop(loopId, -1, true);

	int menu_container_idx = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "Betaflight", 0, 0);
	XPLMMenuID menu_id = XPLMCreateMenu("SITL", XPLMFindPluginsMenu(), menu_container_idx, menu_handler, NULL);
	int arm_id = XPLMAppendMenuItem(menu_id, "Arm", (void *)"Menu Arm", 1);
	int disarm_id = XPLMAppendMenuItem(menu_id, "DisArm", (void *)"Menu DisArm", 1);
	int manual_id = XPLMAppendMenuItem(menu_id, "Manual", (void *)"Menu Manual", 1);
	int acro_id = XPLMAppendMenuItem(menu_id, "Acro", (void *)"Menu Acro", 1);
	int angle_id = XPLMAppendMenuItem(menu_id, "Angle", (void *)"Menu Angle", 1);
	int airplane_id = XPLMAppendMenuItem(menu_id, "Airplane", (void *)"Menu Airplane", 1);
	XPLMAppendMenuItem(menu_id, "Restart SITL", (void *)"Restart SITL", 1);
    
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(
					XPLMPluginID	inFromWho,
					int				inMessage,
					void *			inParam)
{
	
}

