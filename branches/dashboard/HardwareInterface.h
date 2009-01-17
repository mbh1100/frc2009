#ifndef HARDWAREINTERFACE_H
#define HARDWAREINTERFACE_H

#include "WPILib.h"
#include "PCVideoServer.h"

#include "SmartRelay.h"

/**
 * This class is just an example of one way you could organize the data that you want
 * to send to the dashboard.  The PackAndSend method does all the work.  You could
 * put the packing code directly in your code, but this model protects you from
 * packing data in the wrong order throughout your code.
 * 
 * The data and the format of this structure are just an example.  It is written to
 * match the initial data format expected by the LabVIEW Dashboard project.  Feel free
 * to add data elements or remove them.  Just remember to make any changes consistently
 * between the LabVIEW "Dashboard Datatype" and the data that gets packed by this class.
 */
class HardwareInterface : public SensorBase
{
public:
	HardwareInterface(bool camera);
	virtual ~HardwareInterface();
	
	void UpdateDashboard(bool cameraState);
	
	AnalogModule* GetAnalogModule(UINT8 num);
	DigitalModule* GetDigitalModule(UINT8 num);
	Jaguar* GetJaguar(Uint8 moduleNum, UINT8 channel);
	Victor* GetVictor(Uint8 moduleNum, UINT8 channel);
	Servo* GetServo(Uint8 moduleNum, UINT8 channel);
	SmartRelay* GetSmartRelay(UINT8 moduleNum, UINT8 channel);
	Solenoid* GetSolenoid(UINT8 channel);
	
	static const UINT8 kAnalogSlotNumbers[kAnalogModules] = {1, 2};
	static const UINT8 kDigitalSlotNumbers[kDigitalModulels] = {4, 6};
	static const UINT8 kSolenoidSlotNumber = 8;
	
protected:
	AnalogModule *m_analogModules[kAnalogModules];
	DigitalModule *m_digitalModules[kDigitalModules];
	Solenoid *m_solenoids[kSolenoidChannels];
	SmartRelay *m_smartRelays[kDigitalModules][kRelayChannels];
	PWM *m_pwms[kDigitalModules][kPWMChannels];
	
	PCVideoServer *m_cameraFeed;
	
	bool m_cameraState;
	
private:
	DISALLOW_COPY_AND_ASSIGN(HardwareInterface);
	DriverStation *m_ds;
};


#endif // HARDWAREINTERFACE_H
