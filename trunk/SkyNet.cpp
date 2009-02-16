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
	m_shooterMotorOne = m_hardwareInterface->GetJaguar(kShooterOneModule, kShooterOnePWM);
	m_shooterMotorTwo = m_hardwareInterface->GetVictor(kShooterTwoModule, kShooterTwoPWM);
	m_leftEmptyCell = m_hardwareInterface->GetVictor(kLeftCellHolderModule, kLeftCellHolderPWM);
	m_rightEmptyCell = m_hardwareInterface->GetVictor(kRightCellHolderModule, kRightCellHolderPWM);
	m_leftJoystick = m_hardwareInterface->GetJoystick(kLeftJoystickPort);
	m_rightJoystick = m_hardwareInterface->GetJoystick(kRightJoystickPort);
	
	m_leftTestJoystick = m_hardwareInterface->GetJoystick(3);
	m_rightTestJoystick = m_hardwareInterface->GetJoystick(4);
	
	m_shooterMotorOne->EnableDeadbandElimination(true);
	m_shooterMotorOne->SetBounds(255, 136, 128, 120, 0);
	
	m_shooterMotorTwo->EnableDeadbandElimination(true);
	m_shooterMotorTwo->SetBounds(255, 136, 128, 120, 0);
	
	//m_trackingTurret = new TrackingTurret(m_turretMotor, m_turretServoOne);
	
	m_manual = 0;
	m_shoot = 0;
	m_turnMotor = 0;
	m_setDistance = 3;
	
	/* Initialize drive motors and controls */	
	m_leftDriveMotor = m_hardwareInterface->GetPIDJaguar(kLeftDriveModule, kLeftDrivePWM);
	m_leftDriveMotor->EnableDeadbandElimination(true);
	m_leftDriveMotor->SetBounds(255, 136, 128, 120, 0);
	
	m_rightDriveMotor = m_hardwareInterface->GetPIDJaguar(kRightDriveModule, kRightDrivePWM);
	m_rightDriveMotor->EnableDeadbandElimination(true);
	m_rightDriveMotor->SetBounds(255, 136, 128, 120, 0);
	m_rightDriveMotor->SetDirection(-1);
	
	m_leftDriveEncoder = new PIDEncoder(kLeftDriveEncoderAModule, kLeftDriveEncoderAChannel, kLeftDriveEncoderBModule, kLeftDriveEncoderBChannel);
	m_leftDriveEncoder->SetDistancePerTick(.000478536);
	m_leftDriveEncoder->SetType(PIDEncoder::kVelocity);
	m_leftDriveEncoder->SetSampleSize(3);
	m_leftDriveEncoder->Start();
	
	m_rightDriveEncoder = new PIDEncoder(kRightDriveEncoderAModule, kRightDriveEncoderAChannel, kRightDriveEncoderBModule, kRightDriveEncoderBChannel);
	m_rightDriveEncoder->SetDistancePerTick(.000478536);
	m_rightDriveEncoder->SetType(PIDEncoder::kVelocity);
	m_rightDriveEncoder->SetSampleSize(3);
	m_rightDriveEncoder->Start();
		
	m_drive = new TankDrive(m_leftDriveMotor, m_rightDriveMotor, m_leftDriveEncoder, m_rightDriveEncoder);
	
	
	/* Sweeper, Hopper, Shooter */	
	m_turretServoOne = m_hardwareInterface->GetServo(kTurretOneModule, kTurretOnePWM);
	m_turretServoTwo = m_hardwareInterface->GetServo(kTurretTwoModule, kTurretTwoPWM);
	
	m_leftHelixMotor = m_hardwareInterface->GetVictor(kLeftHelixModule, kLeftHelixPWM);
	m_rightHelixMotor = m_hardwareInterface->GetVictor(kRightHelixModule, kRightHelixPWM);
	m_sweeperMotor = m_hardwareInterface->GetJaguar(kSweeperModule,kSweeperPWM);
	
	m_helixSide = 0;
	m_helixDirection = 0;
	
	m_leftHelixEntryLimit = new DigitalInput(kLeftLiftEntryLimitModule, kLeftLiftEntryLimitChannel);
	m_leftHelixBottomLimit = new DigitalInput(kLeftLiftBottomLimitModule, kLeftLiftBottomLimitChannel);
	m_leftHelixTopLimit = new DigitalInput(kLeftLiftTopLimitModule, kLeftLiftTopLimitChannel);
	m_rightHelixEntryLimit = new DigitalInput(kRightLiftEntryLimitModule, kRightLiftEntryLimitChannel);
	m_rightHelixBottomLimit = new DigitalInput(kRightLiftBottomLimitModule, kRightLiftBottomLimitChannel);
	m_rightHelixTopLimit = new DigitalInput(kRightLiftTopLimitModule, kRightLiftTopLimitChannel);
	
	m_hopperControl = new HopperControl(m_leftHelixMotor, m_rightHelixMotor, m_sweeperMotor, m_shooterMotorOne);
	
	
	/* Empty Cell Variables & Constants */
	m_leftEmptyCell = m_hardwareInterface->GetVictor(kLeftCellHolderModule, kLeftCellHolderPWM);
	m_rightEmptyCell = m_hardwareInterface->GetVictor(kRightCellHolderModule, kRightCellHolderPWM);

	m_emptyCellControl = new EmptyCell(m_leftEmptyCell, m_rightEmptyCell);
	
	//TODO: Talk to Mike and see if it should somehow go through HardwareInterface
	//m_leftCellBottom = new DigitalInput(kLeftCellBottomLimitModule, kLeftCellBottomLimitChannel);
	//m_rightCellBottom = new DigitalInput(kRightCellBottomLimitModule, kRightCellBottomLimitChannel);
	//m_leftCellTop = new DigitalInput(kLeftCellTopLimitModule, kLeftCellTopLimitChannel);
	//m_rightCellTop = new DigitalInput(kRightCellTopLimitModule, kRightCellTopLimitChannel);
	
	m_release = 0;
}

void SkyNet::DisabledInit()
{
	printf("Inititializing Disabled Mode..\r\n");
	m_disableCount = 0;
	
	m_hardwareInterface->EmergencyStop();
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
	
	//m_trackingTurret->StopTurret();
}

void SkyNet::DisabledPeriodic()
{
	GetWatchdog().Feed();
	m_disableCount++;
	
	if ((m_disableCount % 4) == 0)
	{
		/* Runs at 50Hz */
	
		m_hardwareInterface->UpdateDashboard(true);
	}
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
		//m_trackingTurret->Update(m_manual, m_turnMotor, m_setDistance);
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
		/* Hopper, Sweeper, and Shooter Control */
		//m_helixSide = 0;//m_ds->GetAnalogIn(1);
		//m_helixDirection = 1;//m_ds->GetAnalogIn(2);
		//m_hopperControl->Update(m_helixSide, m_helixDirection, m_leftHelixEntryLimit->Get(), m_leftHelixBottomLimit->Get(),
		//		m_leftHelixTopLimit->Get(), m_rightHelixEntryLimit->Get(), m_rightHelixBottomLimit->Get(), m_rightHelixTopLimit->Get(), m_shoot, m_setDistance, m_turretServoOne->Get());

	
		m_hardwareInterface->UpdateDashboard(true);
	}
	
	if ((m_teleCount % 2) == 0)
	{
		/* Runs at 100Hz */
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		
		/* Begin Diagnostic Code */
		//m_leftDriveMotor->Set(-m_leftTestJoystick->GetY()*.98);
		//m_rightDriveMotor->Set(m_rightTestJoystick->GetY()*.98);
		
		printf("Encoder Right: %d, Encoder Left: %d \n\r", m_rightDriveEncoder->Get(), m_leftDriveEncoder->Get());
		
		m_turretServoOne->Set(.5);
		m_turretServoTwo->Set(.5);
		
		if (m_rightTestJoystick->GetTrigger())
		{
			m_sweeperMotor->Set(.98);
		}
		else if (m_rightTestJoystick->GetTop())
		{
			m_sweeperMotor->Set(-.98);
		}
		else
		{
			m_sweeperMotor->Set(0.0);
		}
		if (m_leftTestJoystick->GetTrigger())
		{
			m_leftHelixMotor->Set(.98);
			m_rightHelixMotor->Set(.98);
		}
		else
		{
			if (m_rightTestJoystick->GetRawButton(11))
			{
				m_leftHelixMotor->Set(.98);
			}
			else if (m_rightTestJoystick->GetRawButton(6))
			{
				m_leftHelixMotor->Set(-.98);
			}
			else
			{
				m_leftHelixMotor->Set(0.0);
			}
			if (m_rightTestJoystick->GetRawButton(10))
			{
				m_rightHelixMotor->Set(.98);
			}
			else if (m_rightTestJoystick->GetRawButton(7))
			{
				m_rightHelixMotor->Set(-.98);
			}
			else
			{
				m_rightHelixMotor->Set(0.0);
			}
		}
		
		if (m_rightTestJoystick->GetRawButton(3))
		{
			m_shooterMotorOne->Set(.98);
			m_shooterMotorTwo->Set(.98);
		}
		if (m_rightTestJoystick->GetRawButton(4))
		{
			m_shooterMotorOne->Set(0.0);
			m_shooterMotorTwo->Set(0.0);
		}
		
		m_ds->SetDigitalOut(1, true);
		/* End Diagnostic Code */
		
		m_priorPacketNumber = m_ds->GetPacketNumber();
		
		/* Code dependent on driverstation/human input here */
		
		/* Tank Drive */
		//m_drive->SetSetpoint(-m_leftJoystick->GetY()* 5.0, -m_rightJoystick->GetY()* 5.0);
		//m_drive->Update();
		
		/* Empty Cell Control */
		//m_release = m_ds->GetDigitalIn(kCellSwitch);
		//m_emptyCellControl->Update(m_release, m_leftCellBottom->Get(), m_rightCellBottom->Get(), m_leftCellTop->Get(), m_rightCellTop->Get());
		
		//printf("Servo at: %f\r\n", (m_leftJoystick->GetThrottle() + 1.0)/2.1);
	}
	
	if ((m_teleCount % 10) == 0)
	{
		/* Turret, shooter, and camera servo code */
		/*m_manual = m_ds->GetDigitalIn(1);
		m_shoot = m_ds->GetDigitalIn(3);
		m_turnMotor = m_ds->GetAnalogIn(3);
		m_setDistance = m_ds->GetAnalogIn(4);
		
		if (m_trackingTurret->Update(m_manual, m_turnMotor, m_setDistance))
		{
			m_ds->SetDigitalOut(kTargetAquiredLED, 1);
			
		}
		else
		{
			m_ds->SetDigitalOut(kTargetAquiredLED, 0);			
		}*/
	}	
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
