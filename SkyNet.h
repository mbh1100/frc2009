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
#include "Controllers/HopperControl.h"

/* Motor Allocations
 * 
 * PWM1: Left Drive		-Jaguar
 * PWM2: Right Drive	-Jaguar
 * 
 * PWM3: Turret			-Jaguar
 * PWM4: Shooter		-Jaguar
 * PWM5: Gate			-Victor
 * 
 * PWM5: Sweeper		-Victor
 * PWM6: Left Helix		-Victor
 * PWM7: Right Helix	-Victor
 * 
 * PWM9: Cell Holder	-Victor
 * PWM10: Cell Holder 2	-Victor
 * 
 * Camera Servo Somewhere
 * 
 */

/* Sensor Allocations
 * 
 */

/* Driver Station Allocations
 * USB1: Joystick 1
 * USB2: Joystick 2
 * 
 * Analog 1: Helix Side
 * Analog 2: Helix Direction
 * Analog 3: Turret Angle
 * Analog 4: Turret Distance
 * 
 * Digital 1: Auto/Manual
 * Digital 2: Sweeper
 * Digital 3: Shoot
 * 
 * LED's
 * 
 */

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
	
	/* Sweeper and Hopper Variables */
	HopperControl *m_hopperControl;
	Victor *m_leftHelixMotor, *m_rightHelixMotor, *m_sweeperMotor;
	bool m_sweeperState;
	float m_helixState, m_helixDirection;
	
	/* Turret and Shooter Variables */
	PIDJaguar *m_turretMotor;
	Jaguar *m_shooterMotor;
	Servo *m_cameraServo;
	TrackingTurret *m_trackingTurret;
	bool m_manual, m_shoot;
	float m_turnMotor, m_setDistance;
		
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;
		
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
};
