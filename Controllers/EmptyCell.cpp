#include "EmptyCell.h"

EmptyCell::EmptyCell(Victor* leftMotor, Victor* rightMotor)
{
	m_leftMotor = leftMotor;
	m_rightMotor = rightMotor;
	
	m_release = false;
	m_releaseProcess = false;
	
	m_limitLeftBottom = false;
	m_limitRightBottom = false;
	m_limitLeftTop = false;
	m_limitRightTop = false;
}

EmptyCell::~EmptyCell()
{
	
}

void EmptyCell::Update(bool release, bool limitLeftBottom, bool limitRightBottom, bool limitLeftTop, bool limitRightTop)
{
	m_release = release;
	
	m_limitLeftBottom = limitLeftBottom;
	m_limitRightBottom = limitRightBottom;
	m_limitLeftTop = limitLeftTop;
	m_limitRightTop = limitRightTop;
	
	/* Check to see if release button pressed */
	if (m_release && !m_releaseProcess)
	{
		m_releaseProcess = true;
	}
	
	/* Release the empty cell */
	if (m_releaseProcess)
	{
		if (!m_limitLeftTop)
		{
			m_leftMotor->Set(kEmptyCellSpeed);
		}
		else
		{
			m_leftMotor->Set(0.0);
		}
		if (!m_limitRightTop)
		{
			m_rightMotor->Set(kEmptyCellSpeed);
		}
		else
		{
			m_rightMotor->Set(0.0);
		}
	}
	else
	{
		/* Lower Left Side if not lowered or releasing */
		if (!m_limitLeftBottom)
		{
			m_leftMotor->Set(-kEmptyCellSpeed);
		}
		else
		{
			m_leftMotor->Set(0.0);	
		}
		/* Lower Right Side if not lowered or releasing */
		if (!m_limitRightBottom)
		{
			m_rightMotor->Set(-kEmptyCellSpeed);
		}
		else
		{
			m_rightMotor->Set(-kEmptyCellSpeed);
		}
	}
}
