#ifndef PIDJAGUAR_H
#define PIDJAGUAR_H

#include "WPILib.h"

class PIDJaguar : public PIDOutput, public Jaguar
{
	PIDJaguar(UINT32 slot, UINT32 channel);
	~PIDJaguar();
	
	void PIDWrite(float output);
};

#endif //PIDJAGUAR_H
