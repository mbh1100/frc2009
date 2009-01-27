#include "PIDServo.h"

/* Runs the default Servo constructor
 */
PIDServo::PIDServo(UINT32 slot, UINT32 channel) : Servo::Servo(slot, channel)
{
	
}

PIDServo::~PIDServo()
{
	
}

/* Write output value to the Servo
 */
void PIDServo::PIDWrite(float output)
{
	Set(output);
}
