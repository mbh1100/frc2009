#include "PIDJaguar.h"

/* Runs the default Jaguar constructor
 */
PIDJaguar::PIDJaguar(UINT32 slot, UINT32 channel) : Jaguar::Jaguar(slot, channel)
{
	
}

PIDJaguar::~PIDJaguar()
{
	
}

/* Write output value to the Jaguar
 */
void PIDJaguar::PIDWrite(float output)
{
	//printf("PIDJag in value: %f\r\n", output);
	Set(output);
	//printf("PIDJag out value: %f\r\n", Get());
}
