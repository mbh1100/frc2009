#include "HardwareInterface.h"

HardwareInterface::HardwareInterface(bool camera) : m_ds (DriverStation::GetInstance())
{
	/* Slot Numbers for hardware */
	kAnalogSlotNumbers[0] = 1;
	kAnalogSlotNumbers[1] = 2;
	kDigitalSlotNumbers[0] = 4;
	kDigitalSlotNumbers[1] = 6;
	kSolenoidSlotNumber = 8;
	
	if (camera)
	{
		/* Wait to ensure camera is initialized */
		Wait(4.0);
		
		/* Open connection to dashboard comput er then wait for robot to ask for image flow */
		m_cameraFeed = new PCVideoServer();
		m_cameraState = true;
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
			m_pwms[kDigitalSlotNumbers[slot]][channel] = new PWM(kDigitalSlotNumbers[slot], channel + 1);
		}
	}
	
	for (UINT8 slot = 0; slot < kDigitalModules; slot++)
	{
		for (UINT8 channel = 0; channel < kRelayChannels; channel++)
		{
			m_smartRelays[kDigitalSlotNumbers[slot]][channel] = new SmartRelay(kDigitalSlotNumbers[slot], channel + 1);
		}
	}
	
	for (UINT8 channel = 0; channel < kSolenoidChannels; channel++)
	{
		m_solenoids[channel] = new Solenoid(kSolenoidSlotNumber, channel + 1);
	}
	
}

HardwareInterface::~HardwareInterface()
{
	
}

void HardwareInterface::UpdateDashboard(bool cameraState)
{
	Dashboard &dashboardPacker = m_ds->GetDashboardPacker();
	UINT32 module;
	UINT32 channel;

	/* Read and pack the analog modules 
	for (module = 0; module < kAnalogModules; module++)
	{
		dashboardPacker.AddCluster();
		for (channel = 0; channel < kAnalogChannels; channel++)
		{
			dashboardPacker.AddFloat(m_analogModules[module]->GetValue(channel + 1));
		}
		dashboardPacker.FinalizeCluster();
	} */
	
	/* Read and pack the digital modules */
	for (module = 0; module < kDigitalModules; module++)
	{
		dashboardPacker.AddCluster();
		
		/* Read and pack forward relays */
		UINT8 forRelayVal = 0;
		for (channel = kRelayChannels - 1; channel >= 0; channel--)
		{
			forRelayVal += m_smartRelays[module][channel]->GetForward();
			
			if (channel != 0)
			{
				forRelayVal <<= 1;
			}
		}
		dashboardPacker.AddU8(forRelayVal);
		
		/* Read and pack reverse relays */
		UINT8 revRelayVal = 0;
		for (channel = kRelayChannels - 1; channel >= 0; channel--)
		{
			revRelayVal += m_smartRelays[module][channel]->GetReverse();
			
			if (channel != 0)
			{
				revRelayVal <<= 1;
			}
		}
		dashboardPacker.AddU8(revRelayVal);
		
		/* Read and pack digital IO */
		UINT16 dioVal = 0;
		for (channel = kDigitalChannels - 1; channel >= 0; channel--)
		{
			dioVal += m_digitalModules[module]->GetDIO(channel);
			
			if (channel != 0)
			{
				dioVal <<= 1;
			}
		}
		dashboardPacker.AddU16(dioVal);
		
		/* Send a 0 for the digital IO state as software cannot determine it. 
		 * 
		 * TODO: If possible remove this from clusters that dashboard expects
		 * to recieve so it can be removed here*/
		dashboardPacker.AddU16(0);
		
		dashboardPacker.AddCluster();
		
		/* Read and pack PWM values */
		for(channel = 0; channel < kPwmChannels; channel++)
		{
			dashboardPacker.AddU8(m_pwms[module][channel]->GetRaw());
		}
		dashboardPacker.FinalizeCluster();
		
		dashboardPacker.FinalizeCluster();
	}
	
	/* Read and pack solenoid values */
	UINT8 solenoidVal = 0;
	for (channel = kSolenoidChannels - 1; channel >= 0; channel--)
	{
		solenoidVal += m_solenoids[channel]->Get();
		
		if (channel != 0)
		{
			solenoidVal <<= 1;
		}
	}
	dashboardPacker.AddU8(solenoidVal);

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

AnalogModule* HardwareInterface::GetAnalogModule(UINT8 num)
{
	return m_analogModules[num];
}

DigitalModule* HardwareInterface::GetDigitalModule(UINT8 num)
{
	return m_digitalModules[num];
}

Jaguar* HardwareInterface::GetJaguar(UINT8 moduleNum, UINT8 channel)
{
	delete m_pwms[moduleNum][channel - 1];
	m_pwms[moduleNum][channel - 1] = new Jaguar(kDigitalSlotNumbers[moduleNum], channel);
	
	return (Jaguar*)(m_pwms[moduleNum][channel - 1]);
}

Victor* HardwareInterface::GetVictor(UINT8 moduleNum, UINT8 channel)
{
	delete m_pwms[moduleNum][channel - 1];
	m_pwms[moduleNum][channel - 1] = new Victor(kDigitalSlotNumbers[moduleNum], channel);
		
	return (Victor*)(m_pwms[moduleNum][channel - 1]);
}

Servo* HardwareInterface::GetServo(UINT8 moduleNum, UINT8 channel)
{
	delete m_pwms[moduleNum][channel - 1];
	m_pwms[moduleNum][channel - 1] = new Servo(kDigitalSlotNumbers[moduleNum], channel);
		
	return (Servo*)(m_pwms[moduleNum][channel - 1]);
}

SmartRelay* HardwareInterface::GetSmartRelay(UINT8 moduleNum, UINT8 channel)
{
	return m_smartRelays[moduleNum][channel - 1];
}

Solenoid* HardwareInterface::GetSolenoid(UINT8 channel)
{
	return m_solenoids[channel - 1];
}
