#pragma once
#include <stdint.h>
#include "XPLMDataAccess.h"
#include "betaflight.h"
#include "BetaflightData.h"
#include "XmlSetup.h"

struct TXPlaneDataRefs
{
	struct TPlaneState
	{
		//Angles
		struct TAngles
		{
			//degrees, the roll of the aircraft in degrees - OpenGL coordinates
			XPLMDataRef roll;
			//degrees, the pitch relative to the plane normal to the Y axis in degrees - OpenGL coordinates
			XPLMDataRef pitch;
			//degrees, the true heading of the aircraft in degrees from the Z axis - OpenGL coordinates
			XPLMDataRef yaw;
		} Angles;

		// accel
		struct TAccel
		{
			//Gs, total g-forces on the plane as a multiple, along the plane, forward - negative
			XPLMDataRef x;
			//Gs, total g-forces on the plane as a multiple, sideways, right - negative
			XPLMDataRef y;
			//Gs, Total g-forces on the plane as a multiple, downward
			XPLMDataRef z;
		} Accel;

		// gyro
		struct TGyroRate
		{
			//degrees, the roll of the aircraft in degrees - OpenGL coordinates
			XPLMDataRef x;
			//deg/sec, the pitch rotation rates (relative to the flight)
			XPLMDataRef y;
			//deg/sec	The yaw rotation rates (relative to the flight)
			XPLMDataRef z;
		} GyroRate;
			// baro
		//This is the barometric pressure at the point the current flight is at.
		struct TBaro
		{
			XPLMDataRef pressure;
		} Baro;
	}PlaneState;

	struct TPlaneControl
	{
		struct TThrottle
		{
			XPLMDataRef override_throttle;
			XPLMDataRef input_throttle;
			XPLMDataRef output_throttle;
		} Throttle;

		struct TStick
		{
			XPLMDataRef override_joystick_control;
			XPLMDataRef roll;
			XPLMDataRef pitch;
			XPLMDataRef yaw;
		} Stick;

		struct TActualControl
		{
			XPLMDataRef override_flight_control;
			XPLMDataRef roll;
			XPLMDataRef pitch;
			XPLMDataRef yaw;
		}ActualControl;

		struct TPlaneControlLimit
		{
			struct
			{
				XPLMDataRef Up, Down;
			} Aileron, Elevator;
			XPLMDataRef AileronFromPitch, Rudder;
		}PlaneControlLimit;

	}PlaneControl;
};

struct TEnabledPlaneControl
{
	bool Ailerons;
	bool AileronsFromPitch;
	bool Elevator;
	bool Rudder;
};

class TXPlaneData
{
	TXPlaneDataRefs XPlaneDataRefs;
	CBetaflightData *m_pBetaflight;
	TServoControls ServoControls;
	TEnabledPlaneControl EnabledPlaneControl;
	float getOutput(const TServoControl& surface);
public:
	TXPlaneData(CBetaflightData *pBetaflight, const CXmlSetup& xplSetup)
	{
		m_pBetaflight = pBetaflight;
		ServoControls = xplSetup.ServoControls;
	};

	void Initialize();
	void updateBetaflightStateFromXPlane();
	void updateBetaflightControlFromXPlane();
	void sendServoControlToXPlane();

	void computeQuaternionFromRPY(double* quat, double roll, double pitch, double yaw);
};



