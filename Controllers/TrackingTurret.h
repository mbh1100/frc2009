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
	TrackingTurret(PIDOutput* turretMotor, Servo* cameraServo);
	virtual ~TrackingTurret();
	
	/* New method for manual turret control */
	void Manual(float turnMotor, float changeDistance);
	void Automatic();
	bool Update(bool manual, float turnMotor, float changeDistance);
	
	void ScanTarget(float currentX);
	void ResetScan();
	void StopTurret();

protected:
	TrackingCamera *m_trackingCamera;
	
	/* Manual turning and speed variables*/
	float m_turnMotor, m_changeDistance;
	float m_turretDirection;
	
	/* Declare Objects */
	PIDControl *m_calcSpeedX;
	PIDOutput *m_turretMotor;
	
	/* Declare constants */
	float m_pX, m_iX, m_dX;
	float m_setpointX, m_maxInputX, m_minInputX, m_maxOutputX, m_minOutputX;	
	float m_errorPercentX, m_errorIncrementX;
	
	bool m_inView;
	bool m_targetFound;	
	
	Servo *m_cameraServo;
};

#endif

