#ifndef ADV_SERVO_H
#define ADV_SERVO_H

#include "AdvMotorController.h"
#include "Servo.h"

/* Advanced Servo using the features of the Advanced Motor
 * Controller interface.
 */
class AdvServo : public Servo, public AdvMotorController
{
public:
	AdvServo(UINT32 slot, UINT32 channel);
	virtual ~AdvServo();
	
	void Set(float value);
	float Get();
};

#endif //ADV_SERVO_H
