#include "PIDEncoder.h"

/* Initializes using the standard Encoder constructor and then
 * sets the type to the default of kVelocity, and initializes the
 * timer.
 */
PIDEncoder::PIDEncoder(UINT32 aChannel, UINT32 bChannel, bool reverseDirection) :
	Encoder::Encoder(aChannel, bChannel, reverseDirection)
{
	m_type = kVelocity;
	
	m_timer = new Timer();
}

/* Initializes using the standard Encoder constructor and then
 * sets the type to the default of kVelocity, and initializes the
 * timer.
 */
PIDEncoder::PIDEncoder(UINT32 aSlot, UINT32 aChannel, UINT32 bSlot, UINT32 bChannel, bool reverseDirection) :
	Encoder::Encoder(aSlot, aChannel, bSlot, bChannel, reverseDirection)
{
	m_type = kVelocity;
	
	m_timer = new Timer();
}

PIDEncoder::~PIDEncoder()
{
	
}

/* Calls the standard Encoder Start method and also starts the timer.
 */
void PIDEncoder::Start()
{
	Encoder::Start();
	
	m_timer->Start();
}

/* Calls the standard Encoder Reset method and also resets the timer.
 */
void PIDEncoder::Reset()
{
	Encoder::Reset();
	
	m_timer->Reset();
}

/* Calls the standard Encoder Stop method and also stops the timer.
 */
void PIDEncoder::Stop()
{
	Encoder::Stop();
	
	m_timer->Stop();
}

/* Sets the type of the encoder so it knows what value to report when
 * PIDGet is called.
 */
void PIDEncoder::SetType(EncoderType newType)
{
	m_type = newType;
}

/* Sends the the caller the type of data determined by m_type.
 */
float PIDEncoder::PIDGet()
{
	switch (m_type)
	{
	case kDistance :
		return GetDistance();
		
	case kVelocity :
		m_prevVelocity = GetDistance() / m_timer->Get();
		
		Reset();
		
		return m_prevVelocity;
		
	case kAcceleration :
		float tmpVelocity = GetDistance() / m_timer->Get();
		float acceleration = (tmpVelocity - m_prevVelocity) / m_timer->Get();
		m_prevVelocity = tmpVelocity;
		
		Reset();
		
		return acceleration;
	}
}