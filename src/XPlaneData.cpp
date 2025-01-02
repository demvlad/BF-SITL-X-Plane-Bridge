#include "XPlaneData.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stddef.h>

#define DEG2RAD         (M_PI / 180.0)
#define InHg2Pa			3386.39

void TXPlaneData::Initialize()
{
	XPlaneDataRefs.PlaneState.Angles.roll = XPLMFindDataRef("sim/flightmodel/position/phi");
	XPlaneDataRefs.PlaneState.Angles.pitch = XPLMFindDataRef("sim/flightmodel/position/theta");
	XPlaneDataRefs.PlaneState.Angles.yaw = XPLMFindDataRef("sim/flightmodel/position/psi");
  

	// Accelerometer
	XPlaneDataRefs.PlaneState.Accel.x = XPLMFindDataRef("sim/flightmodel/forces/g_axil");
	XPlaneDataRefs.PlaneState.Accel.y = XPLMFindDataRef("sim/flightmodel/forces/g_side");
	XPlaneDataRefs.PlaneState.Accel.z = XPLMFindDataRef("sim/flightmodel/forces/g_nrml");

	// Gyro
	XPlaneDataRefs.PlaneState.GyroRate.x = XPLMFindDataRef("sim/flightmodel/position/P");
	XPlaneDataRefs.PlaneState.GyroRate.y = XPLMFindDataRef("sim/flightmodel/position/Q");
	XPlaneDataRefs.PlaneState.GyroRate.z = XPLMFindDataRef("sim/flightmodel/position/R");

	// Barometer
	XPlaneDataRefs.PlaneState.Baro.pressure = XPLMFindDataRef("sim/weather/barometer_current_inhg");

	XPlaneDataRefs.PlaneControl.Throttle.override_throttle = XPLMFindDataRef("sim/operation/override/override_throttles");
	XPlaneDataRefs.PlaneControl.Throttle.input_throttle = XPLMFindDataRef("sim/flightmodel/engine/ENGN_thro");
	XPlaneDataRefs.PlaneControl.Throttle.output_throttle = XPLMFindDataRef("sim/flightmodel/engine/ENGN_thro_use");

	XPlaneDataRefs.PlaneControl.Stick.override_joystick_control = XPLMFindDataRef("sim/operation/override/override_joystick");
	XPlaneDataRefs.PlaneControl.Stick.roll = XPLMFindDataRef("sim/joystick/yoke_roll_ratio");
	XPlaneDataRefs.PlaneControl.Stick.pitch = XPLMFindDataRef("sim/joystick/yoke_pitch_ratio");
	XPlaneDataRefs.PlaneControl.Stick.yaw = XPLMFindDataRef("sim/joystick/yolk_heading_ratio");

	XPlaneDataRefs.PlaneControl.ActualControl.override_flight_control = XPLMFindDataRef("sim/operation/override/override_flightcontrol");
	XPlaneDataRefs.PlaneControl.ActualControl.roll = XPLMFindDataRef("sim/joystick/FC_roll");
	XPlaneDataRefs.PlaneControl.ActualControl.pitch = XPLMFindDataRef("sim/joystick/FC_ptch");
	XPlaneDataRefs.PlaneControl.ActualControl.yaw = XPLMFindDataRef("sim/joystick/FC_hdng");

	XPlaneDataRefs.PlaneControl.PlaneControlLimit.Aileron.Up = XPLMFindDataRef("sim/aircraft/controls/acf_ail1_up");
	XPlaneDataRefs.PlaneControl.PlaneControlLimit.Aileron.Down = XPLMFindDataRef("sim/aircraft/controls/acf_ail1_dn");

	XPlaneDataRefs.PlaneControl.PlaneControlLimit.AileronFromPitch = XPLMFindDataRef("sim/aircraft/specialcontrols/acf_ail1pitch");

	XPlaneDataRefs.PlaneControl.PlaneControlLimit.Elevator.Up = XPLMFindDataRef("sim/aircraft/controls/acf_elev_up");
	XPlaneDataRefs.PlaneControl.PlaneControlLimit.Elevator.Down = XPLMFindDataRef("sim/aircraft/controls/acf_elev_dn");

	XPlaneDataRefs.PlaneControl.PlaneControlLimit.Rudder = XPLMFindDataRef("sim/aircraft/controls/acf_rudd_lr");

	XPLMSetDatai(XPlaneDataRefs.PlaneControl.Stick.override_joystick_control, 0);
	XPLMSetDatai(XPlaneDataRefs.PlaneControl.ActualControl.override_flight_control, 1);
	XPLMSetDatai(XPlaneDataRefs.PlaneControl.Throttle.override_throttle, 1);

	double ail  = XPLMGetDataf(XPlaneDataRefs.PlaneControl.PlaneControlLimit.Aileron.Up);
	ail  = XPLMGetDataf(XPlaneDataRefs.PlaneControl.PlaneControlLimit.AileronFromPitch);

}



void TXPlaneData::updateBetaflightStateFromXPlane()
{
	double t = GetTickCount() * 0.001;
	m_pBetaflight->bfOutFlightState.timestamp = t;
	m_pBetaflight->bfOutCommandRC.timestamp = t;


	m_pBetaflight->bfOutFlightState.imu_linear_acceleration_xyz[0] = -XPLMGetDataf(XPlaneDataRefs.PlaneState.Accel.x);
	m_pBetaflight->bfOutFlightState.imu_linear_acceleration_xyz[1] =  XPLMGetDataf(XPlaneDataRefs.PlaneState.Accel.y);
	m_pBetaflight->bfOutFlightState.imu_linear_acceleration_xyz[2] =  XPLMGetDataf(XPlaneDataRefs.PlaneState.Accel.z);

	m_pBetaflight->bfOutFlightState.imu_angular_velocity_rpy[0] =  XPLMGetDataf(XPlaneDataRefs.PlaneState.GyroRate.x) * DEG2RAD;
	m_pBetaflight->bfOutFlightState.imu_angular_velocity_rpy[1] = -XPLMGetDataf(XPlaneDataRefs.PlaneState.GyroRate.y) * DEG2RAD;
	m_pBetaflight->bfOutFlightState.imu_angular_velocity_rpy[2] = -XPLMGetDataf(XPlaneDataRefs.PlaneState.GyroRate.z) * DEG2RAD;

	double roll  = XPLMGetDataf(XPlaneDataRefs.PlaneState.Angles.roll);
	double pitch = -XPLMGetDataf(XPlaneDataRefs.PlaneState.Angles.pitch);
	double yaw   = XPLMGetDataf(XPlaneDataRefs.PlaneState.Angles.yaw);
	computeQuaternionFromRPY(m_pBetaflight->bfOutFlightState.imu_orientation_quat, roll, pitch, yaw);

	m_pBetaflight->bfOutFlightState.pressure = XPLMGetDataf(XPlaneDataRefs.PlaneState.Baro.pressure) * InHg2Pa;
}

void TXPlaneData::updateBetaflightControlFromXPlane()
{
	float stickRoll = XPLMGetDataf(XPlaneDataRefs.PlaneControl.Stick.roll);
	float stickPitch = XPLMGetDataf(XPlaneDataRefs.PlaneControl.Stick.pitch);

	m_pBetaflight->setStickPitch(stickPitch);
	m_pBetaflight->setStickRoll(stickRoll);

	float throttle;
	XPLMGetDatavf(XPlaneDataRefs.PlaneControl.Throttle.input_throttle, &throttle, 0, 1);
	m_pBetaflight->setThrottle(throttle);

}

float TXPlaneData::getOutput(const TServoControl& ctrl)
{
	float servoOutput = m_pBetaflight->bfInServos.pwm_output_raw[ctrl.servo];
	if (servoOutput < PWM_MIN)
		servoOutput = PWM_MIN;
	else if (servoOutput > PWM_MAX)
		servoOutput = PWM_MAX;
	float output = 0.0;
	if (ctrl.name == "throttle")
		output = (servoOutput - PWM_MIN) / (PWM_MAX - PWM_MIN);
	else
	{
		output = -1.0f + 2.0f * (servoOutput - PWM_MIN) / (PWM_MAX - PWM_MIN);
		if (ctrl.reverse)
			output *= -1.0f;
	}
	return output;
}

void TXPlaneData::sendServoControlToXPlane()
{	
	float LeftServo = getOutput(ServoControls["aileron_left"]);
	float RightServo = getOutput(ServoControls["aileron_right"]);
	float pitchControl = 0.5f * (LeftServo + RightServo);
	float rollControl  = 0.5f * (LeftServo - RightServo);

	XPLMSetDataf(XPlaneDataRefs.PlaneControl.ActualControl.pitch, pitchControl);
	XPLMSetDataf(XPlaneDataRefs.PlaneControl.ActualControl.roll, rollControl);
	
	float throttle = getOutput(ServoControls["throttle"]);
	XPLMSetDatavf(XPlaneDataRefs.PlaneControl.Throttle.output_throttle, &throttle, 0, 1);
}

void TXPlaneData::computeQuaternionFromRPY( double* quat, double roll, double pitch, double yaw)
{
  const double cosRoll = cos(DEG2RAD * roll * 0.5);
  const double sinRoll = sin(DEG2RAD * roll * 0.5);

  const double cosPitch = cos(DEG2RAD * pitch * 0.5);
  const double sinPitch = sin(DEG2RAD * pitch * 0.5);

  const double cosYaw = cos(DEG2RAD * (-yaw) * 0.5);
  const double sinYaw = sin(DEG2RAD * (-yaw) * 0.5);

  quat[0] = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
  quat[1] = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
  quat[2] = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
  quat[3] = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
}

