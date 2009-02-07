#ifndef __SHOOT_H__
#define __SHOOT_H__

#include "WPILib.h"

class Shoot
{
public:
	Shoot(Jaguar* shootMotor);
	virtual ~Shoot();
	
	void Fire(float targetDistance);
	
protected:
	Jaguar *m_shootMotor;
};

#endif
