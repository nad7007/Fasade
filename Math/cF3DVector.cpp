// **************************************************************************
#include <stdlib.h>
#include <memory.h>
#include "cF3DVector.h"
#include "3ADALib\Math\GeomCalculations.h"

// **************************************************************************
void cF3DVector::FreeRotate( const cF3DVector& _vecU, float _dAlpha)
{
	double dCos = cos(_dAlpha);
	double dSin = sin(_dAlpha);
	double dUxSqrd = _3ADA::sqr(_vecU.x);
	double dUySqrd = _3ADA::sqr(_vecU.y);
	double dUzSqrd = _3ADA::sqr(_vecU.z);

	double m00 = dUxSqrd + dCos * (1.0f - dUxSqrd);
	double m11 = dUySqrd + dCos * (1.0f - dUySqrd);
	double m22 = dUzSqrd + dCos * (1.0f - dUzSqrd);
	double m01 = _vecU.x*_vecU.y *(1 - dCos) - _vecU.z * dSin;
	double m10 = _vecU.x*_vecU.y *(1 - dCos) + _vecU.z * dSin;
	double m02 = _vecU.z*_vecU.x *(1 - dCos) + _vecU.y * dSin;
	double m20 = _vecU.z*_vecU.x *(1 - dCos) - _vecU.y * dSin;
	double m12 = _vecU.y*_vecU.z *(1 - dCos) - _vecU.x * dSin;
	double m21 = _vecU.y*_vecU.z *(1 - dCos) + _vecU.x * dSin;

	double dx = m00 * x + m01 * y + m02 * z;
	double dy = m10 * x + m11 * y + m12 * z;
	double dz =	m20 * x + m21 * y + m22 * z;

	x = (float)dx;
	y = (float)dy;
	z = (float)dz;
}
// **************************************************************************
// **************************************************************************
double cF3DVector::GetAngleBetween(const cF3DVector& _vec)
{
	return _3ADA::GetAngle( (*this), _vec);
}
//**************************************************************************
//**************************************************************************
unsigned short cF3DVector::ExportNormal()const
{
	const double SC_PI =  3.141592653589793;

	double a = atan2( z, sqrt( x * x + y * y ) );
	double b = atan2( y, x );

	
	a = a / SC_PI * 127.0;
	b = b / SC_PI * 127.0;
		
	unsigned short xx = ( ( short )a  << 8 ) | ( ( char )b & 0xFF );	
	return xx;
}
// **************************************************************************
// **************************************************************************
#pragma warning( push )
#pragma warning( disable : 4244 )// conversion from 'int' to 'char'
void cF3DVector::ImportNormal( unsigned short _vec )
{
	const double SC_PI =  3.141592653589793;

	char cb = _vec & 0xFF;
	char ca = ( _vec & 0xFF00 ) >> 8;

	double a = ( double )ca;
	double b = ( double )cb;

	b = b * SC_PI / 127.0;
	a = a * SC_PI / 127.0;

	x = ( float )( cos( b ) * cos( a ) );
	y = ( float )( sin( b ) * cos( a ) );
	z = ( float )( sin( a ) );	
}
#pragma warning(pop)
// **************************************************************************
