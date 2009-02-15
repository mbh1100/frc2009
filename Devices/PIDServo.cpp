#include "PIDServo.h"

/* Runs the default Servo constructor
 */
PIDServo::PIDServo(UINT32 slot, UINT32 channel) : Servo::Servo(slot, channel)
{
	m_direction = 1;
}

PIDServo::~PIDServo()
{
	
}

/* Write output value to the Servo
 */
void PIDServo::PIDWrite(float output)
{
	if (m_direction)
	{
		Set(output);
	}
	else
	{
		Set(1.0 - output);
	}
}

/* Set the direction of positive input */
void PIDServo::SetDirection(int direction)
{
	m_direction = direction;
}
