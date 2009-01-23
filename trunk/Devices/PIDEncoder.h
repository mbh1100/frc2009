#ifndef PIDENCODER_H
#define PIDENCODER_H

#include "WPILib.h"

/* Wrapper class for encoder so not only can it be read by
 * a PIDController, but it can be set to report distance,
 * velocity, or acceleration.
 */
class PIDEncoder : public Encoder, public PIDSource
{
public:
	typedef enum
	{
		kDistance, kVelocity, kAcceleration
	}EncoderType;
	
	PIDEncoder(UINT32 aChannel, UINT32 bChannel, bool reverseDirection = false);
	PIDEncoder(UINT32 aSlot, UINT32 aChannel, UINT32 bSlot, UINT32 bChannel, bool reverseDirection = false);
	virtual ~PIDEncoder();
	
	void Start();
	void Reset();
	void Stop();
	
	void SetType(EncoderType newType);
	float PIDGet();
	
private:
	float m_prevVelocity;
	EncoderType m_type;
	Timer *m_timer;
};

#endif //PIDENCODER_H