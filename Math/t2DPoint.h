//***************************************************************************************
#pragma once

#include <limits>

//**************************************************************************************

#ifdef _MSC_VER
#	pragma warning( push )
//#	pragma warning( disable : 4201 )// nonstandard extension used : nameless struct/union
#endif

//Template class for 3D point of type DataT
//**************************************************************************************
template< typename DataT >
struct temp2DPoint
{

//**************************************************************************************
//	Data Members
	union {
		struct { DataT x, y; };
		DataT arr[2];
	};

//**************************************************************************************
//	Constructors

	temp2DPoint()
		:x(0),y(0){}
	temp2DPoint(DataT rInit )
		:x(rInit),y(rInit){}
	temp2DPoint( DataT rX, DataT rY ) 
		: x(rX),y(rY){}

	//Elementary operations
	//**************************************************************************************
	temp2DPoint<DataT> operator-() const
		{ return temp2DPoint<DataT>(-x, -y); }

	template< typename OtherT>
	const temp2DPoint<DataT>& operator -= ( const temp2DPoint<OtherT> &rPoint )
		{ x -= (DataT)rPoint.x; y -= (DataT)rPoint.y; return (*this);}
	
	template< typename OtherT>
	const temp2DPoint<DataT>& operator += ( const temp2DPoint<OtherT> &rPoint )
		{ x += (DataT)rPoint.x; y += (DataT)rPoint.y; return (*this);}
	
	template< typename OtherT>
	const temp2DPoint<DataT>& operator *= ( const temp2DPoint<OtherT> &rPoint )
		{ x *= (DataT)rPoint.x; y *= (DataT)rPoint.y; return (*this); }
	
	template< typename OtherT>
	const temp2DPoint<DataT>& operator /= ( const temp2DPoint<OtherT> &rPoint )
		{ x /= (DataT)rPoint.x; y /= (DataT)rPoint.y; return (*this);}
	
	template< typename OtherT>
	temp2DPoint<DataT> operator-( const temp2DPoint<OtherT>& Point ) const
		{ return temp2DPoint<DataT>(DataT(x - Point.x), DataT(y - Point.y)); }
	
	template< typename OtherT>
	temp2DPoint<DataT> operator+(const temp2DPoint<OtherT>& Point ) const 
		{ return temp2DPoint<DataT>(DataT(x + Point.x), DataT(y + Point.y)); }

	template< typename OtherT>
	temp2DPoint<DataT> operator*( const temp2DPoint<OtherT>& Point ) const 
		{ return temp2DPoint<DataT>(DataT(x * Point.x), DataT(y * Point.y));}

	template< typename OtherT>
	temp2DPoint<DataT> operator/( const temp2DPoint<OtherT>& Point ) const 
		{ return temp2DPoint<DataT>(DataT(x / Point.x), DataT(y / Point.y)); }

	friend temp2DPoint<DataT> operator - ( DataT  Value, const temp2DPoint<DataT>& Point )
		{ return temp2DPoint<DataT>( Value - Point.x, Value - Point.y); }

	friend  temp2DPoint<DataT> operator+( DataT  Value, const temp2DPoint<DataT>& Point )
		{ return temp2DPoint<DataT>( Value + Point.x, Value + Point.y); }

	template< typename OtherT1, typename OtherT2>
	friend  temp2DPoint<DataT> operator*( const temp2DPoint<OtherT1>& Point1, const temp2DPoint<OtherT2>& Point2 )
		{ return temp2DPoint<DataT>( DataT(Point1.x * Point2.x), DataT(Point1.y * Point2.y)); }

	temp2DPoint<DataT> operator*( double dValue ) const 
		{ return temp2DPoint<DataT>(DataT(x * dValue), DataT(y * dValue));}

	temp2DPoint<DataT> operator/( double dValue ) const 
		{ return temp2DPoint<DataT>(DataT(x / dValue), DataT(y / dValue));}

	const temp2DPoint<DataT>& operator += ( double dValue )
		{ x += dValue; y += dValue; return (*this); }

	const temp2DPoint<DataT>& operator -= ( double dValue )
		{ x -= dValue; y -= dValue; return (*this); }

	const temp2DPoint<DataT>& operator *= ( double dValue )
		{ x *= dValue; y *= dValue; return (*this); }

	const temp2DPoint<DataT>& operator /= ( double dValue )
		{ x /= DataT(dValue); y /= DataT(dValue); return (*this); }
	
	friend  temp2DPoint<DataT> operator/( double  Value, const temp2DPoint<DataT>& Point )
		{ return temp2DPoint<DataT>( Value / Point.x, Value / Point.y); }

	friend  temp2DPoint<DataT> operator*( double  Value, const temp2DPoint<DataT>& Point )
		{ return temp2DPoint<DataT>( DataT(Value * Point.x), DataT(Value * Point.y)); }

	//**************************************************************************************
	//Boolean operations
	//**************************************************************************************
	bool operator==(const temp2DPoint<DataT>& Point ) const 
		{ return ( x == Point.x	&& y == Point.y ); }
	bool operator==(DataT Value ) const 
		{ return ( x == Value	&& y == Value ); } 

	bool operator!=(const temp2DPoint<DataT>& Point ) const 
		{ return ( x != Point.x	|| y != Point.y); }
	bool operator!=(DataT Value ) const 
		{ return ( x != Value	|| y != Value ); }

//**************************************************************************************
//	Member Functions

	void Set( DataT rX, DataT rY )
	{ 
		x = rX; y = rY;
	}
	
	double GetSquaredLength() const 
	{
		return (x*(double)x + y*(double)y );
	}

	double GetLength() const
	{ 
		return _3ADA::fsqrt( x*(double)x + y*(double)y );
	}

	template< typename OtherT>
	double GetDistanceTo(const temp2DPoint<OtherT>& Point) const
	{ 
		return _3ADA::fsqrt( _3ADA::sqr(x - (double)Point.x) + _3ADA::sqr(y -(double)Point.y) );
	}

	template< typename OtherT>
	double GetSquaredDistanceTo(const temp2DPoint<OtherT>& Point) const
	{ 
		return (_3ADA::sqr(x - (double)Point.x) + _3ADA::sqr(y - (double)Point.y) ); 
	}

	void Clear()
	{ x = y = (DataT)(0); }

	void Abs()
	{ 
		x = abs(x);
		y = abs(y);
	}
	
	void Neg()
	{	
		x = -x ;
		y = -y ;
	}

	DataT Min()const
	{ 
		return __min( x, y); 
	}
	DataT AbsMin()const
	{ 
		return __min( abs(x), abs(y) );
	}
	unsigned char MinIndex() const
	{
		return ( x <= y ) ? 0 : 1;
	}
	unsigned char AbsMinIndex() const
	{
		const DataT ax = abs(x);
		const DataT ay = abs(y);
		return ( ax < ay ) ? 0 : 1;
	}

	DataT Max()const
	{ 
		return __max( x, y );
	}
	DataT AbsMax()const
	{ 
		return __max( abs(x), abs(y) );
	}
	unsigned char MaxIndex() const
	{
		return ( x > y ) ? 0 : 1;
	}
	unsigned char AbsMaxIndex() const
	{
		const DataT ax = abs(x);
		const DataT ay = abs(y);
		return ( ax >= ay ) ?  0 : 1;
	}

	DataT MaxPossible()const
	{
		return GetMaxVal(x);
	}
	DataT MinPossible()const
	{ 
		return GetMinVal(x); 
	}
	DataT GetEpsilon()const
	{ 
		return std::numeric_limits<DataT>::epsilon();
	}
};

//**************************************************************************************
//Vector products
//Dot
template<typename T1, typename T2>
inline double Dot(const temp2DPoint<T1>& Point1, const temp2DPoint<T2>& Point2 )
{ 
	return (Point1.x * (double)Point2.x + Point1.y * (double)Point2.y ); 
}
//**************************************************************************************

#ifdef _MSC_VER
#	pragma warning(pop)
#endif

