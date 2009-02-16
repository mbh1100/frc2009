#ifndef ADV_MOTOR_CONTROLLER_H
#define ADV_MOTOR_CONTROLLER_H

#include "PIDOutput.h"

#include <cstdio>
#include <cmath>

typedef enum
{
	kMotorForward = 1,
	kMotorReverse = -1
}MotorDirection;

/* Interface for motors (Servos and Speed Controllers) that
 * provides support for PID Output, direction reversal,
 * Bounding and Deadband (by floating point).
 */
class AdvMotorController : public PIDOutput
{
public:
	AdvMotorController();
	virtual ~AdvMotorController();
	
	virtual void Set(float value);
	virtual float Get();
	
	void SetDirection(MotorDirection direction);
	void SetBounds(float max, float min);
	void SetDeadband(float upper, float lower, float deadband);
	
	void PIDWrite(float output);
	
protected:
	float Ajust(float input);
	
	float m_upperBound, m_lowerBound, m_upperDeadband, m_lowerDeadband, m_deadband;
	MotorDirection m_direction;
};

#endif //ADV_MOTOR_CONTROLLER_H
