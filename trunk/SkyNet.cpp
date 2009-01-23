#include "SkyNet.h"

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	m_hardwareInterface = new HardwareInterface(true, true);
	
	m_ds = m_hardwareInterface->GetDriverStation();
	m_priorPacketNumber = 0;
	
	m_analogModules[0] = m_hardwareInterface->GetAnalogModule(0);
	m_analogModules[1] = m_hardwareInterface->GetAnalogModule(1);
	
	m_analogModules[0]->SetAverageBits(1,8);
	m_analogModules[1]->SetAverageBits(1,8);
	

	
	//m_trackingCamera = new TrackingCamera(true);
	//m_trackingTurret = new TrackingTurret();
}

void SkyNet::DisabledInit()
{
	printf("Inititializing Disabled Mode..\r\n");
}

void SkyNet::AutonomousInit()
{
	printf("Inititializing Autonomous Mode..\r\n");
	m_autoCount = 0;
	
	m_inView = false;
	m_positionX = 0;
	m_positionY = 0;
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
	
	//Finding the position of the camera
	if ((m_autoCount % 10) == 0)
	{
		/* Runs at 20Hz */
		
		/*if (m_inView = m_trackingCamera->Update())
		{
			m_positionX = m_trackingCamera->getTargetX();
			m_positionY = m_trackingCamera->getTargetY();
			printf("Target X: %f\n",m_positionX);
			printf("Target Y: %f\n",m_positionY);
		}
		if (m_inView)
		{
			bool foundTarget = m_trackingTurret->aimTurret(m_positionX,m_positionY);
			printf("Found target: %d\n",(int)foundTarget);
		}
		else
		{
			m_trackingTurret->stopTurret();
		}*/
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
		
		m_hardwareInterface->UpdateDashboard(true);
	}
	
	if ((m_teleCount % 2) == 0)
	{
		/* Runs at 100Hz */
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		/* Code dependent on driverstation/human input here */
		
	}
	
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
