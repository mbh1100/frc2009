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
	
	/* Initialize Digital Modules */
	m_digitalModules[0] = m_hardwareInterface->GetDigitalModule(0);
	m_digitalModules[1] = m_hardwareInterface->GetDigitalModule(1);
	
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
	
	m_manual = 0;
	m_shoot = 0;
	m_turnMotor = 0.0;
	m_setDistance = 3.0;
	
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
	
	m_drivePID = true;
	
	
	/* Sweeper, Hopper, Shooter */	
	m_turretServoOne = m_hardwareInterface->GetAdvServo(kTurretOneModule, kTurretOnePWM);
	((AdvMotorController*)m_turretServoOne)->SetBounds(.8, .2);
	
	m_turretServoTwo = m_hardwareInterface->GetAdvServo(kTurretTwoModule, kTurretTwoPWM);
	((AdvMotorController*)m_turretServoTwo)->SetBounds(.8, .2);
	m_turretServoTwo->SetDirection(kMotorReverse);
	
	m_trackingTurret = new TrackingTurret(m_turretServoOne, m_turretServoTwo);
	
	m_leftHelixMotor = m_hardwareInterface->GetVictor(kLeftHelixModule, kLeftHelixPWM);
	m_rightHelixMotor = m_hardwareInterface->GetVictor(kRightHelixModule, kRightHelixPWM);
	m_sweeperMotor = m_hardwareInterface->GetJaguar(kSweeperModule,kSweeperPWM);
	
	m_helixSide = 0;
	m_helixDirection = 0;
	
	m_hopperControl = new HopperControl(m_leftHelixMotor, m_rightHelixMotor, m_sweeperMotor, m_shooterMotorOne, m_shooterMotorTwo);
	
	m_emptyCellControl = new EmptyCell(m_leftEmptyCell, m_rightEmptyCell);
	
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
		//Need to change so 
		m_shoot = false;
		
		m_trackingTurret->Update();
		
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
		/* Turret, shooter, and camera servo code */
		m_manual = (m_ds->GetDigitalIn(kOverRideSwitch));
		m_turnMotor = m_ds->GetAnalogIn(kTurretAngleSwitch);
		//TODO: MOVE THIS
		m_setDistance = m_ds->GetAnalogIn(kShootingDistanceSwitch);
		
		if (m_manual)
		{
			if (m_trackingTurret->Update(m_turnMotor))
			{
				m_ds->SetDigitalOut(kTargetAquiredLED, 1);
			}
			else
			{
				m_ds->SetDigitalOut(kTargetAquiredLED, 0);			
			}
		}
		else
		{
			if (m_trackingTurret->Update())
			{
				m_ds->SetDigitalOut(kTargetAquiredLED, 1);
				
			}
			else
			{
				m_ds->SetDigitalOut(kTargetAquiredLED, 0);			
			}
		}
		
		
		/* Hopper, Sweeper, and Shooter Control */
		m_shoot = !(m_ds->GetDigitalIn(kShootButton));
		m_helixSide = m_ds->GetAnalogIn(kHelixSideSwitch);
		m_helixDirection = m_ds->GetAnalogIn(kHelixDirectionSwitch);
		m_turnMotor = m_ds->GetAnalogIn(kTurretAngleSwitch);
		m_setDistance = m_ds->GetAnalogIn(kShootingDistanceSwitch);
		m_hopperControl->Update(m_manual, m_helixSide, m_helixDirection, 
				(bool)m_digitalModules[kLeftLiftEntryLimitModule]->GetDIO(kLeftLiftEntryLimitChannel), 
				(bool)m_digitalModules[kLeftLiftBottomLimitModule]->GetDIO(kLeftLiftBottomLimitChannel),
				(bool)m_digitalModules[kLeftLiftTopLimitModule]->GetDIO(kLeftLiftTopLimitChannel),
				(bool)m_digitalModules[kRightLiftEntryLimitModule]->GetDIO(kRightLiftEntryLimitChannel), 
				(bool)m_digitalModules[kRightLiftBottomLimitModule]->GetDIO(kRightLiftBottomLimitChannel),
				(bool)m_digitalModules[kRightLiftTopLimitModule]->GetDIO(kRightLiftTopLimitChannel), m_shoot, m_turnMotor, m_setDistance);
		
		
		/* Empty Cell Control */
		m_release = !(m_ds->GetDigitalIn(kCellSwitch));
		m_emptyCellControl->Update(m_release, 
				m_digitalModules[kLeftCellBottomLimitModule]->GetDIO(kLeftCellBottomLimitChannel), 
				m_digitalModules[kRightCellBottomLimitModule]->GetDIO(kRightCellBottomLimitChannel), 
				m_digitalModules[kLeftCellTopLimitModule]->GetDIO(kLeftCellTopLimitChannel), 
				m_digitalModules[kRightCellTopLimitModule]->GetDIO(kRightCellTopLimitChannel));
		
		m_hardwareInterface->UpdateDashboard(true);
	}
	
	if ((m_teleCount % 2) == 0)
	{
		/* Runs at 100Hz */
		/* Tank Drive */
		m_leftDriveMotor->Set(-m_leftTestJoystick->GetY()*.98);
		m_rightDriveMotor->Set(m_rightTestJoystick->GetY()*.98);
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		
		/* Begin Diagnostic Code */
		/*m_turretServoOne->Set(1.0);
		m_turretServoTwo->Set(1.0);
		m_leftDriveMotor->Set(-m_leftTestJoystick->GetY()*.98);
		m_rightDriveMotor->Set(m_rightTestJoystick->GetY()*.98);
		
		printf("Encoder Right: %d, Encoder Left: %d \n\r", m_rightDriveEncoder->Get(), m_leftDriveEncoder->Get());
		
		m_turretServoOne->Set(1.0);
		m_turretServoTwo->Set(1.0);
		
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
		
		m_ds->SetDigitalOut(1, true);*/
		/* End Diagnostic Code */
		
		m_priorPacketNumber = m_ds->GetPacketNumber();
		
		/* Code dependent on driverstation/human input here */
		
		/* Tank Drive */
		/*if (m_leftJoystick->GetRawButton(3))
		{
			m_drivePID = true;
		}
		if (m_leftJoystick->GetRawButton(2))
		{
			m_drivePID = false;
		}
		if (m_drivePID)
		{
			//m_drive->SetSetpoint(-m_leftJoystick->GetY()* 5.0, -m_rightJoystick->GetY()* 5.0);
			//m_drive->Update();
		}
		else
		{
			m_leftDriveMotor->Set(-m_leftJoystick->GetY());
			m_rightDriveMotor->Set(-m_rightJoystick->GetY());
		}*/

	}
	
	if ((m_teleCount % 10) == 0)
	{

	}	
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
