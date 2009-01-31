#ifndef __TANKDRIVE_H__
#define __TANKDRIVE_H__

#include <iostream.h>
#include <taskLib.h>
#include <cmath>
#include "WPILib.h"
#include "PIDControl.h"

#include "../Devices/PIDEncoder.h"
#include "../Devices/PIDJaguar.h"

class TankDrive
{
public:
	TankDrive(PIDJaguar* leftMotor, PIDJaguar* rightMotor, PIDEncoder* leftEncoder, PIDEncoder* rightEncoder);
	virtual ~TankDrive();
	
	void SetSetpoint(float leftSetpoint, float rightSetpoint);
	void Update();
	void Disable();

	void StopMotor();

protected:
	PIDControl *m_calcSpeedLeft, *m_calcSpeedRight;
	PIDJaguar *m_leftMotor, *m_rightMotor;
	PIDEncoder *m_leftEncoder, *m_rightEncoder;
	
	float m_p, m_i, m_d;
	float m_setpointLeft, m_setpointRight;
	float m_maxInput, m_minInput, m_maxOutput, m_minOutput;	
	float m_errorPercent, m_errorIncrement;
	bool m_increment;
};

#endif
