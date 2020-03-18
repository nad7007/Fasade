//****************************************************************************
#pragma once 
#include <3ADALib\Geom\c3DPoint.h>

//implementation of F(x) = a0*x*x + 2*a1*x*y + 2*a2*x*z + a3*y*y + 2*a4*y*z + a5*z*z + 2*a6*x + 2*a7*y + 2*a8*z + a9
//****************************************************************************
class cQuadric
{
//****************************************************************************
public:

	cQuadric(){ memset( A, 0, 10* sizeof (A[0]) ); }
	
//****************************************************************************
public:

	bool				IsValid() const 
							{ return (fabs( A[0] ) + fabs( A[1] ) + fabs( A[2] ) + fabs( A[3] ) + fabs( A[4])
									+ fabs( A[5] ) + fabs( A[6] ) + fabs( A[7] ) + fabs( A[8] ) + fabs( A[9] ) ) > FLT_EPSILON; }
	void				Reset() { memset( A, 0, 10* sizeof (A[0]) ); }

	const cQuadric&		operator += ( const cQuadric& ToAdd );

	//get duadric minimum if exist ( no when plane )
	template<typename PlaneT>
	bool				GetMinimum( t3DPoint<PlaneT>& Minimum ) const;

	template<typename PlaneT>
	double				GetDistance( const t3DPoint<PlaneT>& Point )const;

	template<typename PlaneT>
	cD3DPoint			GetNormalAtPoint( const t3DPoint<PlaneT>& Point )const;

	template<typename PlaneT>
	cD3DPoint			GetProjection( const t3DPoint<PlaneT>& Point )const;

	// returns  signed distance to the surface and stores projection of the point
	template<typename PlaneT>
	double				Project( const t3DPoint<PlaneT>& ToProject, cD3DPoint& Projection ) const;

//****************************************************************************
protected:

		double		A[10];
};
//****************************************************************************
//Inlines 

//****************************************************************************
template<typename PlaneT>
inline double cQuadric::GetDistance( const t3DPoint<PlaneT>& Point )const
{
	return	A[0]*Point.x*Point.x + A[3]*Point.y*Point.y + A[5]*Point.z*Point.z + A[9] + 
			2.0*(A[1]*Point.x*Point.y + A[2]*Point.x*Point.z + A[4]*Point.y*Point.z +
				  A[6]*Point.x + A[7]*Point.y + A[8]*Point.z);
}
//****************************************************************************
template<typename PlaneT>
inline cD3DPoint cQuadric::GetNormalAtPoint( const t3DPoint<PlaneT>& Point )const
{
	cD3DPoint Normal;
	Normal.x = (A[0]*Point.x + A[1]*Point.y + A[2]*Point.z + A[6]);
	Normal.y = (A[1]*Point.x + A[3]*Point.y + A[4]*Point.z + A[7]);
	Normal.z = (A[2]*Point.x + A[4]*Point.y + A[5]*Point.z + A[8]);

	Normal.Normalize();
	return Normal;
}
//****************************************************************************
template<typename PlaneT>
inline cD3DPoint cQuadric::GetProjection( const t3DPoint<PlaneT>& Point )const
{
	const double dDist = GetDistance( Point );
	const cD3DPoint Normal = GetNormalAtPoint( Point);
	
	Proj = Point - dDist*Normal;
	return Proj;
}
//****************************************************************************
template<typename PlaneT>
inline double	cQuadric::Project( const t3DPoint<PlaneT>& ToProject, cD3DPoint& Projection ) const
{
	const double dDist = GetDistance( ToProject );
	const cD3DPoint Normal = GetNormalAtPoint( ToProject);
	
	Projection = ToProject - dDist*Normal;

#ifdef _DEBUG
	double dD = GetDistance( Projection );
	ASSERT ( fabs( dD )< FLT_EPSILON  );
#endif

	return dDist;
}
//****************************************************************************
template<typename PlaneT>
inline bool cQuadric::GetMinimum( t3DPoint<PlaneT>& Minimum ) const
{ 
	Minimum.Clear();
	const double A1 = A[1]*A[1];
	const double A2 = A[2]*A[2];
	const double A4 = A[4]*A[4];

	double dDet = A[0]*A[3]*A[5] + 2.0*A[1]*A[4]*A[2] - A[3]*A2 - A[5]*A1 - A[0]*A4;
	if( fabs( dDet ) < 1.e-10) 
		return false;

	dDet = 1.0 / dDet;

	Minimum.x = (A[5]*(A[1]*A[7] - A[6]*A[3]) - A[4]*(A[7]*A[2] + A[1]*A[8]) + A[6]*A4 + A[8]*A[3]*A[2] ) * dDet;
	Minimum.y = (A[0]*(A[8]*A[4] - A[7]*A[5]) - A[2]*(A[8]*A[1] + A[6]*A[4]) + A[7]*A2 + A[5]*A[1]*A[6] ) * dDet;
	Minimum.z = (A[3]*(A[2]*A[6] - A[0]*A[8]) - A[1]*(A[6]*A[4] + A[7]*A[2]) + A[8]*A1 + A[0]*A[4]*A[7] ) * dDet;

	return true;
}
//****************************************************************************
inline	const cQuadric&  cQuadric::operator += ( const cQuadric& ToAdd )
{
	A[0] += ToAdd.A[0];
	A[1] += ToAdd.A[1];
	A[2] += ToAdd.A[2];
	A[3] += ToAdd.A[3];
	A[4] += ToAdd.A[4];
	A[5] += ToAdd.A[5];
	A[6] += ToAdd.A[6];
	A[7] += ToAdd.A[7];
	A[8] += ToAdd.A[8];
	A[9] += ToAdd.A[9];
	return (*this);
}
//****************************************************************************

