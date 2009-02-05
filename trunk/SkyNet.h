#include <iostream.h>
#include "WPILib.h"
#include <taskLib.h>
#include <cmath>

#include "AxisCamera.h" 
#include "BaeUtilities.h"
#include "FrcError.h"
#include "TrackAPI.h" 
#include "PCVideoServer.h"

#include "Controllers/TankDrive.h"
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
	PIDEncoder *m_leftDriveEncoder, *m_rightDriveEncoder;
	PIDJaguar *m_leftDriveMotor, *m_rightDriveMotor;
	TankDrive *m_drive;
	
	/* Camera Variables MDV adding shooter motor */
	PIDJaguar *m_turretMotor;
	PIDVictor *m_shooterMotor;
	Servo *m_cameraServo;
	TrackingTurret *m_trackingTurret;
	
	/* Temporary */
	PIDEncoder *m_testEncoder, *m_testEncoder2;
	unsigned int m_testSample;
	
	
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;	
	
	/* MDV vars to control toggle DGIO off driver station */
	bool m_manualOverrideTurretShooter;
	
		
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
};
