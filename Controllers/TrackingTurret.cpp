#include "TrackingTurret.h"

TrackingTurret::TrackingTurret(PIDJaguar* turretMotor, Jaguar* shooterMotor, Servo* cameraServo, Joystick* test1, Joystick* test2)
{
	m_trackingCamera = new TrackingCamera(true);
	
	//Temporary
	m_joystick1 = test1;
	m_joystick2 =  test2;
	
	m_cameraServo = cameraServo;
	m_turretMotor = turretMotor;
	m_shooterMotor = shooterMotor;
	
	//All variables for PIDLoop for horizontal movement
	m_pX = -0.3;  //-0.03    Faster: -0.035
	m_iX = -0.00; //-0.0372  Faster: -0.030
	m_dX = -0.00000;          //Faster: 0.00006
	m_setpointX = -60.0;
	m_maxInputX = 1000.0;
	m_minInputX = -1000.0;
	m_maxOutputX = 1.0;
	m_minOutputX = -1.0;
	m_errorPercentX = 10.0;
	m_errorIncrementX = .001;
	
	m_calcSpeedX = new PIDControl();
	m_calcSpeedX->SetPID(m_pX, m_iX, m_dX);
	m_calcSpeedX->SetSetpoint(m_setpointX);
	m_calcSpeedX->SetSource(m_trackingCamera, m_maxInputX, m_minInputX);
	m_calcSpeedX->SetOutput(m_turretMotor, m_maxOutputX, m_minOutputX);
	m_calcSpeedX->SetError(m_errorPercentX, m_errorIncrementX);
	
	/* Variables for manual turret aiming
	 * initial voltage set to stop
	 * initial direction for PIDWrite is 0
	 * */
	m_turnMotor = 2.5;
	m_turretDirection = 0;
	
	m_inView = false;
	m_targetFound = false;
	m_shoot = false;
	m_targetDistance = 3.0;
	m_shooter = new Shoot(m_shooterMotor);
}

TrackingTurret::~TrackingTurret()
{
	
}

/* Manual override on turret turning */
void TrackingTurret::Manual(float turnMotor, float changeDistance)
{	
	/* Ranges for turning
	 * if less than c. 1.2 V, continue turning LEFT
	 * 
	 * else if greater than 3.7 V, continue turning RIGHT
	 * else stop moving
	 * 
	 * Values set for PIDWrite are based on 
	 * LEFT = 1
	 * RIGHT = -1
	 * STOP = 0
	 */
	
	m_turnMotor = turnMotor;
	m_changeDistance = changeDistance;
	
	m_targetFound = 0;
	
	/* Start turning the turret */
	if (m_turnMotor < 1.2)
	{
		m_turretDirection = .3;
	}
	else if (m_turnMotor > 3.7)
	{
		m_turretDirection = -.3;
	}
	else
	{
		m_turretDirection = 0.0;
	}
	
	m_trackingCamera->Update();
	
	if (fabs(m_trackingCamera->GetTargetX()) < 100)
	{
		m_targetFound = true;
	}
	else
	{
		m_targetFound = false;
	}
	
	m_turretMotor->PIDWrite(m_turretDirection);
	
}

void TrackingTurret::Automatic()
{
	/* Update TrackingCamera */
	if (m_inView = m_trackingCamera->Update())
	{
		printf("Target X: %f\n", m_trackingCamera->GetTargetX());
		printf("Target Y: %f\n", m_trackingCamera->GetTargetY());
	}
	
	/* If we see a target, enable the PID loop and calculate info for it */
	if (m_inView)
	{
		if (m_trackingCamera->TargetMoving())
		{
			m_calcSpeedX->SetSetpoint(m_trackingCamera->GetSetpoint());
		}
		else
		{
			m_calcSpeedX->SetSetpoint(m_setpointX);
		}
		m_calcSpeedX->Enable();
		m_targetFound = m_calcSpeedX->Calculate();
		printf("Found target: %d\n",(int)m_targetFound);
	}
	
	/* Else scan for an image, manually or automatically */
	else
	{
		m_calcSpeedX->Disable();
		printf("No Target Available \r\n");
	}
}

bool TrackingTurret::Update(bool manual, bool shoot, float turnMotor, float changeDistance)
{
	//Temporary code to help determine best values for PID loop
	if (m_joystick1->GetRawButton(11))
	{
		m_pX -= .01;
	}
	else if (m_joystick1->GetRawButton(10))
	{
		m_pX += .01;
	}
	
	
	if (m_joystick2->GetRawButton(7))
	{
		m_iX -= .0001;
	}
	else if (m_joystick2->GetRawButton(6))
	{
		m_iX += .0001;
	}
	
	if (m_joystick2->GetRawButton(11))
	{
		m_dX -= .00001;
	}
	else if (m_joystick2->GetRawButton(10))
	{
		m_dX += .00001;
	}
	printf("P: %f   I: %f   D: %f\r\n",m_pX,m_iX,m_dX);
	m_calcSpeedX->SetPID(m_pX,m_iX,m_dX);
	//End Temporary Code
	
	m_shoot = shoot;
	
	if (manual)
	{
		Manual(turnMotor, changeDistance);
	}
	else
	{
		Automatic();
	}
	
	if (m_shoot)
	{
		m_shooter->Fire(m_targetDistance);
	}
	
	return m_targetFound;
}

void TrackingTurret::ScanTarget(float currentX)
{
	/*if(m_scanLoop <= 0)
	{
		if(currentX > 0)
		{
			m_inverse = 1;
		}
		else
		{
			m_inverse = 0;
		}
	}
	
	m_scanLoop++;*/	
}

void TrackingTurret::ResetScan()
{
	//m_scanLoop = 0;
}

void TrackingTurret::StopTurret()
{
	m_calcSpeedX->Disable();
}

