#include "SkyNet.h"

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	int frameRate = 10, compression=0;
	ImageSize resolution = k320x240;
	ImageRotation imageRotation = ROT_0;
	
	if (StartCameraTask(frameRate, compression, resolution, imageRotation) == -1)
	{
		printf("Failed to spawn camera task; Error code %s\r\n",GetVisionErrorText(GetLastVisionError()));
	}
	else
	{
		printf("Camera is a success \r\n");
	}
	
	m_hardwareInterface = new HardwareInterface(true);
	
	m_ds = m_hardwareInterface->GetDriverStation();
	m_priorPacketNumber = 0;
	
	for (UINT8 channel = 1; channel <= SensorBase::kSolenoidChannels; channel++)
	{
		m_solenoids[channel - 1] = m_hardwareInterface->GetSolenoid(channel);
	}
	
	m_analogModules[0] = m_hardwareInterface->GetAnalogModule(0);
	m_analogModules[1] = m_hardwareInterface->GetAnalogModule(1);
	
	m_analogModules[0]->SetAverageBits(1,8);
	m_analogModules[1]->SetAverageBits(1,8);
	
	m_autoCount = 0;
	m_teleCount = 0;
}

void SkyNet::DisabledInit()
{
	printf("Inititializing Disabled Mode..\r\n");
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
}

void SkyNet::DisabledPeriodic()
{
	GetWatchdog().Feed();
}

void SkyNet::AutonomousPeriodic()
{
	GetWatchdog().Feed();
	m_autoCount++;
	
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
		/* Runs at 20Hz */
	}
	
	if ((m_teleCount % 2) == 0)
	{
		/* Runs at 100Hz */
	}
	
	if ((m_teleCount % 4) == 0)
	{
		/* Runs at 50Hz */
		
		m_hardwareInterface->UpdateDashboard(true);
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		/* Code dependent on driverstation/human input here */
		
		m_priorPacketNumber = m_ds->GetPacketNumber();
	}
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
