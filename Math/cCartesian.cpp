// **************************************************************************
#include "stdafx.h"

#include <float.h>
#include <memory.h>
#include "cCartesian.h"

// **************************************************************************
void cCartesian::FreeRotate(const c3DPointD& _vecU, double _dAlpha)
{
	cCartesian matRot;
	matRot.Identity();
	double dCos = cos(_dAlpha);
	double dSin = sin(_dAlpha);
	double dUxSqrd = (_vecU.x*_vecU.x);
	double dUySqrd = (_vecU.y*_vecU.y);
	double dUzSqrd = (_vecU.z*_vecU.z);

	matRot.oX0 = dUxSqrd + dCos * (1.0f - dUxSqrd);
	matRot.oY1 = dUySqrd + dCos * (1.0f - dUySqrd);
	matRot.oZ2 = dUzSqrd + dCos * (1.0f - dUzSqrd);
	matRot.oY0 = _vecU.x*_vecU.y *(1 - dCos) - _vecU.z * dSin;
	matRot.oX1 = _vecU.x*_vecU.y *(1 - dCos) + _vecU.z * dSin;
	matRot.oZ0 = _vecU.z*_vecU.x *(1 - dCos) + _vecU.y * dSin;
	matRot.oX2 = _vecU.z*_vecU.x *(1 - dCos) - _vecU.y * dSin;
	matRot.oZ1 = _vecU.y*_vecU.z *(1 - dCos) - _vecU.x * dSin;
	matRot.oY2 = _vecU.y*_vecU.z *(1 - dCos) + _vecU.x * dSin;

	*this = matRot * (*this);	
}
// **************************************************************************

// **************************************************************************
void cCartesian::RotateInDirection(double dAngle, const c3DPointD& vecDirection)
{
	c3DPointD V = vecDirection;
	V.Normalize();

	// save the translation
	c3DPointD Translation(t0, t1, t2);
	t0=t1=t2=0.0;

	double SinAngle = sin( dAngle );
	double CosAngle = cos( dAngle );

	double VerCos = 1 - CosAngle;
	double VXY = V.x*V.y*VerCos;
	double VYZ = V.y*V.z*VerCos;
	double VXZ = V.x*V.z*VerCos;
	double XS = V.x*SinAngle;
	double YS = V.y*SinAngle;
	double ZS = V.z*SinAngle;

	cCartesian R; // rotation matrix
	R.Identity();

	R.oX0 = (V.x*V.x*VerCos + CosAngle);
	R.oY0 = (VXY - ZS);
	R.oZ0 = (VXZ + YS);

	R.oX1 = (VXY + ZS);
	R.oY1 = (V.y*V.y*VerCos + CosAngle); 
	R.oZ1 = (VYZ - XS);

	R.oX2 = (VXZ - YS); 
	R.oY2 = (VYZ + XS); 
	R.oZ2 = (V.z*V.z*VerCos + CosAngle);

	(*this) = R*(*this);

	// restore the translation
	t0=Translation.x;
	t1=Translation.y;
	t2=Translation.z;

}
// **************************************************************************

// **************************************************************************
void cCartesian::SetRotation( double dRotationAngleInRadians, const c3DPointD &rUnitAxesOfRotation )
{//Rodrigues' rotation formula 
	const double dSin = sin( dRotationAngleInRadians );
	const double dCos = cos( dRotationAngleInRadians );
	const double d1_Cos = 1.0 - dCos;

	const double dX2 = rUnitAxesOfRotation.x*rUnitAxesOfRotation.x;
	const double dY2 = rUnitAxesOfRotation.y*rUnitAxesOfRotation.y;
	const double dZ2 = rUnitAxesOfRotation.z*rUnitAxesOfRotation.z;

	const double dXY = rUnitAxesOfRotation.x*rUnitAxesOfRotation.y*d1_Cos;
	const double dXZ = rUnitAxesOfRotation.x*rUnitAxesOfRotation.z*d1_Cos;
	const double dYZ = rUnitAxesOfRotation.y*rUnitAxesOfRotation.z*d1_Cos;

	const double dXSin = rUnitAxesOfRotation.x*dSin;
	const double dYSin = rUnitAxesOfRotation.y*dSin;
	const double dZSin = rUnitAxesOfRotation.z*dSin;

	m[0][0] = dX2*d1_Cos + dCos;
	m[1][1] = dY2*d1_Cos + dCos;
	m[2][2] = dZ2*d1_Cos + dCos;

	m[0][1] = dXY - dZSin;
	m[1][0] = dXY + dZSin;

	m[0][2] = dXZ + dYSin;
	m[2][0] = dXZ - dYSin;

	m[1][2] = dYZ - dXSin;
	m[2][1] = dYZ + dXSin;
}
// **************************************************************************

// **************************************************************************
/*void cCartesian::SetRotation( const c3DPointD &_GlobNormal, const c3DPointD &_LocalNormal )
{
	c3DPointD Axis = Cross( _LocalNormal , _GlobNormal);
	if( Axis.GetSquaredLength() < 1e-5 )
		return;
	Axis.Normalize();
	double dAngle = _3ADA::GetSignedAngle( _LocalNormal, _GlobNormal, Axis );
	SetRotation( dAngle, Axis );
}*/
// **************************************************************************

// **************************************************************************
void cCartesian::SetRotation( const c3DPointD &_NormalX, const c3DPointD &_NormalY, const c3DPointD &_NormalZ )
{
	m[0][0] = _NormalX.x;
	m[1][0] = _NormalX.y;
	m[2][0] = _NormalX.z;

	m[0][1] = _NormalY.x;
	m[1][1] = _NormalY.y;
	m[2][1] = _NormalY.z;

	m[0][2] = _NormalZ.x;
	m[1][2] = _NormalZ.y;
	m[2][2] = _NormalZ.z;
}
// **************************************************************************

// **************************************************************************
void cCartesian::SetRotation( const c3DPointD &_Normal1, unsigned AxisInd1, const c3DPointD &_Normal2, unsigned AxisInd2 )
{
	_ASSERT( AxisInd1 >=0 && AxisInd1 < 3 );
	_ASSERT( AxisInd2 >=0 && AxisInd2 < 3 );
	
	c3DPointD N1 = _Normal1;
	N1.Normalize();

	c3DPointD N2 = _Normal2;
	N2.Normalize();

	//set first normal
	m[0][AxisInd1] = N1.x;
	m[1][AxisInd1] = N1.y;
	m[2][AxisInd1] = N1.z;


	//set second normal
	m[0][AxisInd2] = N2.x;
	m[1][AxisInd2] = N2.y;
	m[2][AxisInd2] = N2.z;
	
	c3DPointD Normal3 = ( AxisInd1 == (AxisInd2 + 1 ) % 3 )
		? Cross( _Normal2, _Normal1 )
		: Cross( _Normal1, _Normal2 );
	Normal3.Normalize();

	int iInd3 = 3 - AxisInd1 - AxisInd2;
	_ASSERT( iInd3 >=0 && iInd3 < 3 );

	m[0][iInd3] = Normal3.x;
	m[1][iInd3] = Normal3.y;
	m[2][iInd3] = Normal3.z;
}
// **************************************************************************

// **************************************************************************
void cCartesian::SetZYXRotation( double _dX, double _dY, double _dZ )
{
	double SRoll = sin(_dX);
	double CRoll = cos(_dX);
	double SPitch  = sin(_dY);
	double CPitch  = cos(_dY);
	double SYaw  = sin(_dZ);
	double CYaw  = cos(_dZ);

	double SRSP = SRoll * SPitch;
	double CRSP = CRoll * SPitch;
	
	m[0][0] = CPitch * CYaw;
	m[0][1] =-CPitch * SYaw;
	m[0][2] = SPitch;

	m[1][0] = CRoll * SYaw + SRSP * CYaw;
	m[1][1] = CRoll * CYaw - SRSP * SYaw;
	m[1][2] =-SRoll * CPitch;

	m[2][0] = SRoll * SYaw - CRSP * CYaw;
	m[2][1] = SRoll * CYaw + CRSP * SYaw;
	m[2][2] = CRoll * CPitch;
}
// **************************************************************************

// **************************************************************************
void cCartesian::GetZYX( float& _x, float& _y, float& _z ) const
{	
	GetZYX1( _x, _y, _z );	
}
// **************************************************************************

// **************************************************************************
void cCartesian::GetZYX1( float& _x, float& _y, float& _z ) const
{
	float psi = ( float )atan2( oX1, oX0 );
	float thi = ( float )atan2( -oX2, oX0 * cos( psi ) + oX1 * sin( psi ) );
	float theta = ( float )atan2( oZ0 * sin( psi ) - oZ1 * cos( psi ), 
		-oY0 * sin( psi ) + oY1 * cos( psi ) );
	
	_x = theta;
	_y = thi;
	_z = psi;
}
// **************************************************************************

// **************************************************************************
/*void cCartesian::GetZYX2( float& _x, float& _y, float& _z ) const
{
	double psi = atan2( oX1, oX0 );

	psi += RAD( 180.0f );

	while( psi > RAD( 180.0f ) )
		psi -= RAD( 360.0f );

	double thi = atan2( (double)-oX2, oX0 * cos( psi ) + oX1 * sin( psi ) );
	double theta = atan2( oZ0 * sin( psi ) - oZ1 * cos( psi ), 
		-oY0 * sin( psi ) + oY1 * cos( psi ) );
	
	_x = (float)theta;
	_y = (float)thi;
	_z = (float)psi;
}
*/
// **************************************************************************

// **************************************************************************
void cCartesian::GetRotation( double &dRotationAngleInRadians, c3DPointD &rUnitAxesOfRotation )const
{
	double dCosAng = ( GetTrace() - 1.0 ) * 0.5;
	_ASSERT( dCosAng <= 1.0 && dCosAng >= -1.0 );
	dRotationAngleInRadians = acos( __min( 1.0, __max( dCosAng, -1.0) ) );

	if( fabs( dRotationAngleInRadians ) <= DBL_EPSILON )
	{//no rotation
		rUnitAxesOfRotation.Set(1.0, 0.0, 0.0 );
		return;
	}

	if( fabs( dRotationAngleInRadians - PI ) <= DBL_EPSILON )
	{//180 rotation
		if( m[0][0] >= m[1][1] &&  m[0][0] >= m[2][2] )
		{
			rUnitAxesOfRotation.x = sqrt( m[0][0]*m[0][0] - m[1][1]*m[1][1] - m[2][2]*m[2][2] + 1 ) * 0.5;
			_ASSERT( rUnitAxesOfRotation.x >= DBL_EPSILON );
			const double dDel = 1.0/(2.0*rUnitAxesOfRotation.x);
			rUnitAxesOfRotation.y = m[0][1]*dDel;
			rUnitAxesOfRotation.z = m[0][2]*dDel;
		}
		else if( m[1][1] >= m[0][0] &&  m[1][1] >= m[2][2] ) 
		{
			rUnitAxesOfRotation.y = sqrt( m[1][1]*m[1][1] - m[0][0]*m[0][0] - m[2][2]*m[2][2] + 1 ) * 0.5;
			_ASSERT( rUnitAxesOfRotation.y >= DBL_EPSILON );
			const double dDel = 1.0/(2.0*rUnitAxesOfRotation.y);
			rUnitAxesOfRotation.x = m[0][1]*dDel;
			rUnitAxesOfRotation.z = m[1][2]*dDel;
		}
		else
		{
			rUnitAxesOfRotation.z = sqrt( m[2][2]*m[2][2] - m[0][0]*m[0][0] - m[1][1]*m[1][1] + 1 ) * 0.5;
			_ASSERT( rUnitAxesOfRotation.z >= DBL_EPSILON );
			const double dDel = 1.0/(2.0*rUnitAxesOfRotation.z);
			rUnitAxesOfRotation.x = m[0][2]*dDel;
			rUnitAxesOfRotation.y = m[1][2]*dDel;
		}
		return;
	}

	//random rotation
	rUnitAxesOfRotation.x = (m[2][1] - m[1][2]);
	rUnitAxesOfRotation.y = (m[0][2] - m[2][0]);
	rUnitAxesOfRotation.z = (m[1][0] - m[0][1]);
	rUnitAxesOfRotation.Normalize();
}

// **************************************************************************
//End of file
