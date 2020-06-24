#pragma once

#include "Math\c3DPoint.h"
class IfacePanel;

//////////////////////////////////////////////////////////////////////////////////
class cRhinoScriptFile  
{
//////////////////////////////////////////////////////////////////////////////////
public:
		cRhinoScriptFile();
		cRhinoScriptFile(const std::string& szFullFilePath);
		virtual ~cRhinoScriptFile();

//////////////////////////////////////////////////////////////////////////////////
public:

		void	SetPath(const std::string& szPath);
		const std::string& GetFilePath() const {
			return m_strFileName;
		}

		/*const TCHAR* GetFileShortName() const {
			int iInd = m_strFileName.ReverseFind('\\'); 
			return (iInd  > -1) ? m_strFileName.Right(m_strFileName.GetLength() - iInd -1 ): m_strFileName;
		}
		*/
		bool	IsReadyToWrite(bool bDeleteOldFile = true ){ 
			if( m_bCanNotWrite )
				ResetFile( bDeleteOldFile);
			return !m_bCanNotWrite;
		}

		bool WriteFlatPanel(const c3DPointI P[4], float fPanelWidth, float fPanelHeight, float fPanelThickness);
		bool WriteEdgePanel(const c3DPointI P[6], float fPanelWidthLeft, float fPanelWidthRight, float fPanelHeight, float fPanelThickness);

//////////////////////////////////////////////////////////////////////////////////
protected:

		std::string m_strFileName;
		bool m_bCanNotWrite;
		void ResetFile(bool bDeleteOldFile);

private:

		FILE* m_file;
};
