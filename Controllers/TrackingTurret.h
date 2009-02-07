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
#include "Shoot.h"

class TrackingTurret
{
public:
	TrackingTurret(PIDJaguar* turretMotor, Jaguar* shooterMotor, Servo* cameraServo, Joystick* test1, Joystick* test2);
	virtual ~TrackingTurret();
	
	/* New method for manual turret control */
	void Manual(float turnMotor, float changeDistance);
	void Automatic();
	bool Update(bool manual, bool shoot, float turnMotor, float changeDistance);
	
	void ScanTarget(float currentX);
	void ResetScan();
	void StopTurret();

protected:
	TrackingCamera *m_trackingCamera;
	
	/* Manual turning and speed variables*/
	float m_turnMotor, m_changeDistance;
	float m_turretDirection;
	
	/* Distance of Target Variables */
	Shoot *m_shooter;
	float m_targetDistance;
	
	/* Declare Objects */
	PIDControl *m_calcSpeedX;
	PIDJaguar *m_turretMotor;
	Jaguar *m_shooterMotor;
	
	/* Declare constants */
	float m_pX, m_iX, m_dX;
	float m_setpointX, m_maxInputX, m_minInputX, m_maxOutputX, m_minOutputX;	
	float m_errorPercentX, m_errorIncrementX;
	
	float m_distanceX, m_distanceY;
	bool m_inView;
	bool m_targetFound, m_shoot;	
	
	Servo *m_cameraServo;
	
	Joystick *m_joystick1, *m_joystick2;
};

#endif

