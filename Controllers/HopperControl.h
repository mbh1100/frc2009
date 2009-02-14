#ifndef __HOPPERCONTROL_H__
#define __HOPPERCONTROL_H__

#include "WPILib.h"
#include "Shoot.h"

class HopperControl
{
public:
	HopperControl(Victor* leftHelixMotor, Victor* rightHelixMotor, Jaguar* sweeperMotor, Jaguar* shooterMotor);
	virtual ~HopperControl();
	
	void Update(int helixSide, int helixDirection, bool limitLeftEntry, bool limitLeftBottom, bool limitLeftTop, 
			bool limitRightEntry, bool limitRightBottom, bool limitRightTop, bool shoot, float distance, float direction);
	void SensorIntake();
	void Disable();
	void SetBallCount(int ballsLeft, int ballsRight);
	int GetBallCountLeft();
	int GetBallCountRight();
	
protected:
	Shoot *m_shoot;
	Jaguar *m_shooterMotor, *m_sweeperMotor;
	Victor *m_leftHelixMotor, *m_rightHelixMotor;
	
	float m_helixSide, m_helixDirection;
	
	bool m_limitLeftEntry, m_limitLeftBottom, m_limitLeftTop;
	bool m_limitRightEntry, m_limitRightBottom, m_limitRightTop;
	
	int m_ballsInLeft, m_ballsInRight;
	bool m_leftEntering, m_rightEntering;
	
	bool m_shootPressed, m_justShot;
	float m_direction, m_distance;
	
	Timer *m_leftTimer, *m_rightTimer;
	
	double kMinTimePerEntry;
	double kMaxTimePerEntry;
	float kHelixInSpeed;
	float kHelixOutSpeed;
	float kSweeperSpeed;
};

#endif
