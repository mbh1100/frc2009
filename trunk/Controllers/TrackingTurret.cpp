#include "TrackingTurret.h"

TrackingTurret::TrackingTurret(AdvServo* primary, AdvServo* secondary)
{
	m_trackingCamera = new TrackingCamera(true);
	
	m_primaryServo = primary;
	m_secondaryServo = secondary;
	
	m_scanDirection = 1;
	
	m_inView = false;
	m_targetFound = false;
	
	m_direction = .5;
}

TrackingTurret::~TrackingTurret()
{
	
}

bool TrackingTurret::Update()
{
	m_inView = m_trackingCamera->Update();
	
	/* If we see a target, enable the PID loop and calculate info for it */
	if (m_inView)
	{
		if (fabs(m_trackingCamera->GetTargetX()) > kTurretAllowedError)
		{
			//m_scanDirection = -sign(m_trackingCamera->GetTargetX());
			m_motorPos += -kTurretMaxInc * m_trackingCamera->GetTargetX()/1000;
		
			m_targetFound = false;
		}
		else
		{
			m_targetFound = true;
		}
	}
	else
	{	
		m_motorPos += kTurretManualInc * m_scanDirection;
	}
	
	UpdateMotors();
	
	return m_targetFound;
}

bool TrackingTurret::Update(float direction)
{	
	m_inView = m_trackingCamera->Update();
	
	m_direction = direction;
	if (m_direction > 700)
	{
		m_direction = 700;
	}
	else if (m_direction < 350)
	{
		m_direction = 350;
	}
	m_direction -= 350;
	m_direction = m_direction/350;
	m_direction *= .59;
	m_direction += .2;
	
	if (fabs(m_trackingCamera->GetTargetX()) > kTurretAllowedError)
	{
		m_targetFound = false;
	}
	else
	{
		m_targetFound = true;
	}
	
	m_motorPos = (double)m_direction;
	
	UpdateMotors();
	
	printf("ServoOne Position: %f\r\n", m_primaryServo->Get());
	printf("ServoTwo Position: %f\r\n", m_secondaryServo->Get());
	
	return m_targetFound;
}

void TrackingTurret::UpdateMotors()
{
	if (m_motorPos > .8 || m_motorPos < 0.2)
	{
		m_motorPos = m_scanDirection == -1 ? 0.2 : 0.8;
		
		m_scanDirection *= -1;
	}
	
	m_primaryServo->Set(m_motorPos);
	m_secondaryServo->Set(m_motorPos);
}
