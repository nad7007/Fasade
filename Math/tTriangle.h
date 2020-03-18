#pragma once
#include <3ADALib\Math\GeomCalculations.h>

//******************************************************************************************
template<typename VertexT>
class tTriangle
{
//******************************************************************************************
public://Data

	VertexT*							pV[3];
	t3DPoint< typename VertexT::DataT >	vecNormal;

	union{
		unsigned						uFlag;
		char							bEdgeFl[3];
	};
	
//******************************************************************************************
public://Constructor & destructor	

	tTriangle()
		:uFlag(0)
		{ pV[0] = pV[1]= pV[2] = NULL; }

	tTriangle(VertexT* P1, VertexT* P2, VertexT* P3)
	{
		pV[0] = P1;
		pV[1] = P2;
		pV[2] = P3;
		_3ADA::GetCrossProduct(*P1,  *P2, *P3, vecNormal );
		vecNormal.Normalize();
		uFlag = 0;
	}
	
	tTriangle(VertexT* P1, VertexT* P2, VertexT* P3, const cF3DVector& Normal )
	{
		pV[0] = P1;
		pV[1] = P2;
		pV[2] = P3;
		vecNormal = Normal;
		uFlag = 0;
	}

	tTriangle(const tTriangle& other )
	{
		(*this) = other;
	}

	virtual ~tTriangle(){};

	//! copy operator
	tTriangle& operator = ( const tTriangle& other )
	{
		pV[0] = other.pV[0];
		pV[1] = other.pV[1];
		pV[2] = other.pV[2];
		vecNormal  = other.vecNormal;
		uFlag = other.uFlag;
		return *this;
	}

//******************************************************************************************
public://Methods

	void		CalculateNormal()
					{ 
						_3ADA::GetCrossProduct(*pV[0],  *pV[1], *pV[2], vecNormal );
						vecNormal.Normalize();
					}

	bool		IsValid() const 
					{ return ( pV[0] && pV[1] && pV[2] && pV[0]->IsValid()&& pV[1]->IsValid()&& pV[2]->IsValid() ); }

	double		GetEdgeLength(char Index) const
					{ _ASSERT(pV[0] && pV[1] && pV[2] ); return pV[Index]->GetDistanceTo( *pV[(Index+1)%3] ); }

	double		GetPerimeter() const
					{ return  IsValid() ? ( GetEdgeLength(0) + GetEdgeLength(1) + GetEdgeLength(2) ) : 0; }

	double		GetLongestEdgeLength() const 
					{ return  IsValid() ? __max( GetEdgeLength(0), __max( GetEdgeLength(1), GetEdgeLength(2) ) )  : 0; }

	double		GetShortestEdgeLength() const
					{ return  IsValid() ? __min( GetEdgeLength(0), __min( GetEdgeLength(1), GetEdgeLength(2) ) ) : 0; }

	unsigned char GetShortestEdgeIndex() const
					{ 
						const double Y = GetEdgeLength[1];
						const double Z = GetEdgeLength[2];
						return ( GetEdgeLength[0] < __min( Y, Z) ) ? 0 : ( Y < Z ) ? 1 : 2; 
					}
	unsigned char GetLongestEdgeIndex() const
					{ 
						const double Y = GetEdgeLength[1];
						const double Z = GetEdgeLength[2];
						return ( GetEdgeLength[0] > __max( Y, Z) ) ? 0 : ( Y > Z ) ? 1 : 2; 
					}
	void		GetSortedEdgeIndexByLength( char& Longest, char& Middle, char& Shortest ) const;
	
	cD3DPoint	GetCentroid() const//center of medians
					{ _ASSERT(pV[0] && pV[1] && pV[2] ); return (*pV[0] + *pV[1] + *pV[2] ) /3.0; }

	double		GetArea() const;

};
//******************************************************************************************
//Inlines

//******************************************************************************************
template<typename VertexT>
inline double tTriangle<VertexT>::GetArea() const
{
	if( !IsValid() )
		return 0;

	const double A = GetEdgeLength(0);
	const double B = GetEdgeLength(1);
	const double C = GetEdgeLength(2);
	const double P = (A+B+C) * 0.5;

	return sqrt( P*(P-A)*(P-B)*(P-C));
}
//******************************************************************************************
template<typename VertexT>
inline void tTriangle<VertexT>::GetSortedEdgeIndexByLength( char& Longest, char& Middle, char& Shortest ) const
{
	const double A = GetEdgeLength(0);
	const double B = GetEdgeLength(1);
	const double C = GetEdgeLength(2);
	
	if( A >= B && B >= C )
	{
		Longest = 0;
		Middle = 1;
		Shortest = 2;
	}
	else if( A>=C && C >= B )
	{
		Longest = 0;
		Middle = 2;
		Shortest = 1;
	}
	else if( B >= A && A >= C )
	{
		Longest = 1;
		Middle = 0;
		Shortest = 2;
	}
	else if( B >= C && C >= A )
	{
		Longest = 1;
		Middle = 2;
		Shortest = 0;
	}
	else if( C >= B && B >= A )
	{
		Longest = 2;
		Middle = 1;
		Shortest = 0;
	}
	else 
	{
		Longest = 2;
		Middle = 0;
		Shortest = 1;
	}
}
//******************************************************************************************

