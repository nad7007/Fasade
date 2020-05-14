#pragma once
#include <fstream>

class cInformationFile;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FACADE_ERRORS
{
	//static unsigned s_uErrorCode = 0;
	//static std::string s_sLocalization = "ENGLISH";
	void SetErrorCode(unsigned uErrorCode);
	unsigned GetErrorCode();
	void SetLocalization(const std::string& LanguageName);
	void DisplayErrorMessage(const cInformationFile& ErrorFile, std::wofstream& LogFile);
};
