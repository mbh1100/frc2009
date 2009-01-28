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

#include "Controllers/TankDrive.h"

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
	
	HardwareInterface *m_hardwareInterface;
	
	
	TankDrive *m_leftDrive;
	TankDrive *m_rightDrive;
	Joystick *m_joystick1;
	Joystick *m_joystick2;
	
	PIDEncoder *m_leftDriveEncoder, *m_rightDriveEncoder;
	PIDJaguar *m_rightDriveMotor, *m_leftDriveMotor; 
	
	PIDJaguar *m_turretMotor;
	Servo *m_cameraServo;
	
	
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;	
	
	//Camera Locating
	bool m_inView;
	float m_positionX,m_positionY;
	
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
};
