#include "math.h"
#include <iostream.h>

class PIDcontrol
{
public:
	PIDcontrol(float p, float i, float d, float allowedError);
	~PIDcontrol();
	
	void setDesired(float desiredValue);
	void setDesired(float p, float i, float d);
	void setDesired(float maxOutput, float minOutput, float increment, float desiredValue, float relation);
	void setDesired(float maxOutput, float minOutput, float increment, float desiredValue, float relation, float p, float i, float d, float allowedError);
	void resetLoop();
	float calcPID(float current);
	bool isDone(float current);

protected:
	float m_p, m_i, m_d;
	float m_relateInOut;
	
	float m_maxOutput, m_minOutput;
	float m_increment, m_desiredValue;
	
	float m_errorSum, m_allowedError;
	float m_previousValue;
	float m_cycleCount;
};
