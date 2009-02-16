#include "AdvServo.h"

/* Contructor, sets the inital bounds to be the physical limits of
 * the standard Servos and initializes the underlying objects.
 */
AdvServo::AdvServo(UINT32 slot, UINT32 channel) : Servo::Servo(slot, channel), AdvMotorController::AdvMotorController()
{
	m_upperBound = 1.0;
	m_lowerBound = 0.0;
}

AdvServo::~AdvServo()
{
	
}

/* Sends an ajusted value (by the bounds, deadband and reversal)
 * to the Servo object.
 */
void AdvServo::Set(float value)
{
	value = AdvMotorController::Ajust(value);
	Servo::Set(value);
}

/* Gets the last sent value from the Servo object.
 */
float AdvServo::Get()
{
	return Servo::Get();
}
