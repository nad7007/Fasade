#pragma once

#include <3ADALib\Geom\c3DPoint.h>

//Perspective projection
// **************************************************************************
class cPerspective
// **************************************************************************
{
// **************************************************************************
public://Constructo & destructor

							cPerspective(){ Reset();};

// **************************************************************************
public://Methods

	void					Reset();

	template <typename PointT>
	cD3DPoint				PixelProject( const PointT& LocalPoint ) const;

	// Resolution and aspet ratio
	void					SetXYResolution(int _X, int _Y);
	int						XResolution() const { return (int)(m_HalfXResolution*2.0); };
	int						YResolution() const { return (int)(m_HalfYResolution*2.0); };
	double					XYAspectRatio() const { return m_dXYAspectRatio; };

	// Field Of View
	double					FOVDegrees() const { return DEG(m_FOVDegrees)*2;};
	void					SetFOVDegrees( double _FOV ) { m_FOVDegrees = RAD(_FOV*0.5);};
	void					SetFOVRadians( double _FOV ) { m_FOVDegrees = RAD(DEG(_FOV)*0.5);};

	const cD3DPoint&		Eye() const { return m_vecEye;};
	void					SetEye(  cD3DPoint _Eye) { m_vecEye = _Eye; };

	const cD3DPoint&		UpVector() const { return m_vecUp;};
	void					SetUpVector( cD3DPoint _UpVector) { m_vecUp = _UpVector; };	
	
//*************************************************************************************************************
protected://Data

	cD3DPoint				m_vecEye;
	cD3DPoint				m_vecUp;
	double					m_FOVDegrees;//in radians
	double					m_dXYAspectRatio;

	double 					m_HalfXResolution;
	double					m_HalfYResolution;
};
//*************************************************************************************************************
//Inlines

//*************************************************************************************************************
inline  void cPerspective::Reset()
{ 
	m_vecEye.Set(0.0, 0.0, 1.0);
	m_vecUp.Set(0.0, 1.0, 0.0);
	m_FOVDegrees = 23.0; 
	SetXYResolution(800, 600);
};
//*************************************************************************************************************
//*************************************************************************************************************
inline  void cPerspective::SetXYResolution(int _X, int _Y)
{
	m_dXYAspectRatio = double(_X) / double(_Y);
	m_HalfXResolution = _X*0.5;
	m_HalfYResolution = _Y*0.5;
}
//*************************************************************************************************************
//*************************************************************************************************************
template <typename PointT>
inline	cD3DPoint	cPerspective::PixelProject( const PointT& LocalPoint ) const
{
	const double dZ = LocalPoint.z;
	const double dY = LocalPoint.y;
	const double dMaxY = dZ * tan( m_FOVDegrees );
	if( dMaxY < fabs( dY ) )
		return VOID_DPOINT;//outside projected area

	const double dX = LocalPoint.x;
	const double dMaxX = dMaxY*m_dXYAspectRatio;
	if( dMaxX < fabs( dX ) )
		return VOID_DPOINT;//outside projected area

	double lY = m_vecEye.z * dY / dZ;
	double lX = m_vecEye.z * dX / dZ;

	lY *= m_HalfYResolution;
	lX *= m_HalfXResolution;
	_ASSERT( fabs(lY) < m_HalfYResolution );
	_ASSERT( fabs(lX) < m_HalfXResolution );
	
	return cD3DPoint( lY, lX,  dZ );
}
