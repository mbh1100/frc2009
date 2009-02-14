#include "TrackingCamera.h"

TrackingCamera::TrackingCamera(bool topColorGreen)
{
	/* Set camera and target heights (in inches)*/
	m_cameraHeight = 60;
	m_targetHeight = 66;
	m_colorDistance = 12;
	
	/* Set top color */
	m_topColor = topColorGreen;
	
	/* Color Values */
	m_tdataGreen.hue.minValue = 44;			//135
	m_tdataGreen.hue.maxValue = 136;		//140
	m_tdataGreen.saturation.minValue = 47;	//31.4
	m_tdataGreen.saturation.maxValue = 255;	//29.4
	m_tdataGreen.luminance.minValue = 81;	//39
	m_tdataGreen.luminance.maxValue = 255;	//75.5
	
	m_tdataPink.hue.minValue = 220;
	m_tdataPink.hue.maxValue = 255;
	m_tdataPink.saturation.minValue = 75;
	m_tdataPink.saturation.maxValue = 255;
	m_tdataPink.luminance.minValue = 85;
	m_tdataPink.luminance.maxValue = 255;
	
	m_foundPink = false;
	m_foundGreen = false;
	
	m_minPartToImage = .25;
	m_maxPartToImage = 10.0;
}

TrackingCamera::~TrackingCamera()
{
	
}

bool TrackingCamera::Update()
{
	/* Find green */
	if (FindColor(IMAQ_HSL, &m_tdataGreen.hue, &m_tdataGreen.saturation, &m_tdataGreen.luminance, &m_parGreen)
				&& m_parGreen.particleToImagePercent < m_maxPartToImage
				&& m_parGreen.particleToImagePercent > m_minPartToImage)
	{
		m_foundGreen = true;
		m_greenX = (m_parGreen.center_mass_x_normalized * 1000.0);
		m_greenY = (m_parGreen.center_mass_y_normalized * 1000.0);
	}
	else
	{
		m_foundGreen = false;
	}
	
	/* Find pink */
	if (FindColor(IMAQ_HSL, &m_tdataPink.hue, &m_tdataPink.saturation, &m_tdataPink.luminance, &m_parPink)
				&& m_parPink.particleToImagePercent < m_maxPartToImage
				&& m_parPink.particleToImagePercent > m_minPartToImage) 
	{
		m_foundPink = true;
		m_pinkX = (m_parPink.center_mass_x_normalized * 1000.0);
		m_pinkY = (m_parPink.center_mass_y_normalized * 1000.0);			
	} 
	else
	{
		m_foundPink = false;
	}
	
	/* Determine if pink and green are correctly oriented */
	if (m_foundPink && m_foundGreen)
	{		
		if (m_topColor)
		{
			if (m_greenY < m_pinkY && fabs(m_greenX - m_pinkX) <= kTargetAllowedHorizontalDifference)
			{
				m_targetSize = m_parGreen.particleToImagePercent + m_parPink.particleToImagePercent;
				return true;
			}
			else
			{
				m_targetSize = 0.0;
				return false;
			}
		}
		else
		{
			if (m_greenY > m_pinkY && fabs(m_greenX - m_pinkX) <= kTargetAllowedHorizontalDifference)
			{
				m_targetSize = m_parGreen.particleToImagePercent + m_parPink.particleToImagePercent;
				return true;
			}
			else
			{
				m_targetSize = 0.0;
				return false;
			}
		}
	}
	else
	{
		m_targetSize = 0.0;
		return false;
	}
}

float TrackingCamera::GetTargetX()
{
	/* Get the horizontal center of the target */
	float averageX = (m_greenX + m_pinkX)/2;
	return averageX;
}

float TrackingCamera::GetTargetY()
{
	/* Get the vertical center of the target */
	float y = -(m_greenY+m_pinkY)/2;
	return y;
}

float TrackingCamera::GetTargetSize()
{
	return m_targetSize;
}

bool TrackingCamera::TargetMoving()
{
		
	return false;
}
