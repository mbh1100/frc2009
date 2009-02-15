#ifndef PIDSERVO_H
#define PIDSERVO_H

#include "WPILib.h"

/* Wrapper class for using Servos as PID output
 */
class PIDServo : public PIDOutput, public Servo
{
public:
	PIDServo(UINT32 slot, UINT32 channel);
	virtual ~PIDServo();
	
	void PIDWrite(float output);
	void SetDirection(int direction);
		
protected:
	int m_direction;
};

#endif //PIDSERVO_H
