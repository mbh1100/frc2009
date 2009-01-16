#include "SkyNet.h"

SkyNet::SkyNet()
{
	printf("Initializing..\r\n");
	
	/* PWM assignments for slot 4 */
	m_pwms[0][0] = new Jaguar(4, 1);
	m_pwms[0][1] = new PWM(4, 2);
	m_pwms[0][2] = new PWM(4, 3);
	m_pwms[0][3] = new PWM(4, 4);
	m_pwms[0][4] = new PWM(4, 5);
	m_pwms[0][5] = new PWM(4, 6);
	m_pwms[0][6] = new PWM(4, 7);
	m_pwms[0][7] = new PWM(4, 8);
	m_pwms[0][8] = new PWM(4, 9);
	m_pwms[0][9] = new PWM(4, 10);
	
	/* PWM assingments for slot 6 */
	m_pwms[1][0] = new PWM(6, 1);
	m_pwms[1][1] = new PWM(6, 2);
	m_pwms[1][2] = new PWM(6, 3);
	m_pwms[1][3] = new PWM(6, 4);
	m_pwms[1][4] = new PWM(6, 5);
	m_pwms[1][5] = new PWM(6, 6);
	m_pwms[1][6] = new PWM(6, 7);
	m_pwms[1][7] = new PWM(6, 8);
	m_pwms[1][8] = new PWM(6, 9);
	m_pwms[1][9] = new PWM(6, 10);

	/* Motor assignments */
	m_motor1 = (Jaguar*)m_pwms[0][0];
	
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
	
	m_dashboardDataFormatter = new HardwareInterface(true);
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
			
			/* SkyNet::UpdateDashboard(true); */
			m_dashboardDataFormatter->UpdateDashboard(false);
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
		
		/* UpdateDashboard(false); */
		m_dashboardDataFormatter->UpdateDashboard(false);
	}
	
	if (m_ds->GetPacketNumber() != m_priorPacketNumber)
	{
		//Code dependent on driverstation/human input here
	}
	
	if ((m_teleCount % 200) == 0)
	{
		/*printf("Motor Types... %s %s %s \r\n", typeid(*m_motor1).name(), typeid(*m_pwmTest1).name(), typeid(*m_pwmTest2).name());
		printf("%d \r\n", (int)(typeid(*m_pwmTest1) == typeid(PWM)));*/
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
	
	/* Reading PWM Status */
	
	for (UINT8 i = 0; i <= 1; i++)
	{
		for (UINT8 j = 0; j <= 9; j++)
		{
			m_dashboardDataFormatter->m_pwmChannels[i][j] = m_pwms[i][j]->GetRaw();
		}
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
	
	/* m_dashboardDataFormatter->UpdateDashboard(cameraState); */
}

//DONT EVER FORGET THIS!
START_ROBOT_CLASS(SkyNet);
