#include "HardwareInterface.h"

/* Initializes HardwareInterface.
 * if camera is true then a camera task will be generated and if
 * cameraServer is true it will also start a PCImageServer to
 * feed images to a connected Dashboard computer. Most hardware
 * is initialized to NULL and waits for a formal request using
 * the Get methods before getting instaciated.
 */
HardwareInterface::HardwareInterface(bool camera, bool cameraServer) : m_ds (DriverStation::GetInstance())
{
	/* Slot Numbers for hardware */
	kAnalogSlotNumbers[0] = 1;
	kAnalogSlotNumbers[1] = 2;
	kDigitalSlotNumbers[0] = 4;
	kDigitalSlotNumbers[1] = 6;
	
	int frameRate = 10, compression = 0;
	ImageSize resolution = k320x240;
	ImageRotation imageRotation = ROT_0;
	
	if (camera)
	{
		/* Initialize the camera */
		if (!StartCameraTask(frameRate, compression, resolution, imageRotation))
		{
			printf("Camera failed to initialize! Error Code %s\r\n", GetVisionErrorText(GetLastVisionError()));
		}
		
		if (cameraServer)
		{
			/* Wait to ensure camera is initialized */
			Wait(2.0);
		
			/* Open connection to dashboard comput er then wait for robot to ask for image flow */
			m_cameraFeed = new PCVideoServer();
			m_cameraState = true;
		}
	}
	else
	{
		m_cameraFeed = NULL;
		m_cameraState = false;
	}
	
	/* Initialize hardware and hardware modules */
	
	for (UINT8 slot = 0; slot < kAnalogModules; slot++)
	{
		m_analogModules[slot] = AnalogModule::GetInstance(kAnalogSlotNumbers[slot]);
	}
	
	for (UINT8 slot = 0; slot < kDigitalModules; slot++)
	{
		m_digitalModules[slot] = DigitalModule::GetInstance(kDigitalSlotNumbers[slot]);
	}
	
	for (UINT8 slot = 0; slot < kDigitalModules; slot++)
	{
		for (UINT8 channel = 0; channel < kPwmChannels; channel++)
		{
			m_pwms[slot][channel] = NULL;
		}
	}
	
	for (UINT8 slot = 0; slot < kDigitalModules; slot++)
	{
		for (UINT8 channel = 0; channel < kRelayChannels; channel++)
		{
			m_smartRelays[slot][channel] = NULL;
		}
	}
	
	for (UINT8 channel = 0; channel < kSolenoidChannels; channel++)
	{
		m_solenoids[channel] = NULL;
	}
	
	for (UINT8 port = 0; port < kJoystickPorts; port++)
	{
		m_joysticks[port] = new Joystick(port + 1);
	}
	
}

HardwareInterface::~HardwareInterface()
{
	
}

/* Sends information to a connected computer running Dashboard.
 * cameraState will be compared against the current internal
 * value and the image feed (if enabled) will be turned on or
 * off to match.
 */
void HardwareInterface::UpdateDashboard(bool cameraState)
{
	Dashboard &dashboardPacker = m_ds->GetDashboardPacker();
	UINT32 module;
	UINT32 channel;

	/* Read and pack the analog modules */
	for (module = 0; module < kAnalogModules; module++)
	{
		dashboardPacker.AddCluster();
		
		for (channel = 0; channel < kAnalogChannels; channel++)
		{
			dashboardPacker.AddFloat((float)(m_analogModules[module]->GetAverageValue(channel + 1)));
		}
		dashboardPacker.FinalizeCluster();
	}
	
	/* Read and pack the digital modules */
	for (module = 0; module < kDigitalModules; module++)
	{
		dashboardPacker.AddCluster();
		
		/* Read and pack forward relays */
		UINT8 forRelayVal = 0;
		for (channel = kRelayChannels - 1; channel >= 0; channel--)
		{
			if (m_smartRelays[module][channel] != NULL)
			{
				forRelayVal += m_smartRelays[module][channel]->GetForward();
			}
			
			if (channel != 0)
			{
				forRelayVal <<= 1;
			}
			else
			{
				break;
			}
		}
		dashboardPacker.AddU8(forRelayVal);
		
		/* Read and pack reverse relays */
		UINT8 revRelayVal = 0;
		for (channel = kRelayChannels - 1; channel >= 0; channel--)
		{
			if (m_smartRelays[module][channel] != NULL)
			{
				revRelayVal += m_smartRelays[module][channel]->GetReverse();
			}
			
			if (channel != 0)
			{
				revRelayVal <<= 1;
			}
			else
			{
				break;
			}
		}
		dashboardPacker.AddU8(revRelayVal);
		
		//printf("Relay reverse sent succesfully \r\n");
		
		/* Read and pack digital IO */
		UINT16 dioVal = 0;
		for (channel = kDigitalChannels; channel > 0; channel--)
		{
			dioVal += m_digitalModules[module]->GetDIO(channel);
			
			if (channel != 1)
			{
				dioVal <<= 1;
			}
		}
		dashboardPacker.AddU16(dioVal);
		
		//printf("DIO sent succesfully \r\n");
		
		dashboardPacker.AddCluster();
		
		/* Read and pack PWM values */
		for(channel = 0; channel < kPwmChannels; channel++)
		{
			if (m_pwms[module][channel] != NULL)
			{
				dashboardPacker.AddU8((UINT8)m_pwms[module][channel]->GetRaw());
			}
			else
			{
				dashboardPacker.AddU8(128);
			}
		}
		
		//printf("PWM sent succesfully \r\n");
		
		dashboardPacker.FinalizeCluster();
		
		dashboardPacker.FinalizeCluster();
	}
	
	/* Read and pack solenoid values */
	UINT8 solenoidVal = 0;
	for (channel = kSolenoidChannels - 1; channel >= 0; channel--)
	{
		if (m_solenoids[channel] != NULL)
		{
			solenoidVal += m_solenoids[channel]->Get();
		}
		
		if (channel != 0)
		{
			solenoidVal <<= 1;
		}
		else
		{
			break;
		}
	}
	dashboardPacker.AddU8(solenoidVal);
	
	//printf("Solenoid sent succesfully \r\n");
	
	
	/* Read and pack joystick values */
	dashboardPacker.AddCluster();
	
	for (UINT8 port = 0; port < kJoystickPorts; port++)
	{
		dashboardPacker.AddCluster();
		
		dashboardPacker.AddFloat(m_joysticks[port]->GetX());
		dashboardPacker.AddFloat(m_joysticks[port]->GetY());
		dashboardPacker.AddFloat(m_joysticks[port]->GetZ());
		dashboardPacker.AddFloat(m_joysticks[port]->GetTwist());
		dashboardPacker.AddFloat(m_joysticks[port]->GetThrottle());
		
		dashboardPacker.AddCluster();
		
		UINT16 buttons = 0;
		for (channel = kJoystickButtons; channel >= 1; channel--)
		{
			buttons += m_joysticks[port]->GetRawButton(channel);
			//printf("Joystick button %d \r\n", channel);
			if (channel != 1)
			{
				buttons <<= 1;
			}
		}
		dashboardPacker.AddU16(buttons);
		
		dashboardPacker.FinalizeCluster();
		
		dashboardPacker.FinalizeCluster();
	}
	dashboardPacker.FinalizeCluster();
	
	/* Read and pack Driver Station Analog Inputs */
	dashboardPacker.AddCluster();
	for (channel = 1; channel <= kDriverStationAnalogIns; channel++)
	{
		dashboardPacker.AddFloat(m_ds->GetAnalogIn(channel));
	}
	dashboardPacker.FinalizeCluster();
	
	/* Flush the data to the driver station. */
	dashboardPacker.Finalize();
	
	/* Update camera state */
	if (m_cameraState != cameraState && m_cameraFeed != NULL)
	{
		if (cameraState)
		{
			m_cameraFeed->Start();
		}
		else
		{
			m_cameraFeed->Stop();
		}
		                                                                                 
		m_cameraState = cameraState;
	}
}

/* Sets all initialized PWMs to 0.0
 */
void HardwareInterface::EmergencyStop()
{
	for (std::list<PWM*>::iterator i = m_emergencyDisableList.begin(); i != m_emergencyDisableList.end(); i++)
	{
		(*i)->SetRaw(127);
	}
}

/* Returns a pointer to a Analog Module in the requested slot.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
AnalogModule* HardwareInterface::GetAnalogModule(UINT8 num)
{
	return m_analogModules[num];
}

/* Returns a pointer to a Digital Module in the requested slot.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
DigitalModule* HardwareInterface::GetDigitalModule(UINT8 num)
{
	return m_digitalModules[num];
}

/* If the requested PWM is not in use, return a pointer to a new
 * Jaguar on that channel, otherwise return NULL.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
Jaguar* HardwareInterface::GetJaguar(UINT8 moduleNum, UINT8 channel)
{
	if (m_pwms[moduleNum][channel - 1] == NULL)
	{
		m_pwms[moduleNum][channel - 1] = new Jaguar(kDigitalSlotNumbers[moduleNum], channel);
			
		m_emergencyDisableList.push_back(m_pwms[moduleNum][channel - 1]);
		
		return (Jaguar*)(m_pwms[moduleNum][channel - 1]);
	}
	else
	{
		return NULL;
	}
}

/* If the requested PWM is not in use, return a pointer to a new
 * Victor on that channel, otherwise return NULL.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
Victor* HardwareInterface::GetVictor(UINT8 moduleNum, UINT8 channel)
{
	if (m_pwms[moduleNum][channel - 1] == NULL)
	{
		m_pwms[moduleNum][channel - 1] = new Victor(kDigitalSlotNumbers[moduleNum], channel);
			
		m_emergencyDisableList.push_back(m_pwms[moduleNum][channel - 1]);
		
		return (Victor*)(m_pwms[moduleNum][channel - 1]);
	}
	else
	{
		return NULL;
	}
}

/* If the requested PWM is not in use, return a pointer to a new
 * Servo on that channel, otherwise return NULL.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
Servo* HardwareInterface::GetServo(UINT8 moduleNum, UINT8 channel)
{
	if (m_pwms[moduleNum][channel - 1] == NULL)
	{
		m_pwms[moduleNum][channel - 1] = new Servo(kDigitalSlotNumbers[moduleNum], channel);
			
		return (Servo*)(m_pwms[moduleNum][channel - 1]);
	}
	else
	{
		return NULL;
	}
}

/* If the requested PWM is not in use, return a pointer to a new
 * PIDJaguar on that channel, otherwise return NULL.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
PIDJaguar* HardwareInterface::GetPIDJaguar(UINT8 moduleNum, UINT8 channel)
{
	if (m_pwms[moduleNum][channel - 1] == NULL)
	{
		m_pwms[moduleNum][channel - 1] = new PIDJaguar(kDigitalSlotNumbers[moduleNum], channel);
		
		m_emergencyDisableList.push_back(m_pwms[moduleNum][channel - 1]);
		
		return (PIDJaguar*)(m_pwms[moduleNum][channel - 1]);
	}
	else
	{
		return NULL;
	}
}

/* If the requested PWM is not in use, return a pointer to a new
 * PIDVictor on that channel, otherwise return NULL.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
PIDVictor* HardwareInterface::GetPIDVictor(UINT8 moduleNum, UINT8 channel)
{
	if (m_pwms[moduleNum][channel - 1] == NULL)
	{
		m_pwms[moduleNum][channel - 1] = new PIDVictor(kDigitalSlotNumbers[moduleNum], channel);
			
		m_emergencyDisableList.push_back(m_pwms[moduleNum][channel - 1]);
		
		return (PIDVictor*)(m_pwms[moduleNum][channel - 1]);
	}
	else
	{
		return NULL;
	}
}

/* If the requested PWM is not in use, return a pointer to a new
 * PIDServo on that channel, otherwise return NULL.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
AdvServo* HardwareInterface::GetAdvServo(UINT8 moduleNum, UINT8 channel)
{
	if (m_pwms[moduleNum][channel - 1] == NULL)
	{
		m_pwms[moduleNum][channel - 1] = new AdvServo(kDigitalSlotNumbers[moduleNum], channel);
			
		return (AdvServo*)(m_pwms[moduleNum][channel - 1]);
	}
	else
	{
		return NULL;
	}
}

/* If the requested Relay is not in use, instantiate it, then return a
 * pointer to the Relay.
 * 
 * Note that the module number is not literal, 0 is the first slot 
 * in kDigitalSlotNumbers, 1 is the second, ect...
 */
SmartRelay* HardwareInterface::GetSmartRelay(UINT8 moduleNum, UINT8 channel)
{
	if (m_smartRelays[moduleNum][channel - 1] == NULL)
	{
		m_smartRelays[moduleNum][channel - 1] = new SmartRelay(kDigitalSlotNumbers[moduleNum], channel);
		
		return m_smartRelays[moduleNum][channel - 1];
	}
	else
	{
		return NULL;
	}
}

/* If the requested Solenoid is not in use, instantiate it, then return a
 * pointer to the Solenoid. 
 */
Solenoid* HardwareInterface::GetSolenoid(UINT8 channel)
{
	if (m_solenoids[channel - 1] == NULL)
	{
		m_solenoids[channel - 1] = new Solenoid(kSolenoidSlotNumber, channel);
		
		return m_solenoids[channel - 1];
	}
	else
	{
		return NULL;
	}
}

/* Returns a pointer to the requested joystick
 */
Joystick* HardwareInterface::GetJoystick(UINT8 port)
{
	return m_joysticks[port - 1];
}

/* Returns a pointer to the Driver Station 
 */
DriverStation* HardwareInterface::GetDriverStation()
{
	return m_ds;
}
