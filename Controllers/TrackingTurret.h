#ifndef __TRACKINGTURRET_H__
#define __TRACKINGTURRET_H__

#include <iostream.h>
#include <taskLib.h>
#include <cmath>
#include "WPILib.h"
#include "PIDControl.h"
#include "PIDController.h"
#include "TrackingCamera.h"
#include "../Devices/PIDServo.h"

#define kTurretMaxInc .05
#define kTurretManualInc .02
#define kTurretAllowedError 30.0

#define sign(x) (x < 0 ? -1 : 1)

class TrackingTurret
{
public:
	TrackingTurret(PIDServo* primary, PIDServo* secondary);
	virtual ~TrackingTurret();
	
	bool Update();
	bool Update(int direction);

protected:
	void UpdateMotors();
	
	TrackingCamera *m_trackingCamera;
	
	/* Manual turning and speed variables*/
	float m_motorPos;
	int m_scanDirection;
	
	/* Declare Objects */
	PIDServo *m_primaryServo, *m_secondaryServo;
	
	bool m_inView, m_targetFound;
};

#endif

