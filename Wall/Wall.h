#pragma once
#include	<windef.h>
#include	"GDIImage\cImages.h"
#include	"GDIImage\cImageFile.h"
#include	"Math\cCartesian.h"
#include	"cRow.h"
#include	"cHole.h"

class cColorCodding
{
public:
	stColor m_Background;
	stColor m_BoundaryEdge;
	stColor m_InternalEdge;
	stColor m_HoleEdge;

	cColorCodding()
		:m_Background(BackgroundClr)
			, m_BoundaryEdge({ 0, 255, 0 })
		, m_InternalEdge({ 0, 0, 255 })
		, m_HoleEdge({ 0, 0, 0 })
	{}

	bool InitFromFile(const TCHAR* IniFile);
	bool SaveToIniFile(const TCHAR* IniFile);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cWall
{
public:
	cWall()
		: m_dPixelYToMetric(0)
		, m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_iPixXStart(-1)
		, m_iPixXEnd(-1)
		, m_iPixYStart(-1)
		, m_iPixYEnd(-1)
		, m_iStartRowIndex(0) 
		, m_dStartRowZ(-FLT_MAX)
	{}

public:

	bool		ReadFullCoddedInfo( std::string& IniTxtFile);
	void		FindNextWall(std::vector<std::string>& IniFileNames, int iNumFiles, std::vector< cWall*>& Walls);//recursion
	void		FindNextWall(unsigned iCurrentWall, std::vector< cWall*>& Walls);
	void		CreateRows( int iMaxPanelHeight);//calculate number of rows and create them
	void		DetectRows(double dTolRemaining);//detect length of each row
	void		CorrectRowsLength(int iPanelsWidth);//correct row length by panel width if necessary;
	void		CalculatePanels(int iPanelWidthd, double dTolRemaining);
	bool		ExportToRhinoScript( std::string& szOutputDir, float fPanelThickness );
	void		SetTopRowZero(double dTopZero, double dRemaning, int iMaxPanelHeight);
	const cRow*		GetRow(unsigned ind) const;
	bool		HasRow(unsigned ind) const {
		return (nullptr != GetRow(ind ));
	}
	double	DetectTopZ();
	cWall* Next() { return m_pNext; }

protected:

	void		DetectRowEdges(const cRGBImage& Image, unsigned iY, std::vector< Pixdata >& startRow);
	bool		ReadPartialCoddedInfo(const std::string& IniTxtFile, const stColor& RasterColor, const c3DPointD& RasterPt1, const c3DPointD& RasterPt2, bool& bToFindNext);
	void		SetPreviousWall(cWall* pWall)
	{
		ASSERT(m_pPrev == nullptr);
		m_pPrev = pWall;
	}

	std::string	GetBitmapName() const;
	std::string	GetRinoScriptName(std::string& szOutputDir)const;

	bool		LoadFromCoddedBitmap(cRGBImage& Image, const stColor& RasterColor
																, const c3DPointD& RasterPt1, const c3DPointD& RasterPt2
																, c2DPointI* pMinPixPt = nullptr, c2DPointI* pMaxPixPt = nullptr );
	bool		ReadIniFile(const std::string& TXTFile, c3DPointI& V1, c3DPointI& V2);//only vectors
	bool		ReadIniFile( const std::string& TXTFile, c3DPointI& V1, c3DPointI& V2, stColor& RasterColor, c3DPointD& RasterPt1, c3DPointD& RasterPt2);//full description

	void		Set3DCSRotation(const c3DPointI& OrientV1, const c3DPointI& OrientV2);
	void		Set3DCSTranslationAndScale(const c3DPointD& RasterPt1, const c3DPointD& RasterPt2, const c2DPointI& PixPoint1, const c2DPointI& PixPoint2);

	bool		FindEdge(const stColor& EdgeColor);

	bool		FindRasters(const cRGBImage& Image, const stColor& RasterColor
										, const c3DPointD& MinRastePt, const c3DPointD& MaxRastePt 
										, c2DPointI& MinPixPt, c2DPointI& MaxPixPt);
	bool		FindOwnRasterLine(const cRGBImage& Image, const stColor& PrevRasterColor
														, const c2DPointI& PrevMinPixPt, const c2DPointI& PrevMaxPixPt
														, c2DPointI& MinPixPt, c2DPointI& MaxPixPt);
	

private:
	
	std::string	m_IniFileName;
	cCartesian	m_CS;//From Image 2D space to input 3D space
	cWall*			m_pNext;
	cWall*			m_pPrev;
	stColor			m_RasterColor;
	c3DPointD		m_RasterPt1;
	c3DPointD		m_RasterPt2;

	//double			m_dPixelXToMetric;//from image pixelX to metric
	double			m_dPixelYToMetric;//from image pixelY to metric
	
	int					m_iPixXStart;
	int					m_iPixXEnd;
	int					m_iPixYStart;
	int					m_iPixYEnd;
	int					m_iStartRowIndex;
	double			m_dStartRowZ;

	std::vector<cRow>		m_Rows;
};

