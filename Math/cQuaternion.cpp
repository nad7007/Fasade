// **************************************************************************
#include <math.h>
#include <float.h>
#include <memory.h>
#include <stdlib.h>
#include <crtdbg.h>

#include "cCartesian.h"
#include "cQuaternion.h"

// **************************************************************************
cQuaternion::cQuaternion()
: w(0.0) {}
// ************************************************************************** 

// **************************************************************************
cQuaternion::cQuaternion( double dX, double dY, double dZ, double dW ) 
: cD3DPoint( dX, dY, dZ), w(dW) {}
// **************************************************************************

// **************************************************************************
cQuaternion::cQuaternion( const cD3DPoint &r3DPoint, double dW )
: cD3DPoint( r3DPoint ), w(dW){}	
// **************************************************************************

// **************************************************************************
cQuaternion::cQuaternion( const cQuaternion &Quaternion )
: cD3DPoint( (cD3DPoint &)Quaternion ), w(Quaternion.w){}
// **************************************************************************

// **************************************************************************
void cQuaternion::Set( double dRotationAngleInRadians, cD3DPoint AxesOfRotation )
{
	AxesOfRotation.Normalize();
	dRotationAngleInRadians *= -0.5;//may be 0.5
	double dSin = sin( dRotationAngleInRadians );

	w = cos( dRotationAngleInRadians );
	((cD3DPoint &)(*this)) = dSin * AxesOfRotation;
	Normalize();
}
// **************************************************************************

// **************************************************************************
void cQuaternion::Set( const cCartesian &RotMatrix )
{
	RotMatrix.GetRotation( w, x, y, z );
}
// **************************************************************************

// **************************************************************************
void cQuaternion::GetRotationMatrix( cCartesian &RotMatrix )
{
	double dNorm = GetNorm();
	double S = ( dNorm > DBL_EPSILON ) ? 1.0/dNorm : 0.0;
	
	//Precalculate coordinate products
	double XS = x * S;
	double YS = y * S;
	double ZS = z * S;
	double WS = w * S;

	RotMatrix.SetRotation( WS, XS, YS, ZS );
}
// **************************************************************************

// **************************************************************************
void cQuaternion::GetAxisAndAngle( double &dRotationAngleInRadians, cD3DPoint &rUnitAxesOfRotation )const
{
	cQuaternion TmpQ(*this);
	TmpQ.Normalize();

	dRotationAngleInRadians = acos ( TmpQ.w );
	double dSin = 1.0/sin( dRotationAngleInRadians );

	rUnitAxesOfRotation = dSin * cD3DPoint(TmpQ) ;
	dRotationAngleInRadians *= 2.0;
}
// **************************************************************************
//End of file
