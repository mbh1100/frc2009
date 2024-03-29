#ifndef HARDWAREINTERFACE_H
#define HARDWAREINTERFACE_H

#include "WPILib.h"
#include "PCVideoServer.h"
#include "AxisCamera.h"
#include "FrcError.h"

#include "Devices/SmartRelay.h"
#include "Devices/AdvJaguar.h"
#include "Devices/AdvVictor.h"
#include "Devices/AdvServo.h"

#include <list>


/* This class contains all the nessisary hardware declartions for the robot.
 * Any (non-sensor) hardware that the robot needs should be requested from
 * this class so it has access to all current hardware for debug perposes
 * via dashboard. It provides functionality for both sending data and images 
 * to the dashboard program. Currently it is configured to use the default
 * dashboard pack format (2009).
 * 
 * NOTE: The slot numbers for the cRIO modules are initialized in the
 * constructor, so if those modules are moved be sure to change those values.
 */
class HardwareInterface : public SensorBase
{
public:
	HardwareInterface(bool camera, bool cameraServer);
	virtual ~HardwareInterface();
	
	void UpdateDashboard(bool cameraState);
	void EmergencyStop();
	
	AnalogModule* GetAnalogModule(UINT8 num);
	DigitalModule* GetDigitalModule(UINT8 num);
	Jaguar* GetJaguar(UINT8 moduleNum, UINT8 channel);
	Victor* GetVictor(UINT8 moduleNum, UINT8 channel);
	Servo* GetServo(UINT8 moduleNum, UINT8 channel);
	AdvJaguar* GetAdvJaguar(UINT8 moduleNum, UINT8 channel);
	AdvVictor* GetAdvVictor(UINT8 moduleNum, UINT8 channel);
	AdvServo* GetAdvServo(UINT8 moduleNum, UINT8 channel);
	SmartRelay* GetSmartRelay(UINT8 moduleNum, UINT8 channel);
	Solenoid* GetSolenoid(UINT8 channel);
	Joystick* GetJoystick(UINT8 port);
	DriverStation* GetDriverStation();
	
	UINT8 kAnalogSlotNumbers[kAnalogModules];
	UINT8 kDigitalSlotNumbers[kDigitalModules];
	static const UINT8 kSolenoidSlotNumber = 8;
	static const UINT8 kJoystickPorts = 4;
	static const UINT8 kJoystickButtons = 12;
	static const UINT8 kDriverStationDigitalOuts = 8;
	static const UINT8 kDriverStationDigitalIns = 8;
	static const UINT8 kDriverStationAnalogIns = 4;
	
protected:
	AnalogModule *m_analogModules[kAnalogModules];
	DigitalModule *m_digitalModules[kDigitalModules];
	Solenoid *m_solenoids[kSolenoidChannels];
	SmartRelay *m_smartRelays[kDigitalModules][kRelayChannels];
	PWM *m_pwms[kDigitalModules][kPwmChannels];
	
	std::list<PWM*> m_emergencyDisableList;
	
	Joystick *m_joysticks[kJoystickPorts];
	
	PCVideoServer *m_cameraFeed;
	
	bool m_cameraState;
	
private:
	DISALLOW_COPY_AND_ASSIGN(HardwareInterface);
	DriverStation *m_ds;
};


#endif // HARDWAREINTERFACE_H
