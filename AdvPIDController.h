#ifndef ADVPIDCONTROLLER_H
#define ADVPIDCONTROLLER_H

#include "PIDController.h"
#include "PIDOutput.h"

/* AdvPIDController is a wrapper around PIDController that allows
 * the PIDController itself to be a PID output, allowing for nested
 * PID controls.
 */
class AdvPIDController : public PIDController, public PIDOutput
{
	AdvPIDController(float p, float i, float d, float period = 0.05);
	virtual ~AdvPIDController();
	
	void PIDWrite(float output);
};

#endif //ADVPIDCONTROLLER_H
