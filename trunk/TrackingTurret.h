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
	void scanTarget(float currentX);
	void resetScan();
	void stopTurret();

protected:
	PIDcontrol *m_calcSpeedX,*m_calcSpeedY;
	float m_allowedErrorX, m_allowedErrorY;
	float m_pX, m_iX, m_dX, m_pY, m_iY, m_dY;
	float m_maxOutX, m_minOutX, m_incrementX, m_desiredX;
	float m_maxOutY, m_minOutY, m_incrementY, m_desiredY;
	float m_valueX, m_valueY, m_currentY;
	float m_relationX, m_relationY;
	float m_maxServo;
	
	float m_potValue, m_potMax, m_potMin;
	int m_scanLoop, m_inverse;
	float m_scanP, m_scanI, m_scanD;
	PIDcontrol *m_calcScan;
	float m_maxOutScan, m_minOutScan, m_potLeft, m_potRight;
	Servo *m_servo8;
	Jaguar *m_motor7;
};
