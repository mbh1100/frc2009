#include <iostream.h>
#include "WPILib.h"
#include <taskLib.h>
#include "math.h"

#include "AxisCamera.h" 
#include "BaeUtilities.h"
#include "FrcError.h"
#include "TrackAPI.h" 

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
	
	HardwareInterface *m_hardwareInterface;
	
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;
	
	Solenoid *m_solenoids[SensorBase::kSolenoidChannels];
	
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
};
