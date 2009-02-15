#include "TrackingTurret.h"

TrackingTurret::TrackingTurret(PIDServo* primary, PIDServo* secondary)
{
	m_trackingCamera = new TrackingCamera(true);
	
	m_primaryServo = primary;
	m_secondaryServo = secondary;
	
	m_scanDirection = 1;
	
	m_inView = false;
	m_targetFound = false;
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
			m_scanDirection = sign(m_trackingCamera->GetTargetX());
			m_motorPos += kTurretMaxInc * m_trackingCamera->GetTargetSize() * m_scanDirection;
		
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

bool TrackingTurret::Update(int direction)
{	
	m_inView = m_trackingCamera->Update();
	
	if (fabs(m_trackingCamera->GetTargetX()) > kTurretAllowedError)
	{
		m_targetFound = false;
	}
	else
	{
		m_targetFound = true;
	}
	
	m_motorPos += kTurretManualInc * (double)direction;
	
	UpdateMotors();
	
	return m_targetFound;
}

void TrackingTurret::UpdateMotors()
{
	if (m_motorPos > 1.0 || m_motorPos < 0.0)
	{
		m_motorPos = m_scanDirection == -1 ? 0.0 : 1.0;
		
		m_scanDirection *= -1;
	}
	
	m_primaryServo->Set(m_motorPos);
	m_secondaryServo->Set(m_motorPos);
}
