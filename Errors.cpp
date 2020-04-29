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

void FACADE_ERRORS::SetErrorCode(unsigned uErrorCode)
{
	s_uErrorCode = uErrorCode;
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

void FACADE_ERRORS::DisplayErroMessage(const std::wstring& ErrorFileName)
{
	if (s_uErrorCode == 0)
		return;

	cInformationFile ErrorFile(ErrorFileName.c_str());
	if (!ErrorFile.IsReadyToRead())
		return;

	std::string Key;
	Key = std::to_string(s_uErrorCode);
	TCHAR szError[cInformationFile::BUFF_SIZE];
	memset(szError, 0, sizeof(TCHAR)*cInformationFile::BUFF_SIZE);

	if (!ErrorFile.GetSetting(CA2CT(s_sLocalization.c_str()), CA2CT(Key.c_str()), szError, cInformationFile::BUFF_SIZE))
		return;
	std::wstring String = szError;
	std::wcout << String;
}
