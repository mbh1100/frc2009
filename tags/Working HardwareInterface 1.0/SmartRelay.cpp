#include "SmartRelay.h"

/* Runs the normal Relay::Relay(channel, direction) initializer 
 * and sets the stored values to the default of off.
 */
SmartRelay::SmartRelay(UINT32 channel, Direction direction) : Relay(channel, direction)
{
	m_fVal = false;
	m_rVal = false;
}

/* Runs the normal Relay::Relay(slot, channel, direction) 
 * initializer and sets the stored values to the default of off.
 */
SmartRelay::SmartRelay(UINT32 slot, UINT32 channel, Direction direction) : Relay(slot, channel, direction)
{
	m_fVal = false;
	m_rVal = false;
}
	
SmartRelay::~SmartRelay()
{
	
}

/* Preforms a normal Relay::Set except as updates internal
 * values to keep track of current relay state 
 */
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
	else /* kOff */
	{
		m_fVal = false;
		m_rVal = false;
	}
}

/* Returns true if the relay's forward channel is on 
 */
bool SmartRelay::GetForward()
{
	return m_fVal;
}

/* Returns true if the relay's reverse channel is on 
 */
bool SmartRelay::GetReverse()
{
	return m_rVal;
}
