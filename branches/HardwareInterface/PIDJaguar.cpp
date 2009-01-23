#include "PIDJaguar.h"

PIDJaguar::PIDJaguar(UINT32 slot, UINT32 channel) : Jaguar::Jaguar(slot, channel)
{
	
}

PIDJaguar::~PIDJaguar()
{
	
}
	
void PIDJaguar::PIDWrite(float output)
{
	Set(output);
}
