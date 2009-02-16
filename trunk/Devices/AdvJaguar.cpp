#include "AdvJaguar.h"

/* Contructor, sets the inital bounds to be the physical limits of
 * the standard Jaguars and initializes the underlying objects.
 */
AdvJaguar::AdvJaguar(UINT32 slot, UINT32 channel) : Jaguar::Jaguar(slot, channel), AdvMotorController::AdvMotorController()
{
	m_upperBound = +.98;
	m_lowerBound = -.98;
}

AdvJaguar::~AdvJaguar()
{
	
}

/* Sends an ajusted value (by the bounds, deadband and reversal)
 * to the Jaguar object.
 */
void AdvJaguar::Set(float value)
{
	value = AdvMotorController::Ajust(value);
	Jaguar::Set(value);
}

/* Gets the last sent value from the Jaguar object.
 */
float AdvJaguar::Get()
{
	return Jaguar::Get();
}
