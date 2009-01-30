#include "PIDControl.h"

PIDControl::PIDControl()
{
	/* Initialize Constants */
	m_p = 0.0;
	m_i = 0.0;
	m_d = 0.0;
	m_errorPercent = 5.0;
	m_errorIncrement = .01;
	m_errorSum = 0.0;
	m_errorAllowed = 0.0;
	m_maxOutput = 1.0;
	m_minOutput = -1.0;
	m_maxInput = 1.0;
	m_minInput = -1.0;
	m_setpoint = 0.0;
	m_cycleCount = 0.0;
	m_previousValue = 0.0;
	m_source = NULL;
	m_output = NULL;
	m_increment = false;
	
	m_lastOutput = 0;
	m_enabled = true;
}

PIDControl::~PIDControl()
{
	
}

void PIDControl::SetSetpoint(float setpoint)
{
	m_setpoint = setpoint;
}

void PIDControl::SetPID(float p, float i, float d)
{
	m_p = p;
	m_i = i;
	m_d = d;
}

void PIDControl::SetSource(PIDSource* mySource, float maxInput, float minInput)
{
	m_source = mySource;
	m_maxInput = maxInput;
	m_minInput = minInput;
}

void PIDControl::SetOutput(PIDOutput* myOutput, float maxOutput, float minOutput, bool increment)
{
	m_output = myOutput;
	m_maxOutput = maxOutput;
	m_minOutput = minOutput;
	m_increment = increment;
}

void PIDControl::SetError(float errorPercent, float errorIncrement)
{
	m_errorPercent = errorPercent;
	m_errorIncrement = errorIncrement;
}

void PIDControl::Reset()
{
	printf("...Reset \n");
	m_previousValue = 0;
	m_errorSum = 0;
	m_cycleCount = 0;
	m_lastOutput = 0;
}

bool PIDControl::Calculate()
{
	/* If disabled, set output to 0.0 and reset loop */
	if (m_enabled)
	{
		/* Get current value from PIDSource */
		float current = m_source->PIDGet();
		
		/* Calculate allowed error and set previousValue to current so the derivative won't do anything the first loop */
		if (m_cycleCount == 0)
		{
			m_previousValue = current;
			m_errorAllowed = .5 * fabs(m_maxInput - m_minInput) * (m_errorPercent / 100);
		}
		
		printf("Current Value: %f\r\n", current);
		
		/* Decide if goal has been achieved or not */
		if (fabs(current - m_setpoint) <= m_errorAllowed)
		{
			/* If goal is achieved, give output value of setpoint */
			m_output->PIDWrite(ChangeError(m_setpoint));
			Reset();
			m_lastOutput = ChangeError(m_setpoint);
			return true;
		}
		else
		{
			/* If goal is not achieved... */
			/* Calculate error in terms of the output */
			float error = ChangeError(m_setpoint - current);
			
			float pVal = 0, iVal = 0, dVal = 0;
			
			/* Calculate pVal */
			pVal = m_p * error;
			
			/* Calculate iVal */
			float allowedErrorOut = ChangeError(m_errorAllowed);
			if (error >= allowedErrorOut)
			{
				if (m_errorSum < 0);
				{
					m_errorSum = 0;
				}
				if ((error) < m_errorIncrement)
				{
					m_errorSum += error;
				}
				else
				{
					m_errorSum += m_errorIncrement;
				}
			}
			else if (error <= -allowedErrorOut)
			{
				if(m_errorSum > 0)
				{
					m_errorSum = 0;
				}
				if(error > -m_errorIncrement)
				{
					m_errorSum += error;
				}
				else
				{
					m_errorSum -= m_errorIncrement;
				}			
			}
			else
			{
				m_errorSum = 0;
			}
			iVal = m_i * m_errorSum;
			
			/* Calculate dVal */
			float velocity = current - m_previousValue;
			dVal = m_d * velocity;
			
			/* Calculate Output and Change Motor Speed */
			float output = pVal + iVal - dVal;
			if (m_increment)
			{
				output += m_lastOutput;
			}

			if (output > m_maxOutput)
			{
				output = m_maxOutput;
			}
			else if (output < -m_maxOutput)
			{
				output = -m_maxOutput;
			}
			
			m_output->PIDWrite(output);
			
			m_previousValue = current;
			m_cycleCount++;
			m_lastOutput = output;
			return false;
		}
	}
	m_output->PIDWrite(0.0);
	Reset();
	return false;
}

float PIDControl::ChangeError(float inputError)
{
	return inputError * ((m_maxOutput - m_minOutput)/(m_maxInput - m_minInput)) + (m_maxInput + m_minInput)/2;
}

void PIDControl::Disable()
{
	m_output->PIDWrite(0.0);
	Reset();
	m_enabled = false;
}

void PIDControl::Enable()
{
	m_enabled = true;
}
