#include <iostream.h>
#include <taskLib.h>
#include <cmath>
#include "WPILib.h"
#include "PIDControl.h"
#include "TrackingCamera.h"

class TrackingTurret
{
public:
	TrackingTurret(Jaguar* motor1, Servo* servo2, Joystick* test1, Joystick* test2);
	~TrackingTurret();
	
	void Update();
	bool AimTurret();
	void ScanTarget(float currentX);
	void ResetScan();
	void StopTurret();

protected:
	TrackingCamera *m_trackingCamera;
	PIDControl *m_calcSpeedX,*m_calcSpeedY;
	float m_allowedErrorX, m_allowedErrorY;
	float m_pX, m_iX, m_dX, m_pY, m_iY, m_dY;
	float m_maxOutX, m_minOutX, m_incrementX, m_desiredX;
	float m_maxOutY, m_minOutY, m_incrementY, m_desiredY;
	float m_valueX, m_valueY, m_currentY;
	float m_relationX, m_relationY;
	float m_maxServo;
	
	float m_distanceX, m_distanceY;
	bool m_inView;
	
	float m_potValue, m_potMax, m_potMin;
	int m_scanLoop, m_inverse;
	float m_scanP, m_scanI, m_scanD;
	PIDControl *m_calcScan;
	float m_maxOutScan, m_minOutScan, m_potLeft, m_potRight;
	Servo *m_cameraServo;
	Jaguar *m_turretMotor;
	Joystick *m_joystick1, *m_joystick2;
};
