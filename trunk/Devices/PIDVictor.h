#ifndef PIDVICTOR_H
#define PIDVICTOR_H

#include "WPILib.h"

/* Wrapper class for using Victor as PID output
 */
class PIDVictor : public PIDOutput, public Victor
{
public:
	PIDVictor(UINT32 slot, UINT32 channel);
	~PIDVictor();
	
	void PIDWrite(float output);
};

#endif //PIDVICTOR_H
