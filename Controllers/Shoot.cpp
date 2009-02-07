#include "Shoot.h"

Shoot::Shoot(Jaguar* shootMotor)
{
	m_shootMotor = shootMotor;
}

void Shoot::Fire(float targetDistance)
{
	m_shootMotor->Set(targetDistance);
}
