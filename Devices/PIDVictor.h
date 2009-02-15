#ifndef PIDVICTOR_H
#define PIDVICTOR_H

#include "WPILib.h"

/* Wrapper class for using Victor as PID output
 */
class PIDVictor : public PIDOutput, public Victor
{
public:
	PIDVictor(UINT32 slot, UINT32 channel);
	virtual ~PIDVictor();
	
	void PIDWrite(float output);
	void SetDirection(int direction);
		
protected:
	int m_direction;
};

#endif //PIDVICTOR_H
