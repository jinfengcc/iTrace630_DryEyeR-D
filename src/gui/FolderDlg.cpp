//***************************************************************************************

#include "StdAfx.h"
#include "FolderDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CString CFolderDlg::m_InitialFolderName;

//***************************************************************************************

CFolderDlg::CFolderDlg(CWnd* pParentWnd, const CString& Title)
{
	m_pParentWnd = pParentWnd;
	m_Title = Title;
}

//***************************************************************************************

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		const TCHAR* x = (LPCTSTR)CFolderDlg::m_InitialFolderName;//6.2.0

		CString Folder = CFolderDlg::m_InitialFolderName;
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(x));
		break;
	}
	return 0;
}

//***************************************************************************************

BOOL CFolderDlg::Show()
{
	LPMALLOC pmalloc; // shell's IMalloc interface pointer
	if (::SHGetMalloc(&pmalloc) != NOERROR) return FALSE;

	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	if (m_pParentWnd) bi.hwndOwner = m_pParentWnd->m_hWnd;
	bi.lpszTitle = m_Title;
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;

	ITEMIDLIST* pItemIDList = ::SHBrowseForFolder(&bi);

	TCHAR Buf[1024];
	BOOL res = FALSE;
	if (pItemIDList != NULL)
	{
		// user didn't press Cancel button
		if (::SHGetPathFromIDList(pItemIDList, Buf))
		{
			m_FolderName = Buf;
			res = TRUE;
		}
	}

	pmalloc->Free(pItemIDList);
	pmalloc->Release();

	return res;
}

//***************************************************************************************
