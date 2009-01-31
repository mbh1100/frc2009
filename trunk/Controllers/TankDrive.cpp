#include "TankDrive.h"

TankDrive::TankDrive(PIDJaguar* leftMotor, PIDJaguar* rightMotor, PIDEncoder* leftEncoder, PIDEncoder* rightEncoder)
{	
	//All variables
	
	m_leftMotor = leftMotor;
	m_leftEncoder = leftEncoder;
	m_rightMotor = rightMotor;
	m_rightEncoder = rightEncoder;
	
	//initial P, I, D settings need to be determined for drive motor CIMs
	m_p = -0.01;  		  //
	m_i = -0.00; 		  //
	m_d = -0.00;          //
	
	m_setpointLeft = 0.0;
	m_setpointRight = 0.0;
	
	//Not sure what these should be appropriately set to...Encoder vals no limit...
	m_maxInput = 1.5;
	m_minInput = -1.5;
	
	m_maxOutput = 0.99;
	m_minOutput = -1.0;
	m_increment = true;
	
	m_errorPercent = 10;
	m_errorIncrement = .01;
	
	//Set up boundaries and default values for PID control object to use in Calculate
	m_calcSpeedLeft = new PIDControl();
	
	m_calcSpeedLeft->SetPID(m_p, m_i, m_d);
	m_calcSpeedLeft->SetSetpoint(m_setpointLeft);
	m_calcSpeedLeft->SetSource(m_leftEncoder, m_maxInput, m_minInput);
	m_calcSpeedLeft->SetOutput(m_leftMotor, m_maxOutput, m_minOutput, m_increment);
	m_calcSpeedLeft->SetError(m_errorPercent, m_errorIncrement);
	
	m_calcSpeedLeft->Enable();
	
	m_calcSpeedRight = new PIDControl();
	
	m_calcSpeedRight->SetPID(m_p, m_i, m_d);
	m_calcSpeedRight->SetSetpoint(m_setpointRight);
	m_calcSpeedRight->SetSource(m_rightEncoder, m_maxInput, m_minInput);
	m_calcSpeedRight->SetOutput(m_rightMotor, m_maxOutput, m_minOutput, m_increment);
	m_calcSpeedRight->SetError(m_errorPercent, m_errorIncrement);
	
	m_calcSpeedRight->Enable();
}

TankDrive::~TankDrive()
{
	
}

void TankDrive::SetSetpoint(float leftSetpoint, float rightSetpoint)
{
	m_setpointLeft = leftSetpoint;
	m_setpointRight = rightSetpoint;
}
void TankDrive::Update()
{
	/* Enable PID loops, just in case */
	m_calcSpeedLeft->Enable();
	m_calcSpeedRight->Enable();
	
	/* Set new setpoints */
	m_calcSpeedLeft->SetSetpoint(m_setpointLeft);
	m_calcSpeedRight->SetSetpoint(m_setpointRight);
	
	/* Calculate & set values for motors*/
	m_calcSpeedLeft->Calculate();
	m_calcSpeedRight->Calculate();
}

void TankDrive::Disable()
{
	m_calcSpeedLeft->Disable();
	m_calcSpeedRight->Disable();
}
