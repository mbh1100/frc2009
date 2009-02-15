#ifndef PIDJAGUAR_H
#define PIDJAGUAR_H

#include "WPILib.h"

/* Wrapper class for using Jaguars as PID output
 */
class PIDJaguar : public PIDOutput, public Jaguar
{
public:
	PIDJaguar(UINT32 slot, UINT32 channel);
	virtual ~PIDJaguar();
	
	void PIDWrite(float output);
	void SetDirection(int direction);
	
protected:
	int m_direction;
};

#endif //PIDJAGUAR_H
