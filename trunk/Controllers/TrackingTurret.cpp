#include "TrackingTurret.h"

TrackingTurret::TrackingTurret(PIDJaguar* motor7, Servo* servo8, Joystick* test1, Joystick* test2)
{
	m_trackingCamera = new TrackingCamera(true);
	
	//Temporary
	m_joystick1 = test1;
	m_joystick2 =  test2;
	
	//m_cameraServo = servo8;
	m_turretMotor = motor7;
	
	//All variables for PIDLoop for horizontal movement
	m_pX = -0.035;  //-0.03    Faster: -0.035
	m_iX = -0.03; //-0.0372  Faster: -0.030
	m_dX = -0.00006;          //Faster: 0.00006
	m_setpointX = 0.0;
	m_maxInputX = 1000.0;
	m_minInputX = -1000.0;
	m_maxOutputX = 1.0;
	m_minOutputX = -1.0;
	m_errorPercentX = 10;
	m_errorIncrementX = .001;
	
	m_calcSpeedX = new PIDControl();
	m_calcSpeedX->SetPID(m_pX, m_iX, m_dX);
	m_calcSpeedX->SetSetpoint(m_setpointX);
	m_calcSpeedX->SetSource(m_trackingCamera, m_maxInputX, m_minInputX);
	m_calcSpeedX->SetOutput(m_turretMotor, m_maxOutputX, m_minOutputX);
	m_calcSpeedX->SetError(m_errorPercentX, m_errorIncrementX);
}

TrackingTurret::~TrackingTurret()
{
	
}

void TrackingTurret::Update()
{
	//Temporary code to help determine best values for PID loop
	if (m_joystick1->GetRawButton(11))
	{
		m_pX -= .0001;
	}
	else if (m_joystick1->GetRawButton(10))
	{
		m_pX += .0001;
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
	
	//Update TrackingCamera
	if (m_inView = m_trackingCamera->Update())
	{
		printf("Target X: %f\n", m_trackingCamera->GetTargetX());
		printf("Target Y: %f\n", m_trackingCamera->GetTargetY());
	}
	
	/* If we see a target, enable the PID loop and calculate info for it */
	if (m_inView)
	{
		m_calcSpeedX->Enable();
		bool foundTarget = m_calcSpeedX->Calculate();
		printf("Found target: %d\n",(int)foundTarget);
	}
	
	/* Else scan for an image, manually or automatically */
	else
	{
		m_calcSpeedX->Disable();
		printf("No Target Available \r\n");
	}
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
