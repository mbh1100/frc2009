#ifndef SMARTRELAY_H
#define SMARTRELAY_H

#include "WPILib.h"

/* SmartRelay is a wrapper on top of the standard WPILib Relay
 * class. Its main purpose is to store the relay's current state
 * for debug monitoring purposes. Because the default Relay's
 * implimentation make its direction private, currently this
 * class cannot interpret the kOn state (used for uni-directional
 * relays) so use kForward and kReverse instead.
 */
class SmartRelay : public Relay
{
public:
	SmartRelay(UINT32 channel, Direction direction = kBothDirections);
	SmartRelay(UINT32 slot, UINT32 channel, Direction direction = kBothDirections);
	
	virtual ~SmartRelay();
	
	void Set(Value value);
	bool GetForward();
	bool GetReverse();
		
protected:
	bool m_fVal, m_rVal;
};

#endif // SMARTRELAY_H
