#include "HardwareInterface.h"

HardwareInterface::HardwareInterface(bool camera) : m_ds (DriverStation::GetInstance())
{
	/* Wait to ensure camera is initialized */
	Wait(2.0);
	
	if (camera)
	{
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
	
	m_analogModules[0] = AnalogModule::GetInstance(1);
	m_analogModules[1] = AnalogModule::GetInstance(2);
	
	m_digitalModules[0] = DigitalModule::GetInstance(4);
	m_digitalModules[1] = DigitalModule::GetInstance(6);
	
	for (UINT8 i = 0; i < kRelayChannels; i++)
	{
		m_smartRelays[4][i] = new SmartRelay(4, i + 1);
	}
	
	for (UINT8 i = 0; i < kRelayChannels; i++)
	{
		m_smartRelays[6][i] = new SmartRelay(4, i + 1);
	}
	
	for (UINT8 i = 0; i < kSolenoidChannels; i++)
	{
		m_solenoids[i] = new Solenoid(8, i + 1);
	}
	
}

HardwareInterface::~HardwareInterface()
{
	
}

/**
 * Pack data using the correct types and in the correct order to match the
 * default "Dashboard Datatype" in the LabVIEW Dashboard project.
 */
void HardwareInterface::UpdateDashboard(bool cameraState)
{
	Dashboard &dashboardPacker = m_ds->GetDashboardPacker();
	UINT32 module;
	UINT32 channel;

	/* Pack the analog modules */
	for (module = 0; module < kAnalogModules; module++)
	{
		dashboardPacker.AddCluster();
		for (channel = 0; channel < kAnalogChannels; channel++)
		{
			/* dashboardPacker.AddFloat(m_analogChannels[module][channel]); OLD WAY */
			dashboardPacker.AddFloat(m_analogModules[module]->GetAverageValue(channel));
		}
		dashboardPacker.FinalizeCluster();
	}
	/* Pack the digital modules */
	for (module = 0; module < kDigitalModules; module++)
	{
		dashboardPacker.AddCluster();
		dashboardPacker.AddU8(m_relayFwd[module]);
		dashboardPacker.AddU8(m_relayRev[module]);
		dashboardPacker.AddU16(m_dioChannels[module]);
		dashboardPacker.AddU16(m_dioChannelsOutputEnable[module]);
		dashboardPacker.AddCluster();
		
		for(channel = 0; channel < kPwmChannels; channel++)
		{
			dashboardPacker.AddU8(m_pwmChannels[module][channel]);
		}
		dashboardPacker.FinalizeCluster();
		dashboardPacker.FinalizeCluster();
	}
	/* Pack the solenoid module */
	dashboardPacker.AddU8(m_solenoidChannels);

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
