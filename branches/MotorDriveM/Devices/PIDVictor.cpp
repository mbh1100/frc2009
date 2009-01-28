#include "PIDVictor.h"

/* Runs the default Victor constructor
 */
PIDVictor::PIDVictor(UINT32 slot, UINT32 channel) : Victor::Victor(slot, channel)
{
	
}

PIDVictor::~PIDVictor()
{
	
}

/* Write output value to the Victor
 */
void PIDVictor::PIDWrite(float output)
{
	Set(output);
}
