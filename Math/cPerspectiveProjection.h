#pragma once

#include <3ADALib\Geom\cCartesian.h>
#include <3ADALib\Geom\cPerspective.h>
#include <3ADALib\Localisation\ILocalisation.h>


//Class for Projective coord system with perspective added
// **************************************************************************
class cPerspectiveProjection : public cCartesian, public cPerspective
{
//*************************************************************************************************************
public://Constructors & destructor
							cPerspectiveProjection() { Reset();};
	virtual					~cPerspectiveProjection() {} 

//*************************************************************************************************************
public: // Methods

	void					Reset(){ LoadIdentity(); __super::Reset(); }

	const cPerspective&		Perspective() const { return *this; };
	void					SetPerspective(const cPerspective& _Cal){ ((cPerspective&)(*this)) = _Cal; };

	const cCartesian&		CoordSystem() const { return *this;};
	void					SetCoordSystem(const cCartesian& Position){ ((cCartesian&)(*this)) = Position; };

	template<typename PointT>
	cD3DPoint				GetLocal( const PointT& GlobPoint ) const { return (*this)*GlobPoint; }
	template<typename PointT>
	cD3DPoint				GetPixelPojection( const PointT& GlobPoint ) const { return PixelProject((*this)*GlobPoint); } 

	// Eye access
	// returns the position of the EYE in Local and Global COORDINATE SYSTEM
	const cD3DPoint&		LocalEye() const { return Eye(); };
	cD3DPoint				GlobalEye() const { return (*this)* Eye(); };

	void					SetLocalEye(cD3DPoint _Eye) { SetEye(_Eye); };
	void					SetGlobalEye(cD3DPoint _Eye) { SetEye( (*this) / ( _Eye) ); }

	// UpVector
	cD3DPoint				LocalUpVector() const { return UpVector();};
	cD3DPoint				GlobalUpVector() const { return (*this) * UpVector() - GetTranslation();}

	void					SetLocalUpVector( cD3DPoint _UpVector) { SetUpVector( _UpVector); };
	void					SetGlobalUpVector( cD3DPoint _UpVector) { SetUpVector( (*this) / (_UpVector) - (*this) / ( dZERO_POINT) ); }

	// Plane corners
	void					GetLocalPlaneCorners(double dZ, cD3DPoint& TL, cD3DPoint& TR, cD3DPoint& BL, cD3DPoint& BR) const;
	void					GetGlobaPlaneCorners( cD3DPoint _AtPoint,  cD3DPoint& TL, cD3DPoint& TR, cD3DPoint& BL, cD3DPoint& BR) const;

	cD3DPoint				GetLocalXDir() const;
	cD3DPoint				GetGlobalXDir() const { return (*this) * GetLocalXDir() - GetTranslation();}

	cD3DPoint				GetLocalYDir() const;
	cD3DPoint				GetGlobalYDir() const { return (*this)*  GetLocalYDir() - GetTranslation();}


	// some functions that might be useful

	// moves the target toward the TargetPoint 
	// positive  - moves far from the TargetPoint
	void					SlideEye(float fDistance);
};
//***************************************************************************************
//Inlines

//*************************************************************************************************************
inline cD3DPoint cPerspectiveProjection::GetLocalXDir() const
{
	cD3DPoint vecHorizDir = Cross(UpVector(), m_vecEye );
	vecHorizDir.Normalize();
	return vecHorizDir;
}
//*************************************************************************************************************
//*************************************************************************************************************
inline cD3DPoint cPerspectiveProjection::GetLocalYDir() const
{
	cD3DPoint vecHorizDir = Cross(UpVector(), m_vecEye );
	cD3DPoint vecUpDir = Cross(m_vecEye, vecHorizDir);
	vecUpDir.Normalize();
	return vecUpDir;
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void cPerspectiveProjection::GetLocalPlaneCorners(double dZ, cD3DPoint& TL, cD3DPoint& TR, cD3DPoint& BL, cD3DPoint& BR) const
{
	double dHalfY = dZ *  tan( m_FOVDegrees ) ;
	double dHalfX = dHalfY*XYAspectRatio() ;

	cD3DPoint vecUpDir		= GetLocalYDir();
	cD3DPoint vecHorizDir	= GetLocalXDir();

	cD3DPoint Target( 0,0,dZ );

	TL = Target + (vecUpDir*dHalfY - vecHorizDir*dHalfX );
	TR = Target + (vecUpDir*dHalfY + vecHorizDir*dHalfX);
	BL = Target + (-vecUpDir*dHalfY - vecHorizDir*dHalfX);
	BR = Target + (-vecUpDir*dHalfY + vecHorizDir*dHalfX);
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void cPerspectiveProjection::GetGlobaPlaneCorners(cD3DPoint _AtPoint, cD3DPoint& TL, cD3DPoint& TR, cD3DPoint& BL, cD3DPoint& BR) const
{
	(*this).Apply( _AtPoint );
	GetLocalPlaneCorners(_AtPoint.z, TL, TR, BL, BR);
	TL = (*this) * (TL);
	TR = (*this) * (TR);
	BL = (*this) * (BL);
	BR = (*this) * (BR);
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void cPerspectiveProjection::SlideEye(float fDistance)
{
	m_vecEye.Set( m_vecEye.x, m_vecEye.y, m_vecEye.z + fDistance );
}
//*************************************************************************************************************
inline bool LoadProjection( LPCTSTR szFileName, cPerspectiveProjection& Proj )
{
	FILE *pFile = NULL;
	if ( !( pFile = _tfopen( szFileName, _TSYSTEM("rt")) ) ) 
		return false;

	//coord system
	cCartesian P;
	_ftscanf( pFile, _TSYSTEM("%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf")
		,&P.arr[0], &P.arr[1], &P.arr[2], &P.arr[3], &P.arr[4], &P.arr[5], &P.arr[6], &P.arr[7], &P.arr[8], &P.arr[9], &P.arr[10], &P.arr[11] ); 
	Proj.SetCoordSystem(P);
	
	cD3DPoint Temp = (0,1,0);
	Proj.SetUpVector(Temp);

	//Eye
	_ftscanf( pFile, _TSYSTEM("%lf, %lf, %lf"), &Temp.x, &Temp.y, &Temp.z );
	Proj.SetEye(Temp);

	//FOV degrees all
	double fTemp;
	_ftscanf( pFile, _TSYSTEM("%lf"), &fTemp  ); 
	//Proj.SetFOVRadians( fTemp );
	Proj.SetFOVDegrees( fTemp );

	//resolution
	int XResolution, YResolution;
	_ftscanf( pFile, _TSYSTEM("%d, %d"), &XResolution, &YResolution );
	Proj.SetXYResolution( XResolution, YResolution ); 

	fclose(pFile);
	return true;
}
//*************************************************************************************************************
inline bool SaveProjection( LPCTSTR szFileName, const cPerspectiveProjection& Proj )
{
	FILE *pFile = NULL;
	if ( !( pFile = _tfopen( szFileName, _TSYSTEM("wt") ) ) )
		return false;

	//coord system
	cCartesian P = Proj.CoordSystem();
	_ftprintf( pFile, _TSYSTEM("%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n")
		,P.arr[0], P.arr[1], P.arr[2], P.arr[3], P.arr[4], P.arr[5], P.arr[6], P.arr[7], P.arr[8], P.arr[9], P.arr[10], P.arr[11] ); 
	
	cD3DPoint Temp = Proj.Eye();
	//Eye
	_ftprintf( pFile, _TSYSTEM("%lf, %lf, %lf\n"), Temp.x, Temp.y, Temp.z );
	

	//FOV degrees all
	double fTemp = Proj.FOVDegrees();
	_ftprintf( pFile, _TSYSTEM("%lf\n"), fTemp  ); 
	
	//resolution
	int XResolution = Proj.XResolution(), YResolution = Proj.YResolution();
	_ftprintf( pFile, _TSYSTEM("%d, %d\n"), XResolution, YResolution );
	
	fclose(pFile);
	return true;
}
//*************************************************************************************************************