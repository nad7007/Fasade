#pragma once
#include"Math\cCartesian.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cHole
{
public:

	cHole(double dLeftX, double dRightX, double dTopY, double dBottomY)
		: m_dLeft(dLeftX)
		,	m_dRight(dRightX)
		,m_dTop(dTopY)
		,m_dBottom(dBottomY)
	{}

	virtual ~cHole()
	{}

	bool IsIn(const c2DPointD& pointToCheck) const
	{
		return pointToCheck.x > m_dLeft && pointToCheck.x < m_dRight && pointToCheck.y > m_dTop && pointToCheck.y < m_dBottom;
	}

private:

	double m_dLeft;
	double m_dRight;
	double m_dTop;
	double m_dBottom;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
