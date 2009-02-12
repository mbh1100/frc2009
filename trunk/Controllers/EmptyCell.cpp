#include "EmptyCell.h"

EmptyCell::EmptyCell(Victor* leftMotor, Victor* rightMotor)
{
	m_leftMotor = leftMotor;
	m_rightMotor = rightMotor;
	
	m_release = 0;
	m_loadedLeft = 0;
	m_loadedRight = 0;
	m_centerLeft = 1;
	m_centerRight = 1;
	
	m_counter = 0;
	m_startPoint = 0;
	m_releaseProcess = false;
}

EmptyCell::~EmptyCell()
{
	
}

void EmptyCell::Update(bool release, bool loadedLeft, bool loadedRight, bool limitLeft, bool limitRight, UINT32 counter)
{
	m_release = release;
	m_loadedLeft = loadedLeft;
	m_loadedRight = loadedRight;
	m_centerLeft = limitLeft;
	m_centerRight = limitRight;
	m_counter = counter;
	
	/* Check to see if release button pressed */
	if (m_release)
	{
		m_startPoint = m_counter;
		m_stored = false;
		m_releaseProcess = true;
	}
	
	/* Release the empty cell */
	if (m_releaseProcess)
	{
		Release();
	}
	
	/* If not releasing, check to see if a ball is loaded and not already stored */
	else if ((m_loadedRight || m_loadedLeft) && !m_stored)
	{
		m_startPoint = m_counter;
		m_storeProcess = true;
	}
	
	/* If holding a ball and not stored yet, store it */
	if (m_storeProcess)
	{
		Store();
	}
}

void EmptyCell::Release()
{
	if (m_counter - m_startPoint <= 100)
	{
		m_leftMotor->Set(kEmptyCellSpeed);
		m_rightMotor->Set(kEmptyCellSpeed);
	}
	else if (m_counter - m_startPoint <= 250)
	{
		m_rightMotor->Set(-kEmptyCellSpeed);
		m_leftMotor->Set(-kEmptyCellSpeed);
		
		if (m_centerRight)
		{
			m_rightMotor->Set(0.0);
		}
		if (m_centerLeft)
		{
			m_leftMotor->Set(0.0);
		}
	}
	else
	{			
		m_releaseProcess = false;
	}
}

void EmptyCell::Store()
{
	if (m_counter - m_startPoint <= 50)
	{
		m_rightMotor->Set(-kEmptyCellSpeed);
		m_leftMotor->Set(-kEmptyCellSpeed);
	}
	else
	{
		m_rightMotor->Set(0.0);
		m_leftMotor->Set(0.0);
		m_storeProcess = false;
		m_stored = true;
	}
}
