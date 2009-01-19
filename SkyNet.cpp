#include "SkyNet.h"

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
	
//	m_analogModules[0] = AnalogModule::GetInstance(1);
//	m_analogModules[1] = AnalogModule::GetInstance(2);
	
//	m_analogModules[0]->SetAverageBits(1,8);
	
	
	m_autoCount = 0;
	m_teleCount = 0;
	
	m_trackingCamera = new TrackingCamera(true);
	Wait(2.0);
	m_camToDash = new PCVideoServer;
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
	
	//Tracking Mike with Green
	if ((m_autoCount % 20) == 0)
	{
		if (m_trackingCamera->Update())
		{
			float x = m_trackingCamera->getTargetX();
			float y = m_trackingCamera->getTargetY();
			printf("Target X: %f\n",x);
			printf("Target Y: %f\n",y);
		}
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
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		//Code dependent on driverstation/human input here
		
	}
	
	if ((m_teleCount % 200) == 0)
	{
		printf("TeleOperated Loop Counter: %d\r\n",m_teleCount);
	}
	
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
