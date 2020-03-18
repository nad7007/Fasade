// **************************************************************************
#pragma once
#ifndef __H_cFRVector_INCLUDED__
#define __H_cFVector_INCLUDED__

#include <float.h>
#include <math.h>
#include <memory.h>

#include <3ADALib\Geom\c3DPoint.h>
#include <3ADALib\VoidValue.h>

// **************************************************************************
class cF3DVector : public cF3DPoint
{
//*******************************************************************
public://Constructors

	cF3DVector(){}

	cF3DVector( const cF3DPoint& _vec ): cF3DPoint( _vec ) 
	{
	}

	cF3DVector( const cF3DVector& _vec )
	{
		(*this) = _vec;
	}

	cF3DVector( double _x, double _y, double  _z)
		: cF3DPoint(float(_x), float(_y), float(_z) )
	{}
	
	cF3DVector( const cD3DPoint& _vec )
	{
		*( (cF3DPoint*)(this) ) = _vec;
	}

//*******************************************************************
public://Operators

	const cF3DVector& cF3DVector::operator = (const cF3DVector& _vec )
	{
		memcpy(m_Arr, _vec.m_Arr, 3*sizeof(float));
		return (*this);
	}

	operator cD3DPoint() const
	{
		return cD3DPoint( x,y,z );
	}

//*******************************************************************
public://Inline Methods

	void	Set( double _x, double _y, double _z )
	{ x = float(_x); y = float(_y); z = float(_z); }

	void	GetArr(float* f) const
		{ memcpy(f, m_Arr, 3*sizeof(float)); }
	
//*******************************************************************
public://Methods

	//free rotation around arbitrary axis U at angle dAlpha
	void	FreeRotate( const cF3DVector& _vecU, float _dAlpha);

	double	GetAngleBetween(const cF3DVector& _vec);

	// export normal as compressed unsigned short
	unsigned short	ExportNormal() const;

	// import normal from compressed unsigned short
	void	ImportNormal( unsigned short _vec );
};
//**************************************************************************

static const cF3DVector X_VECTOR	= fX_VECTOR;
static const cF3DVector Y_VECTOR	= fY_VECTOR;
static const cF3DVector Z_VECTOR	= fZ_VECTOR;
static const cF3DVector ZERO_VECTOR = fZERO_POINT;
static const cF3DVector ZERO_POINT	= fZERO_POINT;

//**************************************************************************
#endif






