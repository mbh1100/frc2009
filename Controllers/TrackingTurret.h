#ifndef __TRACKINGTURRET_H__
#define __TRACKINGTURRET_H__

#include <iostream.h>
#include <taskLib.h>
#include <cmath>
#include "WPILib.h"
#include "PIDControl.h"
#include "PIDController.h"
#include "TrackingCamera.h"
#include "../Devices/AdvServo.h"

#define kTurretMaxInc .05
#define kTurretManualInc .02
#define kTurretAllowedError 150.0

#define sign(x) (x < 0 ? -1 : 1)

class TrackingTurret
{
public:
	TrackingTurret(AdvServo* primary, AdvServo* secondary);
	virtual ~TrackingTurret();
	
	bool Update();
	bool Update(float direction);

protected:
	void UpdateMotors();
	
	TrackingCamera *m_trackingCamera;
	
	/* Manual turning and speed variables*/
	float m_motorPos;
	float m_direction;
	int m_scanDirection;
	
	/* Declare Objects */
	AdvServo *m_primaryServo, *m_secondaryServo;
	
	bool m_inView, m_targetFound;
};

#endif

