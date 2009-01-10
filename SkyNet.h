#include <iostream.h>
#include "WPILib.h"
#include <taskLib.h>
#include "math.h"

#include "AxisCamera.h" 
#include "BaeUtilities.h"
#include "FrcError.h"
#include "TrackAPI.h" 

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
	
	Joystick *m_rightStick,*m_leftStick;
	
	RobotDrive *m_RobotDrive;
	Jaguar *m_motor1,*m_motor2,*m_motor3,*m_motor4;
	
	AnalogModule *m_analogModule;
	
	DriverStation *m_ds;
	UINT32 m_priorPacketNumber;
	UINT8 m_dsPacketsPerSecond;
	
	UINT32 m_autoCount,m_teleCount;
	
};
