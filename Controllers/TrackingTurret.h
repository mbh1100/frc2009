#ifndef __TRACKINGTURRET_H__
#define __TRACKINGTURRET_H__

#include <iostream.h>
#include <taskLib.h>
#include <cmath>
#include "WPILib.h"
#include "PIDControl.h"
#include "PIDController.h"
#include "TrackingCamera.h"
#include "../Devices/PIDJaguar.h"

class TrackingTurret
{
public:
	TrackingTurret(PIDJaguar* motor1, Servo* servo2, Joystick* test1, Joystick* test2);
	~TrackingTurret();
	
	void Update();
	void ScanTarget(float currentX);
	void ResetScan();
	void StopTurret();

protected:
	TrackingCamera *m_trackingCamera;
	
	/* Declare Objects */
	PIDControl *m_calcSpeedX;
	PIDJaguar *m_turretMotor;
	
	/* Declare constants */
	float m_pX, m_iX, m_dX;
	float m_setpointX, m_maxInputX, m_minInputX, m_maxOutputX, m_minOutputX;	
	float m_errorPercentX, m_errorIncrementX;
	
	float m_distanceX, m_distanceY;
	bool m_inView;
	
	Servo *m_cameraServo;
	Joystick *m_joystick1, *m_joystick2;
};

#endif

