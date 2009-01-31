#ifndef __PIDCONTROL_H__
#define __PIDCONTROL_H__

#include "math.h"
#include <iostream.h>
#include "PIDSource.h"
#include "PIDOutput.h"

class PIDControl
{
public:
	PIDControl();
	virtual ~PIDControl();
	
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
	
	/* Input and Output Objects */
	PIDSource *m_source;
	PIDOutput *m_output;
	
	/* Constants */
	float m_p, m_i, m_d;
	float m_errorPercent, m_errorIncrement;
	float m_maxOutput, m_minOutput, m_maxInput, m_minInput;
	float m_setpoint;
	bool m_increment;
	
	/* Loop Variables */
	float m_previousValue, m_cycleCount;
	float m_errorSum, m_errorAllowed;
	float m_lastOutput;
};

#endif
