#include "TrackingTurret.h"

TrackingTurret::TrackingTurret()
{
	m_pwmValue = 0;
	
	m_p = 0.0001;
	m_i = 0.01;
	m_d = 0.002;
	m_period = 0.05;
	loopingPID = new PIDController(m_p, m_i, m_d, m_period);
}
float TrackingTurret::aimTurret(float distanceX, float distanceY, float currentX, float currentY)
{
	
	return m_pwmValue;
}
float TrackingTurret::scanTarget(float currentX, float currentY)
{
	return m_pwmValue;
}
