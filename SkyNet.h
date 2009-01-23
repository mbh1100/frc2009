#include <iostream.h>
#include "WPILib.h"
#include <taskLib.h>
#include <cmath>

#include "AxisCamera.h" 
#include "BaeUtilities.h"
#include "FrcError.h"
#include "TrackAPI.h" 
#include "PCVideoServer.h"

#include "TrackingTurret.h"
#include "HardwareInterface.h"
#include "SmartJaguar.h"

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
	
	TrackingCamera *m_trackingCamera;
	TrackingTurret *m_trackingTurret;
	
	HardwareInterface *m_hardwareInterface;
	
	Joystick *m_joystick1;
	Joystick *m_joystick2;
	
	Jaguar *m_turretMotor;
	Servo *m_cameraServo;
	
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;	
	
	//Camera Locating
	bool m_inView;
	float m_positionX,m_positionY;
	
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
};