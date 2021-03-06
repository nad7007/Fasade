// Facade.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vector"

#include "processings.h"
#include <algorithm>

bool FACADE_PROC::LoadWalls(std::vector<std::string>& IniFileNames, unsigned iNumFiles, std::vector< cWall*>& Walls)
{
	for (unsigned i = 0; i < iNumFiles; i++)
	{
		if (!IniFileNames[i].empty())
		{
			cWall* tmpWall = new cWall;
			if (!tmpWall->ReadFullCoddedInfo(IniFileNames[i]))
			{
				delete tmpWall;
				return false;
				//continue;
			}

			IniFileNames[i].clear();
			Walls.push_back(tmpWall);
		}
	}

	_ASSERT(Walls.size());//should not be empty, at least one full codded wall
	const unsigned iSize = Walls.size();
	if (iSize == iNumFiles)
	{
		for (unsigned i = 0; i < iSize; ++i)
		{
			Walls[i]->FindNextWall(i, Walls);
		}
	}
	else
	{
		for (unsigned i = 0; i < iSize; ++i)
		{
			Walls[i]->FindNextWall(IniFileNames, iNumFiles, Walls);
		}
	}
	return true;
}

bool FACADE_PROC::CreateRows(std::vector< cWall*>& Walls, int iPanelHeight, int iPanelWidth, double dTolRemaining)
{
	{//unify start row
		std::vector<cWall*> Passed;
		while (Passed.size() < Walls.size())
		{
			cWall *pStart = nullptr;
			for (auto i = Walls.begin(); i != Walls.end() && pStart == nullptr; ++i)
			{
				pStart = *i;
				for (auto j = Passed.begin(); j != Passed.end(); ++j)
					if (*j == pStart)
					{
						pStart = nullptr;
						break;
					}
			}
			if (!pStart)
				break;
			unsigned iCurInd = Passed.size();
			Passed.push_back(pStart);
			double dTopZ = pStart->DetectTopZ();
			cWall *pCurrent = pStart->Next();
			while (pCurrent && pCurrent != pStart)
			{
				Passed.push_back(pCurrent);
				double dZ = pCurrent->DetectTopZ();
				if (dZ > dTopZ)
					dTopZ = dZ;
				pCurrent = pCurrent->Next();
			}
			for (iCurInd; iCurInd < Passed.size(); ++iCurInd)
				Passed[iCurInd]->SetTopRowZero(dTopZ, dTolRemaining, iPanelHeight);
		}
	}

	for (auto i : Walls)
	{
		i->CreateRows(iPanelHeight);
		i->DetectRows(dTolRemaining);
	}
	for (auto i : Walls)//all rows must be created before that
	{
		i->CorrectRowsLength(iPanelWidth);
	}
	return true;
}

bool FACADE_PROC::CalculatePanels(std::vector< cWall*>& Walls, int iPanelWidth, double dTolRemaining)
{
	for (auto i : Walls)
		i->CalculatePanels(iPanelWidth, dTolRemaining);
	return true;
}











/*bool CompareByPosition(const cStrip& s1, const cStrip& s2)
{	
	if (s1.m_fStartPosY < s2.m_fStartPosY)
		return true;
	if (s1.m_fStartPosY == s2.m_fStartPosY)
	{
		if (s1.m_fStartPosX < s2.m_fStartPosX)
			return true;
		if (s1.m_fStartPosX == s2.m_fStartPosX && s1.m_iWallNumber < s2.m_iWallNumber)
			return true;
	}
	return false;
}

void FACADE_PROC::SortByPosition( std::vector<cStrip>& WallsToSort )
{
	std::sort(WallsToSort.begin(), WallsToSort.end(), CompareByPosition);
}

void FACADE_PROC::SortByResiduals( std::vector<FACADE_PROC::DivisionData>& WallsToSort )
{
	std::sort(WallsToSort.begin(), WallsToSort.end());
}

void FACADE_PROC::ExtractEdgedStripes(const std::vector<cStrip>& Walls, std::vector<cStrip>& _ExtractedWalls)
{
	std::vector<cStrip> tmp;
	for (auto itr = Walls.begin(); itr != Walls.end(); ++itr)
	{
		StripType type = itr->GetType();
		if (type == 1 || type == 2 || type == 3)
			tmp.push_back(*itr);
	}
	_ExtractedWalls = std::move(tmp);
}
*/