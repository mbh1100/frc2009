#include "SkyNet.h"

SkyNet::SkyNet()
{
     printf("Initializing..\r\n");
	
	/* Initialize Hardware Interface & Driver station*/
	m_hardwareInterface = new HardwareInterface(true, true);
	
	m_ds = m_hardwareInterface->GetDriverStation();
	m_priorPacketNumber = 0;
		
	m_leftJoystick = m_hardwareInterface->GetJoystick(kLeftJoystickPort);
	m_rightJoystick = m_hardwareInterface->GetJoystick(kRightJoystickPort);
	m_leftTestJoystick = m_hardwareInterface->GetJoystick(3);
	m_rightTestJoystick = m_hardwareInterface->GetJoystick(4);
	
	/* Initialize drive motors */	
	m_leftDriveMotor = m_hardwareInterface->GetAdvJaguar(kLeftDriveModule, kLeftDrivePWM);
	m_leftDriveMotor->SetDeadband(.05, -.05, 0.0);
	
	m_rightDriveMotor = m_hardwareInterface->GetAdvJaguar(kRightDriveModule, kRightDrivePWM);
	m_rightDriveMotor->SetDeadband(.05, -.05, 0.0);
	m_rightDriveMotor->SetDirection(kMotorReverse);
	
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
	
	/* Initialize Hopper */
	m_hopperControl = new HopperControl(m_leftHelixMotor, m_rightHelixMotor, m_sweeperMotor, m_shooterMotorOne, m_shooterMotorTwo);
	
	m_leftHelixMotor = m_hardwareInterface->GetAdvVictor(kLeftHelixModule, kLeftHelixPWM);
	m_rightHelixMotor = m_hardwareInterface->GetAdvVictor(kRightHelixModule, kRightHelixPWM);
	m_sweeperMotor = m_hardwareInterface->GetAdvJaguar(kSweeperModule,kSweeperPWM);
	
	/* Initialize Shooter and Turret */
	m_turretServoOne = m_hardwareInterface->GetAdvServo(kTurretOneModule, kTurretOnePWM);
	m_turretServoOne->SetBounds(.8, .2);
		
	m_turretServoTwo = m_hardwareInterface->GetAdvServo(kTurretTwoModule, kTurretTwoPWM);
	m_turretServoTwo->SetBounds(.8, .2);
	m_turretServoTwo->SetDirection(kMotorReverse);

	m_shooterMotorOne = m_hardwareInterface->GetAdvJaguar(kShooterOneModule, kShooterOnePWM);
	m_shooterMotorTwo = m_hardwareInterface->GetAdvVictor(kShooterTwoModule, kShooterTwoPWM);
	
	m_trackingTurret = new TrackingTurret((AdvServo*)m_turretServoOne, (AdvServo*)m_turretServoTwo);
	
	/* Empty Cell */
	m_leftEmptyCell = m_hardwareInterface->GetAdvVictor(kLeftCellHolderModule, kLeftCellHolderPWM);
	m_rightEmptyCell = m_hardwareInterface->GetAdvVictor(kRightCellHolderModule, kRightCellHolderPWM);
	
	m_emptyCellControl = new EmptyCell(m_leftEmptyCell, m_rightEmptyCell);
	
	/* Initialize Analog Modules */
	m_analogModules[0] = m_hardwareInterface->GetAnalogModule(0);
	m_analogModules[1] = m_hardwareInterface->GetAnalogModule(1);
	
	m_analogModules[0]->SetAverageBits(1,8);
	m_analogModules[1]->SetAverageBits(1,8);
	
	/* Initialize Digital Modules */
	m_digitalModules[0] = m_hardwareInterface->GetDigitalModule(0);
	m_digitalModules[1] = m_hardwareInterface->GetDigitalModule(1);
	
	/* Initialize Autonomous Timer */
	m_autoTimer = new Timer();
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
}

void SkyNet::TeleopInit()
{
	printf("Inititializing Teleop Mode..\r\n");
	m_teleCount = 0;
	
	m_ioData.rightJoystick.x = m_rightJoystick->GetX();
	m_ioData.rightJoystick.y = m_rightJoystick->GetY();
	for (int i = 1; i <= 12; i++) m_ioData.rightJoystick.buttons[i] = m_rightJoystick->GetRawButton(i);
	
	m_ioData.leftJoystick.x = m_leftJoystick->GetX();
	m_ioData.leftJoystick.y = m_leftJoystick->GetY();
	for (int i = 1; i <= 12; i++) m_ioData.leftJoystick.buttons[i] = m_leftJoystick->GetRawButton(i);
	
	m_ioData.rightTestJoystick.x = m_rightTestJoystick->GetX();
	m_ioData.rightTestJoystick.y = m_rightTestJoystick->GetY();
	for (int i = 1; i <= 12; i++) m_ioData.rightTestJoystick.buttons[i] = m_rightTestJoystick->GetRawButton(i);
			
	m_ioData.leftTestJoystick.x = m_leftTestJoystick->GetX();
	m_ioData.leftTestJoystick.y = m_leftTestJoystick->GetY();
	for (int i = 1; i <= 12; i++) m_ioData.leftTestJoystick.buttons[i] = m_leftTestJoystick->GetRawButton(i);
	
	m_ioData.limitSwitches.helixLeftLower = !m_digitalModules[kLeftHelixLowerLimitModule]->GetDIO(kLeftHelixLowerLimitChannel);
	m_ioData.limitSwitches.helixLeftMid = !m_digitalModules[kLeftHelixMidLimitModule]->GetDIO(kLeftHelixMidLimitChannel);
	m_ioData.limitSwitches.helixLeftUpper = !m_digitalModules[kLeftHelixUpperLimitModule]->GetDIO(kLeftHelixUpperLimitChannel);
	
	m_ioData.limitSwitches.helixRightLower = !m_digitalModules[kRightHelixLowerLimitModule]->GetDIO(kRightHelixLowerLimitChannel);
	m_ioData.limitSwitches.helixRightMid = !m_digitalModules[kRightHelixMidLimitModule]->GetDIO(kRightHelixMidLimitChannel);
	m_ioData.limitSwitches.helixRightUpper = !m_digitalModules[kRightHelixUpperLimitModule]->GetDIO(kRightHelixUpperLimitChannel);
	
	m_ioData.limitSwitches.cellLeftBottom = !m_digitalModules[kLeftCellBottomLimitModule]->GetDIO(kLeftCellBottomLimitChannel);
	m_ioData.limitSwitches.cellLeftTop = !m_digitalModules[kLeftCellTopLimitModule]->GetDIO(kLeftCellTopLimitChannel);
	
	m_ioData.limitSwitches.cellRightBottom = !m_digitalModules[kRightCellBottomLimitModule]->GetDIO(kRightCellBottomLimitChannel);
	m_ioData.limitSwitches.cellRightTop = !m_digitalModules[kRightCellTopLimitModule]->GetDIO(kRightCellTopLimitChannel);
	
	m_ioData.buttonBox.helixSide = m_ds->GetAnalogIn(kHelixSideSwitch);
	m_ioData.buttonBox.helixDirection = m_ds->GetAnalogIn(kHelixDirectionSwitch);
	m_ioData.buttonBox.manualTurretAngle = m_ds->GetAnalogIn(kTurretAngleSwitch);
	m_ioData.buttonBox.manualTurretSpeed = m_ds->GetAnalogIn(kShootingDistanceSwitch);
	
	m_ioData.buttonBox.manual = !m_ds->GetDigitalIn(kManualSwitch);
	m_ioData.buttonBox.shoot = !m_ds->GetDigitalIn(kShootButton);
	m_ioData.buttonBox.cellRelease = !m_ds->GetDigitalIn(kCellSwitch);
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
	
	if (m_autoCount < 3)
	{
		m_autoTimer->Reset();
		m_autoTimer->Start();
	}
	if (m_autoTimer->Get() < 2)
	{
		m_leftDriveMotor->Set(.4);
		m_rightDriveMotor->Set(-.4);
	}
	else if (m_autoTimer->Get() < 6)
	{
		m_leftDriveMotor->Set(.4);
		m_rightDriveMotor->Set(.4);
	}
	else if (m_autoTimer->Get() < 10)
	{
		m_leftDriveMotor->Set(-.4);
		m_rightDriveMotor->Set(-.4);
	}
	else if (m_autoTimer->Get() < 12)
	{
		m_leftDriveMotor->Set(-.4);
		m_rightDriveMotor->Set(.4);
	}
	else
	{
		m_leftDriveMotor->Set(0.0);
		m_rightDriveMotor->Set(0.0);
	}
	
	/* Finding & tracking the target with the camera */
	if ((m_autoCount % 10) == 0)
	{
		
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
	
	if ((m_teleCount % 1) == 0)
	{
		m_rightDriveMotor->Set(m_ioData.rightJoystick.y);
		m_leftDriveMotor->Set(m_ioData.leftJoystick.y);
	}
	
	if ((m_teleCount % 2) == 0)
	{
		/* Runs at 100Hz */
		
		/* Hopper, Sweeper, and Shooter Control */
		m_hopperControl->Update(m_ioData.buttonBox.manual, m_ioData.buttonBox.helixSide, 
				m_ioData.buttonBox.helixDirection, m_ioData.limitSwitches.helixLeftLower, 
				m_ioData.limitSwitches.helixLeftMid, m_ioData.limitSwitches.helixLeftUpper,
				m_ioData.limitSwitches.helixRightLower, m_ioData.limitSwitches.helixRightMid,
				m_ioData.limitSwitches.helixRightUpper, m_ioData.buttonBox.shoot, 
				m_ioData.buttonBox.manualTurretSpeed, m_ioData.buttonBox.manualTurretAngle);
				
				
		/* Empty Cell Control */
		m_emptyCellControl->Update(m_ioData.buttonBox.cellRelease, 
				m_ioData.limitSwitches.cellLeftBottom, m_ioData.limitSwitches.cellLeftTop, 
				m_ioData.limitSwitches.cellRightBottom, m_ioData.limitSwitches.cellRightTop);
	}
	
	if ((m_teleCount % 4) == 0)
	{
		/* Runs at 50Hz */
		
		if (m_ioData.buttonBox.manual)
		{
			if (m_trackingTurret->Update(m_ioData.buttonBox.manualTurretAngle))
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
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		m_priorPacketNumber = m_ds->GetPacketNumber();
		
		m_ioData.rightJoystick.x = m_rightJoystick->GetX();
		m_ioData.rightJoystick.y = m_rightJoystick->GetY();
		for (int i = 1; i <= 12; i++) m_ioData.rightJoystick.buttons[i] = m_rightJoystick->GetRawButton(i);
		
		m_ioData.leftJoystick.x = m_leftJoystick->GetX();
		m_ioData.leftJoystick.y = m_leftJoystick->GetY();
		for (int i = 1; i <= 12; i++) m_ioData.leftJoystick.buttons[i] = m_leftJoystick->GetRawButton(i);
		
		m_ioData.rightTestJoystick.x = m_rightTestJoystick->GetX();
		m_ioData.rightTestJoystick.y = m_rightTestJoystick->GetY();
		for (int i = 1; i <= 12; i++) m_ioData.rightTestJoystick.buttons[i] = m_rightTestJoystick->GetRawButton(i);
				
		m_ioData.leftTestJoystick.x = m_leftTestJoystick->GetX();
		m_ioData.leftTestJoystick.y = m_leftTestJoystick->GetY();
		for (int i = 1; i <= 12; i++) m_ioData.leftTestJoystick.buttons[i] = m_leftTestJoystick->GetRawButton(i);
		
		m_ioData.limitSwitches.helixLeftLower = !m_digitalModules[kLeftHelixLowerLimitModule]->GetDIO(kLeftHelixLowerLimitChannel);
		m_ioData.limitSwitches.helixLeftMid = !m_digitalModules[kLeftHelixMidLimitModule]->GetDIO(kLeftHelixMidLimitChannel);
		m_ioData.limitSwitches.helixLeftUpper = !m_digitalModules[kLeftHelixUpperLimitModule]->GetDIO(kLeftHelixUpperLimitChannel);
		
		m_ioData.limitSwitches.helixRightLower = !m_digitalModules[kRightHelixLowerLimitModule]->GetDIO(kRightHelixLowerLimitChannel);
		m_ioData.limitSwitches.helixRightMid = !m_digitalModules[kRightHelixMidLimitModule]->GetDIO(kRightHelixMidLimitChannel);
		m_ioData.limitSwitches.helixRightUpper = !m_digitalModules[kRightHelixUpperLimitModule]->GetDIO(kRightHelixUpperLimitChannel);
		
		m_ioData.limitSwitches.cellLeftBottom = !m_digitalModules[kLeftCellBottomLimitModule]->GetDIO(kLeftCellBottomLimitChannel);
		m_ioData.limitSwitches.cellLeftTop = !m_digitalModules[kLeftCellTopLimitModule]->GetDIO(kLeftCellTopLimitChannel);
		
		m_ioData.limitSwitches.cellRightBottom = !m_digitalModules[kRightCellBottomLimitModule]->GetDIO(kRightCellBottomLimitChannel);
		m_ioData.limitSwitches.cellRightTop = !m_digitalModules[kRightCellTopLimitModule]->GetDIO(kRightCellTopLimitChannel);
		
		m_ioData.buttonBox.helixSide = m_ds->GetAnalogIn(kHelixSideSwitch);
		m_ioData.buttonBox.helixDirection = m_ds->GetAnalogIn(kHelixDirectionSwitch);
		m_ioData.buttonBox.manualTurretAngle = m_ds->GetAnalogIn(kTurretAngleSwitch);
		m_ioData.buttonBox.manualTurretSpeed = m_ds->GetAnalogIn(kShootingDistanceSwitch);
		
		m_ioData.buttonBox.manual = !m_ds->GetDigitalIn(kManualSwitch);
		m_ioData.buttonBox.shoot = !m_ds->GetDigitalIn(kShootButton);
		m_ioData.buttonBox.cellRelease = !m_ds->GetDigitalIn(kCellSwitch);
		
		m_hardwareInterface->UpdateDashboard(true);
	}
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
