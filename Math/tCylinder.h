#pragma once
#ifndef __H_tCylinder_INCLUDED__
#define __H_tCylinder_INCLUDED__

#include <3ADALib\Voxelization\cAlignedBBox.h>

class cCartesian;

//*******************************************************************
template< typename Data >
class tCylinder
{
//*******************************************************************
protected://Data

	t3DPoint<Data>		m_vecPoint;
	t3DPoint<Data>		m_vecAxis;
	Data				m_Radius;
	Data				m_Length;

//*******************************************************************
public://Constructor

	tCylinder(): m_Radius(0), m_Length(0) {}

//*******************************************************************
public://Methods

	bool			IsValid() const 
	{ return m_Radius>FLT_EPSILON && m_Length>FLT_EPSILON && fabs(m_vecAxis.GetLength() - 1.0) < FLT_EPSILON; }
	
	void			Reset()
	{ m_vecPoint.Clear(); m_vecAxis.Clear(); m_Radius = m_Length = 0; }

	template <typename PointT>
	bool			Set( const t3DPoint<PointT>& _Point1, const t3DPoint<PointT>& _Point2, double _dRadius );

	template <typename PointT>
	void			Set( const t3DPoint<PointT>& _Point, const t3DPoint<PointT>& _Axis, double _dRadius, double dLength )
	{ m_vecPoint = _Point; m_vecAxis = _Axis; m_vecAxis.Normalize(); m_Radius = (Data)_dRadius; m_Length = (Data)dLength; }
	
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
	{	matTR.Apply( m_vecPoint ); matTR.Rotate( m_vecAxis ); }
	
	void			Translate( double _dXOffset, double _dYOffset, double _dZOffset)
	{	m_vecCentre += t3DPoint<double>(_dXOffset, _dYOffset, _dZOffset); }

	void			ReverseAxis()
	{ _ASSERT( IsValid() ); m_vecPoint += m_vecAxis*m_Length; m_vecAxis = -m_vecAxis; }

	cF3DExtents		GetExtents() const;

	const t3DPoint<Data>&	StartPoint() const { return m_vecPoint; }
	const t3DPoint<Data>	EndPoint() const { return m_vecPoint +  m_vecAxis*m_Length; }
	const t3DPoint<Data>&	Axis() const { return m_vecAxis; }
	const Data&				Radius() const { return m_Radius; }
	Data					Length() const { return m_Length; }
	void					SetRadius(double dRadius ) {m_Radius = (Data)dRadius; }
	void					SetLength(double dLength ) {m_Length = (Data)dLength; }

	double					GetVolume()	const	
	{ _ASSERT( IsValid() );return m_Radius*m_Radius*m_Length*PI; }
	double					GetPerimeter()	const
	{ _ASSERT( IsValid() ); return 2.0*PI*m_Radius; }

	/*//Projections
	template <typename InpPointT, typename ResT>
	bool GetProjectionOn( const InpPointT& _Point, ResT& _rProjection ) const
	{ 
		t3DPoint<double> Tmp;
		double Dist = _3DTools::GetProjPointOnLine_Tangent( _Point, m_vecPoint, m_vecAxis, Tmp );
		if( Dist < DBL_EPSILON )
			return false;//many projections
		Tmp += ( (_Point - Tmp) * (m_Radius / Dist) ); 
		_rProjection.Set( ResT::DataT(Tmp.X), ResT::DataT(Tmp.Y), ResT::DataT(Tmp.Z) );
		return true;
	}

	template <typename InpPointT >
	double	GetDistanceTo( const InpPointT& _Point ) const
	{ 
		return _3DTools::GetDistPointToLine_Tangent( _Point, m_vecPoint, m_vecAxis) - m_Radius;
	}

	template <typename InpPointT, typename ResT>
	bool	GetDistanceVectorTo( const InpPointT& _Point, ResT& _rDistanceVector )const
	{
		if( !GetProjectionOn(_Point, _rDistanceVector) )
			return false;
		_rDistanceVector -= _Point; 
		return true;
	}

	//Intersections
	template <typename InpPointT, typename ResT>
	int GetIntersectionWithLine(const InpPointT& _rLinePoint1, const InpPointT& _rLinePoint2, ResT& _rInt1, ResT& _rInt2 ) const
	{
		return _3DTools::GetIntCylinderWithLine(m_vecPoint, m_vecAxis, m_Radius, _rLinePoint1, _rLinePoint2, _rInt1, _rInt2 );
	}

	bool GetIntersectionWithLine(const PointT& _rLinePoint1, const PointT& _rLinePoint2, PointT& _rIntersection ) const
	{
		PointT P1, P2;
		PointT Norm(_rLinePoint2 - _rLinePoint1);
		Norm.Normalize();
		if( !_3DTools::GetIntLineWithLine_Tangent( m_vecPoint, m_vecAxis, _rLinePoint1, Norm, P1, P2) )
			return false;
		_rIntersection = (P1 + P2) * 0.5;
		return true;
	}
	bool GetIntersectionWithPlane(const PointT& _rPlaneNormal, double _dOffset, PointT& _rIntersection )const
	{
		return _3DTools::GetIntLineWithPlane(	m_vecPoint, m_vecPoint + m_vecAxis, _rPlaneNormal, _dOffset, _rIntersection );
	}*/

//*******************************************************************
};

//*******************************************************************
template< typename Data >
template <typename PointT>
bool tCylinder<Data>::Set( const t3DPoint<PointT>& _Point1, const t3DPoint<PointT>& _Point2, double _dRadius )
{	
	t3DPoint<PointT> Axis( _Point2.x, _Point2.y, _Point2.z );
	
	Axis -= _Point1;
	double dLen = Axis.GetLength();
	if( dLen <= FLT_EPSILON )
		return false;
	double dRad = fabs(_dRadius);
	if( dRad < FLT_EPSILON )
		return false;
	Axis /= dLen;

	m_vecAxis = Axis;
	m_vecPoint = _Point1;
	m_Radius = (Data)_dRadius;
	m_Length = dLen;
	return true;
}

//*******************************************************************
//*******************************************************************
template< typename Data >
cF3DExtents tCylinder<Data>::GetExtents() const
{
	const double fX = m_Radius*sqrt( __max( 0 ,1.0 - m_vecAxis.x*m_vecAxis.x) );
	const double fY = m_Radius*sqrt( __max( 0 ,1.0 - m_vecAxis.y*m_vecAxis.y) );
	const double fZ = m_Radius*sqrt( __max( 0 ,1.0 - m_vecAxis.z*m_vecAxis.z) );

	const float fMaxX = (float)__max( (m_vecPoint.x + fX), (m_vecPoint.x + m_vecAxis.x*m_Length + fX) );
	const float fMinX = (float)__min( (m_vecPoint.x - fX), (m_vecPoint.x + m_vecAxis.x*m_Length - fX) );
	const float fMaxY = (float)__max( (m_vecPoint.y + fY), (m_vecPoint.y + m_vecAxis.y*m_Length + fY) );
	const float fMinY = (float)__min( (m_vecPoint.y - fY), (m_vecPoint.y + m_vecAxis.y*m_Length - fY) );
	const float fMaxZ = (float)__max( (m_vecPoint.z + fZ), (m_vecPoint.z + m_vecAxis.z*m_Length + fZ) );
	const float fMinZ = (float)__min( (m_vecPoint.z - fZ), (m_vecPoint.z + m_vecAxis.z*m_Length - fZ) );
	cF3DExtents Ext;
	Ext.Set(fMinX, fMinY, fMinZ, fMaxX, fMaxY, fMaxZ ); 
	return Ext;
}
//**************************************************************************************
#endif//__H_tCylinder_INCLUDED__