#ifndef __EMPTYCELL_H__
#define __EMPTYCELL_H__

#include "WPILib.h"

#include "../Devices/AdvMotorController.h"

class EmptyCell
{
public:
	EmptyCell(AdvMotorController* leftMotor, AdvMotorController* rightMotor);
	virtual ~EmptyCell();
	
	void Update(bool release, bool limitLeftBottom, bool limitRightBottom, bool limitLeftTop, bool limitRightTop);
	
protected:
	AdvMotorController *m_leftMotor, *m_rightMotor;
	
	bool m_release, m_releaseProcess, m_limitLeftBottom, m_limitRightBottom, m_limitLeftTop, m_limitRightTop;
	
	Timer *m_holdTimer;
	
	float kEmptyCellSpeed;
};

#endif
