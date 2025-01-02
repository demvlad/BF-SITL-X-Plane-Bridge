#pragma once
#include <memory.h>
#include <time.h>
#include <winsock.h>
#include "betaflight.h"
#include "XmlSetup.h"
#define UNDEFINED_PWM_PULSE	900

class CBetaflightData
{
	SOCKET recv_fc_motor_output_sock, recv_fc_pwm_output_sock,
	       sendto_fc_state_sock, sendto_fc_rc_sock; 
	struct sockaddr_in dest;
	TSetupRC SetupRC;
public:
	fdm_packet bfOutFlightState;
	rc_packet bfOutCommandRC;

	servo_packet bfInMotors;
	servo_packet_raw bfInServos;

	CBetaflightData(const CXmlSetup& xmlSetup);
	bool Initialize();
	void Terminate();
	void Restart();
	bool SendData();
	bool RecvData();

	void setArm(bool on);
	void setAngleMode();
	void setAcroMode();
	void setManualMode();
	void setThrottle(float throttle);
	void setStickPitch(float stickPitch);
	void setStickRoll(float stickRoll);
	void setStickYaw(float stickYaw);
	~CBetaflightData(void);
};

