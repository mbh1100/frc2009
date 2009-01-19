#include <iostream.h>
#include <taskLib.h>
#include "math.h"
#include "WPILib.h"

class TrackingTurret
{
public:
	TrackingTurret();
	~TrackingTurret();
	
	float aimTurret(float distanceX, float distanceY, float currentX, float currentY);
	float scanTarget(float currentX, float currentY);

protected:
	PIDController *loopingPID;
	float m_p, m_i, m_d, m_period;
	float m_pwmValue;
	
};
