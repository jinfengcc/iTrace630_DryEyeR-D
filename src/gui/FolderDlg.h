//***************************************************************************************

#pragma once

//***************************************************************************************

#include <Shlobj.h>    // folder selection dialog box

//***************************************************************************************

class CFolderDlg
{
public:

	CFolderDlg(CWnd* pParentWnd, const CString& Title);

	CWnd* m_pParentWnd;
	CString m_Title;
	CString m_FolderName;

	static CString m_InitialFolderName;

	BOOL Show();
};

//***************************************************************************************
