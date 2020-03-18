#pragma once

#include <limits>

//**************************************************************************************

#ifdef _MSC_VER
#	pragma warning( push )
//#	pragma warning( disable : 4201 )// nonstandard extension used : nameless struct/union
//#	pragma warning( disable : 4244 )// ASSERT definition
#endif

//**************************************************************************************
//Template class for 3D point of type DataT
//**************************************************************************************
template< typename DataT >
struct temp3DPoint
{
//**************************************************************************************
//	Data Members
	union {
		struct { DataT x, y, z; };
		DataT arr[3];
	};

//**************************************************************************************
//	Constructors

	temp3DPoint()
		:x(0),y(0),z(0){}
	temp3DPoint(DataT rInit )
		:x(rInit),y(rInit),z(rInit){}
	temp3DPoint(DataT rX, DataT rY, DataT rZ )
		: x(rX),y(rY), z(rZ) {}


	//Elementary operations
	//**************************************************************************************
	temp3DPoint<DataT> operator-() const
		{ return temp3DPoint<DataT>(-x, -y, -z); }

	template< typename OtherT>
	const temp3DPoint<DataT>& operator -= ( const temp3DPoint<OtherT> &rPoint )
		{ x -= (DataT)rPoint.x; y -= (DataT)rPoint.y; z -= (DataT)rPoint.z; return (*this);}
	const temp3DPoint<DataT>& operator -= ( double dValue )
		{ x -= dValue; y -= dValue; z -= dValue; return (*this); }
	
	template< typename OtherT>
	const temp3DPoint<DataT>& operator += ( const temp3DPoint<OtherT> &rPoint )
		{ x += (DataT)rPoint.x; y += (DataT)rPoint.y; z += (DataT)rPoint.z; return (*this);}
	const temp3DPoint<DataT>& operator += ( double dValue )
		{ x += dValue; y += dValue; z += dValue; return (*this); }

	template< typename OtherT>
	const temp3DPoint<DataT>& operator /= ( const temp3DPoint<OtherT> &rPoint )
		{ x /= (DataT)rPoint.x; y /= (DataT)rPoint.y; z /= (DataT)rPoint.z; return (*this);}
	const temp3DPoint<DataT>& operator /= ( double dValue )
		{ x /= dValue; y /= dValue; z /= dValue; return (*this); }
	
	template< typename OtherT>
	const temp3DPoint<DataT>& operator *= ( const temp3DPoint<OtherT> &rPoint )
		{ x *= (DataT)rPoint.x; y *= (DataT)rPoint.y; z *= (DataT)rPoint.z; return (*this); }
	const temp3DPoint<DataT>& operator *= ( double dValue )
		{ x *= dValue; y *= dValue; z *= dValue; return (*this); }

	
	template< typename OtherT>
	temp3DPoint<DataT> operator-( const temp3DPoint<OtherT>& Point ) const
		{ return temp3DPoint<DataT>(DataT(x - Point.x), DataT(y - Point.y), DataT(z - Point.z)); }
	temp3DPoint<DataT> operator -( double dValue ) const 
		{ return temp3DPoint<DataT>(DataT(x - dValue), DataT(y - dValue), DataT(z - dValue));}
	friend temp3DPoint<DataT> operator - (DataT  Value, const temp3DPoint<DataT>& Point )
		{ return temp3DPoint<DataT>( Value - Point.x, Value - Point.y, Value - Point.z); }

	template< typename OtherT>
	temp3DPoint<DataT> operator+(const temp3DPoint<OtherT>& Point ) const 
		{ return temp3DPoint<DataT>(DataT(x + Point.x), DataT(y + Point.y), DataT(z + Point.z)); }
	temp3DPoint<DataT> operator +( double dValue ) const 
		{ return temp3DPoint<DataT>(DataT(x + dValue), DataT(y + dValue), DataT(z + dValue));}
	friend  temp3DPoint<DataT> operator+(DataT  Value, const temp3DPoint<DataT>& Point )
		{ return temp3DPoint<DataT>( Value + Point.x, Value + Point.y, Value + Point.z); }


	template< typename OtherT>
	temp3DPoint<DataT> operator/( const temp3DPoint<OtherT>& Point ) const 
		{ return temp3DPoint<DataT>(DataT(x / Point.x), DataT(y / Point.y), DataT(z / Point.z) ); }
	temp3DPoint<DataT> operator/( double dValue ) const 
		{ return temp3DPoint<DataT>(DataT(x / dValue), DataT(y / dValue), DataT(z / dValue));}
	friend  temp3DPoint<DataT> operator/( double  Value, const temp3DPoint<DataT>& Point )
		{ return temp3DPoint<DataT>( Value / Point.x, Value / Point.y, Value / Point.z); }

	template< typename OtherT>
	temp3DPoint<DataT> operator*( const temp3DPoint<OtherT>& Point ) const 
		{ return temp3DPoint<DataT>(DataT(x * Point.x), DataT(y * Point.y), DataT(z * Point.z));}
	temp3DPoint<DataT> operator*( double dValue ) const 
		{ return temp3DPoint<DataT>(DataT(x * dValue), DataT(y * dValue), DataT(z * dValue));}
	friend  temp3DPoint<DataT> operator*( double  Value, const temp3DPoint<DataT>& Point )
		{ return temp3DPoint<DataT>( DataT(Value * Point.x), DataT(Value * Point.y), DataT(Value * Point.z) ); }


	//Boolean operations
	//**************************************************************************************
	bool operator==(const temp3DPoint<DataT>& Point ) const 
		{ return ( x == Point.x	&& y == Point.y	&& z == Point.z ); }
	bool operator==(DataT Value ) const
		{ return ( x == Value	&& y == Value	&& z == Value ); }

	bool operator!=(const temp3DPoint<DataT>& Point ) const 
		{ return ( x != Point.x	|| y != Point.y	|| z != Point.z ); }
	bool operator!=(DataT Value ) const
		{ return ( x != Value	|| y != Value	|| z != Value ); }

//**************************************************************************************
//	Member Functions

	void		Clear()				{ x = y = z = 0; }
	void		Normalize()
	{
		double dS = x * x + y * y + z * z;
		ASSERT(dS > GetEpsilon());

		dS = 1.0 /sqrt(dS);
		x *= dS;
		y *= dS;
		z *= dS;
	}

	bool		IsZero()const		{ return x==y && x==z && x<= GetEpsilon() && x >= - GetEpsilon(); }

	unsigned	GetOrder() const	{return 3;}
	
	void Set( double rX, double rY, double rZ )
		{  x = (DataT)rX; y = (DataT)rY; z = (DataT)rZ; }
	
	double GetSquaredLength() const 
		{ return (x*(double)x + y*(double)y + z*(double)z); }

	double GetLength() const
		{ return sqrt( x*(double)x + y*(double)y + z*(double)z ); }

	template< typename OtherT>
	double GetDistanceTo(const temp3DPoint<OtherT>& Point) const
		{ return sqrt( (x - (double)Point.x)*(x - Point.x) + (y - (double)Point.y)*(y - Point.y) + (z - (double)Point.z)*(z -Point.z) );	}

	template< typename OtherT>
	double GetSquaredDistanceTo(const temp3DPoint<OtherT>& Point) const
		{ return (x - (double)Point.x)*(x - Point.x) + (y -(double)Point.y)*(y - Point.y) + (z -(double)Point.z)*(z - Point.z); }


	void Neg()
		{ x = -x; y = -y; z = -z; }
	void Abs()
		{ x = abs(x); y = abs(y); z = abs(z); }
	
	DataT Min()const
		{ return __min( x, __min( y, z));  }
	DataT Max()const
		{ return __max( x, __max( y, z)); }
	DataT AbsMin()const
		{  return __min( abs(x), __min( abs(y), abs(z))); }
	DataT AbsMax()const
		{ return __max( abs(x), __max( abs(y), abs(z))); }

	unsigned char MinIndex() const
		{ return ( x < __min( y, z) ) ? 0 : ( y < z ) ? 1 : 2; }
	unsigned char MaxIndex() const
		{ return ( x > __max( y, z) ) ? 0 : ( y > z ) ? 1 : 2; }
	unsigned char AbsMinIndex() const
		{ const DataT ay = abs(y); const DataT az = abs(z); return ( abs(x) < __min( ay, az) ) ? 0 : ( ay < az ) ? 1 : 2; }
	unsigned char AbsMaxIndex() const
		{ const DataT ay = abs(y); const DataT az = abs(z); return ( abs(x) > __max( ay, az) )  ?  0 : ( ay > az ) ? 1 : 2; }
	
	DataT GetEpsilon()const
		{
			return std::numeric_limits<DataT>::epsilon();
		}
};

//**************************************************************************************
//Vector products
//Dot
template<typename T1, typename T2>
inline double Dot(const temp3DPoint<T1>& Point1, const temp3DPoint<T2>& Point2 )
{ 
	return (Point1.x * (double)Point2.x + Point1.y * (double)Point2.y + Point1.z * (double)Point2.z); 
}

//Cross
template<typename T1, typename T2>
inline temp3DPoint<double> Cross (const temp3DPoint<T1>& Point1, const temp3DPoint<T2>& Point2 )
{ 
	return temp3DPoint<double>(Point1.y * (double)Point2.z - Point1.z * (double)Point2.y,
							Point1.z * (double)Point2.x - Point1.x * (double)Point2.z,
							Point1.x * (double)Point2.y - Point1.y * (double)Point2.x ); 
}

//Cross Special
template<typename T1, typename T2>
inline void Cross( const temp3DPoint<T1>& Point1, const temp3DPoint<T2>& Point2,
					double& dXres, double& dYres, double& dZres )
{ 
	dXres = Point1.y * (double)Point2.z - Point1.z * (double)Point2.y;
	dYres = Point1.z * (double)Point2.x - Point1.x * (double)Point2.z;
	dZres = Point1.x * (double)Point2.y - Point1.y * (double)Point2.x; 
}

//Triple product: (P1 % P2)^P3
template<typename T1, typename T2, typename T3>
inline double Triple (const temp3DPoint<T1>& Point1, const temp3DPoint<T2>& Point2, const temp3DPoint<T3>& Point3 )
{ 
	return	(	(Point1.y * (double)Point2.z - Point1.z * (double)Point2.y) * Point3.x 
			+	(Point1.z * (double)Point2.x - Point1.x * (double)Point2.z) * Point3.y
			+	(Point1.x * (double)Point2.y - Point1.y * (double)Point2.x) * Point3.z );
}

template<typename T1, typename T2>
inline double AngleBetween(const temp3DPoint<T1>& Point1, const temp3DPoint<T2>& Point2 )
{ 
	double CosAlpha;

	double fL1 = Point1.GetLength();
	double fL2 = Point2.GetLength();

	if ( fL1 < 1E-5 || fL2 < 1E-5 ) 
		return 0.0;

	fL1 = 1/fL1;
	fL2 = 1/fL2;

	CosAlpha = (Point1.x*fL1) * (Point2.x*fL2) + (Point1.y*fL1) * (Point2.y*fL2) + (Point1.z*fL1) * (Point2.z*fL2);

	if (CosAlpha < -1.0) 
	{
		CosAlpha = -1.0;
	}
	else
	{
		if ( CosAlpha > 1.0)
		{
		CosAlpha = 1.0;
		}
	}

	return acos( CosAlpha );
}
//**************************************************************************************

#ifdef _MSC_VER
#	pragma warning(pop)
#endif
