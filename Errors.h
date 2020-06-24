#pragma once
#include <fstream>
#include "cInformationFile.h"

struct stColor;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FACADE_ERRORS
{
	std::wofstream& LogFile();
	cInformationFile& ErrorFile();

	void SetErrorCode(unsigned uErrorCode, const stColor color, const char* pFileName);
	void SetErrorCode(unsigned uErrorCode, const char* pFileName);
	void SetErrorCode(unsigned uErrorCode, const _TCHAR* pFileName);
	void SetErrorCode(unsigned uErrorCode);
	unsigned GetErrorCode();
	void SetLocalization(const std::string& LanguageName);
	void DisplayErrorMessage();
	void DisplayWarningMessage(const std::string WarningKey);
};
