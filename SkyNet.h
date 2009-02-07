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
	
protected:
	HardwareInterface *m_hardwareInterface;
	
	/* Joysticks and other Controls */
	Joystick *m_leftJoystick;
	Joystick *m_rightJoystick;
	
	/* Drive Variables */
	PIDEncoder *m_leftDriveEncoder, *m_rightDriveEncoder;
	PIDJaguar *m_leftDriveMotor, *m_rightDriveMotor;
	TankDrive *m_drive;
	
	/* Sweeper and Hopper Variables */
	HopperControl *m_hopperControl;
	Victor *m_leftHelixMotor, *m_rightHelixMotor, *m_sweeperMotor;
	int m_helixSide, m_helixDirection;
	
	/* Turret and Shooter Variables */
	PIDJaguar *m_turretMotor;
	Jaguar *m_shooterMotor;
	Servo *m_cameraServo, *m_emptyCellServo;
	TrackingTurret *m_trackingTurret;
	bool m_manual, m_shoot;
	float m_turnMotor, m_setDistance;
		
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;
		
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount;
	
	/* PWM Constants (Relative Modules) */
	static const UINT8 kLeftDriveModule       = 0; //J
	static const UINT8 kRightDriveModule      = 0; //J
	static const UINT8 kTurretModule          = 0; //J
	static const UINT8 kShooterModule         = 0; //J
	static const UINT8 kGateModule            = 0; //V
	static const UINT8 kSweeperModule         = 0; //V
	static const UINT8 kLeftHelixModule       = 0; //V
	static const UINT8 kRightHelixModule      = 0; //V
	static const UINT8 kLeftCellHolderModule  = 0; //S
	static const UINT8 kRightCellHolderModule = 0; //S
	static const UINT8 kCameraModule          = 1; //S
	
	static const UINT8 kLeftDrivePWM       = 1; //J
	static const UINT8 kRightDrivePWM      = 2; //J
	static const UINT8 kTurretPWM          = 3; //J
	static const UINT8 kShooterPWM         = 4; //J
	static const UINT8 kGatePWM            = 5; //V
	static const UINT8 kSweeperPWM         = 6; //V
	static const UINT8 kLeftHelixPWM       = 7; //V
	static const UINT8 kRightHelixPWM      = 8; //V
	static const UINT8 kLeftCellHolderPWM  = 9; //S
	static const UINT8 kRightCellHolderPWM = 10;//S
	static const UINT8 kCameraPWM          = 1; //S
	
	/* cRIO Digital IO Constants (Literal Modules)*/
	static const UINT8 kLeftTurretLimitModule    = 4;
	static const UINT8 kRightTurretLimitModule   = 4;
	static const UINT8 kLeftDriveEncoderAModule  = 4;
	static const UINT8 kLeftDriveEncoderBModule  = 4;
	static const UINT8 kRightDriveEncoderAModule = 4;
	static const UINT8 kRightDriveEncoderBModule = 4;
	
	static const UINT8 kLeftTurretLimitChannel    = 1;
	static const UINT8 kRightTurretLimitChannel   = 2;
	static const UINT8 kLeftDriveEncoderAChannel  = 3;
	static const UINT8 kLeftDriveEncoderBChannel  = 4;
	static const UINT8 kRightDriveEncoderAChannel = 5;
	static const UINT8 kRightDriveEncoderBChannel = 6;
	
	/* cRIO Analog In Constants */
	
	/* Joystick Constants */
	static const UINT8 kRightJoystickPort = 1;
	static const UINT8 kLeftJoystickPort  = 2;
	
	/* Driver Station Digital In Constants */
	static const UINT8 kOverrideSwitch = 1;
	static const UINT8 kCellSwitch     = 2;
	
	/* Driver Station Analog In Constants */
	static const UINT8 kHelixSideSwitch        = 1;
	static const UINT8 kHelixDirectionSwitch   = 2;
	static const UINT8 kTurretAngleSwitch      = 3;
	static const UINT8 kShootingDistanceSwitch = 4;
	
	/* Driver Station Digital Out Constants */
	static const UINT8 kTargetAquiredLED = 1;
};
