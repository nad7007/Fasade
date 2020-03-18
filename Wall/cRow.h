#pragma once
#include<vector>
#include<iostream>
#include<string>
#include"panelTypeEnum.h"
#include"Wall\panel.h"
#include"Math\cCartesian.h"
#include"Wall\cHole.h"

class cRGBImage;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Pixdata
{
	int iX;
	stColor col;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum RowType
{
	NN = 0,//nothing-nothing
	NE,//nothing-edge 
	NIn,//int edge-nothing
	EN,//edge-nothing
	EE,//edge-edge
	EIn,
	InN,//int edge-nothing
	InIn,
	InE,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cRow
{
public:

	cRow(int iRowIndex, double fStartPosX, double fStartPosY, double dHeight);

	virtual ~cRow()
	{
		for (int i = 0; i < m_Panels.size(); ++i)
			delete m_Panels[i];
	}

	unsigned GetNumPanels() const{
		return m_Panels.size();
	}

	unsigned GetIndex() const{
		return m_iRowIndex;
	}

	bool Get3DEdgePanelLength(double& dLength)const;
	bool Get3DEdgePanelPoints(bool bFirstPanel, int iPanelWidth, const cCartesian& _3DF, double dEdgeLength, c3DPointI Corner[6]);
	bool Get3DFlatPanelPoints(unsigned iPanelIndex, int iPanelWidth, const cCartesian& _3DF, c3DPointI& TopLeft, c3DPointI& TopRight, c3DPointI& TopFront, c3DPointI& DownLeft);
	void CorrectLengthByPanelWidth(int iPanelWidth, bool bPrev, bool bNext);
	void DetectHoles(const cRGBImage& Image,int iY, double dRasterToMM);

	void ResetPanels(int iNewSize)
	{
		for (int i = 0; i < m_Panels.size(); ++i)
			delete m_Panels[i];
		m_Panels.clear();
		if (iNewSize)
			m_Panels.reserve(iNewSize);
	}

	void SetType(RowType type) {
		m_Type = type;
	}
	void SetLength(double dStartX, double dLength) {
		m_dStartX = dStartX;
		m_dLength = dLength;
	}
	double GetLength() const {
		return m_dLength;
	}
	double GetHeight() const	{
		return m_dHeight;
	}
	bool CheckPanelType(unsigned iPanelIndex, PanelType panelType) {
		_ASSERT(iPanelIndex < m_Panels.size());
		return m_Panels[iPanelIndex]->GetType() == panelType;
	}

	void CreatePanels(double dRemaning, int iPanelWidth );
	void ClearHoles()
	{
		m_Holes.clear();
	}

	void AddHole(double dLeft, double dRight)
	{
		bool bToAdd = true;
		for (int i = 0; i < m_Holes.size(); ++i)
		{
			if (fabs(m_Holes[i].first - dLeft) < 100 && fabs(m_Holes[i].second - dRight) < 100)
			{
				bToAdd = false;
				break;
			}
		}
		if(bToAdd)
			m_Holes.push_back(std::pair<double, double>(dLeft, dRight));
	}

protected:

	void CreateNonEdgePanels(int iRowIndex, double dRemaning);
	void CreateOneEdgePanels(int iRowIndex, bool bEdgeAtStart, double dRemaning, int iPanelWidth);
	void CreateTwoEdgePanels(int iRowIndex, double dRemaning, int iPanelWidth);
	void RemovePanelsFromHoles(double dTolerance);

private:

	RowType				m_Type;
	unsigned			m_iRowIndex;
	double				m_dHeight;
	double				m_dLength;
	double				m_dStartX;
	double				m_dStartY;

	std::vector<std::pair<double, double>>m_Holes;
	std::vector< IfacePanel*> m_Panels;
};
/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cStrip
{
	friend bool CompareByPosition(const cStrip& s1, const cStrip& s2);

	StripType		m_Type;
	int					m_iWallNumber;
	int					m_iStartingRow;

	float				m_fHeight;
	float				m_fLength;
	float				m_fStartPosX;
	float				m_fStartPosY;

	std::vector<cRow>	m_Rows;

public:
	cStrip()
	{}
	cStrip(StripType type, int iWallNumber, float fHeight, float fLength, float fStartPosX, float fStartPosY)
		:m_Type(type)
		, m_iWallNumber(iWallNumber)
		, m_iStartingRow(-1)
		, m_fHeight(fHeight)
		, m_fLength(fLength)
		, m_fStartPosX(fStartPosX)
		, m_fStartPosY(fStartPosY)
	{};

	int GetWallNumber() const {
		return m_iWallNumber;
	}
	float			GetInitLength() const {
		return m_fLength;
	}
	int			GetHeight() const {
		return static_cast<int>(m_fHeight);
	}
	StripType		GetType() const {
		return m_Type;
	}

	virtual float	GetActiveLength() const {
		return m_fLength;
	}

	virtual std::ostream& Serialize(std::ostream& outStr)const
	{
		unsigned iRows = m_Rows.size();
		outStr << m_iWallNumber << " " << (int)m_Type << " " << m_fStartPosY << " " << m_fStartPosX << " " << m_fHeight << " " << m_fLength << " " << iRows << "\n";
		for( auto i= m_Rows.begin(); i!= m_Rows.end(); ++i)
		{
			i->Serialize(outStr);
		}
		return outStr;
	}

	virtual std::istream& Deserialize(std::istream& inStr)
	{
		unsigned iRows = 0;
		int iType;
		inStr >> m_iWallNumber >> iType >> m_fStartPosY >> m_fStartPosX >> m_fHeight >> m_fLength >> iRows;
		m_Type = (StripType)iType;
		std::string s;
		for (unsigned i = 0; i < iRows; ++i)
		{
			//TODO: read rows if necessary
			std::getline(inStr, s);
		}
		return inStr;
	}

	void Draw(cRGBImage& Image, float	 fSmToPixelX, float fSmToPixelY) const;
	void CalculateRows(int iMaxPanelHeight);

};

*/