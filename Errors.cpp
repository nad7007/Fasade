#include "stdafx.h"
#include <filesystem>
#include <algorithm>
#include <string>
#include <iostream>

#include "Errors.h"
#include "cInformationFile.h"
using namespace std::filesystem;

static unsigned s_uErrorCode = 0;
static std::string s_sLocalization = "ENGLISH";
static std::wstring s_FileName;

void FACADE_ERRORS::SetErrorCode(unsigned uErrorCode, const char* pFileName )
{
	s_uErrorCode = uErrorCode;
	if (pFileName)
	{
		int wchars_num = MultiByteToWideChar(CP_UTF8, 0, pFileName, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[wchars_num];
		MultiByteToWideChar(CP_UTF8, 0, pFileName, -1, wstr, wchars_num);
		s_FileName = wstr;
		delete[] wstr;
	}
	else
		s_FileName.clear();
}

void FACADE_ERRORS::SetErrorCode(unsigned uErrorCode, const _TCHAR* pFileName)
{
	s_uErrorCode = uErrorCode;
	if (pFileName)
	{
		s_FileName = *pFileName;
	}
	else
		s_FileName.clear();
}

void FACADE_ERRORS::SetErrorCode(unsigned uErrorCode )
{
	s_uErrorCode = uErrorCode;
	s_FileName.clear();
}

unsigned FACADE_ERRORS::GetErrorCode()
{
	return s_uErrorCode;
}

void FACADE_ERRORS::SetLocalization(const std::string& LanguageName)
{
	s_sLocalization = LanguageName;
	std::transform(s_sLocalization.begin(), s_sLocalization.end(), s_sLocalization.begin(), ::toupper);
}

void FACADE_ERRORS::DisplayErrorMessage(const cInformationFile& ErrorFile, std::wofstream& LogFile)
{
	if (s_uErrorCode == 0)
		return;

	std::string Key;
	Key = std::to_string(s_uErrorCode);
	TCHAR szError[cInformationFile::BUFF_SIZE];
	memset(szError, 0, sizeof(TCHAR)*cInformationFile::BUFF_SIZE);

	if (!ErrorFile.GetSetting(CA2CT(s_sLocalization.c_str()), CA2CT(Key.c_str()), szError, cInformationFile::BUFF_SIZE))
		return;

	std::wstring String = szError;

	if (ErrorFile.IsReadyToRead())
	{
			std::wcout << String;
	}

	if (!LogFile.bad())
	{
		LogFile << _T("Error code=") << s_uErrorCode<<_T(" - ") << String <<_T("\n");
		if (!s_FileName.empty())
		{
			LogFile << _T("File Name=") << s_FileName << _T("\n");
		}
	}
}

void FACADE_ERRORS::DisplayWarningMessage(const std::string WarningKey, const cInformationFile& ErrorFile, std::wofstream& LogFile)
{
	TCHAR szError[cInformationFile::BUFF_SIZE];
	memset(szError, 0, sizeof(TCHAR)*cInformationFile::BUFF_SIZE);

	if (!ErrorFile.GetSetting(CA2CT(s_sLocalization.c_str()), CA2CT(WarningKey.c_str()), szError, cInformationFile::BUFF_SIZE))
		return;

	std::wstring String = szError;

	if (ErrorFile.IsReadyToRead())
	{
		std::wcout << String;
	}

	if (!LogFile.bad())
	{
		LogFile << _T("Warning: ") << String << _T("\n");
	}
}
