#pragma once
#ifndef __H_tCone_INCLUDED__
#define __H_tCone_INCLUDED__

#include <3ADALib\Voxelization\cAlignedBBox.h>

class cCartesian;

//*******************************************************************
template< typename Data >
class tCone
{
//*******************************************************************
protected://Data

	t3DPoint<Data>		m_vecApex;
	t3DPoint<Data>		m_vecAxis;
	Data				m_Angle;//radians
	Data				m_Length;

//*******************************************************************
public://Constructor

	tCone(): m_Angle(0), m_Length(0) {}

//*******************************************************************
public://Methods

	bool	IsValid() const 
	{ return m_Angle>FLT_EPSILON && m_Length>FLT_EPSILON && fabs(m_vecAxis.GetLength() - 1.0) < FLT_EPSILON; }
	
	void	Reset()
	{ m_vecApex.Clear(); m_vecAxis.Clear(); m_Angle = m_Length = 0; }

	template <typename PointT>
	void	Set( const t3DPoint<PointT>& _vecApex, const t3DPoint<PointT>& _vecAxis, double _dAngleInRdians, double _dLength )
	{ m_vecApex = _vecApex; m_vecAxis = _vecAxis; m_vecAxis.Normalize(); m_Angle = (Data)_dAngleInRdians; m_Length = (Data) _dLength; }
	
	template <typename PointT>
	bool	Set( const t3DPoint<PointT>& _vecApex, const t3DPoint<PointT>& _vecAxisSecPoint, double _dRadiusAtSecPoint );

	template <typename PointT>
	bool	SetByFrustum ( const t3DPoint<PointT>& _vecAxisFirstPoint, const t3DPoint<PointT>& _vecAxisSecPoint, double _dRadiusAtFirstPoint, double _dRadiusAtSecPoint );

	template <typename PointT>
	void	Transform(const cCartesian& matTR, const t3DPoint<PointT>& vecOrigin)
	{
		cCartesian NewMat = matTR;
		NewMat.t0 -= ( matTR.m[0][0] * vecOrigin.x + matTR.m[0][1] * vecOrigin.y + matTR.m[0][2] * vecOrigin.z );
		NewMat.t1 -= ( matTR.m[1][0] * vecOrigin.x + matTR.m[1][1] * vecOrigin.y + matTR.m[1][2] * vecOrigin.z );
		NewMat.t2 -= ( matTR.m[2][0] * vecOrigin.x + matTR.m[2][1] * vecOrigin.y + matTR.m[2][2] * vecOrigin.z );
		Transform( NewMat );
	}

	void	Transform(const cCartesian& matTR)
	{	matTR.Apply( m_vecApex); matTR.Rotate( m_vecAxis ); }
	
	void	Translate( double _dXOffset, double _dYOffset, double _dZOffset)
	{	m_vecApex += t3DPoint<double>(_dXOffset, _dYOffset, _dZOffset); }

	cF3DExtents				GetExtents() const;

	const t3DPoint<Data>&	Apex() const { return m_vecApex; }
	const t3DPoint<Data>&	Axis() const { return m_vecAxis; }
	const t3DPoint<Data>	AxisEndPoint() const { return m_vecApex + m_Length*m_vecAxis; }
	Data					RadiusAtEndPoint() const { return tan(m_Angle)*m_Length; }
	Data					Angle() const { return m_Angle; }//Angle is in radians
	Data					Length() const { return m_Length; }
	void					SetLength(double dLength ) {m_Length = (Data)dLength; }
	void					SetAngle(double dAngleInRadians) {m_Angle = (Data)dAngleInRadians; }

	//Projections
/*	template <typename InpPointT, typename ResT>
	bool GetProjectionOn( const InpPointT& _Point, ResT& _rProjection ) const
	{ 
		cD3DPoint Tmp;
		double dA = _3DTools::GetProjPointOnLine_Tangent( _Point, m_vecCenter, m_vecAxis,  Tmp );
		if( dA < DBL_EPSILON )
			return false;//many projections 
		if( Dot( (Tmp - m_vecCenter), m_vecAxis ) < 0 )
			return false;//no projection

		double dB = Tmp.GetDistanceTo( m_vecCenter );
		cD3DPoint D(m_vecCenter.X, m_vecCenter.Y, m_vecCenter.Z );
		D += (m_vecAxis * ( dB + m_Radius*dA ) );
		Tmp = D - _Point;
		Tmp.Normalize();
		double dX = ( dA - m_Radius*dB ) / ( nsMath::fsqrt(m_Radius*m_Radius + 1.0) );
		_rProjection.Set( (ResT::DataT)(_Point.X + dX*Tmp.X), (ResT::DataT)(_Point.Y + dX*Tmp.Y), (ResT::DataT)(_Point.Z + dX*Tmp.Z));
		return true;
	}

	template <typename InpPointT >
	double	GetDistanceTo( const InpPointT& _Point ) const
	{ 
		cD3DPoint Tmp;
		double dA = _3DTools::GetProjPointOnLine_Tangent( _Point, m_vecCenter, m_vecAxis,  Tmp );
		double dB = Tmp.GetDistanceTo( m_vecCenter );
		if( dA < DBL_EPSILON )
			return -dB*m_Radius;//many projections 
		if( Dot( (Tmp - m_vecCenter), m_vecAxis ) < 0 )
			return FLT_MAX;//no projection

		return ( dA - m_Radius*dB ) / ( nsMath::fsqrt(m_Radius*m_Radius + 1.0) );
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
		return _3DTools::GetIntLineWithCone( _rLinePoint1, _rLinePoint2, m_vecAxis, m_vecCenter, m_Radius,_rInt1, _rInt2); 
		
	}*/

//*******************************************************************
};
//*******************************************************************
template< typename Data >
template <typename PointT>
bool tCone<Data>::Set( const t3DPoint<PointT>& _vecApex, const t3DPoint<PointT>& _vecEndPoint, double _dRadiusAtSecPoint )
{	
	cD3DPoint Axis( _vecEndPoint.X, _vecEndPoint.Y, _vecEndPoint.Z );
	Axis -= _vecApex;
	double dLength  = Axis.GetSquaredLength();
	if( dLength <= DBL_EPSILON )
		return false;
	double dRad = fabs(_dRadius);
	if( dRad < FLT_EPSILON )
		return false;
	
	m_Length = sqrt( dLength );
	m_vecAxis = Axis / m_Length;
	m_vecApex = _vecApex;
	m_Angle = atan( dRad, m_Length );
	return true;
}
//**************************************************************************************
//**************************************************************************************
template< typename Data >
template <typename PointT>
bool tCone<Data>::SetByFrustum( const t3DPoint<PointT>& _vecAxisFirstPoint, const t3DPoint<PointT>& _vecAxisSecPoint, double _dRadiusAtFirstPoint, double _dRadiusAtSecPoint )
{
	ASSERT( _dRadiusAtFirstPoint != _dRadiusAtSecPoint &&_dRadiusAtSecPoint >=0 &&  _dRadiusAtFirstPoint >=0 );
	if( _dRadiusAtFirstPoint < DBL_EPSILON )
		return Set( _vecAxisFirstPoint, _vecAxisSecPoint, _dRadiusAtSecPoint );
	if( _dRadiusAtSecPoint < DBL_EPSILON )
		return Set( _vecAxisSecPoint, _vecAxisFirstPoint, _dRadiusAtFirstPoint );

	cD3DPoint Axis( _vecEndPoint.X, _vecEndPoint.Y, _vecEndPoint.Z );
	Axis -= _vecAxisFirstPoint;
	Axis.Normalize();

	cD3DPoint Normal = Cross( Axis.GetArbitraryAxis(), Axis );
	Normal.Normalize();

	cD3DPoint P1 = _vecAxisFirstPoint + _dRadiusAtFirstPoint*Normal;
	cD3DPoint P2 = _vecAxisSecPoint + _dRadiusAtSecPoint*Normal;

	const cD3DPoint Axis2 = P2 - P1;

	const double dK3 = Dot(Axis2,Axis); 
	const double dK4 = Axis.GetSquaredLength();
	const double dK5 = Axis2.GetSquaredLength();
	double dDiv = dK4*dK5 - dK3*dK3;
	
	const cD3DPoint Diff( P1.x - _vecAxisFirstPoint.x,
						  P1.y - _vecAxisFirstPoint.y, 
						  P1.z - _vecAxisFirstPoint.z);
	const double dK1 = Dot(Axis,Diff); 
	const double dK2 = Dot(Axis2,Diff); 
	
	dDiv = 1.0 / dDiv;
	const double dTmp1 = ( dK1*dK5 - dK2*dK3) * dDiv;
	const double dTmp2 = ( dK1*dK3 - dK2*dK4) * dDiv;
	cD3DPoint Int1( (_vecAxisFirstPoint.x + Axis.x*dTmp1),
					(_vecAxisFirstPoint.y + Axis.y*dTmp1),
					(_vecAxisFirstPoint.z + Axis.z*dTmp1));
	cD3DPoint Int2( (P1.x + Axis2.x*dTmp2),
					(P1.y + Axis2.y*dTmp2),
					(P1.z + Axis2.z*dTmp2));

	m_vecApex = (Int1 + Int2)*0.5;
	m_vecAxis = Axis;
	m_Length = (_dRadiusAtFirstPoint > _dRadiusAtSecPoint )
		? m_vecApex.GetDistanseTo( _vecAxisSecPoint)
		: m_vecApex.GetDistanseTo( _vecAxisFirstPoint);
	
	m_Angle = atan( __max( _dRadiusAtFirstPoint,_dRadiusAtSecPoint), m_Length );
	return true;
}
//**************************************************************************************
//**************************************************************************************
template< typename Data >
cF3DExtents tCone<Data>::GetExtents() const
{
	const cD3DPoint& vecP1 = Apex();
	const cD3DPoint vecP2 = AxisEndPoint();
	const double dRadius = RadiusAtEndPoint();
	
	const double fMX = dRadius*sqrt( __max( 0 ,1.0 - m_vecAxis.x*m_vecAxis.x));
	const double fMY = dRadius*sqrt( __max( 0 ,1.0 - m_vecAxis.y*m_vecAxis.y));
	const double fMZ = dRadius*sqrt( __max( 0 ,1.0 - m_vecAxis.z*m_vecAxis.z));

	const float fMaxX = (float)__max( vecP1.x, (vecP2.x + fMX) );
	const float fMinX = (float)__min( vecP1.x, (vecP2.x - fMX) );
	const float fMaxY = (float)__max( vecP1.y, (vecP2.y + fMY) );
	const float fMinY = (float)__min( vecP1.y, (vecP2.y - fMY) );
	const float fMaxZ = (float)__max( vecP1.z, (vecP2.z + fMZ) );
	const float fMinZ = (float)__min( vecP1.z, (vecP2.z - fMZ) );
	
	cF3DExtents Ext;
	Ext.Set(fMinX, fMinY, fMinZ, fMaxX, fMaxY, fMaxZ ); 
	return Ext;
}
//**************************************************************************************
#endif//__H_tCone_INCLUDED__