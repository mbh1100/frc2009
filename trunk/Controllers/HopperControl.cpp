#include "HopperControl.h"

HopperControl::HopperControl(Victor* leftHelixMotor, Victor* rightHelixMotor, Victor* sweeperMotor)
{
	m_leftHelixMotor = leftHelixMotor;
	m_rightHelixMotor = rightHelixMotor;
	m_sweeperMotor = sweeperMotor;
		
	m_helixSide = 0;
	m_helixDirection = 0;
	
	m_limitLeftEntry = false;
	m_limitLeftBottom = false;
	m_limitLeftTop = false;
	
	m_limitRightEntry = false;
	m_limitRightBottom = false;
	m_limitRightTop = false;
	
	m_ballsInLeft = 0;
	m_ballsInRight = 0;
	
	m_leftEntering = false;
	m_rightEntering = false;
	
	m_currentCount = 0;
	m_leftStartCount = 0;
	m_rightStartCount = 0;
}

HopperControl::~HopperControl()
{
	
}

void HopperControl::Update(int helixSide, int helixDirection, bool limitLeftEntry, bool limitLeftBottom, bool limitLeftTop, bool limitRightEntry, bool limitRightBottom, bool limitRightTop, UINT32 counter)
{
	/* Set variables based on input */
	m_helixDirection = helixDirection;
	m_helixSide = helixSide;
	
	m_limitLeftEntry = limitLeftEntry;
	m_limitLeftBottom = limitLeftBottom;
	m_limitLeftTop = limitLeftTop;
	
	m_limitRightEntry = limitRightEntry;
	m_limitRightBottom = limitRightBottom;
	m_limitRightTop = limitRightTop;
	
	m_currentCount = counter;
	
	if (m_helixDirection == 0)
	{
		m_rightHelixMotor->Set(0.0);
		m_leftHelixMotor->Set(0.0);
		m_sweeperMotor->Set(0.0);
		m_leftEntering = false;
		m_rightEntering = false;
	}
	else if (m_helixDirection == -1)
	{
		/* Reverse all motors and spit the balls out the bottom */
		m_rightHelixMotor->Set(kHelixOutSpeed);
		m_leftHelixMotor->Set(kHelixOutSpeed);
		m_sweeperMotor->Set(-kSweeperSpeed);
		m_leftEntering = false;
		m_rightEntering = false;
		m_ballsInLeft = 0;
		m_ballsInRight = 0;
	}
	else
	{
		/* Count number of balls in each side as they enter */
		if (m_limitLeftEntry && !m_leftEntering)
		{
			m_leftEntering = true;
			m_leftStartCount = 0;
			m_ballsInRight++;
		}
		if (m_limitRightEntry && !m_rightEntering)
		{
			m_rightEntering = true;
			m_rightStartCount = 0;
			m_ballsInRight++;
		}	
		
		/* If manual control for one side, set those motors */
		if (m_helixSide > 0) /* Turns on right side only */
		{
			m_rightHelixMotor->Set(kHelixOutSpeed);
			m_leftHelixMotor->Set(0.0);
			
			if (m_rightEntering)
			{
				if ((m_currentCount - m_rightStartCount >= 50 && m_limitRightBottom) || m_currentCount - m_rightStartCount <= 200)
				{
					m_rightEntering = false;
				}
			}
		}
		else if (m_helixSide < 0) /* Turns on left side only */
		{
			m_leftHelixMotor->Set(kHelixOutSpeed);
			m_rightHelixMotor->Set(0.0);
			if ((m_currentCount - m_leftStartCount >= 50 && m_limitLeftBottom) || m_currentCount - m_leftStartCount <= 200)
			{
				m_leftEntering = false;
			}
		}
		/* If sensor controlled, call SensorIntake */
		else
		{
			SensorIntake();
		}
	}
}

void HopperControl::SensorIntake()
{
	/* Control left side */
	if (m_limitLeftTop || !m_leftEntering)
	{
		m_leftHelixMotor->Set(0.0);
	}
	else
	{
		if ((m_currentCount - m_leftStartCount >= 50 && m_limitLeftBottom) || m_currentCount - m_leftStartCount <= 200)
		{
			m_leftHelixMotor->Set(0.0);
			m_leftEntering = false;
		}
		else
		{
			m_leftHelixMotor->Set(kHelixInSpeed);
		}
	}
	
	/* Control right side */
	if (m_limitRightTop || !m_rightEntering)
	{
		m_rightHelixMotor->Set(0.0);
	}
	else
	{
		if ((m_currentCount - m_rightStartCount >= 50 && m_limitRightBottom) || m_currentCount - m_rightStartCount <= 200)
		{
			m_rightHelixMotor->Set(0.0);
			m_rightEntering = false;
		}
		else
		{
			m_rightHelixMotor->Set(kHelixInSpeed);
		}
	}
}

void HopperControl::Disable()
{
	m_rightHelixMotor->Set(0.0);
	m_leftHelixMotor->Set(0.0);
	m_sweeperMotor->Set(0.0);
}

void HopperControl::SetBallCount(int ballsLeft, int ballsRight)
{
	m_ballsInLeft = ballsLeft;
	m_ballsInRight = ballsRight;
}

int HopperControl::GetBallCountLeft()
{
	return m_ballsInLeft;
}

int HopperControl::GetBallCountRight()
{
	return m_ballsInRight;
}
