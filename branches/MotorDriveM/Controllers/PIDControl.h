#ifndef PIDCONTROL_H
#define PIDCONTROL_H

#include "math.h"
#include <iostream.h>
#include "PIDSource.h"
#include "PIDOutput.h"

class PIDControl
{
public:
	PIDControl();
	~PIDControl();
	
	void SetSetpoint(float setpoint);
	void SetPID(float p, float i, float d);
	void SetSource(PIDSource* mySource, float maxInput, float minInput);
	void SetOutput(PIDOutput* myOutput, float maxOutput, float maxInput, bool increment = false);
	void SetError(float errorPercent, float errorIncrement);
	void Reset();
	bool Calculate();
	void Disable();
	void Enable();

private:
	float ChangeError(float inputError);
	
protected:
	bool m_enabled;
	
	PIDSource *m_source;
	PIDOutput *m_output;
	
	float m_p, m_i, m_d;
	float m_errorPercent, m_errorIncrement;
	float m_maxOutput, m_minOutput, m_maxInput, m_minInput;
	float m_setpoint;
	
	float m_previousValue, m_cycleCount;
	float m_errorSum, m_errorAllowed;	
	float m_previousOutput;
	bool m_increment;
};

#endif //PIDCONTROL_H

