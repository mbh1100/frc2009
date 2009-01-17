#include "SkyNet.h"

// Camera tracking target size parameters
#define MIN_PARTICLE_TO_IMAGE_PERCENT 0.25
#define MAX_PARTICLE_TO_IMAGE_PERCENT 10.0

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	m_motor1 = new Jaguar(1);
	m_servo1 = new Servo(4,4);
	
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
	Wait(2.0);
	m_camToDash = new PCVideoServer;

	
	//tdata = GetTrackingData(GREEN, FLUORESCENT);
	sprintf (tdataGreen.name, "GREEN");
	tdataGreen.hue.minValue = 44;
	tdataGreen.hue.maxValue = 136;
	tdataGreen.saturation.minValue = 47;
	tdataGreen.saturation.maxValue = 255;
	tdataGreen.luminance.minValue = 81;
	tdataGreen.luminance.maxValue = 255;
	sprintf (tdataPink.name, "PINK");
	tdataPink.hue.minValue = 220;
	tdataPink.hue.maxValue = 255;
	tdataPink.saturation.minValue = 75;
	tdataPink.saturation.maxValue = 255;
	tdataPink.luminance.minValue = 85;
	tdataPink.luminance.maxValue = 255;	
}
void SkyNet::DisabledInit()
{
	printf("Inititializing Disabled Mode..\r\n");
}
void SkyNet::AutonomousInit()
{
	printf("Inititializing Autonomous Mode..\r\n");
	m_printsPerLoop = 1;
	m_autoCount = 0;
	foundPink = false;
	foundGreen = false;
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
	Dashboard &dashboard = m_ds->GetDashboardPacker();
	
	//Finding the target
	ParticleAnalysisReport parGreen,parPink;
	if (FindColor(IMAQ_HSL, &tdataGreen.hue, &tdataGreen.saturation, &tdataGreen.luminance, &parGreen)
			&& parGreen.particleToImagePercent < MAX_PARTICLE_TO_IMAGE_PERCENT
			&& parGreen.particleToImagePercent > MIN_PARTICLE_TO_IMAGE_PERCENT)
	{
		foundGreen = true;
		greenX = (int)(parGreen.center_mass_x_normalized * 1000.0);
		greenY = (int)(parGreen.center_mass_y_normalized * 1000.0);
		if ((m_autoCount % m_printsPerLoop) == 0)
		{
			dashboard.Printf("Green found at: x: %i y: %i\n", greenX, greenY);
		}
		
	} 
	else
	{
		foundGreen = false;
		dashboard.Printf("No Green Found\n");
	}
	if (FindColor(IMAQ_HSL, &tdataPink.hue, &tdataPink.saturation, &tdataPink.luminance, &parPink)
			&& parPink.particleToImagePercent < MAX_PARTICLE_TO_IMAGE_PERCENT
			&& parPink.particleToImagePercent > MIN_PARTICLE_TO_IMAGE_PERCENT) 
	{
		foundPink = true;
		pinkX = (int)(parPink.center_mass_x_normalized * 1000.0);
		pinkY = (int)(parPink.center_mass_y_normalized * 1000.0);
		if ((m_autoCount % m_printsPerLoop) == 0)
		{
			//printf("Pink found at: x: %i y: %i\n", pinkX, pinkY);
		}
		
	} 
	else if ((m_autoCount % m_printsPerLoop) == 0)
	{
		foundPink = false;
		//printf("No Pink Found\n");
	}
	
	//Tracking Mike with Green
	if (foundGreen && foundPink && (greenY > pinkY))
	{
		if (greenX < -20)
		{
			m_motor1->Set(-.07);
		}
		else if (greenX > 20)
		{
			m_motor1->Set(.07);
			printf("Moving\n");
		}
		else
		{
			m_motor1->Set(0);
		}
	}
	else
	{
		m_motor1->Set(0);
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
		
		m_servo1->Set(.5);
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
