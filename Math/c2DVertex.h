#pragma once
#include <3ADALib\Geom\c2DPoint.h>

#define VERTEX_2D_FLAG_SELECTED							0x01
// **************************************************************************
class  c2DVertex: public cF2DPoint
{
// **************************************************************************
public://Data

	cF2DPoint		m_vecNormal;
	//unsigned int	m_uVertexID;
	mutable unsigned int	m_iFlags;

// **************************************************************************
public://Constructors

	c2DVertex():cF2DPoint(0.0f,0.0f),m_iFlags(0)
		{m_vecNormal.Set(1.0f,0.0f); /* m_uVertexID = 0;*/};
	c2DVertex(float fX, float fY):cF2DPoint(fX, fY),m_iFlags(0)
		{m_vecNormal.Set(1.0f,0.0f); /* m_uVertexID = 0;*/};
	c2DVertex(const cF2DPoint& _vecPoint, const cF2DPoint& _vecNormal )
		:cF2DPoint(_vecPoint), m_vecNormal(_vecNormal),m_iFlags(0)
		{/* m_uVertexID = 0;*/};

// **************************************************************************
public://Operators

	const c2DVertex& operator=(const cF2DPoint& vecPoint) 
		{ *(cF2DPoint*)this = vecPoint; return *this;};
	const c2DVertex& operator=(const c2DVertex& vecPoint) 
		{ *(cF2DPoint*)this = vecPoint; m_vecNormal = vecPoint.m_vecNormal; m_iFlags = vecPoint.m_iFlags;/*m_uVertexID = vecPoint.m_uVertexID;*/ return *this;};

public:

	void			ResetFlags() { m_iFlags = 0; };

	void			Select(bool bSelect=true) const  { m_iFlags = bSelect ? (m_iFlags | VERTEX_2D_FLAG_SELECTED) : (m_iFlags &(~VERTEX_2D_FLAG_SELECTED));};
	bool			IsSelected() const { return (m_iFlags & VERTEX_2D_FLAG_SELECTED) != 0; };

	void			SetFlag( unsigned int Flag, bool bSet ) const  { m_iFlags = bSet ? (m_iFlags | Flag) : (m_iFlags &(~Flag));};
	bool			IsFlagSet(unsigned int Flag)const { return ( m_iFlags & Flag ) != 0; };

};
// **************************************************************************



