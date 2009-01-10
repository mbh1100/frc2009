#include "SkyNet.h"

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	m_motor1 = new Jaguar(1);
	
	m_analogModule = AnalogModule::GetInstance(1);
	m_analogModule->SetAverageBits(1,8);
	
	m_ds = DriverStation::GetInstance();
	m_priorPacketNumber = 0;
	m_dsPacketsPerSecond = 0;
	
	m_rightStick = new Joystick(1);
	m_leftStick = new Joystick(2);
	
	m_autoCount = 0;
	m_teleCount = 0;
	
	int frameRate = 10,compression=0,decode=1;
	ImageSize resolution = k320x240;
	ImageRotation imageRotation = ROT_0;
	if (StartCameraTask(frameRate, compression, resolution, imageRotation, decode) == -1)
	{
		printf("Failed to spawn camera task; Error code %s",GetVisionErrorText(GetLastVisionError()));
	}
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
	
}
void SkyNet::TeleopPeriodic()
{
	GetWatchdog().Feed();
	m_teleCount++;
		
	
	if ((m_teleCount % 2) == 0)
	{
		//For 100Hz Stuff
	}
	if ((m_teleCount % 4) == 0)
	{
		//For 50Hz Stuff
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		//Code dependent on driverstation/human input here
	}
	
	if ((m_teleCount % GetLoopsPerSec()) == 0)
	{
		printf("Print one per second: \r\n");
		m_dsPacketsPerSecond = 0;
	}
}

//DONT EVER FORGET THIS
START_ROBOT_CLASS(SkyNet);
