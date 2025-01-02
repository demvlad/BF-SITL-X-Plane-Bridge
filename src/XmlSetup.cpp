#include "XmlSetup.h"
#include <XPLMPlanes.h>
#include <XPLMUtilities.h>

CXmlSetup::CXmlSetup(void)
{
	char FileNamePath[512], FileName[512], AircraftPath[1024];
	XPLMGetNthAircraftModel(0, FileName, AircraftPath);
	XPLMExtractFileAndPath(AircraftPath);
	strcpy(FileNamePath, AircraftPath);
	strcat(FileNamePath, XPLMGetDirectorySeparator());
	strcat(FileNamePath, "plugins");
	strcat(FileNamePath, XPLMGetDirectorySeparator());
	strcat(FileNamePath, "Betaflight-HITL");
	strcat(FileNamePath, XPLMGetDirectorySeparator());
	strcat(FileNamePath, "setup.xml");
	pugi::xml_document spXMLDOM;
	pugi::xml_parse_result result = spXMLDOM.load_file(FileNamePath);

	pugi::xml_node pRoot;
	pRoot = spXMLDOM.child("setup");

	pugi::xml_node pModes = pRoot.child("Modes"); 
	for (pugi::xml_node mode = pModes.first_child(); mode; mode = mode.next_sibling())
	{
		std::string name = mode.attribute("name").as_string();
		unsigned short aux = mode.attribute("aux").as_uint();
		unsigned short value = mode.attribute("value").as_uint(1000);
		SetupRC.Modes[name] = TModeRC(name, aux, value);
	}


	pugi::xml_node pControls = pRoot.child("Input"); 
	for (pugi::xml_node control = pControls.first_child(); control; control = control.next_sibling())
	{
		std::string name = control.attribute("name").as_string();
		unsigned short aux = control.attribute("aux").as_uint();
		SetupRC.Controls[name] = TControlRC(name, aux);
	}

	pugi::xml_node pServoControls = pRoot.child("Output"); 
	for (pugi::xml_node servoCtrl = pServoControls.first_child(); servoCtrl; servoCtrl = servoCtrl.next_sibling())
	{
		std::string name = servoCtrl.attribute("name").as_string();
		unsigned short servo = servoCtrl.attribute("servo").as_uint();
		bool reverse = servoCtrl.attribute("reverse").as_bool(false);
		ServoControls[name] = TServoControl(name, servo, reverse);
	}
}


CXmlSetup::~CXmlSetup(void)
{
}
