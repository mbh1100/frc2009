#include "HopperControl.h"

HopperControl::HopperControl(Victor* leftHelixMotor, Victor* rightHelixMotor, Victor* sweeperMotor)
{
	m_leftHelixMotor = leftHelixMotor;
	m_rightHelixMotor = rightHelixMotor;
	m_sweeperMotor = sweeperMotor;
	
	kHelixSpeed = .98;
	kSweeperSpeed = .98;
}

HopperControl::~HopperControl()
{
	
}

void HopperControl::Update(int helixSide, int helixDirection)
{
	m_sweeperMotor->Set(helixDirection >= 0 ? kSweeperSpeed : -kSweeperSpeed);
	
	if (helixDirection == 0)
	{
		m_rightHelixMotor->Set(0.0);
		m_leftHelixMotor->Set(0.0);
		m_sweeperMotor->Set(0.0);
	}
	else
	{
		if (helixSide >= 0)
		{
			m_rightHelixMotor->Set(helixDirection > 0 ? kHelixSpeed : -kHelixSpeed);
		}
		if (helixSide <= 0)
		{
			m_leftHelixMotor->Set(helixDirection > 0 ? kHelixSpeed : -kHelixSpeed);
		}
	}
}

void HopperControl::Disable()
{
	m_rightHelixMotor->Set(0.0);
	m_leftHelixMotor->Set(0.0);
	m_sweeperMotor->Set(0.0);
}
