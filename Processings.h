#pragma once
#include"map"
#include"Wall\Wall.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FACADE_PROC
{
	bool LoadWalls(std::vector<std::string>& IniFileNames, unsigned iNumFiles, std::vector< cWall*>& Walls);
	bool CreateRows(std::vector< cWall*>& Walls, int iPanelHeight, int iPanelWidth, double dTolRemaining);
	bool CalculatePanels(std::vector< cWall*>& Walls, int iPanelWidth, double dTolRemaining);
};