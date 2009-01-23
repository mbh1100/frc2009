#include "TrackingTurret.h"

TrackingTurret::TrackingTurret(Jaguar* motor7, Servo* servo8, Joystick* test1, Joystick* test2)
{
	m_valueX = 0;
	m_valueY = 0;
	m_currentY = 0;
	m_maxServo = 1;
	
	m_joystick1 = test1;
	m_joystick2 =  test2;
	m_cameraServo = servo8;
	m_turretMotor = motor7;
	
	//All variables
	m_pX = 0.1;
	m_iX = 0.0001;
	m_dX = 0.0002;
	m_pY = 0.001;
	m_iY = 0.0001;
	m_dY = 0.002;
	m_allowedErrorX = 100;
	m_allowedErrorY = 50;
	m_maxOutX = 0.1;
	m_minOutX = -0.1;
	m_relationX = 0.0005;
	m_incrementX = 0.001;
	m_desiredX = 0.0;
	m_maxOutY = 0.001;
	m_minOutY = 0.0;
	m_relationY = 0.0005;
	m_incrementY = .00001;
	m_desiredY = 0.0;	
	
	m_calcSpeedX = new PIDControl(m_pX, m_iX, m_dX, m_allowedErrorX);
	m_calcSpeedY = new PIDControl(m_pY, m_iY, m_dY, m_allowedErrorY);
	m_calcSpeedX->SetDesired(m_maxOutX, m_minOutX, m_incrementX, m_desiredX, m_relationX);
	m_calcSpeedY->SetDesired(m_maxOutY, m_minOutY, m_incrementX, m_desiredX, m_relationY);
	
	m_scanLoop = 0;
	m_inverse = 0;
	m_scanP = .1;
	m_scanI = 0;
	m_scanD = 0;
	m_maxOutScan = .2;
	m_minOutScan = -.2;
	m_potLeft = 100;
	m_potRight = 500;
	m_calcScan = new PIDControl(m_scanP, m_scanI, m_scanD,10.0);
	m_calcScan->SetDesired(m_maxOutScan, m_minOutScan, 0.0, m_potLeft, m_relationX);
	
	m_trackingCamera = new TrackingCamera(true);
}

void TrackingTurret::Update()
{
	if (m_joystick1->GetRawButton(11))
	{
		m_pX -= .0001;
	}
	else if (m_joystick1->GetRawButton(10))
	{
		m_pX += .0001;
	}
	
	
	if (m_joystick2->GetRawButton(11))
	{
		m_iX -= .0001;
	}
	else if (m_joystick2->GetRawButton(10))
	{
		m_iX += .0001;
	}
	
	if (m_joystick2->GetRawButton(7))
	{
		m_dX -= .0001;
	}
	else if (m_joystick2->GetRawButton(6))
	{
		m_dX += .0001;
	}
	printf("P: %f   I: %f   D: %f\r\n",m_pX,m_iX,m_dX);
	m_calcSpeedX->SetDesired(m_pX,m_iX,m_dX);
	
	if (m_inView = m_trackingCamera->Update())
	{
		m_distanceX = m_trackingCamera->GetTargetX();
		m_distanceY = m_trackingCamera->GetTargetY();
		printf("Target X: %f\n",m_distanceX);
		printf("Target Y: %f\n",m_distanceY);
	}
	if (m_inView)
	{
		bool foundTarget = AimTurret();
		printf("Found target: %d\n",(int)foundTarget);
	}
	else
	{
		StopTurret();
		printf("No Target Available \r\n");
	}
}

bool TrackingTurret::AimTurret()
{	
	bool xDone,yDone;
	xDone = m_calcSpeedX->IsDone(m_distanceX);
	yDone = m_calcSpeedY->IsDone(m_distanceY);
	if(xDone && yDone)
	{
		m_cameraServo->Set(0.0);
		m_turretMotor->Set(0.0);
		return true;
	}
	else
	{
		if(!xDone)
		{
			m_valueX = m_calcSpeedX->CalcPID(m_distanceX);
		}
		else
		{
			m_valueX = 0.0;
		}
		if(!yDone)
		{
			m_valueY = m_calcSpeedY->CalcPID(m_distanceY);
			printf("Y value before added to servo: %f\n", m_valueY);
			m_currentY = m_cameraServo->Get();
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
		
		m_cameraServo->Set(m_currentY);
		m_turretMotor->Set(-(m_valueX));
		return false;
	}
}

void TrackingTurret::ScanTarget(float currentX)
{
	if(m_scanLoop <= 0)
	{
		if(currentX > 0)
		{
			m_inverse = 1;
			m_calcScan->SetDesired(m_potMax);
		}
		else
		{
			m_inverse = 0;
			m_calcScan->SetDesired(m_potMin);
		}
	}
	
	m_scanLoop++;	
}

void TrackingTurret::ResetScan()
{
	m_scanLoop = 0;
}

void TrackingTurret::StopTurret()
{
	m_cameraServo->Set(0.0);
	m_turretMotor->Set(0.0);
}
