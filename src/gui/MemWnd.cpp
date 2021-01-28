//***************************************************************************************

#include "stdAfx.h"
#include "MemWnd.h"
#include "Image.h"
#include "Settings.h"
#include "Registry.h"
#include "TraceyDicom.h"//6.2.0 For Dicom Toolkit
#include "DcmtkDicom.h"//6.2.0 For Dicom Toolkit

#include "INIParser.h"//6.2.0 ini registration

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BOOL CMemWnd::m_Printing = FALSE;

BOOL CMemWnd::m_Saving = FALSE;

//***************************************************************************************

BEGIN_MESSAGE_MAP(CMemWnd, CWnd)

	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()

END_MESSAGE_MAP()

//***************************************************************************************

CMemWnd::CMemWnd()
{
	m_pBackup = NULL;
}

//***************************************************************************************

CMemWnd::~CMemWnd()
{
	SAFE_DELETE_ARRAY(m_pBackup);
}

//***************************************************************************************

void CMemWnd::CreateWnd(RECT& Rect, CWnd* pParentWnd)
{
	CWnd::CreateEx(0, ::WndClassName, _T(""), WS_CHILD, Rect, pParentWnd, 0);

	m_w = Rect.right - Rect.left;
	m_h = Rect.bottom - Rect.top;

	m_MemDC.CreateTrueColorDC(m_w, m_h);

	m_Rgn.SetRectRgn(0, 0, m_w, m_h);

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CMemWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	RECT Rect;
	GetWindowRect(&Rect);

	GetParent()->PostMessage(WM_NOTIFY_LBUTTON_DOWN, (WPARAM)this, 0);
}

//***************************************************************************************

void CMemWnd::OnRButtonDown(uint nFlags, CPoint Point)
{
	RECT Rect;
	GetWindowRect(&Rect);

	GetParent()->PostMessage(WM_NOTIFY_RBUTTON_DOWN, (WPARAM)this, 0);
}

//***************************************************************************************

void CMemWnd::OnRButtonUp(uint nFlags, CPoint Point)
{
	RECT Rect;
	GetWindowRect(&Rect);

	GetParent()->PostMessage(WM_NOTIFY_RBUTTON_UP, (WPARAM)this, (LPARAM)(((ulong)(Rect.left + Point.x) << 16) | (ulong)(Rect.top + Point.y)));
}

//***************************************************************************************

void CMemWnd::OnPaint()
{
	CPaintDC PaintDC(this);
	PaintDC.BitBlt(0, 0, m_w, m_h, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CMemWnd::ClearMemDC()
{
	m_MemDC.FillSolidRect(0, 0, m_w, m_h, m_Printing ? WHITE : BLACK);
}

//***************************************************************************************

void CMemWnd::Border()
{
	RECT Rect = { 0, 0, m_w, m_h };

	//6.2.0 NEAR VISION
	if (SpecialBorder == TRUE)
	{
		COLORREF white = m_Printing ? LIGHT_GRAY : WHITE;
		COLORREF gray = m_Printing ? LIGHT_GRAY : DARK_GRAY;

		if (HighLightOutline[3])
		{
			m_MemDC.DrawLine(Rect.left, Rect.bottom - 1, Rect.right, Rect.bottom - 1, 1, white);
		}
		else
		{
			m_MemDC.DrawLine(Rect.left, Rect.bottom - 1, Rect.right, Rect.bottom - 1, 1, gray);
		}

		if (HighLightOutline[0])
		{
			m_MemDC.DrawLine(Rect.left, Rect.top, Rect.left, Rect.bottom, 1, white);
		}
		else
		{
			m_MemDC.DrawLine(Rect.left, Rect.top, Rect.left, Rect.bottom, 1, gray);
		}

		if (HighLightOutline[1])
		{
			m_MemDC.DrawLine(Rect.left, Rect.top, Rect.right, Rect.top, 1, white);
		}
		else
		{
			m_MemDC.DrawLine(Rect.left, Rect.top, Rect.right, Rect.top, 1, gray);
		}

		if (HighLightOutline[2])
		{
			m_MemDC.DrawLine(Rect.right - 1, Rect.top, Rect.right - 1, Rect.bottom, 1, white);
		}
		else
		{
			m_MemDC.DrawLine(Rect.right - 1, Rect.top - 1, Rect.right - 1, Rect.bottom, 1, gray);
		}
	}
	//6.2.0 NEAR VISION
	else
	{
		m_MemDC.DrawRectangle(Rect, m_Printing ? LIGHT_GRAY : DARK_GRAY/*(m_HasFocus ? WHITE : DARK_GRAY)*/, NOCOLOR);
	}
}

//***************************************************************************************

void CMemWnd::Repaint()
{
	RepaintMemDC();

	InvalidateRgn(&m_Rgn, FALSE);

	UpdateWindow();
}


//***************************************************************************************

void CMemWnd::BackupMemDC()
{
	SAFE_DELETE_ARRAY(m_pBackup);

	int RGBSize = IMAGE_SIZE(m_w, m_h);

	try {
		m_pBackup = new uchar[RGBSize];
	}
	catch (...) {
		m_pBackup = NULL;
		return;
	}

	memcpy(m_pBackup, m_MemDC.m_RGBData, RGBSize);
}

//***************************************************************************************

void CMemWnd::RestoreMemDC()
{
	ASSERT(m_pBackup);
	if (m_pBackup == NULL) return;

	memcpy(m_MemDC.m_RGBData, m_pBackup, IMAGE_SIZE(m_w, m_h));
}

//***************************************************************************************

BOOL CMemWnd::SaveIntoMem(Matrix<uchar>& Mem, int CompressionType)
{
	CBasicImage Image;

	Image.m_w = m_MemDC.m_w;
	Image.m_h = m_MemDC.m_h;
	Image.m_RGBData.Create(m_h, LINE_SIZE(m_w), m_MemDC.m_RGBData);

	int Size;
	uchar* pData;

	if (Image.SaveIntoMem(&Size, &pData, CompressionType)) {

		Mem.Create(Size, 1, pData);

		free(pData);

		return TRUE;
	}

	return FALSE;
}

//***************************************************************************************

void CMemWnd::SaveIntoFile(CDicomInfo DicomInfo)
{
	m_Saving = TRUE;

	//add DICOM here
	CString FileName, sFileName;

	SYSTEMTIME CurTime;
	::GetLocalTime(&CurTime);
	CString Def_FileName;
	Def_FileName.Format(DicomInfo.m_FileName + "_%02i-%02i-%04i-%02i-%02i", CurTime.wMonth, CurTime.wDay, CurTime.wYear, CurTime.wHour, CurTime.wMinute);

	CINIParser IniPar;
	CString Dir = IniPar.GetStrKey(IniSection, "Screenshot Folder", IniPath);//6.2.0 ini registration

	switch (::NewSettings.m_ScreenShotFormat)
	{
	case 0://jpg
	{
		CFileDialog* pDlg = new CFileDialog(FALSE, _T("png"), LPCTSTR(Def_FileName), OFN_OVERWRITEPROMPT, _T("JPEG (*.jpg)|*.jpg|PNG (*.png)|*.png|Bitmap (*.bmp)|*.bmp|DICOM (*.dcm)|*.dcm||"), this);
		pDlg->m_ofn.lpstrInitialDir = Dir;
		BOOL Res = pDlg->DoModal() == IDOK;
		if (!Res) return;
		FileName = pDlg->GetPathName();
		sFileName = pDlg->GetFileName();
		delete pDlg;
	}
	break;
	case 1:// png
	{
		CFileDialog* pDlg = new CFileDialog(FALSE, _T("png"), LPCTSTR(Def_FileName), OFN_OVERWRITEPROMPT, _T("PNG (*.png)|*.png|JPEG (*.jpg)|*.jpg|Bitmap (*.bmp)|*.bmp|DICOM (*.dcm)|*.dcm||"), this);
		pDlg->m_ofn.lpstrInitialDir = Dir;
		BOOL Res = pDlg->DoModal() == IDOK;
		if (!Res) return;
		FileName = pDlg->GetPathName();
		sFileName = pDlg->GetFileName();
		delete pDlg;
	}
	break;
	case 2:// bmp
	{
		CFileDialog* pDlg = new CFileDialog(FALSE, _T("png"), Def_FileName, OFN_OVERWRITEPROMPT, _T("Bitmap (*.bmp)|*.bmp|PNG (*.png)|*.png|JPEG (*.jpg)|*.jpg|DICOM (*.dcm)|*.dcm||"), this);
		pDlg->m_ofn.lpstrInitialDir = Dir;
		BOOL Res = pDlg->DoModal() == IDOK;
		if (!Res) return;
		FileName = pDlg->GetPathName();
		sFileName = pDlg->GetFileName();
		delete pDlg;
	}
	break;
	case 3://dcm
	{
		CFileDialog* pDlg = new CFileDialog(FALSE, _T("png"), Def_FileName, OFN_OVERWRITEPROMPT, _T("DICOM (*.dcm)|*.dcm|PNG (*.png)|*.png|JPEG (*.jpg)|*.jpg|Bitmap (*.bmp)|*.bmp||"), this);
		pDlg->m_ofn.lpstrInitialDir = Dir;
		BOOL Res = pDlg->DoModal() == IDOK;
		if (!Res) return;
		FileName = pDlg->GetPathName();
		sFileName = pDlg->GetFileName();
		delete pDlg;
	}
	break;
	}

	//RepaintMemDC();//6.2.1

	Dir = FileName;
	Dir.Replace(sFileName, _T(""));

	IniPar.SetStrKey(IniSection, "Screenshot Folder", Dir, IniPath);//6.2.0 ini registration


	CString left = FileName.Right(3);
	if (left == "dcm")
	{
		::NewSettings.m_ScreenShotFormat = 3;
		SaveDicomFile(FileName, DicomInfo);//DICOM

		CString Note = "Dicom file has been saved as '" + FileName + "' sucessfully.";//6.2.0

		::Info(Note);
	}
	else
	{
		if (left == "jpg")      ::NewSettings.m_ScreenShotFormat = 0;
		else if (left == "png") ::NewSettings.m_ScreenShotFormat = 1;
		else if (left == "bmp") ::NewSettings.m_ScreenShotFormat = 2;

		CBasicImage Image;

		Image.m_w = m_MemDC.m_w;
		Image.m_h = m_MemDC.m_h;
		Image.m_RGBData.Create(m_h, LINE_SIZE(m_w), m_MemDC.m_RGBData);

		Image.SaveIntoFile(FileName);

		CString Note = "Image file has been saved as '" + FileName + "' sucessfully.";//6.2.0

		::Info(Note);//6.2.0
	}
}

//***************************************************************************************
//void CMemWnd::Print(PRINTDLG* pDlg)
//{
//  m_Printing = TRUE;
//  RepaintMemDC();
//
//  CWnd* pWnd = ::AfxGetApp()->GetMainWnd();
//  pDlg->hwndOwner = pWnd->m_hWnd;
//
//  DOCINFO DocInfo;
//  memset(&DocInfo, 0, sizeof(DOCINFO));
//  DocInfo.cbSize = sizeof(DOCINFO);
//  DocInfo.lpszDocName = _T("Tracey Exam");
//
//  ::StartDoc(pDlg->hDC, &DocInfo);
//
//  ::StartPage(pDlg->hDC);
//
//  ::SetMapMode(pDlg->hDC, MM_TEXT);
//  ::SetStretchBltMode(pDlg->hDC, COLORONCOLOR);
//  int w = ::GetDeviceCaps(pDlg->hDC, HORZRES);
//
//  int h = w * m_h / m_w;
//  
//  ::StretchBlt(pDlg->hDC, 0, 0, w, h, m_MemDC.m_hDC, 0, 0, m_w, m_h, SRCCOPY);
//
//  ::EndPage(pDlg->hDC);
//
//  ::EndDoc(pDlg->hDC);
//
//  m_Printing = FALSE;
//  RepaintMemDC();
//
//  Invalidate(FALSE);
//}

//***************************************************************************************
//6.2.1
void CMemWnd::SetPrint()
{
	m_Printing = TRUE;
}
//6.2.1

//***************************************************************************************

void CMemWnd::Print()
{
	m_Printing = TRUE;
	RepaintMemDC();

	CWnd* pWnd = ::AfxGetApp()->GetMainWnd();

	PRINTDLG* pDlg = new PRINTDLG;

	memset(pDlg, 0, sizeof(PRINTDLG));
	pDlg->lStructSize = sizeof(PRINTDLG);
	pDlg->Flags = PD_RETURNDEFAULT;
	pDlg->hwndOwner = pWnd->m_hWnd;

	if (::PrintDlg(pDlg))
	{
		DEVMODE* pDevMode = (DEVMODE*)::GlobalLock(pDlg->hDevMode);
		pDevMode->dmOrientation = DMORIENT_LANDSCAPE;

		::GlobalUnlock(pDlg->hDevMode);

		pDlg->Flags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION | PD_RETURNDC;

		if (::PrintDlg(pDlg))
		{
			DOCINFO DocInfo;
			memset(&DocInfo, 0, sizeof(DOCINFO));
			DocInfo.cbSize = sizeof(DOCINFO);
			DocInfo.lpszDocName = _T("Tracey Exam");

			::StartDoc(pDlg->hDC, &DocInfo);

			::StartPage(pDlg->hDC);

			::SetMapMode(pDlg->hDC, MM_TEXT);
			::SetStretchBltMode(pDlg->hDC, COLORONCOLOR);
			int w = ::GetDeviceCaps(pDlg->hDC, HORZRES);
			int h = w * m_h / m_w;
			::StretchBlt(pDlg->hDC, 0, 0, w, h, m_MemDC.m_hDC, 0, 0, m_w, m_h, SRCCOPY);

			::EndPage(pDlg->hDC);

			::EndDoc(pDlg->hDC);
		}

		::GlobalFree(pDlg->hDevNames);
		::GlobalFree(pDlg->hDevMode);
		::DeleteDC(pDlg->hDC);
	}

	delete pDlg;

	m_Printing = FALSE;
	RepaintMemDC();

	Invalidate(FALSE);
}

//***************************************************************************************
//Save the screen to the dicom format file 6.2.0
void CMemWnd::SaveDicomFile(CString fileName, CDicomInfo DicomInfo)
{
	CBasicImage Image;

	Image.m_w = m_w;
	Image.m_h = m_h;
	Image.m_RGBData.Create(m_h, LINE_SIZE(m_w), m_MemDC.m_RGBData);
	
	if (m_isComboDicom)
	{
		//for (int i = 0; i < 100; i++)
		{			
			CTraceyDicom TraceyDicom;

			CHAR* pData = new CHAR[(m_w + 200)*m_h * 3];
			memset(pData, 0, (m_w + 200)*m_h * 3);
			int R, G, B;
			int count = 0;
			for (int y = 0; y < m_h; y++)
			{
				for (int x = -150; x < (m_w + 50); x++)
				{
					if (x >= 0 && x < m_w)
					{
						R = Image.GetRAt(x, m_h - y - 1);
						G = Image.GetGAt(x, m_h - y - 1);
						B = Image.GetBAt(x, m_h - y - 1);
					}
					else
					{
						R = 0;
						G = 0;
						B = 0;
					}

					pData[count] = R;
					count++;
					pData[count] = G;
					count++;
					pData[count] = B;
					count++;
				}
			}

			TraceyDicom.OutputDicomImage(fileName, DicomInfo, pData, m_w + 200, m_h);

			delete[] pData;
		}		
	}
	else//DCMTK
	{
		CDcmtkDicom DcmtkDicom;

		BYTE* pData = new BYTE[(m_w + 200)*m_h * 3];
		memset(pData, 0, (m_w + 200)*m_h * 3);
		int R, G, B;
		int count = 0;
		for (int y = 0; y < m_h; y++)
		{
			for (int x = -150; x<(m_w + 50); x++)
			{
				if (x >= 0 && x < m_w)
				{
					R = Image.GetRAt(x, m_h - y - 1);
					G = Image.GetGAt(x, m_h - y - 1);
					B = Image.GetBAt(x, m_h - y - 1);
				}
				else
				{
					R = 0;
					G = 0;
					B = 0;
				}

				pData[count] = R;
				count++;
				pData[count] = G;
				count++;
				pData[count] = B;
				count++;
			}
		}

		DcmtkDicom.SaveDcmtkDicomFile(fileName, DicomInfo, pData, m_w + 200, m_h);
	}
}