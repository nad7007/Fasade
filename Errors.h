#pragma once
#include <fstream>

class cInformationFile;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FACADE_ERRORS
{
	void SetErrorCode(unsigned uErrorCode, const char* pFileName);
	void SetErrorCode(unsigned uErrorCode, const _TCHAR* pFileName);
	void SetErrorCode(unsigned uErrorCode);
	unsigned GetErrorCode();
	void SetLocalization(const std::string& LanguageName);
	void DisplayErrorMessage(const cInformationFile& ErrorFile, std::wofstream& LogFile);
	void DisplayWarningMessage(const std::string WarningKey, const cInformationFile& ErrorFile, std::wofstream& LogFile);
};
