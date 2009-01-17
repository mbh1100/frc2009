#include "TrackingCamera.h"

TrackingCamera::TrackingCamera(bool topColorGreen)
{
	/* Color Values */
	m_tdataGreen.hue.minValue = 44;
	m_tdataGreen.hue.maxValue = 136;
	m_tdataGreen.saturation.minValue = 47;
	m_tdataGreen.saturation.maxValue = 255;
	m_tdataGreen.luminance.minValue = 81;
	m_tdataGreen.luminance.maxValue = 255;
	
	m_tdataPink.hue.minValue = 220;
	m_tdataPink.hue.maxValue = 255;
	m_tdataPink.saturation.minValue = 75;
	m_tdataPink.saturation.maxValue = 255;
	m_tdataPink.luminance.minValue = 85;
	m_tdataPink.luminance.maxValue = 255;
	
	m_foundPink = false;
	m_foundGreen = false;
	int frameRate = 10, compression = 0;
	ImageSize resolution = k320x240;
	ImageRotation imageRotation = ROT_0;
	
	m_minPartToImage = .25;
	m_maxPartToImage = 10.0;
	
	/* Initialize Camera */
	if (StartCameraTask(frameRate, compression, resolution, imageRotation) == -1)
	{
		printf("Failed to spawn camera task; Error code %s\r\n",GetVisionErrorText(GetLastVisionError()));
	}
}
void TrackingCamera::Update()
{
	if (FindColor(IMAQ_HSL, &m_tdataGreen.hue, &m_tdataGreen.saturation, &m_tdataGreen.luminance, &m_parGreen)
				&& m_parGreen.particleToImagePercent < m_maxPartToImage
				&& m_parGreen.particleToImagePercent > m_minPartToImage)
	{
		m_foundGreen = true;
		m_greenX = (int)(m_parGreen.center_mass_x_normalized * 1000.0);
		m_greenY = (int)(m_parGreen.center_mass_y_normalized * 1000.0);
	}
	else
	{
		m_foundGreen = false;
	}
	if (FindColor(IMAQ_HSL, &m_tdataPink.hue, &m_tdataPink.saturation, &m_tdataPink.luminance, &m_parPink)
				&& m_parPink.particleToImagePercent < m_maxPartToImage
				&& m_parPink.particleToImagePercent > m_minPartToImage) 
	{
		m_foundPink = true;
		m_pinkX = (int)(m_parPink.center_mass_x_normalized * 1000.0);
		m_pinkY = (int)(m_parPink.center_mass_y_normalized * 1000.0);			
	} 
	else
	{
		m_foundPink = false;
	}
}

