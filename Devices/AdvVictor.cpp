#include "AdvVictor.h"

/* Contructor, sets the inital bounds to be the physical limits of
 * the standard Victors and initializes the underlying objects.
 */
AdvVictor::AdvVictor(UINT32 slot, UINT32 channel) : Victor::Victor(slot, channel), AdvMotorController::AdvMotorController()
{
	m_upperBound = +.98;
	m_lowerBound = -.98;
}

AdvVictor::~AdvVictor()
{
	
}

/* Sends an ajusted value (by the bounds, deadband and reversal)
 * to the Victor object.
 */
void AdvVictor::Set(float value)
{
	value = AdvMotorController::Ajust(value);
	Victor::Set(value);
}

/* Gets the last sent value from the Victor object.
 */
float AdvVictor::Get()
{
	return Victor::Get();
}
