#include "PIDControl.h"

PIDControl::PIDControl(float p, float i, float d, float allowedError)
{
	m_p = p;
	m_i = i;
	m_d = d;
	m_allowedError = allowedError;
	m_maxOutput = 1;
	m_minOutput = -1;
	m_increment = 0.01;
	m_desiredValue = 0;
	m_cycleCount = 0;
	m_previousValue = 0;
}
void PIDControl::SetDesired(float desiredValue)
{
	m_desiredValue = desiredValue;
}
void PIDControl::SetDesired(float p, float i, float d)
{
	m_p = p;
	m_i = i;
	m_d = d;
}
void PIDControl::SetDesired(float maxOutput, float minOutput, float increment, float desiredValue, float relation)
{
	m_maxOutput = maxOutput;
	m_minOutput = minOutput;
	m_increment = increment;
	m_desiredValue = desiredValue;
	m_relateInOut = relation;
}
void PIDControl::SetDesired(float maxOutput, float minOutput, float increment, float desiredValue, float relation, float p, float i, float d, float allowedError)
{
	m_maxOutput = maxOutput;
	m_minOutput = minOutput;
	m_increment = increment;
	m_desiredValue = desiredValue;
	m_p = p;
	m_i = i;
	m_d = d;
	m_allowedError = allowedError;
	m_relateInOut = relation;
}
void PIDControl::ResetLoop()
{
	printf("...Reset \n");
	m_previousValue = 0;
	m_errorSum = 0;
	m_cycleCount = 0;
}
float PIDControl::CalcPID(float current)
{
	float pVal = 0.0, iVal = 0.0, dVal = 0.0;
	
	if(m_cycleCount == 0)
	{
		m_previousValue = current;
	}
	
	/* Calculate P component */
	float error = m_desiredValue - current;
	error *= m_relateInOut;
	pVal = m_p * error;
	
	/* Calculate I component */
	if(error >= m_allowedError)
	{
		if(m_errorSum < 0)
		{
			m_errorSum = 0;
		}
		if((error) < m_increment)
		{
			m_errorSum += error;
		}
		else
		{
			m_errorSum += m_increment;
		}
	}
	else if(error <= -m_allowedError)
	{
		if(m_errorSum > 0)
		{
			m_errorSum = 0;
		}
		if(error > -m_increment)
		{
			m_errorSum += error;
		}
		else
		{
			m_errorSum -= m_increment;
		}
	}
	else
	{
		m_errorSum = 0;
	}
	iVal = m_i * m_errorSum;
	
	/* Calculate D component */
	float velocity = current - m_previousValue;
	dVal = m_d * velocity;
	
	/* Calculate output */
	float output = pVal + iVal - dVal;
	if(output > m_maxOutput)
	{
		output = m_maxOutput;
	}
	else if(output < -m_maxOutput)
	{
		output = -m_maxOutput;
	}
	
	m_previousValue = current;
	m_cycleCount++;
	return output;
}
bool PIDControl::IsDone(float current)
{
	if (fabs(current) <= m_desiredValue + m_allowedError)
	{
		printf("%ud Resetting in isDone",(unsigned int)this);
		ResetLoop();
		return true;
	}
	return false;
}
