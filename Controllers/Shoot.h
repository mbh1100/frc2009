#ifndef __SHOOT_H__
#define __SHOOT_H__

#include "WPILib.h"

#include "../Devices/AdvMotorController.h"

class Shoot
{
public:
	Shoot(AdvMotorController* shootMotorOne, AdvMotorController* shootMotorTwo, AdvMotorController* leftHelixMotor, AdvMotorController* rightHelixMotor);
	virtual ~Shoot();
	
	void Update(float distance, bool limitLeft, bool limitRight);
	void InitialSet(int ballsLeft, int ballsRight, float direction);
	float Distance();
	int CountBallsLeft();
	int CountBallsRight();
	void SideShooting();
	
	void Disable();
	
protected:
	AdvMotorController *m_shootMotorOne, *m_shootMotorTwo, *m_leftHelixMotor, *m_rightHelixMotor;
	
	Timer *m_leftTimer, *m_rightTimer;
	
	float m_direction, m_distance;
	int m_ballsLeft, m_ballsRight;
	
	bool m_limitLeft, m_limitRight;
	bool m_leftPressed, m_rightPressed;
	
	Timer *m_timer;
	
	/* Direction Constants */
	static const int kLeft = 0;
	static const int kRight = 1;
	
	static const double kTimePerShot = .3;
};

#endif
