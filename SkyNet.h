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
#include "Controllers/EmptyCell.h"

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
	DigitalInput *m_leftHelixEntryLimit, *m_leftHelixBottomLimit, *m_leftHelixTopLimit;
	DigitalInput *m_rightHelixEntryLimit, *m_rightHelixBottomLimit, *m_rightHelixTopLimit;
	
	/* Turret and Shooter Variables */
	PIDJaguar *m_turretMotor;
	Jaguar *m_shooterMotor;
	Servo *m_turretServoOne, *m_turretServoTwo;
	TrackingTurret *m_trackingTurret;
	bool m_manual, m_shoot;
	float m_turnMotor, m_setDistance;
	
	/* Empty Cell Variables & Constants */
	EmptyCell *m_emptyCellControl;
	Victor *m_leftEmptyCell, *m_rightEmptyCell;
	DigitalInput *m_leftCellBottom, *m_rightCellBottom, *m_leftCellTop, *m_rightCellTop;
	bool m_release;
	static const float kEmptyCellSpeed = .2;
		
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	
	DriverStation *m_ds;
		
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount, m_disableCount;
	
	/* PWM Constants (Relative Modules) */
	static const UINT8 kLeftDriveModule       = 0; //J
	static const UINT8 kRightDriveModule      = 0; //J
	static const UINT8 kShooterModule         = 0; //J
	static const UINT8 kSweeperModule         = 0; //V
	static const UINT8 kLeftHelixModule       = 0; //V
	static const UINT8 kRightHelixModule      = 0; //V
	static const UINT8 kLeftCellHolderModule  = 0; //V
	static const UINT8 kRightCellHolderModule = 0; //V
	static const UINT8 kTurretOneModule       = 0; //S
	static const UINT8 kTurretTwoModule		  = 0; //S
	
	static const UINT8 kLeftDrivePWM       = 1;  //J
	static const UINT8 kRightDrivePWM      = 2;  //J
	static const UINT8 kShooterPWM         = 3;  //J
	static const UINT8 kSweeperPWM         = 4;  //V
	static const UINT8 kLeftHelixPWM       = 5;  //V
	static const UINT8 kRightHelixPWM      = 6;  //V
	static const UINT8 kLeftCellHolderPWM  = 7;  //V
	static const UINT8 kRightCellHolderPWM = 8;  //V
	static const UINT8 kTurretOnePWM       = 9;  //S
	static const UINT8 kTurretTwoPWM	   = 10; //S
	
	/* cRIO Digital IO Constants (Literal Modules)*/
	static const UINT8 kLeftDriveEncoderAModule    = 4;
	static const UINT8 kLeftDriveEncoderBModule    = 4;
	static const UINT8 kRightDriveEncoderAModule   = 4;
	static const UINT8 kRightDriveEncoderBModule   = 4;
	static const UINT8 kLeftCellBottomLimitModule  = 4;
	static const UINT8 kRightCellBottomLimitModule = 4;
	static const UINT8 kLeftCellTopLimitModule     = 4;
	static const UINT8 kRightCellTopLimitModule    = 4;
	static const UINT8 kLeftLiftEntryLimitModule   = 4;
	static const UINT8 kLeftLiftBottomLimitModule  = 4;
	static const UINT8 kLeftLiftTopLimitModule     = 4;
	static const UINT8 kRightLiftEntryLimitModule  = 4;
	static const UINT8 kRightLiftBottomLimitModule = 4;
	static const UINT8 kRightLiftTopLimitModule    = 4;
	
	static const UINT8 kLeftDriveEncoderAChannel    = 1;
	static const UINT8 kLeftDriveEncoderBChannel    = 2;
	static const UINT8 kRightDriveEncoderAChannel   = 3;
	static const UINT8 kRightDriveEncoderBChannel   = 4;
	static const UINT8 kLeftCellBottomLimitChannel  = 5;
	static const UINT8 kRightCellBottomLimitChannel = 6;
	static const UINT8 kLeftCellTopLimitChannel     = 7;
	static const UINT8 kRightCellTopLimitChannel    = 8;
	static const UINT8 kLeftLiftEntryLimitChannel   = 9;
	static const UINT8 kLeftLiftBottomLimitChannel  = 10;
	static const UINT8 kLeftLiftTopLimitChannel     = 11;
	static const UINT8 kRightLiftEntryLimitChannel  = 12;
	static const UINT8 kRightLiftBottomLimitChannel = 13;
	static const UINT8 kRightLiftTopLimitChannel    = 14;
	
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