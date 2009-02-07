#include "SkyNet.h"

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	/* Initialize Hardware Interface & Driver station*/
	m_hardwareInterface = new HardwareInterface(true, true);
	
	m_ds = m_hardwareInterface->GetDriverStation();
	m_priorPacketNumber = 0;
	
	/* Initialize Analog Modules */
	m_analogModules[0] = m_hardwareInterface->GetAnalogModule(0);
	m_analogModules[1] = m_hardwareInterface->GetAnalogModule(1);
	
	m_analogModules[0]->SetAverageBits(1,8);
	m_analogModules[1]->SetAverageBits(1,8);
	
	/* Initialize Camera motors and control */
	m_turretMotor = m_hardwareInterface->GetPIDJaguar(0, 3);
	m_shooterMotor = m_hardwareInterface->GetJaguar(0, 4);
	m_cameraServo = m_hardwareInterface->GetServo(0, 8);
	m_joystick1 = m_hardwareInterface->GetJoystick(1);
	m_joystick2 = m_hardwareInterface->GetJoystick(2);
	
	m_turretMotor->EnableDeadbandElimination(true);
	m_turretMotor->SetBounds(255, 136, 128, 120, 0);
	
	m_shooterMotor->EnableDeadbandElimination(true);
	m_shooterMotor->SetBounds(255, 136, 128, 120, 0);
	
	m_trackingTurret = new TrackingTurret(m_turretMotor, m_shooterMotor, m_cameraServo, m_joystick1, m_joystick2);
	
	m_manual = 0;
	m_shoot = 0;
	m_turnMotor = 0;
	m_setDistance = 3;
	
	/* Initialize drive motors and controls */	
	m_leftDriveMotor = m_hardwareInterface->GetPIDJaguar(0, 1);
	m_leftDriveMotor->EnableDeadbandElimination(true);
	m_leftDriveMotor->SetBounds(255, 136, 128, 120, 0);
	
	m_rightDriveMotor = m_hardwareInterface->GetPIDJaguar(0, 2);
	m_rightDriveMotor->EnableDeadbandElimination(true);
	m_rightDriveMotor->SetBounds(255, 136, 128, 120, 0);	
	
	m_leftDriveEncoder = new PIDEncoder(4, 3, 4, 4);
	m_leftDriveEncoder->SetDistancePerTick(-0.001);
	m_leftDriveEncoder->SetType(PIDEncoder::kVelocity);
	m_leftDriveEncoder->SetSampleSize(3);
	m_leftDriveEncoder->Start();
	
	m_rightDriveEncoder = new PIDEncoder(4, 1, 4, 2);
	m_rightDriveEncoder->SetDistancePerTick(-0.001);
	m_rightDriveEncoder->SetType(PIDEncoder::kVelocity);
	m_rightDriveEncoder->SetSampleSize(3);
	m_rightDriveEncoder->Start();
		
	m_drive = new TankDrive(m_leftDriveMotor, m_rightDriveMotor, m_leftDriveEncoder, m_rightDriveEncoder);
	
	
	/* Sweeper and Hopper */	
	m_leftHelixMotor = m_hardwareInterface->GetVictor(0, 6);
	m_rightHelixMotor = m_hardwareInterface->GetVictor(0, 7);
	m_sweeperMotor = m_hardwareInterface->GetVictor(0,5);
	
	m_sweeperState = false;
	m_helixState = 0;
	m_helixDirection = 0;
	
	m_hopperControl = new HopperControl(m_leftHelixMotor, m_rightHelixMotor, m_sweeperMotor);
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
	
	/* Disable TankDrive */
	m_drive->Disable();
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
	
	/* Finding & tracking the target with the camera */
	if ((m_autoCount % 10) == 0)
	{
		//Should only be in teleop mode - should just set values in autonomous
		m_manual = false;
		m_shoot = false;
		m_turnMotor = 0.0;
		m_setDistance = 0.0;
		
		//Change in the near future
		m_trackingTurret->Update(m_manual, m_shoot, m_turnMotor, m_setDistance);
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
	
	}
	
	if ((m_teleCount % 2) == 0)
	{
		/* Runs at 100Hz */
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		m_priorPacketNumber = m_ds->GetPacketNumber();
		
		/* Code dependent on driverstation/human input here */
		
		/* Tank Drive */
		m_drive->SetSetpoint(m_joystick1->GetY()*-1.5, m_joystick2->GetY()*-7.2);
		m_drive->Update();
		
		/* Hopper and Sweeper Control */
		m_sweeperState = m_ds->GetDigitalIn(2);
		m_helixState = m_ds->GetAnalogIn(1);
		m_helixDirection = m_ds->GetAnalogIn(2);
		m_hopperControl->Update(m_sweeperState, m_helixState, m_helixDirection);
	}
	
	if ((m_teleCount % 10) == 0)
	{
		/* Turret, shooter, and camera servo code */
		m_manual = m_ds->GetDigitalIn(1);
		m_shoot = m_ds->GetDigitalIn(3);
		m_turnMotor = m_ds->GetAnalogIn(3);
		m_setDistance = m_ds->GetAnalogIn(4);
		
		m_trackingTurret->Update(m_manual, m_shoot, m_turnMotor, m_setDistance);
	}	
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
