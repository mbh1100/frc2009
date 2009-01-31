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
	m_turretMotor = m_hardwareInterface->GetPIDJaguar(0, 7);
	m_cameraServo = m_hardwareInterface->GetServo(0, 8);
	m_joystick1 = m_hardwareInterface->GetJoystick(1);
	m_joystick2 = m_hardwareInterface->GetJoystick(2);
	
	m_turretMotor->EnableDeadbandElimination(true);
	m_turretMotor->SetBounds(255, 136, 128, 120, 0);
	m_trackingTurret = new TrackingTurret(m_turretMotor, m_cameraServo, m_joystick1, m_joystick2);
	
	/* Initialize drive motors and controls */	
	m_leftDriveMotor = m_hardwareInterface->GetPIDJaguar(0, 5);
	m_leftDriveMotor->EnableDeadbandElimination(true);
	m_leftDriveMotor->SetBounds(255, 136, 128, 120, 0);
	
	m_rightDriveMotor = m_hardwareInterface->GetPIDJaguar(0, 4);
	m_rightDriveMotor->EnableDeadbandElimination(true);
	m_rightDriveMotor->SetBounds(255, 136, 128, 120, 0);	
	
	m_leftDriveEncoder = new PIDEncoder(4, 3, 4, 4);
	m_leftDriveEncoder->SetDistancePerTick(-0.001);
	m_leftDriveEncoder->SetType(PIDEncoder::kVelocity);
	//m_leftDriveEncoder->SetSampleSize(1);
	m_leftDriveEncoder->Start();
	
	m_rightDriveEncoder = new PIDEncoder(4, 1, 4, 2);
	m_rightDriveEncoder->SetDistancePerTick(-0.001);
	m_rightDriveEncoder->SetType(PIDEncoder::kVelocity);
	//m_rightDriveEncoder->SetSampleSize(1);
	m_rightDriveEncoder->Start();
	
	m_testEncoder2 = new PIDEncoder(4, 1, 4, 2);
	m_testEncoder2->SetDistancePerTick(-0.001);
	m_testEncoder2->SetType(PIDEncoder::kVelocity);
	//m_testEncoder2->SetSampleSize(1);
	m_testEncoder2->Start();
	
	m_testEncoder = new PIDEncoder(4, 1, 4, 2);
	m_testEncoder->SetDistancePerTick(-0.001);
	m_testEncoder->SetType(PIDEncoder::kAcceleration);
	//m_testEncoder->SetSampleSize(1);
	m_testEncoder->Start();
	
	m_drive = new TankDrive(m_leftDriveMotor, m_rightDriveMotor, m_leftDriveEncoder, m_rightDriveEncoder);
	
	m_testSample = 2;
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
		
		Dashboard &dashboard = m_ds->GetDashboardPacker();
		dashboard.Printf("Accel : %f , Sample Size : %u \r\n", m_testEncoder->PIDGet(), m_testSample);
		dashboard.Printf("Velo : %f \r\n", m_testEncoder2->PIDGet());
		
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
		
		/* Tank Drive */
		m_drive->SetSetpoint(m_joystick1->GetY()*-1.5, m_joystick2->GetY()*-7.2);
		m_drive->Update();
		
		/* Temporary */
		if (m_joystick2->GetRawButton(5))
		{
			m_testSample++;
			m_testEncoder->SetSampleSize(m_testSample);
		}
		if (m_joystick2->GetRawButton(4))
		{
			m_testSample--;
			m_testEncoder->SetSampleSize(m_testSample);
		}
		
	}
	
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
