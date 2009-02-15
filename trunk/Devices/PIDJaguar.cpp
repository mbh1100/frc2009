#include "PIDJaguar.h"

/* Runs the default Jaguar constructor
 */
PIDJaguar::PIDJaguar(UINT32 slot, UINT32 channel) : Jaguar::Jaguar(slot, channel)
{
	m_direction = 1;
}

PIDJaguar::~PIDJaguar()
{
	
}

/* Write output value to the Jaguar
 */
void PIDJaguar::PIDWrite(float output)
{
	Set(output * (double)m_direction);
}

/* Set the direction of positive input */
void PIDJaguar::SetDirection(int direction)
{
	m_direction = direction;
}
