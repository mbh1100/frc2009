#include "DashboardDataFormat.h"

DashboardDataFormat::DashboardDataFormat() : m_ds (DriverStation::GetInstance())
{
	/* Wait to ensure camera is initialized */
	Wait(2.0);
	
	/* Open connection to dashboard comput er then wait for robot to ask for image flow */
	m_cameraFeed = new PCVideoServer();
	//m_cameraFeed->Stop();
	
	m_cameraState = true;
}

DashboardDataFormat::~DashboardDataFormat()
{
	
}

/**
 * Pack data using the correct types and in the correct order to match the
 * default "Dashboard Datatype" in the LabVIEW Dashboard project.
 */
void DashboardDataFormat::PackAndSend(bool cameraState)
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
			dashboardPacker.AddFloat(m_analogChannels[module][channel]);
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
	if (m_cameraState != cameraState)
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
