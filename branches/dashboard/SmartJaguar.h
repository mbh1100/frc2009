#ifndef SMARTJAGUAR_H
#define SMARTJAGUAR_H

#include "WPILib.h"
#include <list>
#include <cmath>

#define kRampDownMult .8

class SmartJaguar
{
public:
	SmartJaguar(Jaguar* motor);
	~SmartJaguar();
	
	void SetSpeed(float speed);
	void Update();
	
protected:
	Jaguar *m_motor;
	float m_target;
	std::list<float> m_prevValues;
	
	static const UINT8 kMaxSamples = 5;
	static const UINT8 kMinSamples = 2;
};

#endif //SMARTJAGUAR_H
