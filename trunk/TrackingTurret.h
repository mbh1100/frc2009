#include <iostream.h>
#include <taskLib.h>
#include "math.h"
#include "WPILib.h"
#include "PIDcontrol.h"

class TrackingTurret
{
public:
	TrackingTurret();
	~TrackingTurret();
	
	bool aimTurret(float distanceX, float distanceY);
	void scanTarget(float currentX, float currentY);

protected:
	PIDcontrol *calcSpeedX,*calcSpeedY;
	float m_allowedErrorX, m_allowedErrorY;
	float m_p, m_i, m_d, m_period;
	float m_maxOutX, m_minOutX, m_incrementX, m_desiredX;
	float m_maxOutY, m_minOutY, m_incrementY, m_desiredY;
	float m_valueX, m_valueY, m_currentY;
	float m_maxServo;
	Servo *m_servo8;
	Jaguar *m_motor7;
};
