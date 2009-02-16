#include "AdvServo.h"

AdvServo::AdvServo(UINT32 slot, UINT32 channel) : Servo::Servo(slot, channel), AdvMotorController::AdvMotorController()
{
	m_upperBound = 1.0;
	m_lowerBound = 0.0;
}

AdvServo::~AdvServo()
{
	
}
	
void AdvServo::Set(float value)
{
	value = AdvMotorController::Ajust(value);
	Servo::Set(value);
}

float AdvServo::Get()
{
	return Servo::Get();
}
