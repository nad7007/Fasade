#pragma once
#ifndef __H_cQuaternion_INCLUDED__
#define __H_cQuaternion_INCLUDED__
//**************************************************************************************
#include <3ADALib\Geom\c3DPoint.h>

class cCartesian;

//Provides functionality of unit quaternion representing rotation
//**************************************************************************************
class cQuaternion : public cD3DPoint
{
//**************************************************************************************
public://	Data Members

	double w;//Fourth parameter of quaternion - scale.

//**************************************************************************************
protected://	Constructors

	cQuaternion( double dX, double dY, double dZ, double  dW );
	cQuaternion( const cD3DPoint &r3DPoint, double dW );
	
//**************************************************************************************
public://	Constructors

	cQuaternion();
	cQuaternion( const cQuaternion &Quaternion );

//**************************************************************************************
public://	Non-inline Functions

	//Sets the quaternion.
	//REMARKS: dRotationAngleInRadians and XYZAngle must be in RADIANS.
	//**************************************************************************************
	void Set( const cCartesian &RotMatrix//Matrix of rotation
					  );
	void Set( const cD3DPoint &XYZAngle//Angles of roation around X, Y and Z axes
					  );
	void Set( double dRotationAngleInRadians,//Angle of rotaion around axis in RADIANS.
					   cD3DPoint AxesOfRotation//Axis of rotation.
					  );
	
	//Gets axis and rotation angle in radians.
	//**************************************************************************************
	void GetAxisAndAngle( double &dRotationAngleInRadians,//Returned angle in radians.
								   cD3DPoint &rUnitAxesOfRotation//Returned axes of rotation.
								  )const;
	
	//Gets rotation matrix from this quaternion
	//Does not tuch translation of RTMatrix
	//**************************************************************************************
	void cQuaternion::GetRotationMatrix( cCartesian &RotMatrix );

	//Uses set quaternion to rotate a point.
	//RESULTS: Returns rotated point.
	//**************************************************************************************
	cD3DPoint Rotate( const cD3DPoint &rPoint//The point
								   ) const;
	void Rotate( cD3DPoint &rPoint//The point
							 ) const;
	
	//Normalizes the qauternion.
	//**************************************************************************************
	void Normalize();
	
	//gets norm of the qauternion.
	//RESULTS: Returns norm of the qauternion.
	//**************************************************************************************
	double GetNorm() const;

//**************************************************************************************
public://	Inline overloaded Operators

	//**************************************************************************************
	cQuaternion operator = ( const cQuaternion &Quaternion );
	cQuaternion operator * ( const cQuaternion &Quaternion ) const;
	cQuaternion operator + ( const cQuaternion &Quaternion ) const;
	cQuaternion operator - ( const cQuaternion &Quaternion ) const;
	void operator *= ( const cQuaternion &Quaternion );
	void operator += ( const cQuaternion &Quaternion );
	void operator -= ( const cQuaternion &Quaternion );
	
	//conjugate operator
	cQuaternion operator !() const;
};
//**************************************************************************************
//			Inline functions

//**************************************************************************************
inline cQuaternion cQuaternion::operator = ( const cQuaternion &Quaternion ) 
{
	(cD3DPoint &)(*this) = (cD3DPoint &)Quaternion;
	w = Quaternion.w;

	return (*this);
}
//**************************************************************************************

//**************************************************************************************
inline cQuaternion cQuaternion::operator * ( const cQuaternion &Quaternion ) const 
{
	double A = (w + x) * (Quaternion.w + Quaternion.x);
	double B = (z + y) * (Quaternion.y - Quaternion.z);
	double C = (x - w) * (Quaternion.y + Quaternion.z);
	double D = (y + z) * (Quaternion.x - Quaternion.w);
	double E = (x + z) * (Quaternion.x + Quaternion.y);
	double F = (x - z) * (Quaternion.x - Quaternion.y);
	double G = (w + y) * (Quaternion.w - Quaternion.z);
	double H = (w - y) * (Quaternion.w + Quaternion.z);
	
	return cQuaternion( A - (E + F + G + H)*0.5,
						-C + (E - F + G - H)*0.5,
						-D + (E - F - G + H)*0.5,
						B + ( -E - F + G + H)*0.5 );
}
//**************************************************************************************

//**************************************************************************************
inline void cQuaternion::operator *= ( const cQuaternion &Quaternion )
{
	double A = (w + x) * (Quaternion.w + Quaternion.x);
	double B = (z + y) * (Quaternion.y - Quaternion.z);
	double C = (x - w) * (Quaternion.y + Quaternion.z);
	double D = (y + z) * (Quaternion.x - Quaternion.w);
	double E = (x + z) * (Quaternion.x + Quaternion.y);
	double F = (x - z) * (Quaternion.x - Quaternion.y);
	double G = (w + y) * (Quaternion.w - Quaternion.z);
	double H = (w - y) * (Quaternion.w + Quaternion.z);
	
	x = A - (E + F + G + H)*0.5;
	y = -C + (E - F + G - H)*0.5;
	z = -D + (E - F - G + H)*0.5;
	w = B + (-E - F + G + H)*0.5;
}
//**************************************************************************************

//**************************************************************************************
inline cQuaternion cQuaternion::operator + ( const cQuaternion &Quaternion ) const 
{
	return cQuaternion( (cD3DPoint &)(*this) + (cD3DPoint &)Quaternion, w + Quaternion.w );
}
//**************************************************************************************

//**************************************************************************************
inline void cQuaternion::operator -= ( const cQuaternion &Quaternion )
{
	(cD3DPoint &)(*this) -= (cD3DPoint &)Quaternion;
	w -= Quaternion.w;
}
//**************************************************************************************

//**************************************************************************************
inline cQuaternion cQuaternion::operator - ( const cQuaternion &Quaternion ) const 
{
	return cQuaternion( (cD3DPoint &)(*this) - (cD3DPoint &)Quaternion, w - Quaternion.w );
}
//**************************************************************************************

//**************************************************************************************
inline void cQuaternion::operator += ( const cQuaternion &Quaternion )
{
	(cD3DPoint &)(*this) += (cD3DPoint &)Quaternion;
	w += Quaternion.w;
}
//**************************************************************************************

//**************************************************************************************
inline cQuaternion cQuaternion::operator !() const 
{
	return cQuaternion( -x, -y, -z, w );
}
//**************************************************************************************

//**************************************************************************************
inline void cQuaternion::Set( const cD3DPoint &XYZAngle )
{
	double SRoll = sin(XYZAngle.x*0.5);
	double CRoll = cos(XYZAngle.x*0.5);
	double SPitch  = sin(XYZAngle.y*0.5);
	double CPitch  = cos(XYZAngle.y*0.5);
	double SYaw  = sin(XYZAngle.z*0.5);
	double CYaw  = cos(XYZAngle.z*0.5);

	double CPCY = CPitch * CYaw;
	double SPSY = SPitch * SYaw;

	w = CRoll * CPCY + SRoll * SPSY;
	x = SRoll * CPCY - CRoll * SPSY;
	y = CRoll * SPitch * CYaw + SRoll * CPitch * SYaw;
	z = CRoll * CPitch * SYaw - SRoll * SPitch * CYaw;
}
//**************************************************************************************

//**************************************************************************************
inline cD3DPoint cQuaternion::Rotate( const cD3DPoint &rPoint ) const
{
	double A = (w + x) * rPoint.x;
	double B = (z + y) * (rPoint.y - rPoint.z);
	double C = (x - w) * (rPoint.y + rPoint.z);
	double D = (y + z) * rPoint.x;
	double E = (x + z) * (rPoint.x + rPoint.y);
	double F = (x - z) * (rPoint.x - rPoint.y);
	double G = (w + y) *  -rPoint.z;
	double H = (w - y) * rPoint.z;

	cQuaternion Temp( A - (E + F + G + H) *0.5,
					 -C + (E - F + G - H) *0.5,
					 -D + (E - F - G + H) *0.5,
					  B + (-E - F + G + H)*0.5 );

	Temp = Temp * !(*this);
	return (cD3DPoint &)Temp;
}
//**************************************************************************************

//**************************************************************************************
inline void cQuaternion::Rotate( cD3DPoint &rPoint ) const
{
	double A = (w + x) * rPoint.x;
	double B = (z + y) * (rPoint.y - rPoint.z);
	double C = (x - w) * (rPoint.y + rPoint.z);
	double D = (y + z) * rPoint.x;
	double E = (x + z) * (rPoint.x + rPoint.y);
	double F = (x - z) * (rPoint.x - rPoint.y);
	double G = (w + y) *  -rPoint.z;
	double H = (w - y) * rPoint.z;

	cQuaternion Temp( A - (E + F + G + H) *0.5,
					 -C + (E - F + G - H) *0.5,
					 -D + (E - F - G + H) *0.5,
					  B + (-E - F + G + H)*0.5 );

	rPoint = (cD3DPoint )( Temp * !(*this) );
}
//**************************************************************************************

//**************************************************************************************
inline double cQuaternion::GetNorm() const
{
	return ( GetSquaredLength() + w*w );
}
//**************************************************************************************

//**************************************************************************************
inline void cQuaternion::Normalize()
{
	double dNorm = GetNorm();
	if( dNorm <= DBL_EPSILON )
	{
		w = 1.0;
		Clear();
	}

	double dTmp = 1.0 / GetNorm();
	(cD3DPoint &)(*this) *= dTmp;
	w *= dTmp;
}
//**************************************************************************************
#endif//__H_cQuaternion_INCLUDED__