#ifndef PIDJAGUAR_H
#define PIDJAGUAR_H

#include "WPILib.h"

/* Wrapper class for using Jaguars as PID output
 */
class PIDJaguar : public PIDOutput, public Jaguar
{
public:
	PIDJaguar(UINT32 slot, UINT32 channel);
	~PIDJaguar();
	
	void PIDWrite(float output);
};

#endif //PIDJAGUAR_H