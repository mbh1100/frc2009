#include "SmartJaguar.h"

SmartJaguar::SmartJaguar(Jaguar* motor)
{
	m_motor = motor;
	
	for (UINT8 i = 0; i < kMaxSamples; i++)
	{
		m_prevValues.push_back(motor->Get());
	}
}

SmartJaguar::~SmartJaguar()
{
	
}
	
void SmartJaguar::SetSpeed(float speed)
{
	m_target = speed;
}

void SmartJaguar::Update()
{
	float diff = fabs(m_target - m_motor->Get());
	/* regulate difference to at most 1.0 */
	diff = diff > 1.0 ? 1.0 : diff;
	diff = diff * (kMaxSamples - kMinSamples);
	
	/* default sample size */
	UINT8 sampleSize = (UINT8)(kMaxSamples - diff);
	
	/* If on the same side of zero, and the target is lower than the current
	 * OR the target is on the other side of zero use fewer samples */
	if ((((fabs(m_target) == m_target) == (fabs(m_motor->Get()) == m_motor->Get())) 
			&& fabs(m_motor->Get()) > fabs(m_target)) 
			|| ((fabs(m_target) == m_target) != (fabs(m_motor->Get()) == m_motor->Get())))
	{
		sampleSize = (UINT8)((float)sampleSize * kRampDownMult + .5);
	}
	
	float average = m_target;
	std::list<float>::iterator iter = m_prevValues.begin();
	
	for (UINT8 i = 0; i < sampleSize ; i++, iter++)
	{
		average += *iter;
	}
	
	average /= sampleSize + 1;
	
	m_motor->Set(average);
	m_prevValues.push_back(average);
	m_prevValues.pop_front();
}
