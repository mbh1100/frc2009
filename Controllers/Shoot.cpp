#include "Shoot.h"

Shoot::Shoot(AdvMotorController* shootMotorOne, AdvMotorController* shootMotorTwo, AdvMotorController* leftHelixMotor, AdvMotorController* rightHelixMotor)
{
	m_shootMotorOne = shootMotorOne;
	m_shootMotorTwo = shootMotorTwo;
	m_leftHelixMotor = leftHelixMotor;
	m_rightHelixMotor = rightHelixMotor;
	
	m_leftTimer = new Timer();
	m_rightTimer = new Timer();
	
	m_timer = new Timer();	
	
	m_direction = 0;
	m_distance = 3;
	m_ballsLeft = 0;
	m_ballsRight = 0;
	
	m_limitLeft = false;
	m_limitRight = false;
	
	m_leftPressed = false;
	m_rightPressed = false;
}

Shoot::~Shoot()
{
	
}

void Shoot::InitialSet(int ballsLeft, int ballsRight, float direction)
{
	m_ballsLeft = ballsLeft;
	m_ballsRight = ballsRight;
	m_direction = direction;
	if (m_direction <= .5)
	{
		m_direction = kLeft;
	}
	else
	{
		m_direction = kRight;
	}
	
	m_timer->Start();
}

void Shoot::Update(float distance, bool limitLeft, bool limitRight)
{
	m_distance = distance;
	m_limitLeft = limitLeft;
	m_limitRight = limitRight;
	
	m_shootMotorOne->Set(Distance());
	//m_shootMotorOne->Set(.98);
	m_shootMotorTwo->Set(.98);
	
	printf("ShooterSpeed: %lf\r\n", m_shootMotorOne->Get());
	printf("Distance Value: %lf\r\n", m_distance);
	
	SideShooting();
	
	CountBallsLeft();
	CountBallsRight();
}

float Shoot::Distance()
{
	if (m_distance < 200)
	{
		return .17;
	}
	else if (m_distance < 350)
	{
		return .33;
	}
	else if (m_distance < 500)
	{
		return .5;
	}
	else if (m_distance < 650)
	{
		return .67;
	}
	else if (m_distance < 775)
	{
		return .83;
	}
	else
	{
		return .98f;
	}
}

int Shoot::CountBallsLeft()
{
	if (m_limitLeft && !m_leftPressed && m_leftHelixMotor != 0)
	{
		m_leftPressed = true;
		m_ballsLeft--;
		m_leftTimer->Reset();
		m_leftTimer->Start();
	}
	
	if (m_leftPressed && m_leftTimer->Get() >= kTimePerShot)
	{
		m_leftPressed = false;
		m_leftTimer->Stop();
	}	
	
	return m_ballsLeft;
}

int Shoot::CountBallsRight()
{
	if (m_limitRight && !m_rightPressed && m_rightHelixMotor != 0)
	{
		m_rightPressed = true;
		m_ballsRight--;
		m_rightTimer->Reset();
		m_rightTimer->Start();
	}
	
	if (m_rightPressed && m_rightTimer->Get() >= kTimePerShot)
	{
		m_rightPressed = false;
		m_rightTimer->Stop();
	}	
	
	return m_ballsRight;
}

void Shoot::SideShooting()
{
	if (m_timer->Get() < 2)
	{
		if (m_direction == 0)
		{
			if (m_ballsRight > 0)
			{
				m_rightHelixMotor->Set(.98);
				m_leftHelixMotor->Set(0.0);
			}
			else if (m_ballsLeft > 0)
			{
				m_rightHelixMotor->Set(0.0);
				m_leftHelixMotor->Set(.98);			
			}
			else
			{
				m_rightHelixMotor->Set(.98);
				m_leftHelixMotor->Set(.98);				
			}
		}
		else
		{
			if (m_ballsLeft > 0)
			{
				m_rightHelixMotor->Set(0.0);
				m_leftHelixMotor->Set(.98);
			}
			else if (m_ballsRight > 0)
			{
				m_rightHelixMotor->Set(.98);
				m_leftHelixMotor->Set(0.0);
			}
			else
			{
				m_rightHelixMotor->Set(.98);
				m_leftHelixMotor->Set(.98);				
			}
		}
	}
	else
	{
		m_rightHelixMotor->Set(.98);
		m_leftHelixMotor->Set(.98);			
	}	
}

void Shoot::Disable()
{
	m_rightHelixMotor->Set(0.0);
	m_leftHelixMotor->Set(0.0);
	m_shootMotorOne->Set(0.0);
	m_shootMotorTwo->Set(0.0);
	m_timer->Stop();
	m_timer->Reset();
}
