#ifndef __EMPTYCELL_H__
#define __EMPTYCELL_H__

#include "WPILib.h"

class EmptyCell
{
public:
	EmptyCell(Victor* leftMotor, Victor* rightMotor);
	virtual ~EmptyCell();
	
	void Update(bool release, bool loadedLeft, bool loadedRight, bool limitLeft, bool limitRight, UINT32 counter);
	void Release();
	void Store();
	
protected:
	Victor *m_leftMotor, *m_rightMotor;
	bool m_release, m_loadedLeft, m_loadedRight, m_centerLeft, m_centerRight;
	
	bool m_releaseProcess, m_storeProcess, m_stored;
	
	static const float kEmptyCellSpeed = .2;
	
	UINT32 m_counter;
	UINT32 m_startPoint;
};

#endif
