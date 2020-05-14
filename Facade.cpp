// Facade.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <filesystem>
#include <TCHAR.h>
#include <iostream>
#include <fstream>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <iomanip> // put_time

#include "Processings.h"
#include "cInformationFile.h"
#include "Errors.h"

#include "Wall\Wall.h"

//#include <experimental/filesystem> // Header file for pre-standard implementation
//using namespace std::experimental::filesystem::v1;
using namespace std;
using namespace std::filesystem;

static void read_directory(const std::string& dirname, const string& fileExt, vector<string>& files)
{
	files.clear();
	path p(dirname);
	directory_iterator start(p);
	directory_iterator end;

	for (start; start != end; ++start)
	{
		if (start->path().extension() == fileExt)
			files.push_back(start->path().string());
	}
}

int main(int argc, char* argv[], char*)
{
	TCHAR szAppPath[512];
	GetModuleFileName(NULL, szAppPath, 512);
	PathRemoveFileSpec(szAppPath);//application starting dir

	path Path(szAppPath);
	Path.append("errors.ini");//hard codded name of ini file with Error codes and strings
	cInformationFile ErrorFile(Path.c_str());

	Path = szAppPath;
	Path.append("Facade.log");//hard codded name of application log file
	wofstream Log(Path, ios_base::binary);
	
	if (argc >= 4)//check for localization
	{
		FACADE_ERRORS::SetLocalization(argv[3]);
	}

	//Prepare log ouput stream
	wchar_t wBuffer[512];
	Log.rdbuf()->pubsetbuf(wBuffer, (streamsize)512);
	if (!Log.bad())//write data and time
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		Log << std::put_time(std::localtime(&in_time_t), _T("%d-%m-%Y %X")) << _T("\n\n");
	}

	if (argc < 2)
	{
		_ASSERT(false);//not inough input params
		FACADE_ERRORS::SetErrorCode(100);
		FACADE_ERRORS::DisplayErrorMessage(ErrorFile, Log);
		return FACADE_ERRORS::GetErrorCode();
	}

	std::string dirInName = argv[1];
	if(dirInName.empty())
	{
		_ASSERT(false);//not dir name
		FACADE_ERRORS::SetErrorCode(101);
		FACADE_ERRORS::DisplayErrorMessage(ErrorFile, Log);
		return FACADE_ERRORS::GetErrorCode();
	}

	std::string dirOutName = dirInName;
	if (argc >= 3)
	{
		dirOutName = argv[2];
	}

	if (dirOutName.back() != '\\')
	{
		dirOutName += '\\';
	}

	unsigned int iPanelWidth = 60;
	unsigned int iMaxPanelHeight = 600;
	unsigned int iVertTol = 200.0;//MM
	unsigned int iHorTol = 200.0;//MM
	unsigned int iVertShift = 50;//MM

	Path = szAppPath;
	Path.append("settings.ini");

	cInformationFile Config(Path.c_str());
	if (Config.IsReadyToRead())
	{//Read global parameters from INI file
		Config.GetSetting(CA2CT("PANELS"), CA2CT("THICKNESS"), iPanelWidth);
		Config.GetSetting(CA2CT("PANELS"), CA2CT("HEIGHT"), iMaxPanelHeight);
		Config.GetSetting(CA2CT("COVERING"), CA2CT("MIN_HORIZONTAL_SIZE_OF_INTEREST"), iHorTol);
		Config.GetSetting(CA2CT("COVERING"), CA2CT("MAX_VERTICAL_SIZE_OF_INTEREST"), iVertTol);
		Config.GetSetting(CA2CT("COVERING"), CA2CT("MIN_VERTICAL_SHIFT"), iVertShift);
	}
	else
	{
		_ASSERT(false);//no settings.ini in application dir
	}
	double dTolRemaining = iVertTol;//MM

	vector<string> wallNames;
	read_directory(dirInName, ".dop", wallNames);
	const int iNumFiles = wallNames.size();
	
		
	/*CString wallNames[] = { _T("C:\\Source\\Data\\Olives\\Fasada1.dop"),
												_T("C:\\Source\\Data\\Olives\\Fasada2.dop"),
												_T("C:\\Source\\Data\\Olives\\Fasada3.dop"),
												_T("C:\\Source\\Data\\Olives\\Fasada4.dop"),
												_T("C:\\Source\\Data\\Olives\\Fasada5.dop"),
												_T("C:\\Source\\Data\\Olives\\Fasada6.dop"),
												_T("C:\\Source\\Data\\Olives\\Fasada7.dop"),
	};
	CString wallNames[] = { _T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada1.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada2.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada3.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada4.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada5.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada6.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada7.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada8.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada9.dop"),
												_T("C:\\Source\\Data\\Ac1_Encoding_v1\\Fasada10.dop"),
	};*/
	/*CString wallNames[] = { _T("C:\\Source\\Data\\Ac2_Encoding_v3\\Fasada_01.dop"),
													_T("C:\\Source\\Data\\Ac2_Encoding_v3\\Fasada_02.dop"),
													_T("C:\\Source\\Data\\Ac2_Encoding_v3\\Fasada_03.dop"),
													_T("C:\\Source\\Data\\Ac2_Encoding_v3\\Fasada_04.dop"),
													_T("C:\\Source\\Data\\Ac2_Encoding_v3\\Fasada_05.dop"),
													_T("C:\\Source\\Data\\Ac2_Encoding_v3\\Fasada_06.dop"),
	};*/
	/*vector<string> wallNames = { {"C:\\Source\\Data\\Jelez\\Fasada1.dop"},
																	{"C:\\Source\\Data\\Jelez\\Fasada2.dop"},
																	{"C:\\Source\\Data\\Jelez\\Fasada3.dop"},
																	{"C:\\Source\\Data\\Jelez\\Fasada4.dop"},
																	};
	*/
	/*CString wallNames[] = { _T("C:\\Source\\Data\\Jelezn2\\Fasada1.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada2.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada3.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada4.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada5.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada6.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada7.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada8.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada9.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada10.dop"),
												_T("C:\\Source\\Data\\Jelezn2\\Fasada11.dop"),
	};
		CString wallNames[] = { _T("C:\\Source\\Data\\Varna\\Fasada1.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada2.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada3.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada4.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada5.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada6.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada7.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada8.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada9.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada10.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada11.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada12.dop"),
												_T("C:\\Source\\Data\\Varna\\Fasada13.dop"),
	};*/

	//cColorCodding CC;
	//CC.SaveToIniFile(_T("C:\\Source\\Fasade\\settings.ini"));

	
	cGDIStarter StartGDI;
	std::vector<cWall*> Walls;
	if( FACADE_PROC::LoadWalls(wallNames, iNumFiles, Walls) )
	 if( FACADE_PROC::CreateRows(Walls, iMaxPanelHeight, iPanelWidth, dTolRemaining) )
		 if (FACADE_PROC::CalculatePanels(Walls, iPanelWidth, dTolRemaining))
		 {
			 for (auto it = Walls.begin(); it != Walls.end(); ++it)
			 {
				 (*it)->ExportToRhinoScript(dirOutName, iPanelWidth);
			 }
		 }
	
	for (auto it = Walls.begin(); it != Walls.end(); ++it)
		delete(*it);
	
	FACADE_ERRORS::DisplayErrorMessage(ErrorFile, Log);
	Log.close();
	return FACADE_ERRORS::GetErrorCode();
}

