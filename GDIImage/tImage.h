////////////////////////////////////////////////////////////////////
#pragma once
#include <crtdbg.h>
class cCartesian;

////////////////////////////////////////////////////////////////////
class cImageMemAllocator
{
public:

	virtual void* Alloc( unsigned sizeInBytes )		{	return new unsigned char[ sizeInBytes ]; };
	virtual void Free( void* ptr )					{	delete [] ((unsigned char*)ptr);	};

	static cImageMemAllocator& Instance()
	{
		static cImageMemAllocator theInstance;
		return theInstance;
	};
};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
class tImage
{
////////////////////////////////////////////////////////////////////
public://Types
	typedef TData DataT;
	struct stData
	{
		TData Data[m_nChannels];
	};

////////////////////////////////////////////////////////////////////
public://Constriuctors & destructor

	tImage(cImageMemAllocator* memAllocator = &cImageMemAllocator::Instance() ):
		m_pRawData(0)
	,	m_nWidth(0)
	,	m_nHeight(0)
	,	m_nRowLengthBytes(0)
	,	m_nRowAlign(0)
	,	m_pData((char*&)m_pRawData)
	,	m_allocator(memAllocator)
	{}
	
	tImage( const tImage& rhs ):
		m_pRawData(NULL)
	,	m_pData((char*&)m_pRawData)
	,   m_nWidth(0)
	,	m_nHeight(0)
	,	m_nRowLengthBytes(0)
	,	m_nRowAlign(0)
	,	m_allocator(NULL)
	{
		CopyFrom( rhs );
	};
	tImage& operator = ( const tImage& rhs )
	{
		if( this != &rhs )
		{
			CopyFrom( rhs );
		}
		return *this;
	}
	~tImage()
	{
		Destroy();
	};

////////////////////////////////////////////////////////////////////
public://Methods

	void	Transform(const cCartesian& ) { _ASSERT(false); }

	bool	IsValid() const		{	return (m_pRawData!=NULL); };

	TData*	GetRawData() const	{ return m_pRawData;	};

	TData*	operator []( unsigned row ) const	{ return (TData*)( m_pData + row*m_nRowLengthBytes); }

	bool	AreIdentical( const tImage& rhs )const	{	return m_pRawData == rhs.GetRawData();	}

	stData&	GetAt( unsigned x, unsigned y ) const	
		{
			_ASSERT( x < m_nWidth && y < m_nHeight );
			return *((stData*)(m_pData + y*m_nRowLengthBytes + (x*m_nChannels*sizeof(TData) ) ));
		}
	TData&	GetAt( unsigned linearIndex ) const	
		{
			_ASSERT( linearIndex < m_nWidth*m_nHeight );
			return GetAt(linearIndex % m_nWidth, linearIndex / m_nWidth );  
		}

	unsigned GetNumChannels() const	 {	return m_nChannels; };

	unsigned GetWidth() const	{return m_nWidth; };

	unsigned GetHeight() const	{return m_nHeight; };

	unsigned GetRowLengthBytes() const { return m_nRowLengthBytes; };

	unsigned GetDataLengthBytes() const { return m_nChannels*sizeof(TData); };

	unsigned GetRowAlign() const { 	return m_nRowAlign; };

	unsigned GetImageSize() const { return m_nRowLengthBytes * m_nHeight; };

	inline TData GetMax() const;
	inline TData GetMax(unsigned& MaxX, unsigned& MaxY) const;

	void Swap( tImage& rhs )
	{
		tImage Tmp;
		Tmp.AttachFrom(*this);
		AttachFrom(rhs);
		rhs.AttachFrom(Tmp);
	}

	TData * Detach()
	{
		TData * pData = m_pRawData;
		m_pRawData = NULL;
		Destroy();
		return pData;
	};

	void Attach( unsigned nWidth, unsigned nHeight, unsigned nRowAlign, TData* pRawData )
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nRowAlign = nRowAlign;
		m_nRowLengthBytes = CalculateRowLengthBytes( m_nWidth , m_nChannels , sizeof(TData), m_nRowAlign );
		m_pRawData = pRawData;
	};

	void AttachFrom( tImage& rhs )
	{
		if( AreIdentical( rhs ) )
			return;

		Destroy();
		Attach( rhs.GetWidth(), rhs.GetHeight(),rhs.GetRowAlign(),rhs.GetRawData() );
		rhs.Detach();
	}

	inline void FlipVertical();
	inline void FlipHorizontal();
	
	bool CopyFrom( const tImage& rhs );
	bool CopyFrom(  const tImage& rhs, unsigned uFromX, unsigned uFromY, unsigned uToX, unsigned uToY, unsigned uWidth, unsigned uHeight );

	bool Create( unsigned nWidth, unsigned nHeight, unsigned nRowAlign = 4 );

	//destroy data
	void Destroy();
	
	//clear data to 0
	void Clear();

////////////////////////////////////////////////////////////////////
protected:

	//! width in pixels
	unsigned m_nWidth;
	//! height in pixels
	unsigned m_nHeight;
	//! row width in bytes
	unsigned m_nRowLengthBytes;
	//! row alignment
	unsigned m_nRowAlign;
	//! pointer to raw data
	TData *m_pRawData;

	//! allocator
	cImageMemAllocator*	m_allocator;

////////////////////////////////////////////////////////////////////
private:
	char*& m_pData;
};
////////////////////////////////////////////////////////////////////

//Inlines
////////////////////////////////////////////////////////////////////
inline unsigned CalculateRowLengthBytes( unsigned w, unsigned channels, unsigned elem_size, unsigned align )
{
	unsigned l = w * channels * elem_size;
	if( l==0 )
		return 0;
	unsigned mod = ( l % align );
	if( 0 == mod )
		return l;
	else
		return ( l + ( align - mod ) );
}
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
bool tImage<m_nChannels, TData>::CopyFrom( const tImage& rhs )
{
	if( AreIdentical( rhs ) )
		return true;

	if( !rhs.IsValid() )
	{
		//_ASSERT(false);
		Destroy();
		return false;
	}

	const unsigned uThisImageSize = GetImageSize();
	const unsigned uOtherImageSize = rhs.GetImageSize();
	
	if( uThisImageSize != uOtherImageSize )
	{
		//Destroy();
		if( !Create( rhs.GetWidth(),rhs.GetHeight(), rhs.GetRowAlign() ) )
		{
			_ASSERT(false);
			return false;
		}
	}
	else
	{
		this->m_nRowAlign = rhs.GetRowAlign();
		this->m_nRowLengthBytes = rhs.GetRowLengthBytes();
		this->m_nWidth = rhs.GetWidth();
		this->m_nHeight = rhs.GetHeight();
	}
	if( uOtherImageSize )
		memcpy( m_pRawData, rhs.GetRawData(), uOtherImageSize );
	
	return true;
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
bool tImage<m_nChannels, TData>::CopyFrom( const tImage& rhs, unsigned uFromX, unsigned uFromY, unsigned uToX, unsigned uToY, unsigned uWidth, unsigned uHeight )
{
	if( !rhs.IsValid() || uWidth + uFromX > rhs.GetWidth() || uHeight + uFromY > rhs.GetHeight())
		return false;

	unsigned uEndX = __min( m_nWidth,  uWidth + uToX  );
	unsigned uEndY = __min( m_nHeight,  uHeight + uToY  );

	for(unsigned i = uToX, x= uFromX; i<uEndX; ++i, x++ )
	for(unsigned j = uToY, y = uFromY; j<uEndY; ++j, y++ )
		memcpy( &GetAt( i,j), &rhs.GetAt( x,y), GetDataLengthBytes() );
	return true;
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
inline void tImage<m_nChannels, TData>::FlipVertical()
{
	char* pTmp = new char [m_nRowLengthBytes];
	const int iEnd = m_nHeight/2;
	for( int i=0; i<iEnd; ++i )
	{
		memcpy( pTmp, (*this)[i], m_nRowLengthBytes);
		memcpy( (*this)[i], (*this)[m_nHeight - 1- i], m_nRowLengthBytes);
		memcpy( (*this)[m_nHeight - 1- i], pTmp, m_nRowLengthBytes);
	}
	delete pTmp;
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
inline void tImage<m_nChannels, TData>::FlipHorizontal()
{
	const unsigned iEnd = m_nWidth/2;
	for( unsigned i=0; i<m_nHeight; ++i )
	for( unsigned j=0; j<iEnd; ++j )
	{
		stData D = ((stData*)(*this)[i])[j];
		((stData*)(*this)[i])[j] = ((stData*)(*this)[i])[m_nWidth-j-1];
		((stData*)(*this)[i])[m_nWidth-j-1] = D;
	}
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
bool tImage<m_nChannels, TData>::Create( unsigned nWidth, unsigned nHeight, unsigned nRowAlign )
{
	if( !m_nRowAlign )
		m_nRowAlign = 1;
		
	unsigned nImgSize = 0;
	unsigned nRowLengthBytes = CalculateRowLengthBytes( nWidth , m_nChannels, sizeof(TData), nRowAlign );
	nImgSize = nHeight * nRowLengthBytes;
	if( nImgSize != GetImageSize() )
	{
		Destroy();
		m_nHeight = nHeight;
		m_nWidth = nWidth;
		m_nRowAlign = nRowAlign;
		m_nRowLengthBytes = nRowLengthBytes;
		if( nImgSize )
		{
			if( !m_allocator )
				m_pRawData = (TData*)new unsigned char[ nImgSize ];
			else
				m_pRawData = (TData*)m_allocator->Alloc( nImgSize );
			memset( m_pRawData, 0, nImgSize );
		}
		else
		{
			m_pRawData = NULL;
		}
	}
	else
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nRowAlign = nRowAlign;
		m_nRowLengthBytes = nRowLengthBytes;
		memset( m_pRawData, 0, nImgSize );
	}
	return true;
};
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
void tImage<m_nChannels, TData>::Clear()
{
	if( m_pRawData )
		memset( m_pRawData, 0, GetImageSize() );
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
void tImage<m_nChannels, TData>::Destroy()
{
	if( m_pRawData )
	{
		if( !m_allocator )
			delete [] m_pRawData;
		else
			m_allocator->Free( m_pRawData );
		m_pRawData = NULL;
	}
	m_nWidth = m_nHeight = m_nRowLengthBytes = m_nRowAlign = 0;
};
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
inline TData tImage<m_nChannels, TData>::GetMax() const
{
	_ASSERT( IsValid() );
	
	TData Max = GetAt(0,0);
	for( unsigned x = 0; x<m_nWidth; x++ )
		for( unsigned y = 0; y<m_nHeight; y++ )
		{
			TData Tmp = *((TData*)(m_pData + y*m_nRowLengthBytes + (x*m_nChannels*sizeof(TData) ) ));
			if( Max < Tmp )
				Max = Tmp;
		}
	return Max;
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
template< int m_nChannels, typename TData >
inline TData tImage<m_nChannels, TData>::GetMax(unsigned& MaxX, unsigned& MaxY) const
{
	_ASSERT( IsValid() );
	
	MaxX = 0; MaxY = 0;
	TData Max = GetAt(0,0);
	for( unsigned x = 0; x<m_nWidth; x++ )
		for( unsigned y = 0; y<m_nHeight; y++ )
		{
			TData Tmp = *((TData*)(m_pData + y*m_nRowLengthBytes + (x*m_nChannels*sizeof(TData) ) ));
			if( Max < Tmp )
			{
				MaxX = x;
				MaxY = y;
				Max = Tmp;
			}
		}
	return Max;
}
////////////////////////////////////////////////////////////////////
