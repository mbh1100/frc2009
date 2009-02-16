#ifndef __SHOOT_H__
#define __SHOOT_H__

#include "WPILib.h"

class Shoot
{
public:
	Shoot(Jaguar* shootMotorOne, Victor* shootMotorTwo, Victor* leftHelixMotor, Victor* rightHelixMotor);
	virtual ~Shoot();
	
	void Update(float distance, bool limitLeft, bool limitRight);
	void InitialSet(int ballsLeft, int ballsRight, float direction);
	float Distance();
	int CountBallsLeft();
	int CountBallsRight();
	void SideShooting();
	
	void Disable();
	
protected:
	Jaguar *m_shootMotorOne;
	Victor *m_shootMotorTwo, *m_leftHelixMotor, *m_rightHelixMotor;
	
	Timer *m_leftTimer, *m_rightTimer;
	
	float m_direction, m_distance;
	int m_ballsLeft, m_ballsRight;
	
	bool m_limitLeft, m_limitRight;
	bool m_leftPressed, m_rightPressed;
	
	/* Direction Constants */
	static const int kLeft = 0;
	static const int kRight = 1;
	
	static const double kTimePerShot = .3;
};

#endif
