#pragma once
#include <map>
#include "pugiconfig.hpp"
#include "pugixml.hpp"
struct TModeRC
{
	std::string name;
	unsigned short aux;
	unsigned short value;

	TModeRC()
	{
		this->name = "";
		this->aux = 0;
		this->value = 900;
	}

	TModeRC(std::string name, unsigned short aux, unsigned short value)
	{
		this->name = name;
		this->aux = aux;
		this->value = value;
	}

	TModeRC(const TModeRC& mode)
	{
		name = mode.name;
		aux = mode.aux;
		value = mode.value;
	}
};


struct TControlRC
{
	std::string name;
	unsigned short aux;

	TControlRC()
	{
		this->name = "";
		this->aux = 0;
	}

	TControlRC(std::string name, unsigned short aux)
	{
		this->name = name;
		this->aux = aux;
	}

	TControlRC(const TControlRC& control)
	{
		name = control.name;
		aux = control.aux;
	}
};

struct TServoControl
{
	std::string name;
	unsigned short servo;
	bool reverse;

	TServoControl()
	{
		this->name = "";
		this->servo = 0;
		this->reverse = false;
	}

	TServoControl(std::string name, unsigned short servo, bool reverse)
	{
		this->name = name;
		this->servo = servo;
		this->reverse = reverse;
	}

	TServoControl(const TServoControl& surface)
	{
		name = surface.name;
		servo = surface.servo;
		reverse = surface.reverse;
	}
};

struct TPlaneControl
{
	struct
	{
		float Up, Down;
	}AileronLimit, ElevatorLimit;
	float RudderLimit, AileronFromPitch;
};

struct TSetupRC
{
	std::map <std::string, TModeRC> Modes; 
	std::map <std::string, TControlRC> Controls; 
};

typedef std::map <std::string, TServoControl> TServoControls;
class CXmlSetup
{
public:
	CXmlSetup(void);
	~CXmlSetup(void);
	TSetupRC SetupRC;
	TServoControls ServoControls; 	
	TPlaneControl PlaneControl;
};

