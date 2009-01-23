#include "AdvPIDController.h"

/* Run the standard PIDController constructor
 */
AdvPIDController::AdvPIDController(float p, float i, float d, float period) : 
PIDController::PIDController(p, i, d, period)
{
	
}

AdvPIDController::~AdvPIDController()
{
	
}

/* Write the PID output as the current setpoint.
 */
void AdvPIDController::PIDWrite(float output)
{
	SetSetpoint(output);
}
