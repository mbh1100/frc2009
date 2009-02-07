#include "HopperControl.h"

HopperControl::HopperControl(Victor* leftHelixMotor, Victor* rightHelixMotor, Victor* sweeperMotor)
{
	m_leftHelixMotor = leftHelixMotor;
	m_rightHelixMotor = rightHelixMotor;
	m_sweeperMotor = sweeperMotor;
}

void HopperControl::Update(bool sweeperState, float helixState, float helixDirection)
{
	SetMotors(helixDirection);
	
	if (helixState == 1)
	{
		m_rightHelixMotor->Set(0.0);
	}
	else if (helixState == -1)
	{
		m_leftHelixMotor->Set(0.0);
	}
	if (!sweeperState)
	{
		m_sweeperMotor->Set(0.0);
	}
}

void HopperControl::SetMotors(float motorDirection)
{
	m_leftHelixMotor->Set(motorDirection);
	m_rightHelixMotor->Set(motorDirection);
	m_sweeperMotor->Set(motorDirection);
}
