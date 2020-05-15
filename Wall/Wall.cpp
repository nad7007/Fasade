#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Errors.h"
#include "Wall.h"
#include "cInformationFile.h"
#include "panel.h"
#include "cRhinoScriptFile.h"

using namespace std;
using namespace std::filesystem;

bool cColorCodding::InitFromFile(const TCHAR* IniFile)
{
	cInformationFile File(IniFile);
	if (!File.IsReadyToRead())
	{
		return false;
	}

	int tmpArr[3];
	if (ReadArray(File, _T("COLOR_CODDING"), _T("BACKGROUND"), tmpArr, 3) != 3)
	{
		FACADE_ERRORS::SetErrorCode(103);
		return false;
	}
	for (int i = 0; i < 3; ++i)
		m_Background.m_arr[i] = char(tmpArr[i]);
	
	return true;
}

bool cColorCodding::SaveToIniFile(const TCHAR* FileName)
{
	cInformationFile IniFile(FileName);
	if (!IniFile.IsReadyToWrite())
		return false;

	int tmpArr[3];
	for (int i = 0; i < 3; ++i)
		tmpArr[i] = m_Background.m_arr[i];
	if (WriteArray(IniFile, _T("COLOR_CODDING"), _T("BACKGROUND"), tmpArr, 3) == false)
		return false;
	for (int i = 0; i < 3; ++i)
		tmpArr[i] = m_HoleEdge.m_arr[i];
	if (WriteArray(IniFile, _T("COLOR_CODDING"), _T("HOLE_EDGE"), tmpArr, 3) == false)
		return false;

	return true;
}

bool	cWall::ReadFullCoddedInfo( std::string& IniTxtFile)
{
	m_IniFileName.clear();
	stColor RasterColor;
	c3DPointI OrientV1, OrientV2;
	c3DPointD RasterPt1, RasterPt2;
	if (!ReadIniFile(IniTxtFile, OrientV1, OrientV2, RasterColor, RasterPt1, RasterPt2))
	{
		FACADE_ERRORS::SetErrorCode(103);
		return false;
	}

	m_IniFileName = IniTxtFile;

	Set3DCSRotation(OrientV1, OrientV2);

	cRGBImage		Image;
	if (!LoadFromCoddedBitmap(Image, RasterColor,	 RasterPt1,  RasterPt2))
		return false;

	m_RasterColor = RasterColor;
	m_RasterPt1 = RasterPt1;
	m_RasterPt2 = RasterPt2;

	m_pNext = m_pPrev = nullptr;
	return true;
}

bool	cWall::ReadPartialCoddedInfo(const std::string& IniTxtFile, const stColor& RasterColor, const c3DPointD& RasterPt1, const c3DPointD& RasterPt2, bool& bToFindNext)
{
	m_pPrev = m_pNext = nullptr;
	bToFindNext = false;
	m_IniFileName.clear();

	c3DPointI OrientV1, OrientV2;
	if (!ReadIniFile(IniTxtFile, OrientV1, OrientV2))
	{
		FACADE_ERRORS::SetErrorCode(103);
		return false;
	}

	Set3DCSRotation(OrientV1, OrientV2);

	m_IniFileName = IniTxtFile;
	cRGBImage		Image;
	c2DPointI RasterPixPt1, RasterPixPt2;
	if (!LoadFromCoddedBitmap(Image, RasterColor, RasterPt1, RasterPt2, &RasterPixPt1, &RasterPixPt2))
		return false;

	c2DPointI OwnPixPt1, OwnPixPt2;
	bToFindNext = FindOwnRasterLine(Image, RasterColor, RasterPixPt1, RasterPixPt2, OwnPixPt1, OwnPixPt2);
	return true;
}

void	cWall::FindNextWall( std::vector<std::string>& IniFileNames, int iNumFiles, std::vector< cWall*>& Walls)
{
	m_pNext = nullptr;
	for (int i = 0; i < iNumFiles; i++)
	{
		if (!IniFileNames[i].empty())
		{
			cWall* tmpWall = new cWall;
			bool bToFindNext = false;
			if (!tmpWall->ReadPartialCoddedInfo(IniFileNames[i], m_RasterColor, m_RasterPt1, m_RasterPt2, bToFindNext) )
			{
				delete tmpWall;
				continue;
			}

			IniFileNames[i].clear();
			tmpWall->SetPreviousWall(this);
			m_pNext = tmpWall;
			Walls.push_back(tmpWall);
			if (bToFindNext)
			{
				tmpWall->FindNextWall(IniFileNames, iNumFiles, Walls);
			}
			break;
		}
	}
}

void		cWall::FindNextWall(unsigned iCurrentWall, std::vector< cWall*>& Walls)
{
	_ASSERT(m_pNext == NULL);
	m_pNext = nullptr;
	if (m_RasterColor.blue == m_RasterColor.green && m_RasterColor.blue == m_RasterColor.red)
		return;// not connected to a wall
	for (unsigned i = 0; i < Walls.size(); i++)
	{
		if (i == iCurrentWall )
			continue;
		if (Walls[i]->FindEdge(m_RasterColor))
		{
			_ASSERT(!Walls[i]->m_pPrev);
			if (!Walls[i]->m_pPrev)
			{
				m_pNext = Walls[i];
				Walls[i]->SetPreviousWall(this);
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string	cWall::GetBitmapName()const
{
	path txtFile(m_IniFileName);
	//txtFile.replace_extension("dib");
	txtFile.replace_extension("png");
	return txtFile.string();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string	cWall::GetRinoScriptName(std::string& szOutputDir)const
{
	path txtFile(szOutputDir);
	path tmpFile(m_IniFileName);
	txtFile += tmpFile.filename();
	txtFile.replace_filename(txtFile.stem().string() + "_res.txt");
	return txtFile.string();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool cWall::LoadFromCoddedBitmap( cRGBImage& Image, const stColor& RasterColor
																	, const c3DPointD& RasterPt1, const c3DPointD& RasterPt2
																	, c2DPointI* pMinPixPt, c2DPointI* pMaxPixPt)
{
	if (m_IniFileName.empty())
	{
		ASSERT(false);
		return false;
	}

	//need StartGDIP();
	if(!cImageFile::Read(&Image, GetBitmapName().c_str() ) || Image.GetImageSize() <= 0)
	{
		_ASSERT(false);
		FACADE_ERRORS::SetErrorCode(104, GetBitmapName().c_str());
		return false;
	}
	
	c3DPointD TransformedRasterPt1 = m_CS/RasterPt1;
	c3DPointD TransformedRasterPt2 = m_CS/RasterPt2;

	c2DPointI RasterPixPt1, RasterPixPt2;
	if(!FindRasters(Image, RasterColor, TransformedRasterPt1, TransformedRasterPt2, RasterPixPt1, RasterPixPt2))
		return false;
	Set3DCSTranslationAndScale(RasterPt1, RasterPt2, RasterPixPt1, RasterPixPt2);
	
	if (pMinPixPt)
		*pMinPixPt = RasterPixPt1;
	if (pMinPixPt)
		*pMaxPixPt = RasterPixPt2;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cWall::Set3DCSRotation(const c3DPointI& OrientV1, const c3DPointI& OrientV2)
{
	m_CS.Identity();
	if (OrientV2.z == 1)
	{
		m_CS.Rotate90AroundX();
		if (OrientV1.x == -1)
		{
			m_CS.Invert();
			m_CS.RotateZAfter(PI);
			//m_CS.RotateZBefore(PI);
		}
		else if (OrientV1.y == 1)
		{
			m_CS.RotateYAfter(PI/2);
			m_CS.Invert();
		}
		else if (OrientV1.y == -1)
		{
			m_CS.RotateYAfter(-PI / 2);
			m_CS.Invert();
		}
		else
			m_CS.Invert();
		//for operator
	}
	else
	{
		m_CS.RotateAroundGlobalX(-PI / 2);
		if (OrientV1.x == -1)
		{
			m_CS.RotateXAfter(PI);
		}
		else if (OrientV1.y == 1)
		{
			m_CS.RotateYAfter(-PI / 2);
		}
		else if (OrientV1.y == -1)
		{
			m_CS.RotateYAfter(PI / 2);
		}
		m_CS.Invert();//for operator
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cWall::Set3DCSTranslationAndScale(const c3DPointD& RasterPt1, const c3DPointD& RasterPt2, const c2DPointI& _PixPoint1, const c2DPointI& _PixPoint2)
{
	c2DPointI PixPoint1(_PixPoint1);
	c2DPointI PixPoint2(_PixPoint2);

	{//check points correspondence
		c3DPointD Tmp1(PixPoint1.x, PixPoint1.y, 0);
		c3DPointD Tmp2(PixPoint2.x, PixPoint2.y, 0);
		m_CS.Apply(Tmp1);
		m_CS.Apply(Tmp2);
		c3DPointD TmpDist = Tmp1 - Tmp2;
		c3DPointD TmpRastDist = RasterPt1 - RasterPt2;
		int ind = TmpDist.AbsMaxIndex();
		if (TmpDist.arr[ind] * TmpRastDist.arr[ind] < 0)
		{
			PixPoint1 = _PixPoint2;
			PixPoint2 = _PixPoint1;
		}
	}

	const double dRx = RasterPt1.x - RasterPt2.x;
	const double dRy = RasterPt1.y - RasterPt2.y;
	const double dRz = RasterPt1.z - RasterPt2.z;
	const double dPx = PixPoint1.x - PixPoint2.x;
	const double dPy = PixPoint1.y - PixPoint2.y;

	if (fabs(m_CS.m[2][0] * dPx + m_CS.m[2][1] * dPy) > 1)
	{
		const double dDel = m_CS.m[2][0] * dPx + m_CS.m[2][1] * dPy;
		_ASSERT(fabs(dDel) > FLT_EPSILON);
		m_dPixelYToMetric = dRz / dDel;
	} 
	else if (fabs(m_CS.m[0][0] * dPx + m_CS.m[0][1] * dPy)>1)
	{
		const double dDel = m_CS.m[0][0] * dPx + m_CS.m[0][1] * dPy;
		_ASSERT(fabs(dDel) > FLT_EPSILON);
		m_dPixelYToMetric = dRx / dDel;
	}
	else
	{
		const double dDel = m_CS.m[1][0] * dPx + m_CS.m[1][1] * dPy;
		_ASSERT(fabs(dDel) > FLT_EPSILON);
		m_dPixelYToMetric = dRy / dDel;
	}

	c3DPointD Tmp1(PixPoint1.x*m_dPixelYToMetric, PixPoint1.y*m_dPixelYToMetric, 0);
	c3DPointD Tmp2(PixPoint2.x*m_dPixelYToMetric, PixPoint2.y*m_dPixelYToMetric, 0);

	m_CS.Apply(Tmp1);
	m_CS.Apply(Tmp2);

	c3DPointD Translation = RasterPt1 - Tmp1;
	c3DPointD Translation2 = RasterPt2 - Tmp2;
	m_CS.t0 = (Translation.x + Translation2.x)* 0.5;
	m_CS.t1 = (Translation.y + Translation2.y)* 0.5;
	m_CS.t2 = (Translation.z + Translation2.z)* 0.5;
}

bool	cWall::ReadIniFile(const std::string& TXTFile, c3DPointI& V1, c3DPointI& V2)//only vectors
{
	std::ifstream In;
	In.open(TXTFile);
	if (!In.is_open())
	{
		_ASSERT(false);
		return false;
	}
	char ch;
	In >> V1.x >> ch >> V1.y >> ch >> V1.z;
	_ASSERT(In.good());
	In >> V2.x >> ch >> V2.y >> ch >> V2.z;
	bool bOk = In.good();
	In.close();
	return bOk;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool cWall::ReadIniFile(const std::string& TXTFile, c3DPointI& V1, c3DPointI& V2, stColor& RasterColor, c3DPointD& RasterPt1, c3DPointD& RasterPt2)
{
	std::ifstream In;
	In.open(TXTFile);
	if (!In.is_open())
	{
		_ASSERT(false); 
		FACADE_ERRORS::SetErrorCode(102, TXTFile.c_str());
		return false;
	}
	char ch;
	In >> V1.x >> ch >> V1.y >> ch >> V1.z;
	_ASSERT(In.good());
	In >> V2.x >> ch >> V2.y >> ch >> V2.z;
	_ASSERT(In.good());
	int col;
	In >> col >> ch; 
	RasterColor.red = unsigned char(col);
	In >> col >> ch;
	RasterColor.green = unsigned char(col);
	In >> col;
	_ASSERT(In.good());
	RasterColor.blue = unsigned char(col);
	In >> RasterPt1.x >> ch >> RasterPt1.y >> ch >> RasterPt1.z;
	_ASSERT(In.good());
	In >> RasterPt2.x >> ch >> RasterPt2.y >> ch >> RasterPt2.z;
	bool bOk = (!In.fail());
	_ASSERT(bOk);
	In.close(); 
	return bOk;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool	cWall::FindRasters(const cRGBImage& Image, const stColor& RasterColor
												, const c3DPointD& MinRasterPt, const c3DPointD& MaxRasterPt
												, c2DPointI& MinPixPt, c2DPointI& MaxPixPt)
{
	if (Image.GetImageSize() < 1)
		return false;

	MinPixPt = { -1,-1 };
	MaxPixPt = { -1,-1 };

	const int iWidth = Image.GetWidth();
	const int iHeight = Image.GetHeight();

	const int iH = iHeight / 2;
	const int iW = iWidth / 2;

	{//Find top-left zero point
		const unsigned char red = BackgroundClr.red;
		const unsigned char green = BackgroundClr.green;
		const unsigned char blue = BackgroundClr.blue;

		m_iPixYEnd = m_iPixXEnd = m_iPixYStart = m_iPixXStart = -1;

		for (int y = 0; y < iHeight && m_iPixYStart < 0; ++y)
		for (int x = 0; x < iWidth; ++x)
		{
			const cRGBImage::stData& Color = Image.GetAt(x, y);
			if (Color.Data[2] != red || Color.Data[1] != green || Color.Data[0] != blue)
			{
				m_iPixYStart = y;
				break;
			}
		}

		for (int x = 0; x < iWidth && m_iPixXStart < 0; ++x)
		for (int y = 0; y < iHeight; ++y)
		{
			const cRGBImage::stData& Color = Image.GetAt(x, y);
			if (Color.Data[2] != red || Color.Data[1] != green || Color.Data[0] != blue)
			{
				m_iPixXStart = x;
				break;
			}
		}

		for (int y = iHeight-1; y >= 0 && m_iPixYEnd < 0; --y)
		for (int x = 0; x < iWidth; ++x)
		{
			const cRGBImage::stData& Color = Image.GetAt(x, y);
			if (Color.Data[2] != red || Color.Data[1] != green || Color.Data[0] != blue)
			{
				m_iPixYEnd = y;
				break;
			}
		}

		for (int x = iWidth-1; x >= 0 && m_iPixXEnd < 0; --x)
		for (int y = 0; y < iHeight; ++y)
		{
			const cRGBImage::stData& Color = Image.GetAt(x, y);
			if (Color.Data[2] != red || Color.Data[1] != green || Color.Data[0] != blue)
			{
				m_iPixXEnd = x;
				break;
			}
		}
	}

	{//calculate raster
		m_dPixelYToMetric = 0;

		const unsigned char red = RasterColor.red;
		const unsigned char green = RasterColor.green;
		const unsigned char blue = RasterColor.blue;

		const	bool bHorizontal = (fabs(MaxRasterPt.x - MinRasterPt.x) > fabs(MaxRasterPt.y - MinRasterPt.y));
		if (!bHorizontal)
		{
			const int XOffset = (m_iPixXEnd - m_iPixXStart) / 3;
			c2DPointI PixPtST, PixPtEND;
			PixPtST.y = m_iPixYEnd; PixPtEND.y = m_iPixYEnd;
			for (int y = m_iPixYStart; y <= m_iPixYEnd && PixPtST.y == m_iPixYEnd; ++y)
			for (int x = m_iPixXStart; x <= m_iPixXStart+ XOffset; ++x)
			{
				const cRGBImage::stData& Color = Image.GetAt(x, y);
				if (Color.Data[2] == red && Color.Data[1] == green && Color.Data[0] == blue)
				{
					const cRGBImage::stData& Color1 = Image.GetAt(x, y + 1);
					if (Color1.Data[2] == red && Color1.Data[1] == green && Color1.Data[0] == blue)
					{
						PixPtST.x = x;
						PixPtST.y = y;
						break;
					}
				}
			}
			for (int y = m_iPixYStart; y <= m_iPixYEnd && PixPtEND.y == m_iPixYEnd; ++y)
			for (int x = m_iPixXEnd; x >= m_iPixXEnd- XOffset; --x)
			{
				const cRGBImage::stData& Color = Image.GetAt(x, y);
				if (Color.Data[2] == red && Color.Data[1] == green && Color.Data[0] == blue)
				{
					const cRGBImage::stData& Color1 = Image.GetAt(x, y + 1);
					if (Color1.Data[2] == red && Color1.Data[1] == green && Color1.Data[0] == blue)
					{
						PixPtEND.x = x;
						PixPtEND.y = y;
						break;
					}
				}
			}
			MinPixPt = (PixPtST.y < PixPtEND.y) ? PixPtST : PixPtEND;

			PixPtEND.Clear(); PixPtST.Clear();
			for (int y = m_iPixYEnd; y >= m_iPixYStart && PixPtST.y ==0; --y)
			for (int x = m_iPixXStart; x <= m_iPixXStart+ XOffset; ++x)
			{
				const cRGBImage::stData& Color = Image.GetAt(x, y);
				if (Color.Data[2] == red && Color.Data[1] == green && Color.Data[0] == blue)
				{
					const cRGBImage::stData& Color1 = Image.GetAt(x, y + 1);
					if (Color1.Data[2] == red && Color1.Data[1] == green && Color1.Data[0] == blue)
					{
						PixPtST.x = x;
						PixPtST.y = y;
						break;
					}
				}
			}
			for (int y = m_iPixYEnd; y >= m_iPixYStart && PixPtEND.y ==0; --y)
			for (int x = m_iPixXEnd; x >= m_iPixXEnd- XOffset; --x)
			{
				const cRGBImage::stData& Color = Image.GetAt(x, y);
				if (Color.Data[2] == red && Color.Data[1] == green && Color.Data[0] == blue)
				{
					const cRGBImage::stData& Color1 = Image.GetAt(x, y + 1);
					if (Color1.Data[2] == red && Color1.Data[1] == green && Color1.Data[0] == blue)
					{
						PixPtEND.x = x;
						PixPtEND.y = y;
						break;
					}
				}
			}
			MaxPixPt = (PixPtST.y > PixPtEND.y) ? PixPtST : PixPtEND;
		}
		else
		{
			_ASSERT(false);//TODO 
		}

		_ASSERT(MaxPixPt.x >= 0 && MaxPixPt.y >= 0 && MinPixPt.x >= 0 && MinPixPt.y >= 0);
	}
	return true;
}

bool cWall::FindOwnRasterLine(const cRGBImage& Image, const stColor& PrevRasterColor
														, const c2DPointI& PrevMinPixPt, const c2DPointI& PrevMaxPixPt 
														,  c2DPointI& MinPixPt, c2DPointI& MaxPixPt)
{
	if (Image.GetImageSize() < 1)
		return false;

	MinPixPt = { -1,-1 };
	MaxPixPt = { -1,-1 };

	const int iWidth = Image.GetWidth();
	const int iHeight = Image.GetHeight();

	const int iH = iHeight / 2;
	const int iW = iWidth / 2;

	{//calculate raster
		const unsigned char pred = PrevRasterColor.red;
		const unsigned char pgreen = PrevRasterColor.green;
		const unsigned char pblue = PrevRasterColor.blue;

		const double dX = fabs(PrevMinPixPt.x - PrevMaxPixPt.x);
		const double dY = fabs(PrevMinPixPt.y - PrevMaxPixPt.y);

		stColor C1{ 0,0,0 };
		stColor C2{ 255,255,255 };
		const	bool bVertical = (dX < dY);
		_ASSERT(bVertical);
		if (bVertical)
		{
			bool bFromStart = (PrevMinPixPt.x - m_iPixXStart >= m_iPixXEnd - PrevMinPixPt.x);

			for (int y = m_iPixYStart; y <= m_iPixYEnd && MinPixPt.y < 0; ++y)
			{
				if (bFromStart)
				{
					for (int x = m_iPixXStart; x <= m_iPixXEnd; ++x)
					{
						const cRGBImage::stData& Color = Image.GetAt(x, y);
						if (Color.Data[2] != BackgroundClr.red && Color.Data[1] != BackgroundClr.green && Color.Data[0] != BackgroundClr.blue)
						{
							if ((Color.Data[2] != pred && HoleEdgeClr.red) || (Color.Data[1] != pgreen && Color.Data[1] != HoleEdgeClr.green) || (Color.Data[0] != pblue && Color.Data[0] != HoleEdgeClr.blue))
							{
								MinPixPt.x = x;
								MinPixPt.y = y;
								C1.red = Color.Data[2];
								C1.green = Color.Data[1];
								C1.blue = Color.Data[0];
							}
							break;
						}
					}
				}
				else
				{
					for (int x = m_iPixXEnd; x >= m_iPixXStart; --x)
					{
						const cRGBImage::stData& Color = Image.GetAt(x, y);
						if (Color.Data[2] != BackgroundClr.red && Color.Data[1] != BackgroundClr.green && Color.Data[0] != BackgroundClr.blue)
						{
							if ((Color.Data[2] != pred && HoleEdgeClr.red) || (Color.Data[1] != pgreen && Color.Data[1] != HoleEdgeClr.green) || (Color.Data[0] != pblue && Color.Data[0] != HoleEdgeClr.blue))
							{
								MinPixPt.x = x;
								MinPixPt.y = y;
								C1.red = Color.Data[2];
								C1.green = Color.Data[1];
								C1.blue = Color.Data[0];
							}
							break;
						}
					}
				}
			}

			for (int y = m_iPixYEnd; y >= m_iPixYStart && MaxPixPt.y < 0; --y)
			{
				if (bFromStart)
				{
					for (int x = m_iPixXStart; x <= m_iPixXEnd; ++x)
					{
						const cRGBImage::stData& Color = Image.GetAt(x, y);
						if (Color.Data[2] != BackgroundClr.red && Color.Data[1] != BackgroundClr.green && Color.Data[0] != BackgroundClr.blue)
						{
							if ((Color.Data[2] != pred && HoleEdgeClr.red) || (Color.Data[1] != pgreen && Color.Data[1] != HoleEdgeClr.green) || (Color.Data[0] != pblue && Color.Data[0] != HoleEdgeClr.blue))
							{
								MaxPixPt.x = x;
								MaxPixPt.y = y;
								C2.red = Color.Data[2];
								C2.green = Color.Data[1];
								C2.blue = Color.Data[0];
							}
							break;
						}
					}
				}
				else
				{
					for (int x = m_iPixXEnd; x >= m_iPixXStart; --x)
					{
						const cRGBImage::stData& Color = Image.GetAt(x, y);
						if (Color.Data[2] != BackgroundClr.red && Color.Data[1] != BackgroundClr.green && Color.Data[0] != BackgroundClr.blue)
						{
							if ((Color.Data[2] != pred && HoleEdgeClr.red) || (Color.Data[1] != pgreen && Color.Data[1] != HoleEdgeClr.green) || (Color.Data[0] != pblue && Color.Data[0] != HoleEdgeClr.blue))
							{
								MaxPixPt.x = x;
								MaxPixPt.y = y;
								C2.red = Color.Data[2];
								C2.green = Color.Data[1];
								C2.blue = Color.Data[0];
							}
							break;
						}
					}
				}
			}
		}

		m_RasterColor = { 0,0,0 };
		_ASSERT(C2.red == C1.red &&C2.green == C1.green && C2.blue == C1.blue);
		if (MaxPixPt.x >= 0 && MaxPixPt.y >= 0 && MinPixPt.x >= 0 && MinPixPt.y >= 0)
		{
			m_RasterColor = C1;

			c3DPointD Tmp1(MinPixPt.x*m_dPixelYToMetric, MinPixPt.y*m_dPixelYToMetric, 0);
			c3DPointD Tmp2(MaxPixPt.x*m_dPixelYToMetric, MaxPixPt.y*m_dPixelYToMetric, 0);

			m_RasterPt1 = m_CS * Tmp1;
			m_RasterPt2 = m_CS * Tmp2;
			return true;
		}
	}
	return false;
}

bool cWall::FindEdge(const stColor& EdgeColor)
{
	cRGBImage		Image;
	if (!cImageFile::Read(&Image, GetBitmapName().c_str()) || Image.GetImageSize() <= 0)
	{
		_ASSERT(false);
		return false;
	}

	const int iWidth = Image.GetWidth();
	const int iHeight = Image.GetHeight();
	const int iOffset = 200;
	_ASSERT(m_iPixYEnd < iHeight && m_iPixXEnd < iWidth);

	const unsigned char pred = EdgeColor.red;
	const unsigned char pgreen = EdgeColor.green;
	const unsigned char pblue = EdgeColor.blue;

	unsigned dY = (m_iPixYStart + m_iPixYEnd)/2;
	for (int x = m_iPixXStart; x <= m_iPixXEnd; ++x)
	{
		const cRGBImage::stData& Color = Image.GetAt(x, dY);
		if (Color.Data[2] == pred && Color.Data[1] == pgreen && Color.Data[0] == pblue)
		{
			return true;
		}
	}

	dY = (m_iPixYStart + iOffset);
	_ASSERT(dY < m_iPixYEnd);
	for (int x = m_iPixXStart; x <= m_iPixXEnd; ++x)
	{
		const cRGBImage::stData& Color = Image.GetAt(x, dY);
		if (Color.Data[2] == pred && Color.Data[1] == pgreen && Color.Data[0] == pblue)
		{
			return true;
		}
	}

	dY = (m_iPixYEnd - iOffset);
	_ASSERT(dY > m_iPixYStart);
	for (int x = m_iPixXStart; x <= m_iPixXEnd; ++x)
	{
		const cRGBImage::stData& Color = Image.GetAt(x, dY);
		if (Color.Data[2] == pred && Color.Data[1] == pgreen && Color.Data[0] == pblue)
		{
			return true;
		}
	}
	return false;
}

double cWall::DetectTopZ()
{
	c3DPointD TransformedRasterPt = m_iPixYStart;
	TransformedRasterPt *= m_dPixelYToMetric;
	m_CS.Apply(TransformedRasterPt);
	return TransformedRasterPt.z;
}

void cWall::SetTopRowZero(double dTopZero, double dRemaning, int iMaxPanelHeight)
{
	c3DPointD TransformedRasterPt = m_iPixYStart;
	TransformedRasterPt *= m_dPixelYToMetric;
	m_CS.Apply(TransformedRasterPt);

	int m_iStartRow = (int((TransformedRasterPt.z - dTopZero + dRemaning) / iMaxPanelHeight));
	TransformedRasterPt.z = m_iStartRow * iMaxPanelHeight + dTopZero;
	m_CS.ApplyInverse(TransformedRasterPt);

	m_iStartRowIndex = abs(m_iStartRow);
	m_dStartRowZ = TransformedRasterPt.y;

}
void cWall::CreateRows(int iMaxPanelHeight )
{
	_ASSERT(m_dStartRowZ > -FLT_MAX);

	m_Rows.clear();
	double dHeight = (m_iPixYEnd - m_iPixYStart) * m_dPixelYToMetric;
	int iNumRows = int(dHeight / iMaxPanelHeight +0.5);
	m_Rows.reserve(iNumRows);

	const double dStartPosX = m_iPixXStart * m_dPixelYToMetric;
	double dStartY = m_dStartRowZ;
	int iNRow = m_iStartRowIndex;
	for (int i = 0; i < iNumRows; ++i, iNRow++)
	{
		m_Rows.push_back(cRow(iNRow, dStartPosX, dStartY, iMaxPanelHeight));
		dStartY += iMaxPanelHeight;
	}
}

void cWall::DetectRowEdges(const cRGBImage& Image, unsigned iY, std::vector< Pixdata >& startRow)
{
	startRow.clear();
	if (iY >= Image.GetHeight() || iY < 0)
		return;

	const unsigned char red = BackgroundClr.red;
	const unsigned char green = BackgroundClr.green;
	const unsigned char blue = BackgroundClr.blue;
	
	for (int x = m_iPixXStart; x <= m_iPixXEnd; ++x)
	{
		const cRGBImage::stData& Color = Image.GetAt(x, iY);
		if (Color.Data[2] != red || Color.Data[1] != green || Color.Data[0] != blue)
		{
			startRow.push_back(Pixdata{ x, stColor{Color.Data[2],Color.Data[1],Color.Data[0]} });
			break;
		}
	}

	const int y = __min(iY + 10, m_iPixYEnd);
	for (int x = m_iPixXEnd;  x >= m_iPixXStart; --x)
	{
		const cRGBImage::stData& Color = Image.GetAt(x, y);
		if (Color.Data[2] != red || Color.Data[1] != green || Color.Data[0] != blue)
		{
			startRow.push_back(Pixdata{ x, stColor{Color.Data[2],Color.Data[1],Color.Data[0]} });
			break;
		}
	}
}

RowType DetectRowType(stColor& begCol, stColor& endCol)
{
	if (begCol.red == begCol.green && begCol.green == begCol.blue)
	{
		if (endCol.red == endCol.green && endCol.green == endCol.blue)
			return NN;
		return (endCol.red == endCol.blue) ? NE : NIn;
	}
	
	if (begCol.red == begCol.green)
	{
		if (endCol.red == endCol.green && endCol.green == endCol.blue)
			return InN;
		return (endCol.red == endCol.blue) ? InE : InIn;
	}

	if (endCol.red == endCol.green && endCol.green == endCol.blue)
		return EN;
	return (endCol.red == endCol.blue) ? EE : EIn;

}

void	cWall::DetectRows( double dTolerance)
{
	_ASSERT(m_dStartRowZ > -FLT_MAX);

	//need StartGDIP();
	cRGBImage		Image;
	if (!cImageFile::Read(&Image, GetBitmapName().c_str()) || Image.GetImageSize() <= 0)
	{
		_ASSERT(false);
		return;
	}

	std::vector< Pixdata > startRow, endRow;
	int  iStartY = int(m_dStartRowZ / m_dPixelYToMetric + 0.5);
	for (auto i = m_Rows.begin(); i != m_Rows.end(); ++i)
	{
		cRow& row = (*i);
		DetectRowEdges(Image, iStartY+2, startRow);
		
		int  iEndY = iStartY + int(row.GetHeight() / m_dPixelYToMetric + 0.5);
		DetectRowEdges(Image, iEndY-2, endRow);

		Pixdata data1, data2;
		RowType startType, endType;
		int iL1 = 0, iL2 = 0;
		int iSX1 = INT_MAX, iSX2 = INT_MAX;

		if (!startRow.empty())
		{
			data1 = startRow.front();
			data2 = startRow.back();
			startType = DetectRowType(data1.col, data2.col);
			iL1 = data2.iX - data1.iX;
			iSX1 = data1.iX;
		}
		if (!endRow.empty())
		{
			data1 = endRow.front();
			data2 = endRow.back();
			endType = DetectRowType(data1.col, data2.col);
			iL2 = data2.iX - data1.iX;
			iSX2 = data1.iX;
		}

		_ASSERT(iL1 > 0 || iL2 > 0);
		row.SetLength(__min(iSX1, iSX2)*m_dPixelYToMetric, __max(iL2, iL1)*m_dPixelYToMetric);
		if (iL1 > 0 || iL2 > 0)
		{
			if (iL2 > iL1 + 10)
				startType = endType;
			if (iL1 > iL2 + 10)
				endType = startType;
			if (startType != endType && endType != NN && endType != NE && endType != NIn && endType != InN && endType != EN)
				startType = endType;

			row.SetType(startType);
			int iTolerancePix = int(dTolerance / m_dPixelYToMetric + 0.5);
			if ((iStartY + iTolerancePix) > 0 && (iStartY + iTolerancePix) < m_iPixYEnd)
				row.DetectHoles(Image, (iStartY + iTolerancePix), m_dPixelYToMetric);
			if ((iEndY - iTolerancePix) > 0 && (iEndY - iTolerancePix) < m_iPixYEnd)
				row.DetectHoles(Image, (iEndY - iTolerancePix), m_dPixelYToMetric);
		}

		iStartY = iEndY;
		startRow = endRow;
	}
}

void	cWall::CorrectRowsLength(int iPanelsWidth)
{
	if (!m_pPrev && !m_pNext)
		return;

	for (auto i = m_Rows.begin(); i != m_Rows.end(); ++i)
	{
		cRow& row = (*i);
		row.CorrectLengthByPanelWidth(iPanelsWidth, m_pPrev && m_pPrev->HasRow(row.GetIndex()), m_pNext && m_pNext->HasRow(row.GetIndex()));
	}
}

void	cWall::CalculatePanels(int iPanelsWidth, double dTolRemaining)
{
	for (int i = 0; i < m_Rows.size(); ++i)
	{
		m_Rows[i].CreatePanels( dTolRemaining, iPanelsWidth);
	}
}

bool	cWall::ExportToRhinoScript(std::string& szOutputDir, int iPanelsWidth)
{
	std::string	Fn = GetRinoScriptName(szOutputDir);
	cRhinoScriptFile Rfile(Fn);
	if (!Rfile.IsReadyToWrite())
	{
		FACADE_ERRORS::SetErrorCode(105, Fn.c_str());
		return false;
	}

	double dEdgeLength;
	c3DPointI Edges[6];
	for (int i = 0; i < m_Rows.size(); ++i)
	{
		if (m_Rows[i].CheckPanelType(0, LD) && m_pPrev)
		{
			if (m_pPrev->GetRow(m_Rows[i].GetIndex()) && m_pPrev->GetRow(m_Rows[i].GetIndex())->Get3DEdgePanelLength(dEdgeLength) )
				if (m_Rows[i].Get3DEdgePanelPoints(true, iPanelsWidth, m_CS, dEdgeLength, Edges))
				{
					Rfile.WriteEdgePanel(Edges, m_Rows[i].GetHeight());
				}
		}
		else if (m_Rows[i].CheckPanelType(0, PDF))
		{
			if (m_Rows[i].Get3DFlatPanelPoints(0, iPanelsWidth, m_CS, Edges[0], Edges[1], Edges[3], Edges[2]))
			{
				Rfile.WriteFlatPanel(Edges);
			}
		}

		for(unsigned j=1; j< m_Rows[i].GetNumPanels(); ++j)
			if (m_Rows[i].CheckPanelType(j, PDF))
			{
				if (m_Rows[i].Get3DFlatPanelPoints(j, iPanelsWidth, m_CS, Edges[0], Edges[1], Edges[3], Edges[2]))
				{
					Rfile.WriteFlatPanel(Edges);
				}
			}
			
	}
	return true;
}

const cRow*	cWall::GetRow(unsigned ind)const
{
	if (ind < m_Rows.size() && m_Rows[ind].GetIndex() == ind)
		return &m_Rows[ind];
	for (auto& r : m_Rows)
	{
		if (r.GetIndex() == ind)
			return &r;
	}
	return nullptr;
}



