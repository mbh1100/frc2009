#include <iostream.h>
#include <taskLib.h>
#include "math.h"
#include "TrackAPI.h"

class TrackingCamera
{
public:
	TrackingCamera(bool topColorGreen);
	~TrackingCamera();
	
	bool Update();
	float getTargetX();
	float getTargetY();

protected:
	/* Target & Camera Data */
	float m_cameraHeight,m_targetHeight,m_colorDistance;
	
	/* Tracking Data */
	TrackingThreshold m_tdataGreen,m_tdataPink;
	bool m_topColor,m_foundPink,m_foundGreen;
	ParticleAnalysisReport m_parGreen,m_parPink;
	float m_greenX, m_greenY, m_pinkX, m_pinkY;
	float m_minPartToImage,m_maxPartToImage;
};
