#ifndef __TRACKINGCAMERA_H__
#define __TRACKINGCAMERA_H__

#include <iostream.h>
#include <taskLib.h>
#include <cmath>
#include "TrackAPI.h"
#include "PIDSource.h"

#define kTargetAllowedHorizontalDifference 50.0

class TrackingCamera
{
public:
	TrackingCamera(bool topColorGreen);
	virtual ~TrackingCamera();
	
	bool Update();
	float GetTargetX();
	float GetTargetY();
	float GetTargetSize();
	
	bool TargetMoving();


protected:
	/* Target & Camera Data */
	float m_cameraHeight, m_targetHeight, m_colorDistance;
	
	/* Tracking Data */
	TrackingThreshold m_tdataGreen, m_tdataPink;
	bool m_topColor, m_foundPink, m_foundGreen;
	ParticleAnalysisReport m_parGreen, m_parPink;
	float m_greenX, m_greenY, m_pinkX, m_pinkY;
	float m_targetSize;
	float m_minPartToImage, m_maxPartToImage;
};

#endif
