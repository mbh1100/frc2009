#ifndef PIDENCODER_H
#define PIDENCODER_H

#include "WPILib.h"
#include <list>
#include <numeric>

/* Wrapper class for encoder so not only can it be read by
 * a PIDController, but it can be set to report distance,
 * velocity, or acceleration. Uses a sample average for
 * determining the latter two.
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
	
	void SetSampleSize(unsigned int sample);
	void SetType(EncoderType newType);
	float PIDGet();
	
private:
	unsigned int m_samples;
	
	std::list<float> m_prevVelocity;
	std::list<float> m_prevAcceleration;
	EncoderType m_type;
	Timer *m_timer;
};

#endif //PIDENCODER_H
