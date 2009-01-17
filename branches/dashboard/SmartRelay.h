#ifndef SMARTRELAY_H
#define SMARTRELAY_H

#include "WPILib.h"

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
