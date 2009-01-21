#include "TrackingTurret.h"

TrackingTurret::TrackingTurret()
{
	m_valueX = 0;
	m_valueY = 0;
	m_currentY = 0;
	m_maxServo = 1;
	
	m_servo8 = new Servo(8);
	m_motor7 = new Jaguar(7);
	
	//All variables
	m_pX = 0.1;
	m_iX = 0;//0.0001;
	m_dX = 0;//0.0002;
	m_pY = 0.01;
	m_iY = 0.0001;
	m_dY = 0.002;
	m_allowedErrorX = 100;
	m_allowedErrorY = 50;
	m_maxOutX = 0.1;
	m_minOutX = -0.1;
	m_relationX = 0.0005;
	m_incrementX = 0.001;
	m_desiredX = 0.0;
	m_maxOutY = 0.01;
	m_minOutY = 0.0;
	m_relationY = 0.0005;
	m_incrementY = .001;
	m_desiredY = 0.0;	
	
	m_calcSpeedX = new PIDcontrol(m_pX, m_iX, m_dX, m_allowedErrorX);
	m_calcSpeedY = new PIDcontrol(m_pY, m_iY, m_dY, m_allowedErrorY);
	m_calcSpeedX->setDesired(m_maxOutX, m_minOutX, m_incrementX, m_desiredX, m_relationX);
	m_calcSpeedY->setDesired(m_maxOutY, m_minOutY, m_incrementX, m_desiredX, m_relationY);
	
	m_scanLoop = 0;
	m_inverse = 0;
	m_scanP = .1;
	m_scanI = 0;
	m_scanD = 0;
	m_maxOutScan = .2;
	m_minOutScan = -.2;
	m_potLeft = 100;
	m_potRight = 500;
	m_calcScan = new PIDcontrol(m_scanP, m_scanI, m_scanD,10.0);
	m_calcScan->setDesired(m_maxOutScan, m_minOutScan, 0.0, m_potLeft, m_relationX);
}
bool TrackingTurret::aimTurret(float distanceX, float distanceY)
{	
	bool xDone,yDone;
	xDone = m_calcSpeedX->isDone(distanceX);
	yDone = m_calcSpeedY->isDone(distanceY);
	if(xDone && yDone)
	{
		m_servo8->Set(0.0);
		m_motor7->Set(0.0);
		return true;
	}
	else
	{
		if(!xDone)
		{
			m_valueX = m_calcSpeedX->calcPID(distanceX);
		}
		else
		{
			m_valueX = 0.0;
		}
		if(!yDone)
		{
			m_valueY = m_calcSpeedY->calcPID(distanceY);
			m_currentY = m_servo8->Get();
			m_currentY += m_valueY;
			if (m_currentY > m_maxServo);
			{
				m_currentY = m_maxServo;
			}
		}
		else
		{
			m_currentY = 0.0;
		}
		
		printf("Output to motor: %f\n", m_valueX);
		printf("Output to servo: %f\n", m_currentY);
		
		if(m_valueX > 0)
		{
			m_valueX += .05;
		}
		else
		{
			m_valueX -= .05;
		}
		
		m_servo8->Set(m_currentY);
		m_motor7->Set(-(m_valueX));
		return false;
	}
}
void TrackingTurret::scanTarget(float currentX)
{
	if(m_scanLoop <= 0)
	{
		if(currentX > 0)
		{
			m_inverse = 1;
			m_calcScan->setDesired(m_potMax);
		}
		else
		{
			m_inverse = 0;
			m_calcScan->setDesired(m_potMin);
		}
	}
	
	
	m_scanLoop++;	
}
void TrackingTurret::resetScan()
{
	m_scanLoop = 0;
}
void TrackingTurret::stopTurret()
{
	m_servo8->Set(0.0);
	m_motor7->Set(0.0);
}
