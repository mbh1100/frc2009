#include "TrackingTurret.h"

TrackingTurret::TrackingTurret()
{
	m_valueX = 0;
	m_valueY = 0;
	m_currentY = 0;
	m_maxServo = 1;
	
	m_servo8 = new Servo(8);
	m_motor7 = new Jaguar(7);
	
	m_p = 0.0001;
	m_i = 0.01;
	m_d = 0.002;
	m_allowedError = 30;
	m_period = 0.05;
	m_maxOutX = 1.0;
	m_minOutX = -1.0;
	m_incrementX = 0.01;
	m_desiredX = 0.0;
	m_maxOutY = 1.0;
	m_minOutY = 0.0;
	m_incrementY = .005;
	m_desiredY = 0.0;	
	
	calcSpeedX = new PIDcontrol(m_p, m_i, m_d, m_period);
	calcSpeedY = new PIDcontrol(m_p, m_i, m_d, m_period);
	calcSpeedX->setDesired(m_maxOutX, m_minOutX, m_incrementX, m_desiredX);
	calcSpeedY->setDesired(m_maxOutY, m_minOutY, m_incrementX, m_desiredX);
}
bool TrackingTurret::aimTurret(float distanceX, float distanceY)
{	
	if(calcSpeedX->isDone() && calcSpeedY->isDone())
	{
		return true;
	}
	else
	{
		if(!calcSpeedX->isDone())
		{
			m_valueX = calcSpeedX->calcPID(distanceX);
		}
		if(!calcSpeedY->isDone())
		{
			m_valueY = calcSpeedY->calcPID(distanceY);
		}
		m_currentY = m_servo8->Get();
		m_currentY += m_valueY;
		if (m_currentY > m_maxServo);
		{
			m_currentY = m_maxServo;
		}
		m_servo8->Set(m_currentY);
		m_motor7->Set(m_valueX);
		return false;
	}
}
void TrackingTurret::scanTarget(float currentX, float currentY)
{
}
