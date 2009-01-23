#include "math.h"
#include <iostream.h>

class PIDControl
{
public:
	PIDControl(float p, float i, float d, float allowedError);
	~PIDControl();
	
	void SetDesired(float desiredValue);
	void SetDesired(float p, float i, float d);
	void SetDesired(float maxOutput, float minOutput, float increment, float desiredValue, float relation);
	void SetDesired(float maxOutput, float minOutput, float increment, float desiredValue, float relation, float p, float i, float d, float allowedError);
	void ResetLoop();
	float CalcPID(float current);
	bool IsDone(float current);

protected:
	float m_p, m_i, m_d;
	float m_relateInOut;
	
	float m_maxOutput, m_minOutput;
	float m_increment, m_desiredValue;
	
	float m_errorSum, m_allowedError;
	float m_previousValue;
	float m_cycleCount;
};
