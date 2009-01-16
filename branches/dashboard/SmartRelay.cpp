#include "SmartRelay.h"

SmartRelay::SmartRelay(UINT32 channel, Direction direction) : Relay(channel, direction)
{
	m_fVal = false;
	m_rVal = false;
}

SmartRelay::SmartRelay(UINT32 slot, UINT32 channel, Direction direction) : Relay(slot, channel, direction)
{
	m_fVal = false;
	m_rVal = false;
}
	
SmartRelay::~SmartRelay()
{
	
}
	
void SmartRelay::Set(Value value)
{
	Relay::Set(value);
	
	if (value == kForward)
	{
		m_fVal = true;
		m_rVal = false;
	}
	else if (value = kReverse)
	{
		m_fVal = false;
		m_rVal = true;
	}
	else if (value = kOn)
	{
		m_fVal = true;
		m_rVal = true;
	}
	else /* kOff */
	{
		m_fVal = false;
		m_rVal = false;
	}
}
