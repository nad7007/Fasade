#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FACADE_ERRORS
{
	//static unsigned s_uErrorCode = 0;
	//static std::string s_sLocalization = "ENGLISH";
	void SetErrorCode(unsigned uErrorCode);
	unsigned GetErrorCode();
	void SetLocalization(const std::string& LanguageName);
	void DisplayErroMessage(const std::wstring& ErrorFile);
};
