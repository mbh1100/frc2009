#include <iostream.h>
#include "WPILib.h"
#include <taskLib.h>
#include "math.h"

#include "AxisCamera.h" 
#include "BaeUtilities.h"
#include "FrcError.h"
#include "TrackAPI.h" 

#include "HardwareInterface.h"

class SkyNet : public IterativeRobot
{
public:
	SkyNet();
	void DisabledInit();
	void AutonomousInit();
	void TeleopInit();
	void DisabledPeriodic();
	void AutonomousPeriodic();
	void TeleopPeriodic();
	
	void UpdateDashboard(bool cameraState);
	
	HardwareInterface *m_hardwareInterface;
	
	Joystick *m_rightStick,*m_leftStick;
	
	RobotDrive *m_robotDrive;
	
	Jaguar *m_motor1,*m_motor2,*m_motor3,*m_motor4;
	
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;
	
	Solenoid *m_solenoids[SensorBase::kSolenoidChannels];
	
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
	UINT8 m_dsPacketsPerSecond;
};
