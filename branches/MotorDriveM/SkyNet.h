#include <iostream.h>
#include "WPILib.h"
#include <taskLib.h>
#include <cmath>

#include "AxisCamera.h" 
#include "BaeUtilities.h"
#include "FrcError.h"
#include "TrackAPI.h" 
#include "PCVideoServer.h"

#include "Controllers/TrackingTurret.h"
#include "HardwareInterface.h"
#include "Devices/PIDEncoder.h"
#include "Devices/PIDJaguar.h"

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
	
	TrackingTurret *m_trackingTurret;
	//Motor pointers
	MotorDrive *m_rightMotor, *m_leftMotor;
	
	
	HardwareInterface *m_hardwareInterface;
	
	Joystick *m_joystick1;
	Joystick *m_joystick2;
	
	PIDEncoder *m_rightEncoder, *m_leftEncoder;
	PIDJaguar *m_rightDriveMotor, *m_leftDriveMotor; 
	
	PIDJaguar *m_turretMotor;
	Servo *m_cameraServo;
	
	
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;	
	
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
};
