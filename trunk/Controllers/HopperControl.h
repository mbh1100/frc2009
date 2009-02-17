#ifndef __HOPPERCONTROL_H__
#define __HOPPERCONTROL_H__

#include "WPILib.h"
#include "Shoot.h"

#include "../Devices/AdvMotorController.h"

class HopperControl
{
public:
	HopperControl(AdvMotorController* leftHelixMotor, AdvMotorController* rightHelixMotor, AdvMotorController* sweeperMotor, AdvMotorController* shooterMotorOne, AdvMotorController* shooterMotorTwo);
	virtual ~HopperControl();
	
	void Update(bool manual, float helixSide, float helixDirection, bool limitLeftEntry, bool limitLeftBottom, bool limitLeftTop, 
			bool limitRightEntry, bool limitRightBottom, bool limitRightTop, bool shoot, float distance, float direction);
	void SensorIntake();
	void Disable();
	void SetBallCount(int ballsLeft, int ballsRight);
	int GetBallCountLeft();
	int GetBallCountRight();
	
protected:
	Shoot *m_shoot;
	AdvMotorController *m_shooterMotorOne, *m_sweeperMotor, *m_leftHelixMotor, *m_rightHelixMotor, *m_shooterMotorTwo;
	
	float m_helixSide, m_helixDirection;
	
	bool m_manual;
	bool m_limitLeftEntry, m_limitLeftBottom, m_limitLeftTop;
	bool m_limitRightEntry, m_limitRightBottom, m_limitRightTop;
	bool m_lastLeftEntry, m_lastRightEntry;
	bool m_lastLeftBottom, m_lastRightBottom;
	
	int m_ballsInLeft, m_ballsInRight;
	bool m_leftEntering, m_rightEntering;
	
	bool m_shootPressed, m_justShot;
	float m_direction, m_distance;
	
	Timer *m_leftTimer, *m_rightTimer;
	
	double kMinTimePerEntry;
	double kMaxTimePerEntry;
	float kHelixInSpeed;
	float kHelixOutSpeed;
	float kSweeperSpeedIn;
	float kSweeperSpeedOut;
};

#endif
