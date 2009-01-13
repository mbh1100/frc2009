#include "SkyNet.h"

// Camera tracking target size parameters
#define MIN_PARTICLE_TO_IMAGE_PERCENT 0.25
#define MAX_PARTICLE_TO_IMAGE_PERCENT 10.0

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	m_motor1 = new Jaguar(1);
	
	m_ds = DriverStation::GetInstance();
	m_priorPacketNumber = 0;
	m_dsPacketsPerSecond = 0;
	
	m_rightStick = new Joystick(1);
	m_leftStick = new Joystick(2);
	
	for (int i = 0; i <= 7; i++)
	{
		m_solenoids[i] = new Solenoid(8, i - 1);
	}
	
	m_analogModules[0] = AnalogModule::GetInstance(1);
	m_analogModules[1] = AnalogModule::GetInstance(2);
	
	m_analogModules[0]->SetAverageBits(1,8);
	
	
	m_autoCount = 0;
	m_teleCount = 0;
	
	int frameRate = 10,compression=0;
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
	
	tdata = GetTrackingData(GREEN, PASSIVE_LIGHT);
	tdata.hue.minValue = 5;
	tdata.hue.maxValue = 114;
	tdata.saturation.minValue = 5;
	tdata.saturation.maxValue = 255;
	tdata.luminance.minValue = 1;
	tdata.luminance.maxValue = 101;
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
		
	ParticleAnalysisReport par;
	if (FindColor(IMAQ_HSL, &tdata.hue, &tdata.saturation, &tdata.luminance, &par)
			&& par.particleToImagePercent < MAX_PARTICLE_TO_IMAGE_PERCENT
			&& par.particleToImagePercent > MIN_PARTICLE_TO_IMAGE_PERCENT) 
	{
		int lightX = (int)(par.center_mass_x_normalized * 1000.0);
		int lightY = (int)(par.center_mass_y_normalized * 1000.0);
		printf("Light found: x: %i y: %i\n", lightX, lightY);
	} 
	else 
	{
		printf("Light NOT found\n");
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
		
		SkyNet::UpdateDashboard();
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		//Code dependent on driverstation/human input here
	}
	
	if ((m_teleCount % 200) == 0)
	{
		printf("TeleOperated Loop Counter: %d\r\n",m_teleCount);
		m_dsPacketsPerSecond = 0;
	}
	
}

void SkyNet::UpdateDashboard()
{
	for (int i = 0; i <= 6; i++)
	{
		m_dashboardDataFormatter.m_analogChannels[0][i] = m_analogModules[0]->GetValue(i + 1);
	}
	
	for (int i = 0; i <= 7; i++)
	{
		m_dashboardDataFormatter.m_analogChannels[1][i] = m_analogModules[1]->GetValue(i + 1);
	}
	
	UINT8 solenoidVals = 0;
	for (int i = 7; i >= 0; i--)
	{
		solenoidVals += m_solenoids[i]->Get();
		
		if (i != 0)
		{
			solenoidVals <<= 1;
		}
	}
	
	m_dashboardDataFormatter.m_solenoidChannels = solenoidVals;
	
	m_dashboardDataFormatter.PackAndSend();
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
