///////////////////////////////////////////////////////////////////////////
#pragma once
#include <gdipluspixelformats.h>
///////////////////////////////////////////////////////////////////////////
class cRGBImage;
///////////////////////////////////////////////////////////////////////////
class cImageFile
{
///////////////////////////////////////////////////////////////////////////
public:
	//! read rgb image from disk
	static bool Read(cRGBImage *rgbBoard, LPCTSTR szFileName) ;
	
	//! write rgb image to disk
	static bool Write(const cRGBImage *rgbBoard, const TCHAR* szFileName) ;

///////////////////////////////////////////////////////////////////////////
};

///////////////////////////////////////////////////////////////////////////
class cGDIStarter
{
	ULONG_PTR m_gdipToken;

public:
	cGDIStarter()
	{
		Gdiplus::GdiplusStartupInput startupInput;
		Gdiplus::GdiplusStartup(&m_gdipToken, &startupInput, NULL);
	}

	~cGDIStarter()
	{
		Gdiplus::GdiplusShutdown(m_gdipToken);
	}
};
///////////////////////////////////////////////////////////////////////////