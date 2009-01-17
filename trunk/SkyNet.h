#include <iostream.h>
#include "WPILib.h"
#include <taskLib.h>
#include "math.h"

#include "AxisCamera.h" 
#include "BaeUtilities.h"
#include "FrcError.h"
#include "TrackAPI.h" 
#include "PCVideoServer.h"
//#include "TrackTarget.h"

#include "DashboardDataFormat.h"

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
	
	void UpdateDashboard();
	
	DashboardDataFormat m_dashboardDataFormatter;
	
	Joystick *m_rightStick,*m_leftStick;
	
	RobotDrive *m_RobotDrive;
	Jaguar *m_motor1,*m_motor2,*m_motor3,*m_motor4;
	
	Solenoid *m_solenoids[8];
	
	AnalogModule *m_analogModules[2];
	
	DriverStation *m_ds;
	UINT32 m_priorPacketNumber;
	UINT8 m_dsPacketsPerSecond;
	
	UINT32 m_autoCount,m_teleCount,m_printsPerLoop;
	
	//Camera Stuff
	TrackingThreshold tdataGreen,tdataPink;
	bool foundPink,foundGreen;
	int greenX, greenY, pinkX, pinkY;
	
	//Dashboard Stuff
	PCVideoServer *m_camToDash;
	
};
