#include "SkyNet.h"

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	m_hardwareInterface = new HardwareInterface(true, true);
	
	m_ds = m_hardwareInterface->GetDriverStation();
	m_priorPacketNumber = 0;
	
	m_analogModules[0] = m_hardwareInterface->GetAnalogModule(0);
	m_analogModules[1] = m_hardwareInterface->GetAnalogModule(1);
	
	m_analogModules[0]->SetAverageBits(1,8);
	m_analogModules[1]->SetAverageBits(1,8);
	
	m_turretMotor = m_hardwareInterface->GetPIDJaguar(0, 7);
	m_cameraServo = m_hardwareInterface->GetServo(0, 8);
	m_joystick1 = m_hardwareInterface->GetJoystick(1);
	m_joystick2 = m_hardwareInterface->GetJoystick(2);
	
	m_leftDriveEncoder = new PIDEncoder(4,1,4,2);
	m_leftDriveEncoder->SetDistancePerTick(-0.001);
	m_leftDriveEncoder->SetType(PIDEncoder::kVelocity);
	m_leftDriveEncoder->Start();
	
	m_leftDriveMotor = m_hardwareInterface->GetPIDJaguar(0,5);
	m_leftDriveMotor->EnableDeadbandElimination(true);
	m_leftDriveMotor->SetBounds(255, 136, 128, 120, 0);
	
	m_leftDrive = new TankDrive(m_leftDriveMotor, m_joystick1, m_leftDriveEncoder);
	/*m_driveP = .1;
	m_driveI = .00;
	m_driveD = .00;
	
	m_leftDriveMotor = m_hardwareInterface->GetPIDJaguar(0, 5);
	m_rightDriveMotor = m_hardwareInterface->GetPIDJaguar(0, 4);
	
	m_leftDriveMotor->EnableDeadbandElimination(true);
	m_leftDriveMotor->SetBounds(255, 136, 128, 120, 0);
	
	m_encoderLeft = new PIDEncoder(4,1,4,2);
	m_encoderLeft->SetDistancePerTick(-0.001);
	m_encoderLeft->SetType(PIDEncoder::kAcceleration);
	m_encoderLeft->Start();
	
	m_calcLeftDrive = new PIDController(m_driveP, m_driveI, m_driveD);
	m_calcLeftDrive->SetTolerence(1.0);
	m_calcLeftDrive->SetInput(m_encoderLeft, -1.0, 1.0);	
	m_calcLeftDrive->SetOutput(m_leftDriveMotor, -1.0, 1.0);
	m_calcLeftDrive->SetSetpoint(0.0);*/
	
	m_turretMotor->EnableDeadbandElimination(true);
	m_turretMotor->SetBounds(255, 136, 128, 120, 0);
	m_trackingTurret = new TrackingTurret(m_turretMotor, m_cameraServo, m_joystick1, m_joystick2);
}

void SkyNet::DisabledInit()
{
	printf("Inititializing Disabled Mode..\r\n");
	
	//m_calcLeftDrive->Disable();
}

void SkyNet::AutonomousInit()
{
	printf("Inititializing Autonomous Mode..\r\n");
	m_autoCount = 0;
	
	//m_calcLeftDrive->Disable();
}

void SkyNet::TeleopInit()
{
	printf("Inititializing Teleop Mode..\r\n");
	m_teleCount = 0;
	
	//m_calcLeftDrive->Enable();
	m_trackingTurret->StopTurret();
}

void SkyNet::DisabledPeriodic()
{
	GetWatchdog().Feed();
}

void SkyNet::AutonomousPeriodic()
{
	GetWatchdog().Feed();
	m_autoCount++;
	
	//Finding the position of the camera
	if ((m_autoCount % 10) == 0)
	{
		m_trackingTurret->Update();
	}
	
	if ((m_autoCount % 4) == 0)
	{
		/* Runs at 50Hz */
			
		m_hardwareInterface->UpdateDashboard(true);
	}
}

void SkyNet::TeleopPeriodic()
{
	GetWatchdog().Feed();
	m_teleCount++;
		
	if ((m_teleCount % 200) == 0)
	{
		/* Runs at 1Hz */
	}
	
	if ((m_teleCount % 20) == 0)
	{
		/* Runs at 10Hz */
	}
	
	if ((m_teleCount % 4) == 0)
	{
		/* Runs at 50Hz */
		
		m_hardwareInterface->UpdateDashboard(true);
	
	}
	
	if ((m_teleCount % 2) == 0)
	{
		/* Runs at 100Hz */
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		m_priorPacketNumber = m_ds->GetPacketNumber();
		
		/* Code dependent on driverstation/human input here */
		
		m_leftDrive->Update();
	}
	
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
