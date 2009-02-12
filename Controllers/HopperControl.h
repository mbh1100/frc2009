#ifndef __HOPPERCONTROL_H__
#define __HOPPERCONTROL_H__

#include "WPILib.h"

class HopperControl
{
public:
	HopperControl(Victor* leftHelixMotor, Victor* rightHelixMotor, Victor* sweeperMotor);
	virtual ~HopperControl();
	
	void Update(int helixSide, int helixDirection, bool limitLeftEntry, bool limitLeftBottom, bool limitLeftTop, 
			bool limitRightEntry, bool limitRightBottom, bool limitRightTop, UINT32 counter);
	void SensorIntake();
	void Disable();
	void SetBallCount(int ballsLeft, int ballsRight);
	int GetBallCountLeft();
	int GetBallCountRight();
	
protected:
	Victor *m_leftHelixMotor, *m_rightHelixMotor, *m_sweeperMotor;
	static const float kHelixInSpeed = .2;
	static const float kHelixOutSpeed = -.98;
	static const float kSweeperSpeed = .98;
	float m_helixSide, m_helixDirection;
	
	bool m_limitLeftEntry, m_limitLeftBottom, m_limitLeftTop;
	bool m_limitRightEntry, m_limitRightBottom, m_limitRightTop;
	
	int m_ballsInLeft, m_ballsInRight;
	bool m_leftEntering, m_rightEntering;
	
	UINT32 m_leftStartCount, m_rightStartCount, m_currentCount;
};

#endif
