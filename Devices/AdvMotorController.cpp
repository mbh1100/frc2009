#include "AdvMotorController.h"

/* Constructor, initializes all bounds and deadbands to 0.0
 * so the motor would always be set to 0.0 if not propperly
 * initialized.
 */
AdvMotorController::AdvMotorController()
{
	m_upperBound = 0.0;
	m_lowerBound = 0.0;
	m_upperDeadband = 0.0;
	m_lowerDeadband = 0.0;
	m_deadband = 0.0;
	
	m_direction = kMotorForward;
}

AdvMotorController::~AdvMotorController()
{
	
}

/* Set function for the motor, this should never be called. Inheriting
 * classes should impliment this method through the physical motor.
 */
void AdvMotorController::Set(float value)
{
	printf("AdvMotorController::Set(%lf) --- This should NOT be called.\n\r", value);
}

/* Get function for the motor, this should never be called. Inheriting
 * classes should impliment this method through the physical motor.
 */
float AdvMotorController::Get()
{
	printf("AdvMotorController::Get() --- This should NOT be called.\n\r");
	
	return 0.0;
}

/* Sets the direction of positive power, kMotorForward means
 * normal operation range, from m_upperBound(highest power)-
 * m_lowerBound(lowest power). kMotorReverse changes any input 
 * to the reverse range, from m_lowerBound-m_upperBound
 */
void AdvMotorController::SetDirection(MotorDirection direction)
{
	m_direction = direction;
}

/* Sets the max bounds of the motor, if the input is beyond these
 * bounds it will default to the bound instead.
 */
void AdvMotorController::SetBounds(float max, float min)
{
	m_upperBound = max;
	m_lowerBound = min;
}

/* Sets the deadband for the motor, including the deadband set value itself.
 */
void AdvMotorController::SetDeadband(float upper, float lower, float deadband)
{
	m_upperDeadband = upper;
	m_lowerDeadband = lower;
	m_deadband = deadband;
}

/* Wrapper for using the motor as a PID Output.
 */
void AdvMotorController::PIDWrite(float output)
{
	Set(output);
}

/* Ajusts the input to fit the bounds, deadband and direction.
 */
float AdvMotorController::Ajust(float input)
{
	if (input > m_upperBound)
	{
		input = m_upperBound;
	}
	else if (input < m_lowerBound)
	{
		input = m_lowerBound;
	}
	
	if (m_direction == kMotorReverse)
	{
		input = (1.0 - ((input - m_lowerBound) / (fabs(m_upperBound - m_lowerBound)))) * fabs(m_upperBound - m_lowerBound) + m_lowerBound;
	}
	
	if (input < m_upperDeadband && input > m_lowerDeadband)
	{
		input = m_deadband;
	}
	
	return input;
}
