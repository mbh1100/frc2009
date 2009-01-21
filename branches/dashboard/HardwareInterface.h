#ifndef HARDWAREINTERFACE_H
#define HARDWAREINTERFACE_H

#include "WPILib.h"
#include "PCVideoServer.h"

#include "SmartRelay.h"


/* This class contains all the nessisary hardware declartions for the robot.
 * Any (non-sensor) hardware that the robot needs should be requested from
 * this class so it has access to all current hardware for debug perposes
 * via dashboard. It provides functionality for both sending data and images 
 * to the dashboard program. Currently it is configured to use the default
 * dashboard pack format (2009).
 * 
 * NOTE: The slot numbers for the cRIO modules are initialized in the
 * constructor, so if those modules are moved be sure to change those values.
 * 
 * TODO: Add Joystick support to both HardwareInterface and to the packets
 * that dashboard expects so that joystick data can be read from dashboard.
 */
class HardwareInterface : public SensorBase
{
public:
	HardwareInterface(bool camera);
	virtual ~HardwareInterface();
	
	void UpdateDashboard(bool cameraState);
	
	AnalogModule* GetAnalogModule(UINT8 num);
	DigitalModule* GetDigitalModule(UINT8 num);
	Jaguar* GetJaguar(UINT8 moduleNum, UINT8 channel);
	Victor* GetVictor(UINT8 moduleNum, UINT8 channel);
	Servo* GetServo(UINT8 moduleNum, UINT8 channel);
	SmartRelay* GetSmartRelay(UINT8 moduleNum, UINT8 channel);
	Solenoid* GetSolenoid(UINT8 channel);
	DriverStation* GetDriverStation();
	
	UINT8 kAnalogSlotNumbers[kAnalogModules];
	UINT8 kDigitalSlotNumbers[kDigitalModules];
	static const UINT8 kSolenoidSlotNumber = 8;
	static const UINT8 kJoystickPorts = 4;
	static const UINT8 kJoystickButtons = 12;
	
protected:
	AnalogModule *m_analogModules[kAnalogModules];
	DigitalModule *m_digitalModules[kDigitalModules];
	Solenoid *m_solenoids[kSolenoidChannels];
	SmartRelay *m_smartRelays[kDigitalModules][kRelayChannels];
	PWM *m_pwms[kDigitalModules][kPwmChannels];
	
	Joystick *m_joysticks[kJoystickPorts];
	
	PCVideoServer *m_cameraFeed;
	
	bool m_cameraState;
	
private:
	DISALLOW_COPY_AND_ASSIGN(HardwareInterface);
	DriverStation *m_ds;
};


#endif // HARDWAREINTERFACE_H
