////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "cImages.h"
#include "cImageFile.h"

////////////////////////////////////////////////////////////////
bool cImageFile::Read( cRGBImage* pImage, const char* szFileName ) 
{
	if( !pImage )
		return false;

	pImage->Destroy();

	USES_CONVERSION;
	Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(CA2CT(szFileName));
	if( pBitmap == NULL )
		return false;

	Gdiplus::Graphics * pGraphics = new Gdiplus::Graphics( pBitmap );

	if( pGraphics == NULL )
	{
		delete pBitmap;
		return false;
	}

	if( !pImage->Create( pBitmap->GetWidth(), pBitmap->GetHeight(), 4 ) )
	{
		delete pGraphics;
		delete pBitmap;
		return false;
	}


	Gdiplus::BitmapData bmpData;

	Gdiplus::Rect rcDest;
	rcDest.X = 0;
	rcDest.Y = 0;
	rcDest.Width = pBitmap->GetWidth();
	rcDest.Height = pBitmap->GetHeight();
	
	if( Gdiplus::Ok != pBitmap->LockBits(	&rcDest, 
											Gdiplus::ImageLockModeRead,
											PixelFormat24bppRGB ,
											&bmpData ) )
	{
		pImage->Destroy();
		delete pGraphics;
		delete pBitmap;
		return false;
	}

	BYTE * pRawData = reinterpret_cast<BYTE*>(bmpData.Scan0);
	
	if( !pRawData )
	{
		pImage->Destroy();
		delete pGraphics;
		delete pBitmap;
		return false;
	}
	else
	{
		memcpy( pImage->GetRawData(), pRawData, pImage->GetImageSize() );
		pBitmap->UnlockBits(&bmpData);
	}

	delete pGraphics;
	delete pBitmap;

	return true;
}

////////////////////////////////////////////////////////////////
static bool GetCodecClsid(const WCHAR *format, CLSID &Clsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		 return false;  // Failure

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		  return false;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			Clsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return true;  // Success
		}    
	}
	free(pImageCodecInfo);
	return false;  // Failure	
}

////////////////////////////////////////////////////////////////
bool cImageFile::Write( const cRGBImage * pImage, const TCHAR* szFileName ) 
{
	if( !pImage->IsValid() )
		return false;

	unsigned uWidth = pImage->GetWidth();
	unsigned uHeight = pImage->GetHeight();

	Gdiplus::Bitmap * pBitmap = 
		new Gdiplus::Bitmap( uWidth, uHeight, 
								PixelFormat24bppRGB );
	if( !pBitmap )
		return false;

	Gdiplus::Graphics * pGraphics = new Gdiplus::Graphics( pBitmap );
	if( !pGraphics )
	{
		delete pBitmap;
		return false;
	}
	
	Gdiplus::BitmapData bmpData;

	Gdiplus::Rect rcDest;
	rcDest.X = 0;
	rcDest.Y = 0;
	rcDest.Width = uWidth;
	rcDest.Height = uHeight;
	
	if( Gdiplus::Ok != pBitmap->LockBits(	&rcDest, 
											Gdiplus::ImageLockModeWrite,
											PixelFormat24bppRGB ,
											&bmpData ) )
	{
		delete pGraphics;
		delete pBitmap;
		return false;
	}

	BYTE * pRawData = reinterpret_cast<BYTE*>(bmpData.Scan0);
	
	if( !pRawData )
	{
		delete pGraphics;
		delete pBitmap;
		return false;
	}
	else
	{
		memcpy( pRawData, pImage->GetRawData(), pImage->GetImageSize() );
		pBitmap->UnlockBits(&bmpData);
	}
	
	LPCTSTR pszExt = _tcsrchr( szFileName, '.' );
	if( pszExt == NULL )
	{
		delete pGraphics;
		delete pBitmap;
		return false; // unknown extension
	}

	pszExt++;

	CLSID  Clsid;
	bool bRes;
	if( !_tcsicmp( pszExt, _T("bmp") ) )
		bRes = GetCodecClsid( L"image/bmp", Clsid);  // Non-API helper function
	else if( !_tcsicmp( pszExt, _T("jpg")) || !_tcsicmp( pszExt, _T("jpeg" )))
		bRes = GetCodecClsid( L"image/jpeg", Clsid);
	else if( !_tcsicmp( pszExt, _T("gif") ) )
		bRes = GetCodecClsid( L"image/gif", Clsid);
	else if( !_tcsicmp( pszExt, _T("png") ) )
		bRes = GetCodecClsid( L"image/png", Clsid);
	else if( !_tcsicmp( pszExt, _T("tif") ) )
		bRes = GetCodecClsid( L"image/tiff", Clsid);
	else
		bRes = false; // unknown file type

	if( bRes )
	{
		USES_CONVERSION;
		Gdiplus::Status status = pBitmap->Save( szFileName, &Clsid, NULL);
		bRes = (status == Gdiplus::Ok);
	}

	delete pGraphics;
	delete pBitmap;

	return bRes;
}
////////////////////////////////////////////////////////////////