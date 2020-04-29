#include "stdafx.h"
#include "cRhinoScriptFile.h"
#include "Wall\panel.h"

template <typename T>
unsigned countof(T* arr)
{
	return sizeof(arr) / sizeof(arr[0]);
}

//////////////////////////////////////////////////////////////////////////////////
cRhinoScriptFile::cRhinoScriptFile()
	: m_file(nullptr)
	, m_bCanNotWrite(true)
{}


//////////////////////////////////////////////////////////////////////////////////
cRhinoScriptFile::cRhinoScriptFile(const std::string& szFullFilePath)
	: m_file(nullptr)
	, m_bCanNotWrite(true)
	, m_strFileName(szFullFilePath)
{
	ResetFile(true);
}

//////////////////////////////////////////////////////////////////////////////////
cRhinoScriptFile::~cRhinoScriptFile()
{
	if (m_file)
		fclose(m_file);
}

//////////////////////////////////////////////////////////////////////////////////
void cRhinoScriptFile::ResetFile(bool bDeleteOldFile)
{
	if(m_file)
		fclose(m_file);
	if ( bDeleteOldFile )
	{
		// delete old LOG file... ;-)
		if(fopen_s(&m_file, m_strFileName.c_str(), ("wt")) || !m_file)
		{
			m_bCanNotWrite = true;
			return;
		}
		// create empty file
		fwprintf(m_file, (const wchar_t*)"");
		m_bCanNotWrite = false;
	}
	else
	{
		if(fopen_s(&m_file, m_strFileName.c_str(), ("at")) || !m_file) // if the file doesnot exist or the directory is read only
		{
			if(fopen_s(&m_file, m_strFileName.c_str(), ("wt")) || !m_file) // the directory is readonly
			{
				m_bCanNotWrite = true;
				return;
			}
			// create empty file
			fwprintf(m_file, (const wchar_t*)"");
		}
		m_bCanNotWrite = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////
void cRhinoScriptFile::SetPath( const std::string& szPath )
{
	if( m_strFileName.empty() || m_strFileName != szPath )
	{
		m_bCanNotWrite = true;
		m_strFileName = szPath;
	}
}

//////////////////////////////////////////////////////////////////////////////////
bool cRhinoScriptFile::WriteFlatPanel( const c3DPointI P[4])
{
	if (m_bCanNotWrite)
		return false;

	if( fwprintf(m_file, _T("_box 3Point ")) <=0 )
		return false;
	for (int i = 0; i < 4; ++i)
	{
		if(fwprintf(m_file, _T("%d,%d,%d "), P[i].x, P[i].y, P[i].z) <=0 )
			return false;
	}
	return fwprintf(m_file, _T("_Enter\n") ) > 0;
}

bool cRhinoScriptFile::WriteEdgePanel(const c3DPointI P[6], unsigned iPanelHeight)
{
	if (m_bCanNotWrite)
		return false;

	if (fwprintf(m_file, _T("_Polyline\n")) <= 0)
		return false;
	for (int i = 0; i < 6; ++i)
	{
		if (fwprintf(m_file, _T("%d,%d,%d\n"), P[i].x, P[i].y, P[i].z) <= 0)
			return false;
	}
	if (fwprintf(m_file, _T("%d,%d,%d\n"), P[0].x, P[0].y, P[0].z) <= 0)
		return false;
	if (fwprintf(m_file, _T("_Enter\n_SelLast\n_PlanarSrf\n_SelPrev\n_Delete\n_SelLast\n")) <= 0)
			return false;
	return  fwprintf(m_file, _T("_ExtrudeSrf BothSides=Yes Solid=Yes DeleteInput=Yes  %d\n"), iPanelHeight/2) > 0;
}