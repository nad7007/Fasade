#include "stdafx.h"
#include	"GDIImage\cImages.h"
#include "cRow.h"

cRow::cRow(int iRowIndex, double fStartPosX, double fStartPosY, double dHeight)
	: m_dStartX(fStartPosX)
	, m_dStartY(fStartPosY)
	, m_dHeight(dHeight)
	, m_dLength(0)
	, m_Type(NN)
	, m_iRowIndex(iRowIndex)
{}

void cRow::CreatePanels( double dRemaning, int iPanelWidth)
{
	switch (m_Type)
	{
		case NN:
		case InIn:
		case InN:
		case NIn:
			return CreateNonEdgePanels(m_iRowIndex, dRemaning);
		case NE:
		case EN:
		case InE:
		case EIn:
			return CreateOneEdgePanels(m_iRowIndex, m_Type == EN || m_Type == EIn,  dRemaning, iPanelWidth);
		case EE:
			return CreateTwoEdgePanels(m_iRowIndex, dRemaning, iPanelWidth);
		default:
			_ASSERT(false);
//todo
	}


} 

void cRow::CreateTwoEdgePanels(int iRowIndex, double dRemaning, int iPanelWidth)
{
	const  int dMaxFlatPanelLength = cPDFSize::GetMaxLength();
	const  int EvеnRow = iRowIndex % 2 == 0;
	const double dFirstEdgePanelLength = EvеnRow ? cLDSize::GetMaxLongLength(iPanelWidth) : cLDSize::GetMaxShortLength(iPanelWidth);
	const double dLastEdgePanelLength = EvеnRow ? cLDSize::GetMaxShortLength(iPanelWidth) : cLDSize::GetMaxLongLength(iPanelWidth);
	//double dMaxFirstEdgePanelLength = (EvеnRow ) ? cLDSize::GetMaxLongLength(iPanelWidth) : cLDSize::GetMaxShortLength(iPanelWidth);
	//double dMinFirstEdgePanelLength = (EvеnRow ) ? cLDSize::GetMinLongLength(iPanelWidth) : cLDSize::GetMinShortLength(iPanelWidth);
	//double dMaxLastEdgePanelLength = (EvеnRow ) ? cLDSize::GetMaxShortLength(iPanelWidth) : cLDSize::GetMaxLongLength(iPanelWidth);
	//double dMinLastEdgePanelLength = (EvеnRow )? cLDSize::GetMinShortLength(iPanelWidth) : cLDSize::GetMinLongLength(iPanelWidth);

	int iPanelsNum = (int)m_dLength / dMaxFlatPanelLength;
	double dRem = (int)m_dLength % dMaxFlatPanelLength;
	//double dFirstEdgePanelLength = dMaxFirstEdgePanelLength;
	//double dLastEdgePanelLength = dMaxLastEdgePanelLength;
	ResetPanels(iPanelsNum + 3);

	bool bTwoSmallPanels = false;
	dRem -= (dFirstEdgePanelLength + dLastEdgePanelLength);
	if (dRem < dRemaning && iPanelsNum > 0)
	{
		iPanelsNum--;
		dRem += dMaxFlatPanelLength;
	}
	if (dRem > dMaxFlatPanelLength)
	{
		dRem /= 2;
		bTwoSmallPanels = true;
	}
	
	double dStartX = m_dStartX;
	m_Panels.push_back(IfacePanel::Create(LD, dStartX, m_dStartY, m_dHeight, dFirstEdgePanelLength));
	dStartX += dFirstEdgePanelLength;
	if (dRem >= dMaxFlatPanelLength/2 || bTwoSmallPanels && dRem>0)
	{//first flat pannel
		m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dRem));

		dStartX += dRem;
	}
	for (int i = 0; i < iPanelsNum; ++i)
	{
		m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dMaxFlatPanelLength));
		dStartX += dMaxFlatPanelLength;
	}
	if (dRem > 0 && (dRem < dMaxFlatPanelLength / 2 || bTwoSmallPanels ) )
	{//last flat pannel
		m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dRem));
		dStartX += dRem;
	}
	m_Panels.push_back(IfacePanel::Create(LD, dStartX, m_dStartY, m_dHeight, dLastEdgePanelLength));
	
	RemovePanelsFromHoles(dRemaning);
}

void cRow::CreateOneEdgePanels(int iRowIndex, bool bEdgeAtStart, double dRemaning, int iPanelWidth)
{
	const  int dMaxFlatPanelLength = cPDFSize::GetMaxLength();
	const  int EvеnRow = iRowIndex % 2 == 0;
	const double dEdgePanelLength = bEdgeAtStart 
		? (EvеnRow ? cLDSize::GetMaxLongLength(iPanelWidth) : cLDSize::GetMaxShortLength(iPanelWidth))
		: (EvеnRow ? cLDSize::GetMaxShortLength(iPanelWidth) : cLDSize::GetMaxLongLength(iPanelWidth));
	//const double dMaxEdgePanelLength = bEdgeAtStart
	//	? (EvеnRow ? cLDSize::GetMaxLongLength(iPanelWidth) : cLDSize::GetMaxShortLength(iPanelWidth))
	//	: (EvеnRow ? cLDSize::GetMaxShortLength(iPanelWidth) : cLDSize::GetMaxLongLength(iPanelWidth));
	
	int iPanelsNum = (int)m_dLength / dMaxFlatPanelLength;
	double dRem = (int)m_dLength % dMaxFlatPanelLength;
	//double dEdgePanelLength = dMaxEdgePanelLength;
	double dSecRemFlatPanelLength = 0;

	ResetPanels(iPanelsNum + 2);
	dRem -= dEdgePanelLength;
	if (dRem < dRemaning && iPanelsNum > 0)
	{
		iPanelsNum--;
		dRem += dMaxFlatPanelLength;
	}
	if (dRem > dMaxFlatPanelLength)
	{
		dRem /= 2;
		dSecRemFlatPanelLength = dRem;
	}

	if (bEdgeAtStart)
	{
		double dStartX = m_dStartX;
		m_Panels.push_back(IfacePanel::Create(LD, dStartX, m_dStartY, m_dHeight, dEdgePanelLength));
		dStartX += dEdgePanelLength; 
		if (dSecRemFlatPanelLength != 0)
		{//first flat pannel
			m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dSecRemFlatPanelLength));
			dStartX += dSecRemFlatPanelLength;
		}
		for (int i = 0; i < iPanelsNum; ++i)
		{
			m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dMaxFlatPanelLength));
			dStartX += dMaxFlatPanelLength;
		}
		if ( dRem != 0)
		{//last flat pannel
			m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dRem));
			dStartX += dRem;
		}
	}
	else 
	{
		double dStartX = m_dStartX;
		if ( dRem != 0)
		{//first flat pannel
			m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dRem));
			dStartX += dRem;
		}
		for (int i = 0; i < iPanelsNum; ++i)
		{
			m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dMaxFlatPanelLength));
			dStartX += dMaxFlatPanelLength;
		}
		if (dSecRemFlatPanelLength != 0)
		{//first flat pannel
			m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dSecRemFlatPanelLength));
			dStartX += dSecRemFlatPanelLength;
		}
		m_Panels.push_back(IfacePanel::Create(LD, dStartX, m_dStartY, m_dHeight, dEdgePanelLength));
	}
	
	RemovePanelsFromHoles(dRemaning);
}

void cRow::CreateNonEdgePanels(int iRowIndex, double dRemaning)
{
	const  int MaxPanelLength = cPDFSize::GetMaxLength();
	int iPanelsNum = (int)m_dLength / MaxPanelLength;
	ResetPanels(iPanelsNum + 1);

	double dRem = (int)m_dLength % MaxPanelLength;
	if (iPanelsNum == 0 )
	{
		if (m_Holes.empty())
		{
			if (iRowIndex % 2)
			{
				m_Panels.push_back(IfacePanel::Create(PDF, m_dStartX, m_dStartY, m_dHeight, dRem));
			}
			else
			{
				dRem *= 0.5;
				m_Panels.push_back(IfacePanel::Create(PDF, m_dStartX, m_dStartY, m_dHeight, dRem));
				m_Panels.push_back(IfacePanel::Create(PDF, m_dStartX + dRem, m_dStartY, m_dHeight, dRem));
			}
		}
		return;
	}

	iPanelsNum--;
	double dFirstLength = 0;
	double dLastLength = 0;
	if(dRem < dRemaning || (MaxPanelLength - dRem) < dRemaning )
	{
		dRem = MaxPanelLength + dRem - dRemaning;
		dRem *= 0.5;
		dFirstLength = dRem;
		dLastLength = dRem + dRemaning;
	}
	else
	{
		dFirstLength = MaxPanelLength;
		dLastLength = dRem;
	}
		
	if (iRowIndex % 2)
		std::swap(dFirstLength, dLastLength);
	
	double dStartX = m_dStartX;
	//first panel
	m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dFirstLength));
	dStartX += dFirstLength;
	for (int i = 0; i < iPanelsNum; ++i)
	{
		m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, MaxPanelLength));
		dStartX += MaxPanelLength;
	}
	//last panel
	m_Panels.push_back(IfacePanel::Create(PDF, dStartX, m_dStartY, m_dHeight, dLastLength));
	
	RemovePanelsFromHoles(dRemaning);
}

void cRow::CorrectLengthByPanelWidth(int iPanelWidth, bool bPrev, bool bNext)
{
	const  int EvеnRow = m_iRowIndex % 2 == 0;
	if (EvеnRow && bPrev && (m_Type == InIn  || m_Type == InE || m_Type == InN))
	{
		m_dLength -= iPanelWidth;
		m_dStartX += iPanelWidth;
	}
	else if (!EvеnRow && bNext && (m_Type == InIn || m_Type == EIn || m_Type == NIn))
	{
		m_dLength -= iPanelWidth;
	}
}

bool cRow::Get3DFlatPanelPoints(unsigned iPanelIndex, int iPanelWidth, const cCartesian& _3DF, c3DPointI& TopLeft, c3DPointI& TopRight, c3DPointI& TopFront, c3DPointI& DownLeft)
{
	_ASSERT(iPanelIndex < m_Panels.size());
	const IfacePanel* pP = m_Panels[iPanelIndex];
	if (pP->GetType() != PDF)
	{
		_ASSERT(false);
		return false;
	}

	const double	dH = pP->GetHeight();
	const double	dL = pP->GetLength();
	const double	dW = iPanelWidth;
	double dX, dY;
	pP->GetStartPoint(dX, dY);
	c3DPointD TL(dX,		dY,		0);
	c3DPointD TR(dX+dL, dY,		0);
	c3DPointD TF(dX,		dY,	-dW);
	c3DPointD DL(dX,		dY+dH,0);
	_3DF.Apply(TL);
	_3DF.Apply(TR);
	_3DF.Apply(TF);
	_3DF.Apply(DL);

	TopLeft.Set(int(TL.x+0.5), int(TL.y + 0.5), int(TL.z + 0.5));
	TopRight.Set(int(TR.x + 0.5), int(TR.y + 0.5), int(TR.z + 0.5));
	TopFront.Set(int(TF.x + 0.5), int(TF.y + 0.5), int(TF.z + 0.5));
	DownLeft.Set(int(DL.x + 0.5), int(DL.y + 0.5), int(DL.z + 0.5));
	return true;
}

bool cRow::Get3DEdgePanelLength(double& dLength)const
{
	dLength = 0;
	_ASSERT(!m_Panels.empty());
	const IfacePanel* pP = m_Panels.back();
	if ( pP->GetType() != LD)
	{
		_ASSERT(pP->GetType() == LD);
		return false;
	}
	dLength = pP->GetLength();
	return true;
}

bool cRow::Get3DEdgePanelPoints(bool bFirstPanel, int iPanelWidth, const cCartesian& _3DF, double dEdgeLength, c3DPointI Corner[6])
{
	_ASSERT(!m_Panels.empty());
	const IfacePanel* pP = m_Panels[bFirstPanel ? 0: m_Panels.size() - 1];
	if (pP->GetType() != LD)
	{
		_ASSERT(false);
		return false;
	}

	const double	dH = pP->GetHeight();
	const double	dL = pP->GetLength();
	const double	dW = iPanelWidth;
	double dX, dY;
	pP->GetStartPoint(dX, dY);
	dY += dH/2;
	c3DPointD Edges[6];
	Edges[5].Set(dX, dY, 0);
	Edges[4].Set(dX + dL, dY, 0);
	Edges[3].Set(dX + dL, dY, -dW);
	Edges[2].Set(dX - dW, dY, -dW);
	Edges[1].Set(dX - dW, dY, dEdgeLength);
	Edges[0].Set(dX, dY, dEdgeLength);

	for (int i = 0; i < 6; ++i)
	{
		_3DF.Apply(Edges[i]);
		Corner[i].Set(int(Edges[i].x + 0.5), int(Edges[i].y + 0.5), int(Edges[i].z + 0.5));
	}
	return true;
}

void cRow::DetectHoles(const cRGBImage& Image, int iY, double dRasterToMM)
{
	ClearHoles();
	const int MinHoleDist = int(250 / dRasterToMM);
	const int iStartX = m_dStartX / dRasterToMM + 50;
	const int iEndX = (m_dStartX + m_dLength) / dRasterToMM - 50;

	const unsigned char red = HoleEdgeClr.red;
	const unsigned char green = HoleEdgeClr.green;
	const unsigned char blue = HoleEdgeClr.blue;
	for (int x = iStartX; x <= iEndX; ++x)
	{
		int iHoleLeft = 0, iHoleRight = 0;
		cRGBImage::stData& Color = Image.GetAt(x, iY);
		if (Color.Data[2] == red && Color.Data[1] == green && Color.Data[0] == blue)
		{
			iHoleLeft = x;
			for (int x1 = x + 10; x1 <= iEndX; ++x1)
			{
				Color = Image.GetAt(x1, iY);
				if (Color.Data[2] == red && Color.Data[1] == green && Color.Data[0] == blue)
				{
					iHoleRight = x1;
					x = x1 + MinHoleDist;
					break;
				}
			}

			if (iHoleLeft > 0 && iHoleRight > 0 && iHoleRight - iHoleLeft > MinHoleDist)
			{
				AddHole(iHoleLeft*dRasterToMM, iHoleRight*dRasterToMM);
			}
		}
	}
}

void cRow::RemovePanelsFromHoles(double dTolerance)
{
	if (m_Holes.empty())
		return;

	std::vector< IfacePanel* > TmpArr = std::move(m_Panels);
	for (auto i= TmpArr.begin(); i!= TmpArr.end(); ++i)
	{
		bool bToAdd = true;
		const double dStartX = (*i)->GetStartPointX() + dTolerance;
		const double dEndX = (*i)->GetEndPointX() - dTolerance;
		for (auto hole : m_Holes)
		{
			if (dStartX >= hole.first && dEndX <= hole.second)
			{
				bToAdd = false;
				delete(*i);
				(*i) = nullptr;
				break;
			}
		}
		if (bToAdd)
			m_Panels.push_back(std::move(*i));
	}
}

/*void cRow::DrawRow(cRGBImage& Image, float fSmToPixelX, float fSmToPixelY, float iStartY, float iStartX)const
{
	stColor color;
	cPanelMap& panelMap = GetPanelMap();
	const int iY1 = int(iStartY * fSmToPixelY + 0.5);
	const int iY2 = int((iStartY + m_fHeight ) * fSmToPixelY + 0.5);
	for (auto it = m_Panels.begin(); it != m_Panels.end(); ++it)
	{
		const stOptPanel& panel = (*it);
		const IfacePanel* pPanel = panelMap.Find(panel.m_Type, m_fHeight, panel.m_fLength);
		if (pPanel)
		{
			pPanel->GetColor(color.red, color.green, color.blue);
		}
		else
		{
			pPanel = panelMap.Add(panel.m_Type, m_fHeight, panel.m_fLength);
			ASSERT(pPanel);
			pPanel->GetColor(color.red, color.green, color.blue);
		}
		const int iX1 = int(iStartX* fSmToPixelX + 0.5);
		iStartX += panel.m_fLength;
		const int iX2 = int(iStartX* fSmToPixelX + 0.5);

		for( int row = iY1; row<= iY2; ++row)
			for (int col = iX1; col <= iX2; ++col)
			{
				cRGBImage::stData& data = Image.GetAt(col, row);
				data.Data[0] = color.red;
				data.Data[1] = color.green;
				data.Data[2] = color.blue;
			}
	}
}

void cStrip::Draw(cRGBImage& Image, float	 fSmToPixelX, float fSmToPixelY) const
{
	int irow = 0;
	for (auto it = m_Rows.begin(); it != m_Rows.end(); ++it, ++irow)
	{
		const cRow& row = (*it);
		row.DrawRow(Image, fSmToPixelX,  fSmToPixelY, m_fStartPosX, m_fStartPosY + irow * m_fHeight);
	}
}


*/


