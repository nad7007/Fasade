#include "stdafx.h"
#include "cInformationFile.h"

template <typename T>
unsigned countof(T* arr)
{
	return sizeof(arr) / sizeof(arr[0]);
}

//////////////////////////////////////////////////////////////////////////////////
cInformationFile::cInformationFile()
{
	emptyStr = "";
	m_bCanNotWrite = true;	
}


//////////////////////////////////////////////////////////////////////////////////
cInformationFile::cInformationFile(const TCHAR* szFullFilePath)
{
	m_strFileName = szFullFilePath;
	m_bCanNotWrite = true;	
}

//////////////////////////////////////////////////////////////////////////////////
cInformationFile::~cInformationFile()
{

}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::IsReadyToRead() const
{
	FILE* f;
	if(_tfopen_s(&f, m_strFileName, _T("rt")) || !f )
		return false;
	fclose(f);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
void cInformationFile::ResetFile(bool bDeleteOldFile)
{
	if ( bDeleteOldFile )
	{
		// delete old LOG file... ;-)
		FILE* f;
		if(_tfopen_s(&f, m_strFileName, _T("wt")) || !f )
		{
			m_bCanNotWrite = true;
			return;
		}
		// create empty file
		fwprintf(f, (const wchar_t*)"");
		fclose(f);
		m_bCanNotWrite = false;
	}
	else
	{
		FILE* f;
		_tfopen_s(&f, m_strFileName, _T("at"));
		if(_tfopen_s(&f, m_strFileName, _T("at")) || !f ) // if the file doesnot exist or the directory is read only
		{
			if(_tfopen_s(&f, m_strFileName, _T("wt")) || !f ) // the directory is readonly
			{
				m_bCanNotWrite = true;
				return;
			}
			// create empty file
			fwprintf(f, (const wchar_t*)"");
			fclose(f);
			m_bCanNotWrite = false;
			return;
		}
		fclose(f);
		m_bCanNotWrite = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////
void cInformationFile::SetPath( LPCTSTR szPath )
{
	if( m_strFileName.IsEmpty() || m_strFileName != szPath )
	{
		m_bCanNotWrite = true;
		m_strFileName = szPath;
	}
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::FindSection( LPCTSTR szSection )
{
	size_t uLen = _tcslen(szSection);
	LPTSTR szSectionCopy = new TCHAR[ uLen + 2 ];
	_tcscpy_s( szSectionCopy, uLen, szSection );
	szSectionCopy[ uLen ] = _T('\\');
	szSectionCopy[ uLen + 1 ] = _T('\0');
	TCHAR * szBuffer = NULL;
	DWORD dwSize = 4096;
	DWORD dwRequired = 0;
	do
	{
		delete szBuffer;
		szBuffer = new TCHAR[ dwSize ];
		dwRequired = GetPrivateProfileSectionNames( szBuffer, dwSize-1, m_strFileName );

		//If the buffer is not large enough to contain all the section 
		// names associated with the specified initialization file,
		// the return value is equal to the size specified by nSize minus two
		if( (dwSize-3) == dwRequired )
		{
			dwSize *= 2;
		}
		else
			break;
	}
	while( 1 );

	LPTSTR szString = szBuffer;
	while( szString && ( szString < (szBuffer + dwRequired ) ) )
	{
		// check for full match
		if( _tcsicmp( szSection, szString ) == 0 )
		{
			delete szSectionCopy;
			delete szBuffer;
			return true;
		}
		// check for substring ( szSection is root\test\section, but in the ini file we have root\test\section\subsection )
		if( _tcsstr( szString, szSectionCopy ) == szString )
		{
			delete szSectionCopy;
			delete szBuffer;
			return true;
		}

		szString = szString + _tcslen( szString ) + 1;
	}

	delete szSectionCopy;
	delete szBuffer;
	return false;
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::SaveSetting( LPCTSTR section, LPCTSTR key, const TCHAR* setting )
{
	if(m_bCanNotWrite)
		return false;
	
	if( WritePrivateProfileString( section, key, setting, m_strFileName))
	{
		return true;
	}
	
	return false;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::SaveSetting( LPCTSTR section, LPCTSTR key, const float &setting )
{
	if(m_bCanNotWrite)
		return false;

	TCHAR szSetting[ 200 ];
	_stprintf_s( szSetting, 200, _T("%.6f"), setting );

	if(WritePrivateProfileString( section, key, (LPCTSTR)szSetting, (LPCTSTR)m_strFileName))
	{
		return true;
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::SaveSetting( LPCTSTR section, LPCTSTR key, const double &setting )
{
	if(m_bCanNotWrite)
		return false;

	TCHAR szSetting[ 200 ];
	_stprintf_s( szSetting, 200, _T("%.12f"), setting );

	if( WritePrivateProfileString( section, key, (LPCTSTR)szSetting, (LPCTSTR)m_strFileName) )
	{
		return true;
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::SaveSetting( LPCTSTR section, LPCTSTR key, const int &setting )
{
	if(m_bCanNotWrite)
		return false;

	TCHAR szSetting[ 200 ];
	_stprintf_s( szSetting, 200, _T("%d"), setting );

	if(WritePrivateProfileString( section, key, (LPCTSTR)szSetting, (LPCTSTR)m_strFileName))
	{
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::SaveSetting( LPCTSTR section, LPCTSTR key, const unsigned int &setting )
{
	if(m_bCanNotWrite)
		return false;

	TCHAR szSetting[ 200 ];
	_stprintf_s( szSetting, 200, _T("%u"), setting );

	if(WritePrivateProfileString( section, key, (LPCTSTR)szSetting, (LPCTSTR)m_strFileName))
	{
		return true;
	}

	return false;
	
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::GetSetting( LPCTSTR section, LPCTSTR key, TCHAR* setting, unsigned uMaxCharsCount )
{
	_ASSERT( uMaxCharsCount <= BUFF_SIZE );
	TCHAR szResult[ BUFF_SIZE ];
	if( GetPrivateProfileString( section, key, emptyStr, szResult, countof(szResult), m_strFileName ) )
	{
		_tcsncpy_s ( setting, BUFF_SIZE, szResult, min( BUFF_SIZE, uMaxCharsCount ) );
		setting[ min( BUFF_SIZE, uMaxCharsCount ) - 1 ] = 0;
		return true;
	}


	DWORD err = GetLastError();
	(err);
	return false;
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::GetSetting( LPCTSTR section, LPCTSTR key, float &setting )
{
	TCHAR szResult[ 200 ];

	if( GetPrivateProfileString( section, key, (LPCTSTR)emptyStr, szResult, sizeof(szResult), (LPCTSTR)m_strFileName ) )
	{
#ifdef UNICODE	
		swscanf_s( szResult, _T("%f"), &setting );
#else
		sscanf_s( szResult, _T("%f"), &setting );
#endif
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::GetSetting( LPCTSTR section, LPCTSTR key, double &setting )
{
	TCHAR szResult[ 200 ];

	if( GetPrivateProfileString( section, key, emptyStr, szResult,sizeof(szResult), m_strFileName ) )
	{
#ifdef UNICODE	
		swscanf_s( szResult, _T("%lf"), &setting );
#else
		sscanf_s( szResult, _T("%lf"), &setting );
#endif
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::GetSetting( LPCTSTR section, LPCTSTR key, int &setting )
{
	TCHAR szResult[ 200 ];
	if( GetPrivateProfileString( section, key, emptyStr, szResult,sizeof(szResult), m_strFileName ) )
	{
#ifdef UNICODE	
		swscanf_s( szResult, _T("%d"), &setting );
#else
		sscanf_s( szResult, _T("%d"), &setting );
#endif
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////
bool cInformationFile::GetSetting( LPCTSTR section, LPCTSTR key, unsigned int &setting )
{
	TCHAR szResult[ 200 ];

	if( GetPrivateProfileString( section, key,_T(""), szResult,sizeof(szResult), m_strFileName ) )
	{
#ifdef UNICODE	
		swscanf_s( szResult, _T("%u"), &setting );
#else
		sscanf_s( szResult, _T("%u"), &setting );
#endif
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////

//Global Functions
//////////////////////////////////////////////////////////////////////////////////
bool ReadMatrix( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, double* MatArray )
{
	TCHAR szMatrix[12 * 32 ] = { 0 };
	if( !_InfoFile.GetSetting( (LPCTSTR)szSectionName, (LPCTSTR)szKeyName, szMatrix, countof(szMatrix) ) )
		return false;


	// read directly the 12 values
#ifdef UNICODE
	swscanf_s( szMatrix, _T("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "),
							&MatArray[0], &MatArray[1], &MatArray[2], &MatArray[3], 
							&MatArray[4], &MatArray[5], &MatArray[6], &MatArray[7], 
							&MatArray[8], &MatArray[9], &MatArray[10], &MatArray[11]);
#else
	sscanf_s( szMatrix, _T("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "), 
							&MatArray[0], &MatArray[1], &MatArray[2], &MatArray[3], 
							&MatArray[4], &MatArray[5], &MatArray[6], &MatArray[7], 
							&MatArray[8], &MatArray[9], &MatArray[10], &MatArray[11]);
#endif
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
bool ReadMatrix( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, float* MatArray )
{
	TCHAR szMatrix[ 12 * 32 ]={0};
	if( !_InfoFile.GetSetting( (LPCTSTR)szSectionName, (LPCTSTR)szKeyName, szMatrix, countof(szMatrix) ) )
		return false;


#ifdef UNICODE
	swscanf_s( szMatrix, _T("%f %f %f %f %f %f %f %f %f %f %f %f "),
							&MatArray[0], &MatArray[1], &MatArray[2], &MatArray[3], 
							&MatArray[4], &MatArray[5], &MatArray[6], &MatArray[7], 
							&MatArray[8], &MatArray[9], &MatArray[10], &MatArray[11]);
#else
	sscanf_S( szMatrix, _T("%f %f %f %f %f %f %f %f %f %f %f %f "), 
							&MatArray[0], &MatArray[1], &MatArray[2], &MatArray[3], 
							&MatArray[4], &MatArray[5], &MatArray[6], &MatArray[7], 
							&MatArray[8], &MatArray[9], &MatArray[10], &MatArray[11]);
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
bool WriteMatrix( cInformationFile& _Infofile, const TCHAR* SectionName, const TCHAR* szKeyName, const double* MatArray )
{
	CString szMatrix;
	szMatrix.Format( _T("%.17f %.17f %.17f %.17f %.17f %.17f %.17f %.17f %.17f %.17f %.17f %.17f"),
					MatArray[0], MatArray[1], MatArray[2], MatArray[3], 
					MatArray[4], MatArray[5], MatArray[6], MatArray[7], 
					MatArray[8], MatArray[9], MatArray[10],MatArray[11] );
	if( !_Infofile.SaveSetting( SectionName, szKeyName, szMatrix ) )
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
bool WriteMatrix( cInformationFile& _Infofile, const TCHAR* SectionName, const TCHAR* szKeyName, const float*  MatArray )
{
	CString szMatrix;
	szMatrix.Format( _T("%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f"),
					MatArray[0], MatArray[1], MatArray[2], MatArray[3], 
					MatArray[4], MatArray[5], MatArray[6], MatArray[7], 
					MatArray[8], MatArray[9], MatArray[10],MatArray[11] );
	if( !_Infofile.SaveSetting( SectionName, szKeyName, szMatrix ) )
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
unsigned ReadArray( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, float* MatArray, unsigned char iNumElements )
{
	_ASSERT( iNumElements * 32 <= cInformationFile::BUFF_SIZE );
	TCHAR szMatrixBuff[ cInformationFile::BUFF_SIZE ];
	memset( szMatrixBuff, 0, sizeof(TCHAR)*iNumElements * 32);
	memset( MatArray, 0, sizeof( float)* iNumElements );
	if( !_InfoFile.GetSetting( (LPCTSTR)szSectionName, (LPCTSTR)szKeyName, szMatrixBuff, cInformationFile::BUFF_SIZE ) )
		return 0;
	
	CString szMatElement;
	CString szMatrix = szMatrixBuff;
	unsigned iMatIndex = 0;
	int iBeg = 0;
	int iEnd = 0;
	while(iEnd>=0 && iMatIndex<iNumElements )
	{
		iEnd = szMatrix.Find(_T(' '), iBeg );
		szMatElement = szMatrix.GetBuffer()+iBeg;
		( iEnd < 0 )
			? szMatElement.ReleaseBuffer(szMatrix.StringLength(szMatrix) - iBeg)
			: szMatElement.ReleaseBuffer(iEnd - iBeg);
		MatArray[iMatIndex] = float( _tstof(szMatElement )); 
		iBeg = iEnd+1;
		iMatIndex++;
	};
	return iMatIndex;
}

//////////////////////////////////////////////////////////////////////////////////
unsigned ReadArray( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, double* MatArray, unsigned char iNumElements )
{
	_ASSERT( iNumElements * 32 <= cInformationFile::BUFF_SIZE );
	TCHAR szMatrixBuff[ cInformationFile::BUFF_SIZE ];
	memset( szMatrixBuff, 0, sizeof(TCHAR)*iNumElements * 32);
	memset( MatArray, 0, sizeof( double)* iNumElements );
	if( !_InfoFile.GetSetting( (LPCTSTR)szSectionName, (LPCTSTR)szKeyName, szMatrixBuff, cInformationFile::BUFF_SIZE ) )
		return 0;
	
	CString szMatElement;
	CString szMatrix = szMatrixBuff;
	int iBeg = 0, iEnd = 0;
	unsigned iMatIndex = 0;
	while(iEnd>=0 && iMatIndex<iNumElements )
	{
		iEnd = szMatrix.Find(_T(' '), iBeg );
		szMatElement = szMatrix.GetBuffer()+iBeg;
		( iEnd < 0 )
			? szMatElement.ReleaseBuffer(szMatrix.StringLength(szMatrix) - iBeg)
			: szMatElement.ReleaseBuffer(iEnd - iBeg);
		MatArray[iMatIndex] = ( _tstof(szMatElement )); 
		iBeg = iEnd+1;
		iMatIndex++;
	};
	return iMatIndex;
}

//////////////////////////////////////////////////////////////////////////////////
bool WriteArray( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, const float* MatArray, unsigned char iNumElements )
{
	CString StrBuffer;
#ifndef UNICODE
	char Buff[128];
#endif

	for( unsigned char i=0; i<iNumElements; ++i )
	{
#ifdef UNICODE
		CString strInfo;
		strInfo.Format( _T("%.12f"), MatArray[i] ); 
		//TCHAR Buffer[128];
		//::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, Buff, (int)strlen(Buff), Buffer, (int)wcslen(Buffer) );
		//StrBuffer += Buffer;
		StrBuffer   += strInfo;
#else
		if( !_gcvt( MatArray[i], 12, Buff ) )
			return false;
		StrBuffer += Buff;
#endif
		StrBuffer += _T(' ');
	}

	if( !_InfoFile.SaveSetting( (LPCTSTR)szSectionName, (LPCTSTR)szKeyName, StrBuffer ) )
		return false;
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
unsigned ReadArray( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, int* MatArray, unsigned char iNumElements )
{
	_ASSERT( iNumElements * sizeof(int) <= cInformationFile::BUFF_SIZE );
	TCHAR szMatrixBuff[ cInformationFile::BUFF_SIZE ];
	memset( szMatrixBuff, 0, sizeof(TCHAR)*iNumElements * sizeof(int));
	memset( MatArray, 0, sizeof(MatArray[0])* iNumElements );
	if( !_InfoFile.GetSetting( (LPCTSTR)szSectionName, (LPCTSTR)szKeyName, szMatrixBuff, cInformationFile::BUFF_SIZE ) )
		return 0;
	
	CString szMatElement;
	CString szMatrix = szMatrixBuff;
	int iBeg = 0, iEnd = 0;
	unsigned iMatIndex = 0;
	while(iEnd>=0 && iMatIndex<iNumElements )
	{
		iEnd = szMatrix.Find(_T(' '), iBeg );
		szMatElement = szMatrix.GetBuffer()+iBeg;
		( iEnd < 0 )
			? szMatElement.ReleaseBuffer(szMatrix.StringLength(szMatrix) - iBeg)
			: szMatElement.ReleaseBuffer(iEnd - iBeg);
		MatArray[iMatIndex] = ( _tstoi(szMatElement )); 
		iBeg = iEnd+1;
		iMatIndex++;
	};
	return iMatIndex;
}

//////////////////////////////////////////////////////////////////////////////////
bool WriteArray( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, const int* MatArray, unsigned char iNumElements )
{
	CString StrBuffer;
	for( unsigned char i=0; i<iNumElements; ++i )
	{
		CString strInfo;
		strInfo.Format( _T("%d "), MatArray[i] ); 
		StrBuffer   += strInfo;
	}

	if( !_InfoFile.SaveSetting( (LPCTSTR)szSectionName, (LPCTSTR)szKeyName, StrBuffer ) )
		return false;
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
