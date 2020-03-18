#pragma once
#ifndef __H_tPlane_INCLUDED__
#define __H_tPlane_INCLUDED__

#include <3ADALib\Voxelization\cAlignedBBox.h>

class cCartesian;

//*******************************************************************
template< typename Data >
class tPlane
{
//*******************************************************************
protected://Data

	t3DPoint<Data>		m_vecNormal;
	t3DPoint<Data>		m_vecCentre;
	t3DPoint<Data>		m_vecTangent;
	Data				m_HalfTangentLength;
	Data				m_HalfOtherLength;
	Data				m_Offset;

//*******************************************************************
public://Constructor

	tPlane() : m_Offset(0), m_HalfTangentLength(0), m_HalfOtherLength(0) {}

//*******************************************************************
public://Methods

	bool		IsValid() const
	{ return fabs(m_vecNormal.GetLength() - 1.0) < FLT_EPSILON; }
	
	void		Reset() 
	{ m_vecNormal.Clear(); m_vecCentre.Clear(); m_vecTangent.Clear(); m_Offset= m_HalfTangentLength = m_HalfOtherLength = 0.0f;}

	//Sets plane by three points
	//Returns false if three ponts are colinear
	template <typename PointT>
	bool		Set(const t3DPoint<PointT>& _Point1, const t3DPoint<PointT>& _Point2, const t3DPoint<PointT>& _Point3);
	
	template <typename PointT>
	void		Set(const t3DPoint<PointT>& _vecCentre, const t3DPoint<PointT>& _vecNormal, const t3DPoint<PointT>& _vecTangent, double fHalfLengthAlongTnagent, double fOtherHalfLength);

	template <typename PointT>
	void		GetCorners (t3DPoint<PointT>& vecCorner1, t3DPoint<PointT>& vecCorner2, t3DPoint<PointT>& vecCorner3, t3DPoint<PointT>& vecCorner4) const;

	void		FlipNormal()
	{ m_Offset = -m_Offset; m_vecNormal.Neg(); }

	void		SwapTangents() 
	{ m_vecTangent = TangentH(); m_vecTangent.Normalize(); }

	void		Translate( double _dXOffset, double _dYOffset, double _dZOffset)
	{
		m_vecCentre += t3DPoint<double>(_dXOffset, _dYOffset, _dZOffset);
		m_Offset = Data(-Dot( m_vecNormal, m_vecCentre));
	}
	void		Rotate90()
	{ double f = m_HalfTangentLength; m_HalfTangentLength = m_HalfOtherLength; m_HalfOtherLength = f; }
	
	template <typename PointT>
	void		Transform(const cCartesian& matTR, const t3DPoint<PointT>& vecOrigin)
	{
		cCartesian NewMat = matTR;
		NewMat.t0 -= ( matTR.m[0][0] * vecOrigin.x + matTR.m[0][1] * vecOrigin.y + matTR.m[0][2] * vecOrigin.z );
		NewMat.t1 -= ( matTR.m[1][0] * vecOrigin.x + matTR.m[1][1] * vecOrigin.y + matTR.m[1][2] * vecOrigin.z );
		NewMat.t2 -= ( matTR.m[2][0] * vecOrigin.x + matTR.m[2][1] * vecOrigin.y + matTR.m[2][2] * vecOrigin.z );
		Transform( NewMat );
	}

	void		Transform(const cCartesian& matTR);
	cF3DExtents	GetExtents() const;

	const t3DPoint<Data>&	Normal() const { return m_vecNormal; }
	const t3DPoint<Data>&	Center() const { return m_vecCentre; }
	const t3DPoint<Data>&	TangentW() const { return m_vecTangent; }
	const t3DPoint<Data>	TangentH() const  { return Cross( m_vecNormal, m_vecTangent); }
	const Data&				Offset() const { return m_Offset; } 
	Data					HalfWidth() const { return m_HalfTangentLength; }
	Data					HalfHeight() const { return m_HalfOtherLength; }
	void					SetHalfWidth(double fValue) { m_HalfTangentLength = (Data)fValue; }
	void					SetHalfHeight(double fValue) { m_HalfOtherLength = (Data)fValue; }
	double					GetZOnPlane( double dX, double dY) { return ( fabs(m_vecNormal.Z) < DBL_EPSILON  ) ? 0.0 : (-m_Offset - m_vecNormal.x*dX - m_vecNormal.y*dY)/m_vecNormal.z; }

//*******************************************************************
};

//*******************************************************************
template< typename DataT >
template <typename PointT>
bool tPlane<DataT>::Set(const t3DPoint<PointT>& _Point1, const t3DPoint<PointT>& _Point2, const t3DPoint<PointT>& _Point3)
{ 	
	cD3DPoint Normal;
	_3ADA::GetCrossProduct(_Point1, _Point2, _Point3, Normal);
	if(Normal.GetSquaredLength() < DBL_EPSILON )
		return false;//points are colinear
	Normal.Normalize();
	m_vecNormal.Set((DataT)Normal.x, (DataT)Normal.y, (DataT)Normal.z);
	cD3DPoint Point = (_Point1 + _Point2 + _Point3)/3.0;
	m_Offset = (DataT)-Dot( Normal , Point);
	m_vecCentre = ((_Point1 + _Point2 + _Point3)/3.0);

	double dDist1 = m_vecCentre.GetSquaredDistanceTo( _Point1 );
	double dDist2 = m_vecCentre.GetSquaredDistanceTo( _Point2 );
	double dDist3 = m_vecCentre.GetSquaredDistanceTo( _Point3 );
	cD3DPoint OtherNormal, Proj;
	if( dDist1 >= dDist2 && dDist1 >= dDist3 )
	{
		m_vecTangent = _Point1 - m_vecCentre;
		m_HalfTangentLength = (sqrt( dDist1) *0.5);
		OtherNormal = Cross( m_vecNormal, m_vecTangent);
		OtherNormal.Normalize();
		double dDist11 = _3ADA::GetProjPointOnLine_Tangent( _Point2, m_vecCentre, OtherNormal, Proj );
		double dDist22 = _3ADA::GetProjPointOnLine_Tangent( _Point3, m_vecCentre, OtherNormal, Proj );
		m_HalfOtherLength = ( dDist11 > dDist22 ) ? ( dDist11 *0.5) : ( dDist22 *0.5);
	}
	else if( dDist2 >= dDist1 && dDist2 >= dDist3 )
	{
		m_vecTangent = _Point2 - m_vecCentre;
		m_HalfTangentLength = (sqrt( dDist2) *0.5);
		OtherNormal = Cross( m_vecNormal, m_vecTangent);
		OtherNormal.Normalize();
		double dDist11 = _3ADA::GetProjPointOnLine_Tangent( _Point1, m_vecCentre, OtherNormal, Proj );
		double dDist22 = _3ADA::GetProjPointOnLine_Tangent( _Point3, m_vecCentre, OtherNormal, Proj );
		m_HalfOtherLength = ( dDist11 > dDist22 ) ?  ( dDist11 *0.5) :( dDist22 *0.5);
	}
	else
	{
		m_vecTangent = _Point3 - m_vecCentre;
		m_HalfTangentLength = (sqrt( dDist3) *0.5);
		OtherNormal = Cross( m_vecNormal, m_vecTangent);
		OtherNormal.Normalize();
		double dDist11 = _3ADA::GetProjPointOnLine_Tangent( _Point1, m_vecCentre, OtherNormal, Proj );
		double dDist22 = _3ADA::GetProjPointOnLine_Tangent( _Point2, m_vecCentre, OtherNormal, Proj );
		m_HalfOtherLength = ( dDist11 > dDist22 ) ?  (dDist11 *0.5) : ( dDist22 *0.5);
	}
	m_vecTangent.Normalize();
	return true;
}
//**************************************************************************************
//**************************************************************************************
template< typename Data >
template <typename PointT>
void tPlane<Data>::Set(const t3DPoint<PointT>& _vecCentre, const t3DPoint<PointT>& _vecNormal, const t3DPoint<PointT>& _vecTangent, double fHalfLengthAlongTangent, double fOtherHalfLength)
{
	m_vecCentre = _vecCentre;
	m_vecNormal = _vecNormal;
	m_vecTangent = _vecTangent;
	m_HalfTangentLength = (Data)fHalfLengthAlongTangent;
	m_HalfOtherLength = (Data)fOtherHalfLength;
	m_Offset = Data(-Dot( m_vecNormal, m_vecCentre));
}
// **************************************************************************
// **************************************************************************
template< typename Data >
void tPlane<Data>::Transform(const cCartesian& matTR )
{
	matTR.Rotate( m_vecNormal );
	matTR.Rotate( m_vecTangent );
	matTR.Apply( m_vecCentre );
	m_Offset = -Dot( m_vecNormal, m_vecCentre);
}
//*******************************************************************
//*******************************************************************
template< typename Data >
template <typename PointT>
void tPlane<Data>::GetCorners (t3DPoint<PointT>& vecCorner1, t3DPoint<PointT>& vecCorner2, t3DPoint<PointT>& vecCorner3, t3DPoint<PointT>& vecCorner4) const
{
	cD3DPoint vecOtherTangent = Cross( m_vecNormal, m_vecTangent);
	vecOtherTangent.Normalize();
	
	// set to half width
	cD3DPoint vecTangent = m_vecTangent;
	vecTangent *= m_HalfTangentLength;
	vecOtherTangent *= m_HalfOtherLength;

	//!!Do not change the order!!
	vecCorner1  = m_vecCentre  - vecTangent + vecOtherTangent;
	vecCorner2 = m_vecCentre  +  vecTangent + vecOtherTangent;
	vecCorner3  = m_vecCentre  + vecTangent - vecOtherTangent;
	vecCorner4 = m_vecCentre  -  vecTangent - vecOtherTangent;
}
//*******************************************************************
//*******************************************************************
template< typename Data >
cF3DExtents tPlane<Data>::GetExtents() const
{
	cD3DPoint vec1, vec2, vec3, vec4;
	GetCorners(vec1, vec2, vec3, vec4);

	cF3DExtents Extents;
	Extents.Update(vec1);
	Extents.Update(vec2);
	Extents.Update(vec3);
	Extents.Update(vec4);
	return Extents;
}
//**************************************************************************************
#endif//__H_tPlane_INCLUDED__