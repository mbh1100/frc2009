#include <iostream.h>
#include <taskLib.h>
#include "math.h"

#include "TrackAPI.h"

class TrackingCamera
{
public:
	TrackingCamera(bool topColorGreen);
	~TrackingCamera();
	
	void Update();

protected:	
	/* Tracking Data */
	TrackingThreshold m_tdataGreen,m_tdataPink;
	bool m_foundPink,m_foundGreen;
	ParticleAnalysisReport m_parGreen,m_parPink;
	int m_greenX, m_greenY, m_pinkX, m_pinkY;
	float m_minPartToImage,m_maxPartToImage;
};
