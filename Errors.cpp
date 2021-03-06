#include "stdafx.h"

#include <filesystem>
#include <algorithm>
#include <string>
#include <iostream>

#include "Errors.h"
#include "cInformationFile.h"
#include "Wall\panelTypeEnum.h"

using namespace std::filesystem;

static std::wofstream s_LogFile;
static cInformationFile s_ErrorFile;
static bool s_bToDisplayErrorWarning = true;
static unsigned s_uErrorCode = 0;
static std::string s_sLocalization = "ENGLISH";
static std::wstring s_FileName;
static stColor s_Color = { 0,0,0,false };


std::wofstream& FACADE_ERRORS::LogFile()
{
	return s_LogFile;
}
cInformationFile& FACADE_ERRORS::ErrorFile()
{
	return s_ErrorFile; 
}

void FACADE_ERRORS::SetErrorCode(unsigned uErrorCode, const stColor color, const char* pFileName)
{
	s_uErrorCode = uErrorCode; s_Color = color;
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

void FACADE_ERRORS::DisplayErrorMessage()
{
	if (s_uErrorCode == 0)
		return;

	std::string Key;
	Key = std::to_string(s_uErrorCode);
	TCHAR szError[cInformationFile::BUFF_SIZE];
	memset(szError, 0, sizeof(TCHAR)*cInformationFile::BUFF_SIZE);
	
	std::wstring String;
	if (!s_ErrorFile.IsReadyToRead() || !s_ErrorFile.GetSetting(CA2CT(s_sLocalization.c_str()), CA2CT(Key.c_str()), szError, cInformationFile::BUFF_SIZE))
	{
		if (s_bToDisplayErrorWarning)
		{
			s_bToDisplayErrorWarning = false;
			std::wcout << _T("Warning: ") << _T("Missing or corrupt errors.ini. Notification codes without strings will be logged as codes only.") << _T("\n");
			if (!s_LogFile.bad())
				s_LogFile << _T("Warning: ") << _T("Missing or corrupt errors.ini. Notification codes without strings will be logged as codes only.") << _T("\n");
		}
		std::wcout << s_uErrorCode;
		if (!s_LogFile.bad())
		{
			s_LogFile << _T("Error code=") << s_uErrorCode << _T("\n");
		}
	}
	else
	{
		String = szError;

		std::wcout << String;
		if (!s_LogFile.bad())
		{
			s_LogFile << _T("Error code=") << s_uErrorCode << _T(" - ") << String;
			if (s_Color.m_bValid)
				s_LogFile << s_Color.red << _T(",") << s_Color.green << _T(",") << s_Color.blue;
			s_LogFile << _T("\n");
			if (!s_FileName.empty())
			{
				s_LogFile << _T("File Name=") << s_FileName << _T("\n");
			}
		}
	}

	s_Color.m_bValid = false;
	s_uErrorCode = 0;
	s_FileName.clear();
}

void FACADE_ERRORS::DisplayWarningMessage(const std::string WarningKey)
{
	TCHAR szError[cInformationFile::BUFF_SIZE];
	memset(szError, 0, sizeof(TCHAR)*cInformationFile::BUFF_SIZE);
	std::wstring String;
	if (!s_ErrorFile.IsReadyToRead() || !s_ErrorFile.GetSetting(CA2CT(s_sLocalization.c_str()), CA2CT(WarningKey.c_str()), szError, cInformationFile::BUFF_SIZE))
	{
		if (s_bToDisplayErrorWarning)
		{
			s_bToDisplayErrorWarning = false;
			std::wcout << _T("Warning: ") << _T("Missing or corrupt errors.ini. Notification codes without strings will be logged as codes only.") << _T("\n");
			if (!s_LogFile.bad())
				s_LogFile << _T("Warning: ") << _T("Missing or corrupt errors.ini. Notification codes without strings will be logged as codes only.") << _T("\n");
		}
		String = CA2CT(WarningKey.c_str());
	}
	else
		String = szError;

	std::wcout << _T("Warning: ") << String << _T("\n");
	if (!s_LogFile.bad())
	{
		s_LogFile << _T("Warning: ") << String << _T("\n");
	}
}
