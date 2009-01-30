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
		
	HardwareInterface *m_hardwareInterface;
	
	/* Joysticks and other Controls */
	Joystick *m_joystick1;
	Joystick *m_joystick2;
	
	/* Drive Variables */
	//PIDEncoder *m_encoderLeft, *m_encoderRight;
	//PIDController *m_calcLeftDrive, *m_calcRightDrive;
	//float m_driveP, m_driveI, m_driveD;
	//PIDJaguar *m_rightDriveMotor, *m_leftDriveMotor; 
	
	/* Camera Variables */
	PIDJaguar *m_turretMotor;
	Servo *m_cameraServo;
	TrackingTurret *m_trackingTurret;
	
	
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;	
		
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
};
