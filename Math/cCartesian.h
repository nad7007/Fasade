// **************************************************************************
#pragma once

#include "Math\c3DPoint.h"
#include "Math\c2DPoint.h"

// **************************************************************************

#ifdef _MSC_VER
#	pragma warning( push )
//#	pragma warning( disable : 4201 )// nonstandard extension used : nameless struct/union
#endif

const double PI = 3.1415926535897932384626433832795;
const double RAD = 0.01745329251994329576923690768489;//PI/180
const double DEG = 57.295779513082320876798154814105;//180 /PI

//Class for rotation and translation matrix in 3D space.
// **************************************************************************
class cCartesian  
{
// **************************************************************************
public://Data

	union
	{
		double arr[ 12 ];
		double m[3][4];//[3][3] matrix of rotation and last column for translations.
		struct
		{
			double oX0, oY0, oZ0, t0;
			double oX1, oY1, oZ1, t1;
			double oX2, oY2, oZ2, t2;
		};
	};

// **************************************************************************
public:// Constructor & Destructor

	cCartesian() { Identity(); };

// **************************************************************************
public:// Methods

	// reset Cartesian to unit matrix
	inline void		Identity();
	
	// reset Cartesian to zero matrix
	inline void		Clear();

	//Invert Cartesian
	inline void			Invert();
	inline cCartesian	Inverse() const { cCartesian Res = (*this); Res.Invert(); return Res; }

	//Orthogonalize rotation part of the matrix
	//inline bool			MakeOrthogonal();

	//gets determinant of the rotation part
	inline double	GetDeterminant()	const; 

	//gets trace of the rotation part
	inline double	GetTrace()			const;

	inline bool		AreIdentical( const cCartesian& Mat, const double dTolerance = FLT_EPSILON ) const;

	//Add transformation to current one
	//void			Add ( cCartesian const& _mat );

	// loads the matrix from a OpenGL matrix data
	void			SetGLMatrix( const double _GLMatrix[16] );
	// store this Cartesian in OpenGL compatible form
	// returns _GLMatrix
	double*			GetGLMatrix( double _GLMatrix[16] ) const;

	// NOTE RETURNS ONLY ROTATIONS. TRASLATION VECTORS ARE ZERO
	// convert to a "float" matrix
	inline float*	GetMatrixData( float * _dSpace ) const;
	inline void		SetMatrixData( const float * _fSpace ) ;
	// convert to a 2D "float" matrix
	inline float*	Get2DMatrixData( float * _dSpace ) const;

	void GetFrom( double* pData )
	{
		memcpy( arr, pData, 12*sizeof(double) );
	}
	void SetTo( double* pData )
	{
		memcpy( pData, arr, 12*sizeof(double) );
	}
	void GetRotFrom( double* pData )
	{
		m[0][0] = pData[0];
		m[0][1] = pData[1];
		m[0][2] = pData[2];
		m[1][0] = pData[3];
		m[1][1] = pData[4];
		m[1][2] = pData[5];
		m[2][0] = pData[6];
		m[2][1] = pData[7];
		m[2][2] = pData[8];
	}
	void SetRotTo( double* pData )
	{
		pData[0] = m[0][0];
		pData[1] = m[0][1];
		pData[2] = m[0][2];
		pData[3] = m[1][0];
		pData[4] = m[1][1];
		pData[5] = m[1][2];
		pData[6] = m[2][0];
		pData[7] = m[2][1];
		pData[8] = m[2][2];
	}


//SCALING
	// **************************************************************************
	inline c3DPointD GetScale( void );
	// set scaling
	inline void	Scale( const c3DPointD& _vecScale );
	inline void	ScaleX( float _scale );
	inline void	ScaleY( float _scale );
	inline void	ScaleZ( float _scale );

//ROTATION
	// **************************************************************************
	//Gets coordiantes of rotation matrix axis
	inline c3DPointD GetZOrientation()const 
		{ return c3DPointD(oZ0, oZ1, oZ2); }
	inline c3DPointD GetYOrientation()const 
		{ return c3DPointD(oY0, oY1, oY2); }
	inline c3DPointD GetXOrientation()const 
		{ return c3DPointD(oX0, oX1, oX2); }
	inline c3DPointD GetOrientation(int iIndex)const 
		{ return c3DPointD(m[0][iIndex], m[1][iIndex], m[2][iIndex] ); }

	//get/set rotation part of the matrix
	inline void		GetRotation( cCartesian &RotationMatrix )	const;
	inline void		SetRotation( const cCartesian& RotationMatrix );
	
	//get/set rotation part of the matrix as unit quaternion
	void			GetRotation( double& dQuatW, double& dQuatX, double& dQuatY, double& dQuatZ )	const;
	void			SetRotation( double dQuatW, double dQuatX, double dQuatY, double dQuatZ );
	
	// Get/set rotation by axis of rotation and angle of rotation
	//!ATTENTION axis of rotation must be normalized before
	void			GetRotation( double &dRotationAngleInRadians, c3DPointD &rUnitAxesOfRotation ) const;
	//Rodrigues' rotation formula 
	void			SetRotation( double dRotationAngleInRadians, const c3DPointD &rUnitAxesOfRotation );
	
	// Set rotation by direct set of three axes of rotation
	//!ATTENTION normals must be normalized before
	void			SetRotation( const c3DPointD &_NormalX, const c3DPointD &_NormalY, const c3DPointD &_NormalZ );

	//Set rotation such as: _GlobNormal = CurrentTransform.Rotate( _LocalNormal );
	//void			SetRotation( const c3DPointD &_GlobNormal, const c3DPointD &_LocalNormal );

	// Set rotation by two orthogonal normals for choosen axes of rotaion
	//AxisInd = 0 means axis OX
	//AxisInd = 1 means axis OY
	//AxisInd = 2 means axis OZ
	//!ATTENTION normals must be normalized before
	void			SetRotation( const c3DPointD &_Normal1, unsigned AxisInd1, const c3DPointD &_Normal2, unsigned AxisInd2 );

	//free rotation around arbitrary axis U at angle dAlpha
	void			FreeRotate(const c3DPointD& _vecU, double _dAlpha);
	
	// vecDirection shows the direction for rotation. dAngle is in Radians
	void			RotateInDirection(double dAngle, const c3DPointD& vecDirection);
	
	// R = _mat.R * R;
	inline void		UpdateRotation(cCartesian const& _mat);

	inline void		SetRotationXY( c3DPointD Z,  c3DPointD X);
	inline void		SetRotationYZ( c3DPointD Z,  c3DPointD Y);
	inline void		SetRotationZX( c3DPointD Z,  c3DPointD X);

	// ATTENTION: Angles are in Radians. First rotation around OX of angle _dX,
	//second - around OY of angle _dY and so on
	inline void		SetXYZRotation( double _dX, double _dY, double _dZ );
	//inline void		GetXYZRotation( double &_rdX, double &_rdY, double &_rdZ ) const;
	/*inline void		GetXYZRotation( c3DPointD &rReturnXYZAngle ) const
	{
		GetXYZRotation( rReturnXYZAngle.x, rReturnXYZAngle.y, rReturnXYZAngle.z );
	}*/

	// fast rotation around axis
	inline void		Rotate180AroundX();
	inline void		Rotate180AroundY();
	inline void		Rotate180AroundZ();

	inline void		Rotate90AroundX();
	inline void		Rotate90AroundY();
	inline void		Rotate90AroundZ();
	
	// ATTENTION: Angles are in Radians.
	//Sets rotation matrix by roll, pitch and yaw angles
	inline void		SetRPYRotation( double _dRoll, double _dPitch, double _dYaw )
	{
		SetXYZRotation( _dRoll, _dPitch, _dYaw );
	}
	/*inline void		GetRPYRotation( double& _dRoll, double& _dPitch, double& _dYaw )const
	{
		GetXYZRotation( _dRoll, _dPitch, _dYaw );
	}*/

	/*inline void		GetTRPY( double& dX, double& dY, double& dZ, double& _dRoll, double& _dPitch, double& _dYaw )const
	{
		GetRPYRotation( _dRoll, _dPitch,  _dYaw );
		dX = t0; dY = t1; dZ = t2;
	}*/

	// ATTENTION: Angles are in Radians. First rotation around OZ of angle _dZ,
	//second - around OY of angle _dY and so on
	inline void		SetZYXRotation( double _dX, double _dY, double _dZ  );

	// get Euler angles
	void			GetZYX( float& _x, float& _y, float& _z ) const;
	void			GetZYX1( float& _x, float& _y, float& _z ) const;
	//void			GetZYX2( float& _x, float& _y, float& _z ) const;

	// next functions rotate point before making current Cartesian:
	// LocalPoint = CurrentTransform * RotateXXBefore * GlobalPoint
	// !angle is in Radians
	inline void		RotateXBefore( double _dAngle ); 
	inline void		RotateYBefore( double _dAngle );
	inline void		RotateZBefore( double _dAngle );

	// next functions add rotation to current Cartesian:
	// LocalPoint = RotateXXAfter * CurrentTransform * GlobalPoint
	// !angle is in Radians
	inline void		RotateXAfter( double _dAngle ); 
	inline void		RotateYAfter( double _dAngle );
	inline void		RotateZAfter( double _dAngle );

	// M' = R(iAxis)
	//does not modify translation part
	inline void		SetRotationAroundCoordAxis( double _fAngle, int iAxis );
	// M' = R(x)
	inline void		SetRotateX( double _fAngle );
	// M' = R(y)
	inline void		SetRotateY( double _fAngle );
	// M' = R(z)
	inline void		SetRotateZ( double _fAngle );

	// M' = Rx( _fAngle ).M  radians
	inline void		RotateAroundGlobalX( double _fAngle );
	// M' = Ry( _fAngle ).M radians
	inline void		RotateAroundGlobalY( double _fAngle );
	// M' = Rz( _fAngle ).M radians
	inline void		RotateAroundGlobalZ( double _fAngle );

	inline void		RotateAroundLocalX( double _fAngle );
	inline void		RotateAroundLocalY( double _fAngle );
	inline void		RotateAroundLocalZ( double _fAngle );

	// M' = M . Rx( _fAngle ) radians
	inline void		PostRotateX( double _fAngle );
	// M' = M . Ry( _fAngle ) radians
	inline void		PostRotateY( double _fAngle );
	// M' = M . Rz( _fAngle ) radians
	inline void		PostRotateZ( double _fAngle );

//TRANSLATION
	//***********************************************************************
	// adds pure translation to current Cartesian:
	// LocalPoint = Translation * ( CurrentTransform * GlobalPoint )
	inline void		TranslateAfter( double dOffsetX, double dOffsetY, double dOffsetZ );
	inline void		TranslateAfter( const c3DPointD &Translation )
	{
		TranslateAfter( Translation.x, Translation.y, Translation.z );
	}

	// translates points before making current Cartesian:
	// LocalPoint = CurrentTransform * ( Translation * GlobalPoint )
	inline void		TranslateBefore( double dOffsetX, double dOffsetY, double dOffsetZ );
	inline void		TranslateBefore( const c3DPointD &Translation )
	{
		TranslateBefore( Translation.x, Translation.y, Translation.z );
	}
	
	//get/set translation only
	inline void		GetTranslation( double &_rdX, double &_rdY, double &_rdZ )	const;
	inline void		GetTranslation( c3DPointD &Translation )	const
	{
		GetTranslation( Translation.x, Translation.y, Translation.z );
	}
	inline c3DPointD GetTranslation()	const
	{
		return c3DPointD( m[0][3], m[1][3], m[2][3] );
	}
	inline void		SetTranslation( double dOffsetX, double dOffsetY, double dOffsetZ );
	inline void		SetTranslation( const c3DPointD& rTranslation )
	{
		SetTranslation( rTranslation.x, rTranslation.y, rTranslation.z );
	}

	//ResultMat = CurrentMat * ( UnitRotation + Translation )
	template< typename DataT >
	inline void	UpdateTranslation( temp3DPoint<DataT> const& Translation ) 
	{
		temp3DPoint<DataT> NewTranslation = (*this) * Translation;
		SetTranslation( NewTranslation );
	}

// **************************************************************************
public://Operations

	// simple overload operators
	void			operator *=( cCartesian const& _mat );
	cCartesian		operator *( cCartesian const& _mat ) const;

	inline bool		operator ==( cCartesian const& _mat ) const { return memcmp(_mat.arr, this->arr, sizeof(this->arr))==0; }
	inline bool		operator !=( cCartesian const& _mat ) const { return !(_mat==*this); }
	// returns Inv(M) . _mat
	inline cCartesian	operator/( cCartesian const& _mat ) const;
	// returns M . Inv(_mat)
	inline cCartesian	operator^(cCartesian const& _mat) const;

	// direct Cartesian:
	// LocalPoint = Rotation*GlobalPoint + Translation
	c3DPointD	operator*( c3DPointD const* pGlobalPoint ) const;
	template< typename DataT >
	inline temp3DPoint<DataT> operator*( temp3DPoint<DataT> const& rGlobalPoint ) const;
	template< typename DataT >
	inline temp2DPoint<DataT> operator*( temp2DPoint<DataT> const& rGlobalPoint ) const;
	template< typename DataT >
	inline void Apply( temp3DPoint<DataT>& _rGlobalPoint ) const;
	template< typename DataT >
	inline DataT ApplyZ( const temp3DPoint<DataT>& _rGlobalPoint ) const { return ApplyZ(_rGlobalPoint.x, _rGlobalPoint.y, _rGlobalPoint.z); }
	template< typename DataT >
	inline DataT ApplyZ( const DataT x, const DataT y, const DataT z ) const;

	
	// inverse Cartesian:
	// GlobalPoint = Rotation-1 *( LocalPoint - Translation)
	c3DPointD operator/( c3DPointD const *pLocalPoint ) const;
	template< typename DataT >
	inline temp3DPoint<DataT> operator/( temp3DPoint<DataT> const& rLocalPoint ) const;
	template< typename DataT >
	inline void ApplyInverse( temp3DPoint<DataT>& rLocalPoint ) const;
	template< typename DataT >
	inline DataT ApplyInverseZ( const temp3DPoint<DataT>& rLocalPoint ) const;
	
	//apply inverse rotation only
	template< typename DataT >
	inline void InvRotate( temp3DPoint<DataT>& _rLocalPoint ) const;
	template< typename DataT >
	inline temp3DPoint<DataT> InvRotate( const temp3DPoint<DataT>* pLocalPoint ) const;

	template< typename DataT >
	inline void Rotate( temp3DPoint<DataT>& _rGlobalPoint ) const;
	template< typename DataT >
	inline temp3DPoint<DataT> Rotate( const temp3DPoint<DataT>* _pGlobalPoint ) const;
	template< typename DataT >
	inline void Translate( temp3DPoint<DataT>& _rGlobalPoint ) const;

	//Translation updates
	// returns T + _vec
	inline cCartesian	operator+( c3DPointD const& _vec ) const;
	// T' = T + vec
	inline void			operator+=( c3DPointD const& _vec );
	// returns T - _vec
	inline cCartesian	operator-( c3DPointD const& _vec ) const;
	// T' = T - vec
	inline void			operator-=( c3DPointD const& _vec );

};
// **************************************************************************
//		Inline Functions

// **************************************************************************
inline	void cCartesian::Identity()
{
	oX0 = oY1 = oZ2 = 1.0;
	oY0 = oZ0 = t0 = 0.0;
	oX1 = oZ1 = t1 = 0.0;
	oX2 = oY2 = t2 = 0.0;
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::Clear()
{
	m[0][0] = m[0][1] = m[0][2] = m[0][3] = 0.0;
	m[1][0] = m[1][1] = m[1][2] = m[1][3] = 0.0;
	m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::SetRotationAroundCoordAxis( double _fAngle, int iAxis )
{
	m[0][iAxis] = m[1][iAxis] = m[2][iAxis] = m[iAxis][0] = m[iAxis][1] = m[iAxis][2] = 0.0;
	m[iAxis][iAxis] = 1.0;

	const int iIndex1 = (iAxis+1)%3;
	const int iIndex2 = (iAxis+2)%3;
	const double s = sin( _fAngle );
	
	m[iIndex1][iIndex1] = m[iIndex2][iIndex2] = cos( _fAngle );
	m[iIndex1][iIndex2] = -s;
	m[iIndex2][iIndex1] = s;
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::RotateXBefore( double _dAngle ) // _dAngle is in radians
{
	double c = cos( _dAngle );
	double s = sin( _dAngle );
	
	double Y0 = m[0][1] * c + m[0][2] * s;
	double Y1 = m[1][1] * c + m[1][2] * s;
	double Y2 = m[2][1] * c + m[2][2] * s;

	double Z0 = -m[0][1] * s + m[0][2] * c;
	double Z1 = -m[1][1] * s + m[1][2] * c;
	double Z2 = -m[2][1] * s + m[2][2] * c;

	m[0][1] = Y0;
	m[1][1] = Y1;
	m[2][1] = Y2;

	m[0][2] = Z0;
	m[1][2] = Z1;
	m[2][2] = Z2;
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::RotateYBefore( double _dAngle ) // _dAngle is in radians
{
	double c = cos( _dAngle );
	double s = sin( _dAngle );

	double X0 = m[0][0] * c - m[0][2] * s;
	double X1 = m[1][0] * c - m[1][2] * s;
	double X2 = m[2][0] * c - m[2][2] * s;

	double Z0 = m[0][0] * s + m[0][2] * c;
	double Z1 = m[1][0] * s + m[1][2] * c;
	double Z2 = m[2][0] * s + m[2][2] * c;

	m[0][0] = X0;
	m[1][0] = X1;
	m[2][0] = X2;

	m[0][2] = Z0;
	m[1][2] = Z1;
	m[2][2] = Z2;
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::RotateZBefore( double _dAngle ) // _dAngle is in radians
{
	double c = cos( _dAngle );
	double s = sin( _dAngle );

	double X0 = m[0][0] * c + m[0][1] * s;
	double X1 = m[1][0] * c + m[1][1] * s;
	double X2 = m[2][0] * c + m[2][1] * s;

	m[0][1] = -m[0][0] * s + m[0][1] * c;
	m[1][1] = -m[1][0] * s + m[1][1] * c;
	m[2][1] = -m[2][0] * s + m[2][1] * c;

	m[0][0] = X0;
	m[1][0] = X1;
	m[2][0] = X2;
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::RotateXAfter( double _dAngle ) // _dAngle is in radians
{
	double c = cos( _dAngle );
	double s = sin( _dAngle );

	double X1 = m[1][0] * c - m[2][0] * s;
	double Y1 = m[1][1] * c - m[2][1] * s;
	double Z1 = m[1][2] * c - m[2][2] * s;
	double T1 = m[1][3] * c - m[2][3] * s;

	m[2][0] = m[1][0] * s + m[2][0] * c;
	m[2][1] = m[1][1] * s + m[2][1] * c;
	m[2][2] = m[1][2] * s + m[2][2] * c;
	m[2][3] = m[1][3] * s + m[2][3] * c;

	m[1][0] = X1;
	m[1][1] = Y1;
	m[1][2] = Z1;
	m[1][3] = T1;
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::RotateYAfter( double _dAngle ) // _dAngle is in radians
{
	double c = cos( _dAngle );
	double s = sin( _dAngle );

	double X0 = m[0][0] * c + m[2][0] * s;
	double Y0 = m[0][1] * c + m[2][1] * s;
	double Z0 = m[0][2] * c + m[2][2] * s;
	double T0 = m[0][3] * c + m[2][3] * s;

	m[2][0] = -m[0][0] * s + m[2][0] * c;
	m[2][1] = -m[0][1] * s + m[2][1] * c;
	m[2][2] = -m[0][2] * s + m[2][2] * c;
	m[2][3] = -m[0][3] * s + m[2][3] * c;

	m[0][0] = X0;
	m[0][1] = Y0;
	m[0][2] = Z0;
	m[0][3] = T0;
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::RotateZAfter( double _dAngle ) // _dAngle is in radians
{
	double c = cos( _dAngle );
	double s = sin( _dAngle );

	double X0 = m[0][0] * c - m[1][0] * s;
	double Y0 = m[0][1] * c - m[1][1] * s;
	double Z0 = m[0][2] * c - m[1][2] * s;
	double T0 = m[0][3] * c - m[1][3] * s;

	m[1][0] = m[0][0] * s + m[1][0] * c;
	m[1][1] = m[0][1] * s + m[1][1] * c;
	m[1][2] = m[0][2] * s + m[1][2] * c;
	m[1][3] = m[0][3] * s + m[1][3] * c;

	m[0][0] = X0;
	m[0][1] = Y0;
	m[0][2] = Z0;
	m[0][3] = T0;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::SetRotateX( double _fAngle )
{
	double c = cos( _fAngle );
	double s = sin( _fAngle );

	oX0 = 1.0;	oY0 = 0.0;	oZ0 = 0.0;	t0 = 0.0;
	oX1 = 0.0;	oY1 = c;	oZ1 = -s;	t1 = 0.0;
	oX2 = 0.0;	oY2 = s;	oZ2 = c;	t2 = 0.0;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::SetRotateY( double _fAngle )
{
	double c =  cos( _fAngle );
	double s =  sin( _fAngle );

	oX0 = c;	oY0 = 0.0;	oZ0 = s;	t0 = 0.0;
	oX1 = 0.0;	oY1 = 1.0;	oZ1 = 0.0;	t1 = 0.0;
	oX2 = -s;	oY2 = 0.0;	oZ2 = c;	t2 = 0.0;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::SetRotateZ( double _fAngle )
{
	double c =  cos( _fAngle );
	double s =  sin( _fAngle );

	oX0 = c;	oY0 = -s;	oZ0 = 0.0;	t0 = 0.0;
	oX1 = s;	oY1 = c;	oZ1 = 0.0;	t1 = 0.0;
	oX2 = 0.0;	oY2 = 0.0;	oZ2 = 1.0;	t2 = 0.0;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::RotateAroundLocalX( double _fAngle )
{
	c3DPointD vecDirection(oX0, oX1, oX2);
	RotateInDirection(_fAngle, vecDirection);
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::RotateAroundLocalY( double _fAngle )
{
	c3DPointD vecDirection(oY0, oY1, oY2);
	RotateInDirection(_fAngle, vecDirection);
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::RotateAroundLocalZ( double _fAngle )
{
	c3DPointD vecDirection(oZ0, oZ1, oZ2);
	RotateInDirection(_fAngle, vecDirection);
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::RotateAroundGlobalX( double _fAngle )
{
	double c = cos( _fAngle );
	double s = sin( _fAngle );

	double _m10 = oX1 * c - oX2 * s;
	double _m11 = oY1 * c - oY2 * s;
	double _m12 = oZ1 * c - oZ2 * s;

	double _m20 = oX1 * s + oX2 * c;
	double _m21 = oY1 * s + oY2 * c;
	double _m22 = oZ1 * s + oZ2 * c;

	oX1 = _m10;
	oY1 = _m11;
	oZ1 = _m12;

	oX2 = _m20;
	oY2 = _m21;
	oZ2 = _m22;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::RotateAroundGlobalY( double _fAngle )
{
	double c = cos( _fAngle );
	double s = sin( _fAngle );

	double _m00 = oX0 * c + oX2 * s;
	double _m01 = oY0 * c + oY2 * s;
	double _m02 = oZ0 * c + oZ2 * s;

	double _m20 = -oX0 * s + oX2 * c;
	double _m21 = -oY0 * s + oY2 * c;
	double _m22 = -oZ0 * s + oZ2 * c;

	oX0 = _m00;
	oY0 = _m01;
	oZ0 = _m02;

	oX2 = _m20;
	oY2 = _m21;
	oZ2 = _m22;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::RotateAroundGlobalZ( double _fAngle )
{
	double c = cos( _fAngle );
	double s = sin( _fAngle );

	double _m00 = oX0 * c - oX1 * s;
	double _m01 = oY0 * c - oY1 * s;
	double _m02 = oZ0 * c - oZ1 * s;

	double _m10 = oX0 * s + oX1 * c;
	double _m11 = oY0 * s + oY1 * c;
	double _m12 = oZ0 * s + oZ1 * c;

	oX0 = _m00;
	oY0 = _m01;
	oZ0 = _m02;

	oX1 = _m10;
	oY1 = _m11;
	oZ1 = _m12;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::PostRotateX( double _fAngle )
{
	double c = cos( _fAngle );
	double s = sin( _fAngle );

	double _m01 = oY0 * c + oZ0 * s;
	double _m11 = oY1 * c + oZ1 * s;
	double _m21 = oY2 * c + oZ2 * s;

	double _m02 = -oY0 * s + oZ0 * c;
	double _m12 = -oY1 * s + oZ1 * c;
	double _m22 = -oY2 * s + oZ2 * c;

	oY0 = _m01;
	oY1 = _m11;
	oY2 = _m21;

	oZ0 = _m02;
	oZ1 = _m12;
	oZ2 = _m22;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::PostRotateY( double _fAngle )
{
	double c = cos( _fAngle );
	double s = sin( _fAngle );

	double _m00 = oX0 * c - oZ0 * s;
	double _m10 = oX1 * c - oZ1 * s;
	double _m20 = oX2 * c - oZ2 * s;

	double _m02 = oX0 * s + oZ0 * c;
	double _m12 = oX1 * s + oZ1 * c;
	double _m22 = oX2 * s + oZ2 * c;

	oX0 = _m00;
	oX1 = _m10;
	oX2 = _m20;

	oZ0 = _m02;
	oZ1 = _m12;
	oZ2 = _m22;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::PostRotateZ( double _fAngle )
{

	if (fabs(_fAngle) < 0.00001 ) // the angle is very small, division by
		return;

	double c = cos( _fAngle );
	double s = sin( _fAngle );

	double _m00 = oX0 * c + oY0 * s;
	double _m10 = oX1 * c + oY1 * s;
	double _m20 = oX2 * c + oY2 * s;

	double _m01 = -oX0 * s + oY0 * c;
	double _m11 = -oX1 * s + oY1 * c;
	double _m21 = -oX2 * s + oY2 * c;

	oX0 = _m00;
	oX1 = _m10;
	oX2 = _m20;

	oY0 = _m01;
	oY1 = _m11;
	oY2 = _m21;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::TranslateAfter( double dOffsetX, double dOffsetY, double dOffsetZ )
{
	m[0][3] += dOffsetX;
	m[1][3] += dOffsetY;
	m[2][3] += dOffsetZ;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::TranslateBefore( double dOffsetX, double dOffsetY, double dOffsetZ )
{
	m[0][3] += ( m[0][0]*dOffsetX + m[0][1]*dOffsetY + m[0][2]*dOffsetZ );
	m[1][3] += ( m[1][0]*dOffsetX + m[1][1]*dOffsetY + m[1][2]*dOffsetZ );
	m[2][3] += ( m[2][0]*dOffsetX + m[2][1]*dOffsetY + m[2][2]*dOffsetZ );
}
// **************************************************************************

//// **************************************************************************
//inline void cCartesian::Add ( cCartesian const& _mat )
//{
//	cCartesian result;
//
//	result.m[0][0] = _mat.m[0][0] * m[0][0] + _mat.m[0][1] * m[1][0] + _mat.m[0][2] * m[2][0];
//	result.m[1][0] = _mat.m[1][0] * m[0][0] + _mat.m[1][1] * m[1][0] + _mat.m[1][2] * m[2][0];
//	result.m[2][0] = _mat.m[2][0] * m[0][0] + _mat.m[2][1] * m[1][0] + _mat.m[2][2] * m[2][0];
//	
//	result.m[0][1] = _mat.m[0][0] * m[0][1] + _mat.m[0][1] * m[1][1] + _mat.m[0][2] * m[2][1];
//	result.m[1][1] = _mat.m[1][0] * m[0][1] + _mat.m[1][1] * m[1][1] + _mat.m[1][2] * m[2][1];
//	result.m[2][1] = _mat.m[2][0] * m[0][1] + _mat.m[2][1] * m[1][1] + _mat.m[2][2] * m[2][1];
//	
//	result.m[0][2] = _mat.m[0][0] * m[0][2] + _mat.m[0][1] * m[1][2] + _mat.m[0][2] * m[2][2];
//	result.m[1][2] = _mat.m[1][0] * m[0][2] + _mat.m[1][1] * m[1][2] + _mat.m[1][2] * m[2][2];
//	result.m[2][2] = _mat.m[2][0] * m[0][2] + _mat.m[2][1] * m[1][2] + _mat.m[2][2] * m[2][2];
//	
//	result.m[0][3] = _mat.m[0][0] * m[0][3] + _mat.m[0][1] * m[1][3] + _mat.m[0][2] * m[2][3] + _mat.m[0][3];
//	result.m[1][3] = _mat.m[1][0] * m[0][3] + _mat.m[1][1] * m[1][3] + _mat.m[1][2] * m[2][3] + _mat.m[1][3];
//	result.m[2][3] = _mat.m[2][0] * m[0][3] + _mat.m[2][1] * m[1][3] + _mat.m[2][2] * m[2][3] + _mat.m[2][3];
//	
//	result.MakeOrthogonal();
//	memcpy( this, &result, sizeof( cCartesian ) );
//}
//// **************************************************************************

// **************************************************************************
inline void cCartesian::SetTranslation( double dOffsetX, double dOffsetY, double dOffsetZ )
{
	m[0][3] = dOffsetX;
	m[1][3] = dOffsetY;
	m[2][3] = dOffsetZ;
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::operator *=( cCartesian const& _mat )
{
	cCartesian result;

	result.oX0 = oX0 * _mat.oX0 + oY0 * _mat.oX1  + oZ0  * _mat.oX2;
	result.oX1 = oX1 * _mat.oX0 + oY1 * _mat.oX1  + oZ1  * _mat.oX2;
	result.oX2 = oX2 * _mat.oX0 + oY2 * _mat.oX1  + oZ2  * _mat.oX2;
																
	result.oY0 = oX0 * _mat.oY0 + oY0 * _mat.oY1  + oZ0  * _mat.oY2;
	result.oY1 = oX1 * _mat.oY0 + oY1 * _mat.oY1  + oZ1  * _mat.oY2;
	result.oY2 = oX2 * _mat.oY0 + oY2 * _mat.oY1  + oZ2  * _mat.oY2;
																
	result.oZ0 = oX0 * _mat.oZ0 + oY0 * _mat.oZ1  + oZ0  * _mat.oZ2;
	result.oZ1 = oX1 * _mat.oZ0 + oY1 * _mat.oZ1  + oZ1  * _mat.oZ2;
	result.oZ2 = oX2 * _mat.oZ0 + oY2 * _mat.oZ1  + oZ2  * _mat.oZ2;
																
	result.t0 = oX0 * _mat.t0 + oY0 * _mat.t1  + oZ0  * _mat.t2 + t0;
	result.t1 = oX1 * _mat.t0 + oY1 * _mat.t1  + oZ1  * _mat.t2 + t1;
	result.t2 = oX2 * _mat.t0 + oY2 * _mat.t1  + oZ2  * _mat.t2 + t2;
	
	memcpy( this, &result, sizeof( cCartesian ) );
}
// **************************************************************************

// **************************************************************************
inline	cCartesian cCartesian::operator *( cCartesian const& _mat ) const
{
	cCartesian result;

	result.oX0 = oX0 * _mat.oX0 + oY0 * _mat.oX1  + oZ0  * _mat.oX2;
	result.oX1 = oX1 * _mat.oX0 + oY1 * _mat.oX1  + oZ1  * _mat.oX2;
	result.oX2 = oX2 * _mat.oX0 + oY2 * _mat.oX1  + oZ2  * _mat.oX2;
																
	result.oY0 = oX0 * _mat.oY0 + oY0 * _mat.oY1  + oZ0  * _mat.oY2;
	result.oY1 = oX1 * _mat.oY0 + oY1 * _mat.oY1  + oZ1  * _mat.oY2;
	result.oY2 = oX2 * _mat.oY0 + oY2 * _mat.oY1  + oZ2  * _mat.oY2;
																
	result.oZ0 = oX0 * _mat.oZ0 + oY0 * _mat.oZ1  + oZ0  * _mat.oZ2;
	result.oZ1 = oX1 * _mat.oZ0 + oY1 * _mat.oZ1  + oZ1  * _mat.oZ2;
	result.oZ2 = oX2 * _mat.oZ0 + oY2 * _mat.oZ1  + oZ2  * _mat.oZ2;
																
	result.t0 = oX0 * _mat.t0 + oY0 * _mat.t1  + oZ0  * _mat.t2 + t0;
	result.t1 = oX1 * _mat.t0 + oY1 * _mat.t1  + oZ1  * _mat.t2 + t1;
	result.t2 = oX2 * _mat.t0 + oY2 * _mat.t1  + oZ2  * _mat.t2 + t2;

	return result;
}
// **************************************************************************

// **************************************************************************
inline cCartesian cCartesian::operator^( cCartesian const& _mat ) const
{
	cCartesian result;

	// calculate _matinverted, then will simply multiply
	double nx = _mat.oX0, ny = _mat.oX1, nz = _mat.oX2;
	double ox = _mat.oY0, oy = _mat.oY1, oz = _mat.oY2;
	double ax = _mat.oZ0, ay = _mat.oZ1, az = _mat.oZ2;
	double px = _mat.t0, py = _mat.t1, pz = _mat.t2;


	double _m00 = nx; double _m01 = ny; double _m02 = nz; double _m03 = -( px*nx + py*ny +pz*nz );
	double _m10 = ox; double _m11 = oy; double _m12 = oz; double _m13 = -( px*ox + py*oy +pz*oz );
	double _m20 = ax; double _m21 = ay; double _m22 = az; double _m23 = -( px*ax + py*ay +pz*az  );


	result.oX0 = ( oX0 * _m00 + oY0 * _m10 + oZ0 * _m20 );
	result.oX1 = ( oX1 * _m00 + oY1 * _m10 + oZ1 * _m20 );
	result.oX2 = ( oX2 * _m00 + oY2 * _m10 + oZ2 * _m20 );
	
	result.oY0 = ( oX0 * _m01 + oY0 * _m11 + oZ0 * _m21 );
	result.oY1 = ( oX1 * _m01 + oY1 * _m11 + oZ1 * _m21 );
	result.oY2 = ( oX2 * _m01 + oY2 * _m11 + oZ2 * _m21 );
	
	result.oZ0 = ( oX0 * _m02 + oY0 * _m12 + oZ0 * _m22 );
	result.oZ1 = ( oX1 * _m02 + oY1 * _m12 + oZ1 * _m22 );
	result.oZ2 = ( oX2 * _m02 + oY2 * _m12 + oZ2 * _m22 );
	
	result.t0 = ( oX0 * _m03 + oY0 * _m13 + oZ0 * _m23 + t0 );
	result.t1 = ( oX1 * _m03 + oY1 * _m13 + oZ1 * _m23 + t1 );
	result.t2 = ( oX2 * _m03 + oY2 * _m13 + oZ2 * _m23 + t2 );

	return result;
}
// **************************************************************************

// **************************************************************************
inline cCartesian cCartesian::operator/( cCartesian const& _mat ) const
{
	cCartesian result;

	result.oX0 = oX0 * _mat.oX0 + oX1 * _mat.oX1 + oX2 * _mat.oX2;
	result.oX1 = oY0 * _mat.oX0 + oY1 * _mat.oX1 + oY2 * _mat.oX2;
	result.oX2 = oZ0 * _mat.oX0 + oZ1 * _mat.oX1 + oZ2 * _mat.oX2;
	
	result.oY0 = oX0 * _mat.oY0 + oX1 * _mat.oY1 + oX2 * _mat.oY2;
	result.oY1 = oY0 * _mat.oY0 + oY1 * _mat.oY1 + oY2 * _mat.oY2;
	result.oY2 = oZ0 * _mat.oY0 + oZ1 * _mat.oY1 + oZ2 * _mat.oY2;
	
	result.oZ0 = oX0 * _mat.oZ0 + oX1 * _mat.oZ1 + oX2 * _mat.oZ2;
	result.oZ1 = oY0 * _mat.oZ0 + oY1 * _mat.oZ1 + oY2 * _mat.oZ2;
	result.oZ2 = oZ0 * _mat.oZ0 + oZ1 * _mat.oZ1 + oZ2 * _mat.oZ2;
	
	result.t0 = oX0 * _mat.t0 + oX1 * _mat.t1 + oX2 * _mat.t2 - t0*oX0 - t1*oX1 - t2*oX2;
	result.t1 = oY0 * _mat.t0 + oY1 * _mat.t1 + oY2 * _mat.t2 - t0*oY0 - t1*oY1 - t2*oY2;
	result.t2 = oZ0 * _mat.t0 + oZ1 * _mat.t1 + oZ2 * _mat.t2 - t0*oZ0 - t1*oZ1 - t2*oZ2;

	return result;
}
// **************************************************************************

// **************************************************************************
inline cCartesian cCartesian::operator+( c3DPointD const& _vec ) const
{
	cCartesian result;
	memcpy( result.arr, arr, sizeof( float ) * 12 );
	result.t0 += _vec.x;
	result.t1 += _vec.y;
	result.t2 += _vec.z;

	return result;
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::operator +=( c3DPointD const& _vec )
{
	t0 += _vec.x; 
	t1 += _vec.y;
	t2 += _vec.z;
}
// **************************************************************************
// **************************************************************************
inline cCartesian cCartesian::operator-( c3DPointD const& _vec ) const
{
	cCartesian result;
	memcpy( result.arr, arr, sizeof( float ) * 12 );
	result.t0 -= _vec.x;
	result.t1 -= _vec.y;
	result.t2 -= _vec.z;

	return result;
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::operator -=( c3DPointD const& _vec )
{
	t0 -= _vec.x;
	t1 -= _vec.y;
	t2 -= _vec.z;
}// **************************************************************************

// **************************************************************************
template< typename DataT >
inline temp3DPoint<DataT> cCartesian::operator*( temp3DPoint<DataT> const& rGlobalPoint ) const
{
	return temp3DPoint<DataT>( (DataT)(oX0 * rGlobalPoint.x + oY0 * rGlobalPoint.y + oZ0 * rGlobalPoint.z + t0),
							(DataT)(oX1 * rGlobalPoint.x + oY1 * rGlobalPoint.y + oZ1 * rGlobalPoint.z + t1),
							(DataT)(oX2 * rGlobalPoint.x + oY2 * rGlobalPoint.y + oZ2 * rGlobalPoint.z + t2) );
}

// **************************************************************************
template< typename DataT >
inline temp2DPoint<DataT> cCartesian::operator*( temp2DPoint<DataT> const& rGlobalPoint ) const
{
	return temp2DPoint<DataT>( (DataT)(oX0 * rGlobalPoint.x + oY0 * rGlobalPoint.y + t0),
							(DataT)(oX1 * rGlobalPoint.x + oY1 * rGlobalPoint.y + t1),
							(DataT)(oX2 * rGlobalPoint.x + oY2 * rGlobalPoint.y + t2) );
}

// **************************************************************************
inline	c3DPointD cCartesian::operator*( c3DPointD const* pGlobalPoint ) const
{
	return c3DPointD (	oX0 * pGlobalPoint->x + oY0 * pGlobalPoint->y + oZ0 * pGlobalPoint->z + t0,
						oX1 * pGlobalPoint->x + oY1 * pGlobalPoint->y + oZ1 * pGlobalPoint->z + t1,
						oX2 * pGlobalPoint->x + oY2 * pGlobalPoint->y + oZ2 * pGlobalPoint->z + t2 );

}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline temp3DPoint<DataT> cCartesian::operator/( temp3DPoint<DataT> const& rLocalPoint ) const
{
  double dX = rLocalPoint.x - t0;
  double dY = rLocalPoint.y - t1;
  double dZ = rLocalPoint.z - t2;

  return temp3DPoint<DataT>(	DataT (dX * oX0 + dY * oX1 + dZ * oX2),
							DataT (dX * oY0 + dY * oY1 + dZ * oY2),
							DataT (dX * oZ0 + dY * oZ1 + dZ * oZ2) );
}
// **************************************************************************

// **************************************************************************
inline	c3DPointD cCartesian::operator/( c3DPointD const *pLocalPoint ) const
{
  double dX = pLocalPoint->x - t0;
  double dY = pLocalPoint->y - t1;
  double dZ = pLocalPoint->z - t2;

  return c3DPointD(	float (dX * oX0 + dY * oX1 + dZ * oX2),
					float (dX * oY0 + dY * oY1 + dZ * oY2),
					float (dX * oZ0 + dY * oZ1 + dZ * oZ2) );
}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline void cCartesian::Apply( temp3DPoint<DataT>& _rGlobalPoint ) const
{
	const double dX = _rGlobalPoint.x; 
	const double dY = _rGlobalPoint.y;
	const double dZ = _rGlobalPoint.z;
	_rGlobalPoint.x = DataT(oX0 * dX + oY0 * dY + oZ0 * dZ + t0);
	_rGlobalPoint.y = DataT(oX1 * dX + oY1 * dY + oZ1 * dZ + t1);
	_rGlobalPoint.z = DataT(oX2 * dX + oY2 * dY + oZ2 * dZ + t2);
}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline DataT cCartesian::ApplyZ( const DataT x, const DataT y, const DataT z ) const
{
	return DataT(oX2*x + oY2*y +oZ2*z + t2);
}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline void cCartesian::Rotate( temp3DPoint<DataT>& _rGlobalPoint ) const
{
	const double dX = _rGlobalPoint.x;
	const double dY = _rGlobalPoint.y;
	const double dZ = _rGlobalPoint.z;
	_rGlobalPoint.x = DataT(oX0 * dX + oY0 * dY + oZ0 * dZ);
	_rGlobalPoint.y = DataT(oX1 * dX + oY1 * dY + oZ1 * dZ);
	_rGlobalPoint.z = DataT(oX2 * dX + oY2 * dY + oZ2 * dZ);
}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline temp3DPoint<DataT> cCartesian::Rotate( const temp3DPoint<DataT>* _pGlobalPoint ) const
{
	const double dX = _pGlobalPoint->x;
	const double dY = _pGlobalPoint->y;
	const double dZ = _pGlobalPoint->z;
	return temp3DPoint<DataT>( DataT(oX0 * dX + oY0 * dY + oZ0 * dZ),
							DataT(oX1 * dX + oY1 * dY + oZ1 * dZ),
							DataT(oX2 * dX + oY2 * dY + oZ2 * dZ) );
}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline void cCartesian::Translate( temp3DPoint<DataT>& _rGlobalPoint ) const
{
	_rGlobalPoint.x += m[0][3];
	_rGlobalPoint.y += m[1][3];
	_rGlobalPoint.z += m[2][3];
}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline temp3DPoint<DataT> cCartesian::InvRotate( const temp3DPoint<DataT>* pLocalPoint ) const
{
  const double dX = pLocalPoint->x ;
  const double dY = pLocalPoint->y ;
  const double dZ = pLocalPoint->z ;

  return temp3DPoint<DataT>( DataT( dX * oX0 + dY * oX1 + dZ * oX2 ),
						  DataT( dX * oY0 + dY * oY1 + dZ * oY2 ),
						  DataT( dX * oZ0 + dY * oZ1 + dZ * oZ2 ));
}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline void cCartesian::InvRotate( temp3DPoint<DataT>& rLocalPoint ) const
{
  const double dX = rLocalPoint.x ;
  const double dY = rLocalPoint.y ;
  const double dZ = rLocalPoint.z ;

  rLocalPoint.x = DataT( dX * oX0 + dY * oX1 + dZ * oX2 );
  rLocalPoint.y = DataT( dX * oY0 + dY * oY1 + dZ * oY2 );
  rLocalPoint.z = DataT( dX * oZ0 + dY * oZ1 + dZ * oZ2 );
}

// **************************************************************************

// **************************************************************************
template< typename DataT >
inline DataT cCartesian::ApplyInverseZ( const temp3DPoint<DataT>& rLocalPoint ) const
{
  return DataT( (rLocalPoint.x - m[0][3]) * m[0][2] + 
				(rLocalPoint.y - m[1][3]) * m[1][2] + 
				(rLocalPoint.z - m[2][3]) * m[2][2] );
}
// **************************************************************************

// **************************************************************************
template< typename DataT >
inline void cCartesian::ApplyInverse( temp3DPoint<DataT>& rLocalPoint ) const
{
  const double dX = rLocalPoint.x - m[0][3];
  const double dY = rLocalPoint.y - m[1][3];
  const double dZ = rLocalPoint.z - m[2][3];

  rLocalPoint.x = DataT( dX * m[0][0] + dY * m[1][0] + dZ * m[2][0] );
  rLocalPoint.y = DataT( dX * m[0][1] + dY * m[1][1] + dZ * m[2][1] );
  rLocalPoint.z = DataT( dX * m[0][2] + dY * m[1][2] + dZ * m[2][2] );
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::GetTranslation(double &_rdX, double &_rdY, double &_rdZ) const
{
	_rdX = m[0][3];
	_rdY = m[1][3];
	_rdZ = m[2][3];
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::GetRotation( cCartesian &RotationMatrix ) const
{

	RotationMatrix.oX0 = oX0;
	RotationMatrix.oY0 = oY0;
	RotationMatrix.oZ0 = oZ0;

	RotationMatrix.oX1 = oX1;
	RotationMatrix.oY1 = oY1;
	RotationMatrix.oZ1 = oZ1;

	RotationMatrix.oX2 = oX2;
	RotationMatrix.oY2 = oY2;
	RotationMatrix.oZ2 = oZ2;

	//RotationMatrix = ( *this );
	//RotationMatrix.m[0][3] = RotationMatrix.m[1][3] = RotationMatrix.m[2][3] = 0.0;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::Invert()
{
	c3DPointD n( m[0][0], m[1][0], m[2][0] );
	c3DPointD o( m[0][1], m[1][1], m[2][1] );
	c3DPointD a( m[0][2], m[1][2], m[2][2] );
	c3DPointD p( m[0][3], m[1][3], m[2][3] );

	m[0][1] = n.y; m[0][2] = n.z; m[0][3] = -Dot(p, n);
	m[1][0] = o.x; m[1][2] = o.z; m[1][3] = -Dot(p, o);
	m[2][0] = a.x; m[2][1] = a.y; m[2][3] = -Dot(p, a);	
}
// **************************************************************************

//// **************************************************************************
//inline bool cCartesian::MakeOrthogonal( void )
//{
//	cDSMatrix Q(3);
//	cDSMatrix U(3);
//	SetRotTo( Q.GetArray() );
//	if( !Q.PolarDecomposition(U) )
//		return false;
//	GetRotFrom(U.GetArray());
//	return true;
//}
//// **************************************************************************

// **************************************************************************
inline double cCartesian::GetDeterminant() const
{
	return( m[0][0] * ( m[1][1] * m[2][2] - m[2][1] * m[1][2] )
		-   m[0][1] * ( m[1][0] * m[2][2] - m[2][0] * m[1][2] )
		+   m[0][2] * ( m[1][0] * m[2][1] - m[2][0] * m[1][1] ) );
}
// **************************************************************************

// **************************************************************************
inline double cCartesian::GetTrace() const
{
	return m[0][0] + m[1][1] + m[2][2];
}
// **************************************************************************

// **************************************************************************
inline	void cCartesian::SetXYZRotation( double _dX, double _dY, double _dZ )
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
	m[0][1] = SRSP * CYaw - CRoll * SYaw;
	m[0][2] = CRSP * CYaw + SRoll * SYaw;

	m[1][0] = CPitch * SYaw;
	m[1][1] = SRSP * SYaw + CRoll * CYaw;
	m[1][2] = CRSP * SYaw - SRoll * CYaw;

	m[2][0] =-SPitch;
	m[2][1] = SRoll * CPitch;
	m[2][2] = CRoll * CPitch;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::SetRotation( double dQuatW, double dQuatX, double dQuatY, double dQuatZ )
{
	double dUnit = dQuatW*dQuatW + dQuatX*dQuatX + dQuatY*dQuatY + dQuatZ*dQuatZ;
	if( fabs(dUnit - 1.0) > FLT_EPSILON && fabs(dUnit) > DBL_EPSILON )
	{
		dUnit = 1.0/sqrt(dUnit);
		dQuatW *= dUnit;
		dQuatX *= dUnit;
		dQuatY *= dUnit;
		dQuatZ *= dUnit;
	}

	const double dQX = 2.0 * dQuatX;
	const double dQY = 2.0 * dQuatY;
	const double dQZ = 2.0 * dQuatZ;
	
	const double WX = dQuatW * dQX;
	const double WY = dQuatW * dQY;
	const double WZ = dQuatW * dQZ;

	const double XX = dQX * dQuatX;
	const double XY = dQX * dQuatY;
	const double XZ = dQX * dQuatZ;

	const double YY = dQY * dQuatY;
	const double YZ = dQY * dQuatZ;
	const double ZZ = dQZ * dQuatZ;

	//Set 3x3 matrix from orthonormal basis
	//x axis
	m[0][0] = 1.0 - (YY + ZZ);
	m[0][1] = XY - WZ;
	m[0][2] = XZ + WY;
	
	//y axis
	m[1][0] = XY + WZ;
	m[1][1] = 1.0 - (XX + ZZ);
	m[1][2] = YZ - WX;
	
	//z axis
	m[2][0] = XZ - WY;
	m[2][1] = YZ + WX;
	m[2][2] = 1.0 - (XX + YY);
}
// **************************************************************************
// **************************************************************************
inline void	cCartesian::GetRotation( double& dQuatW, double& dQuatX, double& dQuatY, double& dQuatZ )	const
{
	const double dTrace = GetTrace();
	if ( dTrace >= 0.0 )
	{
		double dTmp = sqrt( dTrace + 1.0 );
		dQuatW = dTmp * 0.5;

		dTmp = 0.5/dTmp;
		dQuatX = (m[2][1] - m[1][2])*dTmp;
		dQuatY = (m[0][2] - m[2][0])*dTmp;
		dQuatZ = (m[1][0] - m[0][1])*dTmp;
	}
	else
	{
		//Find the largest component.  
		int i, j, k;
		if( m[0][0] >= m[1][1] )
		{
			if( m[0][0] >= m[2][2] )
			{//00 max element
				i = 0; j = 1; k = 2;
			}
			else
			{//22 max element
				i = 2; j = 0; k = 1;
			}
		}
		else if ( m[1][1] >= m[2][2] )
		{//11 max element
			i = 1; j = 2; k = 0;
		}
		else
		{//22 max element
			i = 2; j = 0; k = 1;
		}

		double* dArrPtr[3] = 
		{
			&dQuatX,
			&dQuatY,
			&dQuatZ,
		};
		
		double dTmp = sqrt(1.0 + m[i][i] - ( m[j][j] + m[k][k] ) );

		//Set the largest component
		*( dArrPtr[i] ) = dTmp * 0.5;
		dTmp = 0.5/dTmp;
		
		//Calculate remaining components
		*(dArrPtr[j]) = (m[j][i] + m[i][j]) * dTmp;
		*(dArrPtr[k]) = (m[k][i] + m[i][k]) * dTmp;
		dQuatW = (m[k][j] - m[j][k]) * dTmp;
	}
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::SetRotationYZ(c3DPointD Z, c3DPointD Y)
{
	Z.Normalize();
	Y.Normalize();

	c3DPointD X = Cross(Y,Z);
	X.Normalize();

	oX0 = X.x; oX1 = X.y; oX2 = X.z;
	oY0 = Y.x; oY1 = Y.y; oY2 = Y.z;
	oZ0 = Z.x; oZ1 = Z.y; oZ2 = Z.z;
}


// **************************************************************************
// **************************************************************************
inline void cCartesian::SetRotationZX(c3DPointD Z, c3DPointD X)
{
	Z.Normalize();
	X.Normalize();

	c3DPointD Y = Cross(Z,X);
	Y.Normalize();

	oX0 = X.x; oX1 = X.y; oX2 = X.z;
	oY0 = Y.x; oY1 = Y.y; oY2 = Y.z;
	oZ0 = Z.x; oZ1 = Z.y; oZ2 = Z.z;
}

// **************************************************************************
// **************************************************************************
inline void cCartesian::SetRotationXY(c3DPointD X, c3DPointD Y)
{
	X.Normalize();
	Y.Normalize();
	c3DPointD Z = Cross(X,Y);
	Z.Normalize();

	oX0 = X.x; oY0 = Y.x; oZ0 = Z.x;
	oX1 = X.y; oY1 = Y.y; oZ1 = Z.y;
	oX2 = X.z; oY2 = Y.z; oZ2 = Z.z;

}
// **************************************************************************
// **************************************************************************
inline void	cCartesian::Rotate180AroundX()
{
	// flip X rotation
	oY0 = -oY0;	oY1 = -oY1;	oY2 = -oY2;

	// flip Z rotation
	oZ0 = -oZ0;	oZ1 = -oZ1;	oZ2 = -oZ2;
}
// **************************************************************************
// **************************************************************************
inline void	cCartesian::Rotate180AroundY()
{
	// flip X rotation
	oX0 = -oX0;	oX1 = -oX1;	oX2 = -oX2;

	// flip Z rotation
	oZ0 = -oZ0;	oZ1 = -oZ1;	oZ2 = -oZ2;
}
// **************************************************************************
// **************************************************************************
inline void	cCartesian::Rotate180AroundZ()
{
	// flip X rotation
	oX0 = -oX0;	oX1 = -oX1;	oX2 = -oX2;

	// flip Y rotation
	oY0 = -oY0;	oY1 = -oY1;	oY2 = -oY2;
}
// **************************************************************************
// **************************************************************************
inline void	cCartesian::Rotate90AroundX()
{
	c3DPointD Tmp( -oY0, -oY1, -oY2 );
	oY0 = oZ0;	oY1 = oZ1;	oY2 = oZ2;
	oZ0 = Tmp.x; oZ1 = Tmp.y;	oZ2 = Tmp.z;
}
// **************************************************************************
// **************************************************************************
inline void	cCartesian::Rotate90AroundY()
{
	c3DPointD Tmp( -oZ0, -oZ1, -oZ2 );
	oZ0 = oX0;	oZ1 = oX1;	oZ2 = oX2;
	oX0 = Tmp.x; oX1 = Tmp.y;	oX2 = Tmp.z;
}
// **************************************************************************
// **************************************************************************
inline void	cCartesian::Rotate90AroundZ()
{
	c3DPointD Tmp( -oX0, -oX1, -oX2 );
	oX0 = oY0;	oX1 = oY1;	oX2 = oY2;
	oY0 = Tmp.x; oY1 = Tmp.y;	oY2 = Tmp.z;
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::SetRotation( const cCartesian& mat )
{
	oX0 = mat.oX0 ;  oY0 = mat.oY0 ; oZ0 = mat.oZ0 ;
	oX1 = mat.oX1 ;  oY1 = mat.oY1 ; oZ1 = mat.oZ1 ;
	oX2 = mat.oX2 ;  oY2 = mat.oY2 ; oZ2 = mat.oZ2 ;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::UpdateRotation(cCartesian const& _mat)
{
	cCartesian result;

	result.oX0 = _mat.oX0 * oX0 + _mat.oY0 * oX1 + _mat.oZ0 * oX2;
	result.oX1 = _mat.oX1 * oX0 + _mat.oY1 * oX1 + _mat.oZ1 * oX2;
	result.oX2 = _mat.oX2 * oX0 + _mat.oY2 * oX1 + _mat.oZ2 * oX2;
												  
	result.oY0 = _mat.oX0 * oY0 + _mat.oY0 * oY1 + _mat.oZ0 * oY2;
	result.oY1 = _mat.oX1 * oY0 + _mat.oY1 * oY1 + _mat.oZ1 * oY2;
	result.oY2 = _mat.oX2 * oY0 + _mat.oY2 * oY1 + _mat.oZ2 * oY2;
												  
	result.oZ0 = _mat.oX0 * oZ0 + _mat.oY0 * oZ1 + _mat.oZ0 * oZ2;
	result.oZ1 = _mat.oX1 * oZ0 + _mat.oY1 * oZ1 + _mat.oZ1 * oZ2;
	result.oZ2 = _mat.oX2 * oZ0 + _mat.oY2 * oZ1 + _mat.oZ2 * oZ2;

	
	oX0 =result.oX0;
	oX1 =result.oX1;
	oX2 =result.oX2;
						
	oX0 =result.oY0;
	oX1 =result.oY1;
	oX2 =result.oY2;
						
	oX0 =result.oZ0;
	oX1 =result.oZ1;
	oX2 =result.oZ2;
}
// **************************************************************************

// **************************************************************************
inline double* cCartesian::GetGLMatrix( double _GLMatrix[16] ) const
{
	_GLMatrix[ 0] = m[0][0];
	_GLMatrix[ 1] = m[1][0];
	_GLMatrix[ 2] = m[2][0];
	_GLMatrix[ 3] = 0.0;
	_GLMatrix[ 4] = m[0][1];
	_GLMatrix[ 5] = m[1][1];
	_GLMatrix[ 6] = m[2][1];
	_GLMatrix[ 7] = 0.0;
	_GLMatrix[ 8] = m[0][2];
	_GLMatrix[ 9] = m[1][2];
	_GLMatrix[10] = m[2][2];
	_GLMatrix[11] = 0.0;
	_GLMatrix[12] = m[0][3];
	_GLMatrix[13] = m[1][3];
	_GLMatrix[14] = m[2][3];
	_GLMatrix[15] = 1.0;

	return _GLMatrix;
}
// **************************************************************************

// **************************************************************************
inline void cCartesian::SetGLMatrix( const double _GLMatrix[16] ) 
{
	m[0][0] = _GLMatrix[0];
	m[1][0] = _GLMatrix[1];
	m[2][0] = _GLMatrix[2];
	m[0][1] = _GLMatrix[4];
	m[1][1] = _GLMatrix[5];
	m[2][1] = _GLMatrix[6];
	m[0][2] = _GLMatrix[8];
	m[1][2] = _GLMatrix[9];
	m[2][2] = _GLMatrix[10];
	m[0][3] = _GLMatrix[12];
	m[1][3] = _GLMatrix[13];
	m[2][3] = _GLMatrix[14];
}
// **************************************************************************
// **************************************************************************
inline float* cCartesian::GetMatrixData( float * _fSpace ) const
{
	_fSpace[ 0] = (float)oX0;
	_fSpace[ 1] = (float)oX1;
	_fSpace[ 2] = (float)oX2;
	_fSpace[ 3] = 0.0f;
	_fSpace[ 4] = (float)oY0;
	_fSpace[ 5] = (float)oY1;
	_fSpace[ 6] = (float)oY2;
	_fSpace[ 7] = 0.0f;
	_fSpace[ 8] = (float)oZ0;
	_fSpace[ 9] = (float)oZ1;
	_fSpace[10] = (float)oZ2;
	_fSpace[11] = 0.0f;
	_fSpace[12] = (float)t0;
	_fSpace[13] = (float)t1;
	_fSpace[14] = (float)t2;
	_fSpace[15] = 1.0f;

	return _fSpace;
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::SetMatrixData( const float * _fSpace )
{
	oX0 = _fSpace[ 0];
	oX1 = _fSpace[ 1];
	oX2 = _fSpace[ 2];

	oY0 = _fSpace[ 4];
	oY1 = _fSpace[ 5];
	oY2 = _fSpace[ 6];	
	
	oZ0 = _fSpace[ 8];
	oZ1 = _fSpace[ 9];
	oZ2 = _fSpace[10];	
	
	t0 = _fSpace[12];
	t1 = _fSpace[13];
	t2 = _fSpace[14];
}
// **************************************************************************

// **************************************************************************
inline float* cCartesian::Get2DMatrixData( float * _fSpace ) const
{
	_fSpace[ 0] = (float)oX0;
	_fSpace[ 1] = (float)oX1;
	_fSpace[ 2] = (float)oX2;
	_fSpace[ 3] = (float)0.0;
	
	_fSpace[ 4] = (float)oY0;
	_fSpace[ 5] = (float)oY1;
	_fSpace[ 6] = (float)oY2;
	_fSpace[ 7] = (float)0.0;
	
	_fSpace[ 8] = (float)t0;
	_fSpace[ 9] = (float)t1;
	_fSpace[10] = (float)t2;
	_fSpace[11] = (float)0.0;
	
	_fSpace[12] = (float)0.0f;
	_fSpace[13] = (float)0.0f;
	_fSpace[14] = (float)0.0f;
	_fSpace[15] = (float)1.0;

	return _fSpace;
}
// **************************************************************************
// **************************************************************************
inline c3DPointD cCartesian::GetScale( void )
{
	c3DPointD scale(	sqrt( oX0 * oX0 + oX1 * oX1 + oX2 * oX2 ),
						sqrt( oY0 * oY0 + oY1 * oY1 + oY2 * oY2 ),
						sqrt( oZ0 * oZ0 + oZ1 * oZ1 + oZ2 * oZ2 ) );
	if( GetDeterminant() < 0.0f )
		scale.x *= -1.0f;

	return scale;
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::Scale( const c3DPointD& _vecScale )
{
	oX0 *= _vecScale.x;
	oX1 *= _vecScale.x;
	oX2 *= _vecScale.x;

	oY0 *= _vecScale.y;
	oY1 *= _vecScale.y;
	oY2 *= _vecScale.y;

	oZ0 *= _vecScale.z;
	oZ1 *= _vecScale.z;
	oZ2 *= _vecScale.z;
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::ScaleX( float _scale )
{
	oX0 *= _scale;
	oX1 *= _scale;
	oX2 *= _scale;
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::ScaleY( float _scale )
{
	oY0 *= _scale;
	oY1 *= _scale;
	oY2 *= _scale;
}
// **************************************************************************
// **************************************************************************
inline void cCartesian::ScaleZ( float _scale )
{
	oZ0 *= _scale;
	oZ1 *= _scale;
	oZ2 *= _scale;
}
// **************************************************************************
// **************************************************************************
inline bool cCartesian::AreIdentical( const cCartesian& Mat, const double dTolerance/*=FLT_EPSILON*/ ) const
{
	for( int i = 0; i < 12; ++i )
		if( fabs( arr[i] - Mat.arr[i] ) > dTolerance )
			return false;
	return true;
}

#ifdef _MSC_VER
#	pragma warning(pop)
#endif
