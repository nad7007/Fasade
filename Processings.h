#pragma once
#include"map"
#include"Wall\Wall.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FACADE_PROC
{
	void LoadWalls(CString* IniFileNames, unsigned iNumFiles, std::vector< cWall*>& Walls);
	void CreateRows(std::vector< cWall*>& Walls, int iPanelHeight, int iPanelWidth, double dTolRemaining);
	void CalculatePanels(std::vector< cWall*>& Walls, int iPanelWidth, double dTolRemaining);

	/*struct DivisionData
	{
		int m_iStripeInd;
		int m_iQuotient;
		int m_iReminder;
		bool operator<(const DivisionData& data)
		{
			return m_iReminder < data.m_iReminder;
		}
		bool operator>(const DivisionData& data)
		{
			return m_iReminder > data.m_iReminder;
		}
	};
	typedef std::map<int, std::vector<DivisionData> >DivisionColectorType;
	*/
	//void SortByHeight(std::vector<cStrip>& WallsToSort);
	//void SortByPosition(std::vector<cStrip>& WallsToSort);
	//void SortByResiduals(std::vector<FACADE_PROC::DivisionData>& WallsToSort);
	//void ExtractEdgedStripes(const std::vector<cStrip>& Walls, std::vector<cStrip>& _ExtractedWalls);

};