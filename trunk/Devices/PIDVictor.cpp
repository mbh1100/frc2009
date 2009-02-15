#include "PIDVictor.h"

/* Runs the default Victor constructor
 */
PIDVictor::PIDVictor(UINT32 slot, UINT32 channel) : Victor::Victor(slot, channel)
{
	m_direction = 1;
}

PIDVictor::~PIDVictor()
{
	
}

/* Write output value to the Victor
 */
void PIDVictor::PIDWrite(float output)
{
	Set(output * (double)m_direction);
}

/* Set the direction of positive input */
void PIDVictor::SetDirection(int direction)
{
	m_direction = direction;
}
