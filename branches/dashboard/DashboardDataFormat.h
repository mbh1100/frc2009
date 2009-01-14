#ifndef DASHBOARDFORMAT_H
#define DASHBOARDFORMAT_H

#include "WPILib.h"
#include "PCVideoServer.h"

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
class DashboardDataFormat : public SensorBase
{
public:
	DashboardDataFormat();
	virtual ~DashboardDataFormat();
	
	void PackAndSend(bool cameraState);
	
	PCVideoServer *m_cameraFeed;
	
	bool m_cameraState;
	
	float m_analogChannels[kAnalogModules][kAnalogChannels];
	
	UINT8 m_relayFwd[kDigitalModules];
	UINT8 m_relayRev[kDigitalModules];
	
	UINT16 m_dioChannels[kDigitalModules];
	UINT16 m_dioChannelsOutputEnable[kDigitalModules];
	
	UINT8 m_pwmChannels[kDigitalModules][kPwmChannels];
	UINT8 m_solenoidChannels;
	
private:
	DISALLOW_COPY_AND_ASSIGN(DashboardDataFormat);
	DriverStation *m_ds;
};


#endif // DASHBOARDFORMAT_H
