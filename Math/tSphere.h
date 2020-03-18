#pragma once
#ifndef __H_tSphere_INCLUDED__
#define __H_tSphere_INCLUDED__

#include <3ADALib\Voxelization\cAlignedBBox.h>
class cCartesian;

//*******************************************************************
template< typename Data >
class tSphere
{
//*******************************************************************
protected://Data

	t3DPoint<Data>		m_vecCentre;
	Data				m_Radius;

//*******************************************************************
public://Constructor

	tSphere() : m_Radius(0.0){}

//*******************************************************************
public://Methods

	bool			IsValid()  const
	{ return m_Radius > FLT_EPSILON; }

	void			Reset() 
	{ m_vecCentre.Clear(); m_Radius = 0.0;}

	template <typename PointT>
	bool			Set(	const t3DPoint<PointT>& _rPoint1, const t3DPoint<PointT>& _rPoint2,
					const t3DPoint<PointT>& _rPoint3, const t3DPoint<PointT>& _rPoint4 );
	
	template <typename PointT>
	void			Set( const t3DPoint<PointT>& vecC, double dR )
	{ m_vecCentre = vecC, m_Radius = dR; }

	cF3DExtents		GetExtents() const
	{ 	cF3DExtents Extents((float)(m_vecCentre.x-m_Radius), (float)(m_vecCentre.y-m_Radius), (float)(m_vecCentre.z-m_Radius),
							(float)(m_vecCentre.x+m_Radius), (float)(m_vecCentre.y+m_Radius), (float)(m_vecCentre.z+m_Radius) );
		return Extents;
	}

	template <typename PointT>
	void			Transform(const cCartesian& matTR, const t3DPoint<PointT>& vecOrigin)
	{
		cCartesian NewMat = matTR;
		NewMat.t0 -= ( matTR.m[0][0] * vecOrigin.x + matTR.m[0][1] * vecOrigin.y + matTR.m[0][2] * vecOrigin.z );
		NewMat.t1 -= ( matTR.m[1][0] * vecOrigin.x + matTR.m[1][1] * vecOrigin.y + matTR.m[1][2] * vecOrigin.z );
		NewMat.t2 -= ( matTR.m[2][0] * vecOrigin.x + matTR.m[2][1] * vecOrigin.y + matTR.m[2][2] * vecOrigin.z );
		Transform( NewMat );
	}

	void			Transform(const cCartesian& matTR)
	{	matTR.Apply( m_vecCentre ); }

	void			Translate( double _XOffset, double _YOffset, double _ZOffset)
	{	m_vecCentre += t3DPoint<double>(_dXOffset, _dYOffset, _dZOffset); }

	const t3DPoint<Data>&	Center() const { return m_vecCentre; }
	const Data&				Radius() const { return m_Radius; }
	void					SetRadius(double dValue) { m_Radius = (Data)dValue; }
	double					GetArea() const { return 4.0*m_Radius*m_Radius*PI; }
	double					GetVolume()	const { return (4.0*m_Radius*m_Radius*m_Radius*PI)/3.0; }

//*******************************************************************
};

//*******************************************************************
template< typename Data > 
template <typename PointT>
bool tSphere<Data>::Set(const t3DPoint<PointT>& _rPoint1, const t3DPoint<PointT>& _rPoint2,
						const t3DPoint<PointT>& _rPoint3, const t3DPoint<PointT>& _rPoint4 )
{
	double VecTmp[3];
	const t3DPoint<PointT>* Points[4] = 
	{
		&_rPoint1,
		&_rPoint2,
		&_rPoint3,
		&_rPoint4,
	};

	cLinSys System(3);
	if (!System )
		return false; // Out of mem error

	for( int i=0; i<3; ++i )
	{
		double dRes = 0;
        for( int j=0; j<3; ++j )
		{
			VecTmp[j] = Points[i]->m_Arr[j] - Points[3]->m_Arr[j];
			dRes += ( nsMath::sqr(Points[i]->m_Arr[j]) - nsMath::sqr(Points[3]->m_Arr[j]) );
		}
		dRes *= 0.5;
		System.SetEquation( i, VecTmp, dRes );
	}
	if ( !System.FindGaussSolution( VecTmp, FLT_EPSILON ) )
	{
		_ASSERT(false);
		return false; // Some points coincide - transormation undefined
	}

	double dRad = fabs( nsMath::fsqrt( nsMath::sqr(VecTmp[0] - _rPoint4.X)
								+ nsMath::sqr(VecTmp[1] - _rPoint4.Y)
								+ nsMath::sqr(VecTmp[2] - _rPoint4.Z)));
	if( dRad < FLT_EPSILON )
		return false;

	m_vecCentre.Set( Data( VecTmp[0] ), Data( VecTmp[1] ), Data( VecTmp[2] ) );
	m_Radius = (Data)dRad;
	return true;
}
//**************************************************************************************
#endif//__H_tSphere_INCLUDED__