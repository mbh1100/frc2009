#include "SkyNet.h"

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	m_motor1 = new Jaguar(1);
	
	m_ds = DriverStation::GetInstance();
	m_priorPacketNumber = 0;
	m_dsPacketsPerSecond = 0;
	
	m_rightStick = new Joystick(1);
	m_leftStick = new Joystick(2);
	
	for (UINT8 i = 0; i <= (SensorBase::kSolenoidChannels - 1); i++)
	{
		m_solenoids[i] = new Solenoid(8, i - 1);
	}
	
	m_analogModules[0] = AnalogModule::GetInstance(1);
	m_analogModules[1] = AnalogModule::GetInstance(2);
	
	m_analogModules[0]->SetAverageBits(1,8);
	m_analogModules[1]->SetAverageBits(1,8);
	
	m_autoCount = 0;
	m_teleCount = 0;
	
	int frameRate = 30,compression=0;
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
	
	m_dashboardDataFormatter = new DashboardDataFormat();
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
			//For 50Hz Stuff
			
			SkyNet::UpdateDashboard(true);
	}

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
		
		SkyNet::UpdateDashboard(false);
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		//Code dependent on driverstation/human input here
	}
	
	if ((m_teleCount % 200) == 0)
	{
		printf("Print one per second: \r\n");
		m_dsPacketsPerSecond = 0;
	}
}

void SkyNet::UpdateDashboard(bool cameraState)
{
	/* Reading Analog Modules skipping channel 8 for the first slot as it is used for battery */
	
	for (UINT8 i = 0; i <= (SensorBase::kAnalogChannels - 2); i++)
	{
		m_dashboardDataFormatter->m_analogChannels[0][i] = m_analogModules[0]->GetValue(i + 1);
	}
	
	for (UINT8 i = 0; i <= (SensorBase::kAnalogChannels - 1); i++)
	{
		m_dashboardDataFormatter->m_analogChannels[1][i] = m_analogModules[1]->GetValue(i + 1);
	}
	
	/* Reading Solenoid Status */
	
	UINT8 solenoidVals = 0;
	for (int i = (SensorBase::kSolenoidChannels - 1); ; i--)
	{
		solenoidVals += m_solenoids[i]->Get();
		
		if (i != 0)
		{
			solenoidVals <<= 1;
		}
		else
		{
			break;
		}
	}
	
	m_dashboardDataFormatter->m_solenoidChannels = solenoidVals;
	
	/* Sending data to the Dashboard */
	
	m_dashboardDataFormatter->PackAndSend(cameraState);
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
