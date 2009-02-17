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
#include "Devices/AdvJaguar.h"
#include "Devices/AdvVictor.h"
#include "Devices/AdvServo.h"
#include "Devices/AdvMotorController.h"
#include "Controllers/HopperControl.h"
#include "Controllers/EmptyCell.h"

typedef struct
{
	float x, y;
	bool buttons[12];
}JoystickData;

typedef struct
{
	bool manual, shoot, cellRelease;
	float helixSide, helixDirection, manualTurretSpeed, manualTurretAngle;
}ButtonBoxData;

typedef struct
{
	bool helixLeftLower,  helixLeftMid,  helixLeftUpper;
	bool helixRightLower, helixRightMid, helixRightUpper;
	bool cellLeftBottom,  cellLeftTop;
	bool cellRightBottom, cellRightTop;
}LimitSwitchData;

typedef struct
{
	JoystickData rightJoystick, leftJoystick;
	JoystickData rightTestJoystick, leftTestJoystick;
	
	ButtonBoxData buttonBox;
	
	LimitSwitchData limitSwitches;
}IOData;

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
	
	IOData m_ioData;
	DriverStation *m_ds;
	
	/* Joysticks */
	Joystick *m_leftJoystick, *m_rightJoystick;
	Joystick *m_rightTestJoystick, *m_leftTestJoystick;
	
	/* Drive Variables */
	AdvMotorController /*J*/*m_leftDriveMotor, /*J*/*m_rightDriveMotor;
	PIDEncoder *m_leftDriveEncoder, *m_rightDriveEncoder;
	
	/* Sweeper and Hopper Variables */
	HopperControl *m_hopperControl;
	AdvMotorController /*V*/*m_leftHelixMotor, /*V*/*m_rightHelixMotor, /*J*/*m_sweeperMotor;
	
	/* Turret and Shooter Variables */
	AdvMotorController /*J*/*m_shooterMotorOne, /*V*/*m_shooterMotorTwo;
	AdvMotorController /*S*/*m_turretServoOne, /*S*/*m_turretServoTwo;
	TrackingTurret *m_trackingTurret;
	
	/* Empty Cell Variables & Constants */
	EmptyCell *m_emptyCellControl;
	AdvMotorController /*V*/*m_leftEmptyCell, /*V*/*m_rightEmptyCell;
		
	AnalogModule *m_analogModules[SensorBase::kAnalogModules];
	DigitalModule *m_digitalModules[SensorBase::kDigitalModules];
		
	UINT32 m_priorPacketNumber, m_autoCount, m_teleCount, m_disableCount;
	
	/* Autonomous Timer */
	Timer *m_autoTimer;
	
	/* PWM Constants (Relative Modules) */
	static const UINT8 kLeftDriveModule       = 0; //J
	static const UINT8 kRightDriveModule      = 1; //J
	static const UINT8 kShooterOneModule      = 1; //J
	static const UINT8 kShooterTwoModule	  = 1; //V
	static const UINT8 kSweeperModule         = 0; //J
	static const UINT8 kLeftHelixModule       = 0; //V
	static const UINT8 kRightHelixModule      = 1; //V
	static const UINT8 kLeftCellHolderModule  = 0; //V
	static const UINT8 kRightCellHolderModule = 1; //V
	static const UINT8 kTurretOneModule       = 0; //S
	static const UINT8 kTurretTwoModule		  = 0; //S
	
	static const UINT8 kLeftDrivePWM       = 1;  //J
	static const UINT8 kRightDrivePWM      = 1;  //J
	static const UINT8 kShooterOnePWM      = 3;  //J
	static const UINT8 kShooterTwoPWM	   = 6;  //V
	static const UINT8 kSweeperPWM         = 3;  //J
	static const UINT8 kLeftHelixPWM       = 2;  //V
	static const UINT8 kRightHelixPWM      = 2;  //V
	static const UINT8 kLeftCellHolderPWM  = 4;  //V
	static const UINT8 kRightCellHolderPWM = 4;  //V
	static const UINT8 kTurretOnePWM       = 9;  //S
	static const UINT8 kTurretTwoPWM	   = 10; //S
	
	
	/* cRIO Digital IO Constants (Literal Modules) */
	static const UINT8 kLeftDriveEncoderAModule    = 4;
	static const UINT8 kLeftDriveEncoderBModule    = 4;
	static const UINT8 kRightDriveEncoderAModule   = 6;
	static const UINT8 kRightDriveEncoderBModule   = 6;
	
	/* cRIO Digital IO Constants (Relative Modules)*/
	static const UINT8 kLeftCellBottomLimitModule  = 0;
	static const UINT8 kRightCellBottomLimitModule = 1;
	static const UINT8 kLeftCellTopLimitModule     = 0;
	static const UINT8 kRightCellTopLimitModule    = 1;
	static const UINT8 kLeftHelixLowerLimitModule  = 0;
	static const UINT8 kLeftHelixMidLimitModule    = 0; 
	static const UINT8 kLeftHelixUpperLimitModule  = 0;
	static const UINT8 kRightHelixLowerLimitModule = 1;
	static const UINT8 kRightHelixMidLimitModule   = 1;
	static const UINT8 kRightHelixUpperLimitModule   = 1;
	
	static const UINT8 kLeftDriveEncoderAChannel    = 1;
	static const UINT8 kLeftDriveEncoderBChannel    = 2;
	static const UINT8 kRightDriveEncoderAChannel   = 1;
	static const UINT8 kRightDriveEncoderBChannel   = 2;
	static const UINT8 kLeftCellBottomLimitChannel  = 14;
	static const UINT8 kRightCellBottomLimitChannel = 14;
	static const UINT8 kLeftCellTopLimitChannel     = 13;
	static const UINT8 kRightCellTopLimitChannel    = 13;
	static const UINT8 kLeftHelixLowerLimitChannel  = 5;
	static const UINT8 kLeftHelixMidLimitChannel    = 6;
	static const UINT8 kLeftHelixUpperLimitChannel  = 7;
	static const UINT8 kRightHelixLowerLimitChannel = 5;
	static const UINT8 kRightHelixMidLimitChannel   = 6;
	static const UINT8 kRightHelixUpperLimitChannel = 7;
	
	/* cRIO Analog In Constants */
	
	/* Joystick Constants */
	static const UINT8 kLeftJoystickPort  = 1;
	static const UINT8 kRightJoystickPort = 2;
	
	/* Driver Station Digital In Constants */
	static const UINT8 kShootButton	   = 1;
	static const UINT8 kCellSwitch     = 2;
	static const UINT8 kManualSwitch = 3;
	
	/* Driver Station Analog In Constants */
	static const UINT8 kHelixSideSwitch        = 1;
	static const UINT8 kHelixDirectionSwitch   = 2;
	static const UINT8 kTurretAngleSwitch      = 3;
	static const UINT8 kShootingDistanceSwitch = 4;
	
	/* Driver Station Digital Out Constants */
	static const UINT8 kTargetAquiredLED = 1;
	
	/* Diag */
	static const UINT8 kDiagnostic = 0;
};
