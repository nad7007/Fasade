#pragma once
#if !defined(AFX_INFORMATIONFILE_H__36F3D0AF_9170_11D4_AEEB_99E9274EB233__INCLUDED_)
#define AFX_INFORMATIONFILE_H__36F3D0AF_9170_11D4_AEEB_99E9274EB233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////
class cInformationFile  
{
//////////////////////////////////////////////////////////////////////////////////
public:
		cInformationFile();
		cInformationFile(const TCHAR* szFullFilePath);
		virtual ~cInformationFile();

//////////////////////////////////////////////////////////////////////////////////
public:

		static const unsigned BUFF_SIZE = 1500;//size of char array to set or get settings
	
		// saves	
		bool	SaveSetting( LPCTSTR section, LPCTSTR key, const float &setting );
		bool	SaveSetting( LPCTSTR section, LPCTSTR key, const double &setting );
		bool	SaveSetting( LPCTSTR section, LPCTSTR key, const int &setting );
		bool	SaveSetting( LPCTSTR section, LPCTSTR key, const unsigned int &setting );
		bool	SaveSetting( LPCTSTR section, LPCTSTR key, const TCHAR* setting);
		//BOOL	SaveSetting( LPCTSTR section, LPCTSTR key, const wchar_t* setting );

		// restores	
		bool	GetSetting( LPCTSTR section, LPCTSTR key, float &setting ) const;
		bool	GetSetting( LPCTSTR section, LPCTSTR key, double &setting )const;
		bool	GetSetting( LPCTSTR section, LPCTSTR key, int &setting )const;
		bool	GetSetting( LPCTSTR section, LPCTSTR key, unsigned int &setting )const;
		
		//!ATTENTION: the following String settings are limitted to BUFF_SIZE symbols
		//If you need more go to cpp and change the BUFF_SIZE constant.
		bool	GetSetting( LPCTSTR section, LPCTSTR key, TCHAR* setting, unsigned uMaxCountChars ) const;
		
		// sets m_strFileName to \\Path\appname_log.txt
		void	SetPath(LPCTSTR szPath);

		bool	IsReadyToRead() const;
		bool	IsReadyToWrite(bool bDeleteOldFile = true )
		{ 
			if( m_bCanNotWrite )
				ResetFile( bDeleteOldFile);
			return !m_bCanNotWrite;
		}

		const TCHAR* GetFilePath() const {return m_strFileName;}
		const TCHAR* GetFileShortName() const { int iInd = m_strFileName.ReverseFind('\\'); return (iInd  > -1) ? m_strFileName.Right(m_strFileName.GetLength() - iInd -1 ): m_strFileName;}

		bool FindSection( LPCTSTR section );

//////////////////////////////////////////////////////////////////////////////////
protected:
		CString m_strFileName;

		bool m_bCanNotWrite;
		void ResetFile(bool bDeleteOldFile);

//////////////////////////////////////////////////////////////////////////////////
private:
		mutable CString emptyStr;
};
//////////////////////////////////////////////////////////////////////////////////

//Global Function Declarations
//////////////////////////////////////////////////////////////////////////////////
extern	bool ReadMatrix( cInformationFile& _Infofile, const TCHAR* SectionName, const TCHAR* szKeyName, double* MatArray );
extern	bool ReadMatrix( cInformationFile& _Infofile, const TCHAR* SectionName, const TCHAR* szKeyName, float*  MatArray );
extern	bool WriteMatrix( cInformationFile& _Infofile, const TCHAR* SectionName, const TCHAR* szKeyName, const double* MatArray );
extern	bool WriteMatrix( cInformationFile& _Infofile, const TCHAR* SectionName, const TCHAR* szKeyName, const float*  MatArray );
//////////////////////////////////////////////////////////////////////////////////
extern	unsigned ReadArray( cInformationFile& _Infofile, const TCHAR* SectionName, const TCHAR* szKeyName, float* MatArray, unsigned char iNumElements );
extern	unsigned ReadArray( cInformationFile& _Infofile, const TCHAR* SectionName, const TCHAR* szKeyName, double* MatArray, unsigned char iNumElements );
extern  unsigned ReadArray( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, int* MatArray, unsigned char iNumElements );
extern  bool WriteArray( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, const float* MatArray, unsigned char iNumElements );
extern  bool WriteArray( cInformationFile& _InfoFile, const TCHAR* szSectionName, const TCHAR* szKeyName, const int* MatArray, unsigned char iNumElements );

#endif // !defined(AFX_INFORMATIONFILE_H__36F3D0AF_9170_11D4_AEEB_99E9274EB233__INCLUDED_)