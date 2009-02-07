#ifndef __HOPPERCONTROL_H__
#define __HOPPERCONTROL_H__

#include "WPILib.h"

class HopperControl
{
public:
	HopperControl(Victor* leftHelixMotor, Victor* rightHelixMotor, Victor* sweeperMotor);
	virtual ~HopperControl();
	
	void Update(int helixSide, int helixDirection);
	void Disable();
	
protected:
	Victor *m_leftHelixMotor, *m_rightHelixMotor, *m_sweeperMotor;
	static float kHelixSpeed, kSweeperSpeed;
};

#endif
