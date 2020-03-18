// Facade.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <TCHAR.h>
#include <iostream>
#include <fstream>

#include "Processings.h"
#include "Wall\Wall.h"

using namespace std;
int main()
{
	cGDIStarter StartGDI;

	/*CString wallNames[] = { _T("C:\\Source\\Facade\\Data\\Olives\\Fasada1.dop"),
												_T("C:\\Source\\Facade\\Data\\Olives\\Fasada2.dop"),
												_T("C:\\Source\\Facade\\Data\\Olives\\Fasada3.dop"),
												_T("C:\\Source\\Facade\\Data\\Olives\\Fasada4.dop"),
												_T("C:\\Source\\Facade\\Data\\Olives\\Fasada5.dop"),
												_T("C:\\Source\\Facade\\Data\\Olives\\Fasada6.dop"),
												_T("C:\\Source\\Facade\\Data\\Olives\\Fasada7.dop"),
	};
	CString wallNames[] = { _T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada1.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada2.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada3.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada4.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada5.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada6.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada7.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada8.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada9.dop"),
												_T("C:\\Source\\Facade\\Data\\Ac1_Encoding_v1\\Fasada10.dop"),
	};*/
	/*CString wallNames[] = { _T("C:\\Source\\Facade\\Data\\Ac2_Encoding_v3\\Fasada_01.dop"),
													_T("C:\\Source\\Facade\\Data\\Ac2_Encoding_v3\\Fasada_02.dop"),
													_T("C:\\Source\\Facade\\Data\\Ac2_Encoding_v3\\Fasada_03.dop"),
													_T("C:\\Source\\Facade\\Data\\Ac2_Encoding_v3\\Fasada_04.dop"),
													_T("C:\\Source\\Facade\\Data\\Ac2_Encoding_v3\\Fasada_05.dop"),
													_T("C:\\Source\\Facade\\Data\\Ac2_Encoding_v3\\Fasada_06.dop"),
	};
	CString wallNames[] = { _T("C:\\Source\\Facade\\Data\\Jelez\\Fasada1.dop"),
													_T("C:\\Source\\Facade\\Data\\Jelez\\Fasada2.dop"),
													_T("C:\\Source\\Facade\\Data\\Jelez\\Fasada3.dop"),
													_T("C:\\Source\\Facade\\Data\\Jelez\\Fasada4.dop"),
	};*/
	CString wallNames[] = { _T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada1.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada2.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada3.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada4.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada5.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada6.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada7.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada8.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada9.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada10.dop"),
												_T("C:\\Source\\Facade\\Data\\Jelezn2\\Fasada11.dop"),
	};

	const int iPanelWidth = 60;
	const int iMaxPanelHeight = 600;
	const int iNumFiles = sizeof(wallNames) / sizeof(CString);
	const double dTolRemaing = 200.0;//MM

	std::vector<cWall*> Walls;
	FACADE_PROC::LoadWalls(wallNames, iNumFiles, Walls);
	FACADE_PROC::CreateRows(Walls, iMaxPanelHeight, iPanelWidth, dTolRemaing);
	FACADE_PROC::CalculatePanels(Walls, iPanelWidth, dTolRemaing);

	for (auto it = Walls.begin(); it != Walls.end(); ++it)
	{
		(*it)->ExportToRhinoScript(iPanelWidth);
	}
	
	for (auto it = Walls.begin(); it != Walls.end(); ++it)
		delete(*it);

	return 0;
}

