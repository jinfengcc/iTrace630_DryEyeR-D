//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "CustomColorsDlg.h"
#include "NumDlg.h"
#include "MGdi.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CCustomColorsDlg, CDialog)

	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_COLOR_01, OnColor01ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_02, OnColor02ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_03, OnColor03ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_04, OnColor04ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_05, OnColor05ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_06, OnColor06ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_07, OnColor07ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_08, OnColor08ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_09, OnColor09ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_10, OnColor10ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_11, OnColor11ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_12, OnColor12ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_13, OnColor13ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_14, OnColor14ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_15, OnColor15ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_16, OnColor16ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_17, OnColor17ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_18, OnColor18ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_19, OnColor19ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_20, OnColor20ButtonClicked)
	ON_BN_CLICKED(IDC_COLOR_21, OnColor21ButtonClicked)

	ON_BN_CLICKED(IDC_NEW_PALETTE, OnNewButtonClicked)
	ON_BN_CLICKED(IDC_LOAD_PALETTE, OnOpenButtonClicked)
	ON_BN_CLICKED(IDC_SAVE_PALETTE, OnSaveButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CCustomColorsDlg::CCustomColorsDlg(CScale* pExtScale, CWnd* pParentWnd) :
	CDialog(IDD_CUSTOM_COLORS_DLG, pParentWnd)
{
	m_pExtScale = pExtScale;
	m_Scale = *pExtScale;
}

//***************************************************************************************

BOOL CCustomColorsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RECT Rect1;
	GetClientRect(&Rect1);
	int h1 = Rect1.bottom - Rect1.top;

	RECT Rect2;
	GetWindowRect(&Rect2);
	int h2 = Rect2.bottom - Rect2.top;
	int w2 = Rect2.right - Rect2.left;

	h2 = (h2 - h1) + 10 + m_Scale.m_NumColors * 30 + 10;

	SetWindowPos(NULL, 0, 0, w2, h2, SWP_NOZORDER | SWP_NOMOVE);

	for (int i = 0; i < m_Scale.m_NumColors; i++) {
		int j = m_Scale.m_NumColors - 1 - i;
		m_ColorButtons[i].SetWindowPos(NULL, 130, 10 + j * 30, 70, 25, SWP_NOZORDER); // SWP_NOZORDER - здесь существенно - иначе меняется TAB order
		m_ColorButtons[i].ShowWindow(SW_SHOW);
	}
	for (int i = m_Scale.m_NumColors; i < 21; i++) {
		m_ColorButtons[i].ShowWindow(SW_HIDE);
	}

	return TRUE;
}

//***************************************************************************************

void CCustomColorsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	for (int i = 0; i < 21; i++) {
		DDX_Control(pDX, IDC_COLOR_01 + i, m_ColorButtons[i]);
	}
}

//***************************************************************************************

void CCustomColorsDlg::OnOK()
{
	*m_pExtScale = m_Scale;

	CDialog::OnOK();
}

//***************************************************************************************

void CCustomColorsDlg::OnPaint()
{
	CPaintDC PaintDC(this);

	for (int i = 0; i < m_Scale.m_NumColors; i++) {
		int j = m_Scale.m_NumColors - 1 - i;
		RECT Rect = { 10, 10 + j * 30, 120, 10 + j * 30 + 25 };
		PaintDC.FillSolidRect(&Rect, m_Scale.m_Colors[i]);
	}
}

//***************************************************************************************

void CCustomColorsDlg::OnColor(const int i)
{
	CColorDialog* pDlg = new CColorDialog;
	//pDlg->m_cc.Flags = CC_RGBINIT | CC_FULLOPEN;
	//pDlg->m_cc.rgbResult = m_Scale.m_Colors[i - 1];
	if (pDlg->DoModal() == IDOK) {
		m_Scale.m_Colors[i - 1] = pDlg->GetColor();
		Invalidate(FALSE);
	}
	delete pDlg;
}

//***************************************************************************************

void CCustomColorsDlg::OnNewButtonClicked()
{
	real_t t;
	if (!EnterNumber(t, this, "Number of colors")) return;

	int NumColors = (int)t;
	if (NumColors < 11 || NumColors > 21) {
		::Error("Number of colors must be between 11 and 21");
		return;
	}

	m_Scale.m_NumColors = NumColors;
	for (int i = 0; i < NumColors; i++) {
		m_Scale.m_Colors[i] = 0x00ffffff;
	}

	int h = 45 + NumColors * 25;
	SetWindowPos(NULL, 0, 0, 270, h, SWP_NOZORDER | SWP_NOMOVE);
	for (int i = 0; i < NumColors; i++) {
		int j = NumColors - 1 - i;
		m_ColorButtons[i].SetWindowPos(NULL, 100, 10 + j * 25, 50, 23, SWP_NOZORDER);
		m_ColorButtons[i].ShowWindow(SW_SHOW);
	}
	for (int i = NumColors; i < 21; i++) {
		m_ColorButtons[i].ShowWindow(SW_HIDE);
	}

	CenterWindow();

	Invalidate(TRUE);
}

//***************************************************************************************

void CCustomColorsDlg::OnOpenButtonClicked()
{
	CString FolderName(MAIN_DIR + "\\Palettes");
	::CreateDirectory(FolderName, NULL);

	CFileDialog* pDlg = new CFileDialog(TRUE, _T("pal"), (LPCTSTR)"", OFN_HIDEREADONLY, _T("Palette files (*.pal)|*.pal||"), this);
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK) {
		m_Scale.LoadColors(pDlg->GetPathName());
		int NumColors = m_Scale.m_NumColors;
		int h = 45 + NumColors * 25;
		SetWindowPos(NULL, 0, 0, 270, h, SWP_NOZORDER | SWP_NOMOVE);
		for (int i = 0; i < NumColors; i++) {
			int j = NumColors - 1 - i;
			m_ColorButtons[i].SetWindowPos(NULL, 100, 10 + j * 25, 50, 23, SWP_NOZORDER);
			m_ColorButtons[i].ShowWindow(SW_SHOW);
		}
		for (int i = NumColors; i < 21; i++) {
			m_ColorButtons[i].ShowWindow(SW_HIDE);
		}
		CenterWindow();
		Invalidate(TRUE);
	}
	delete pDlg;
}

//***************************************************************************************

void CCustomColorsDlg::OnSaveButtonClicked()
{
	CString FolderName(MAIN_DIR + "\\Palettes");
	::CreateDirectory(FolderName, NULL);

	CFileDialog* pDlg = new CFileDialog(FALSE, _T("pal"), NULL, OFN_OVERWRITEPROMPT, _T("Palette files (*.pal)|*.pal||"), this);
	pDlg->m_ofn.lpstrInitialDir = FolderName;
	if (pDlg->DoModal() == IDOK)
	{
		m_Scale.SaveColors(pDlg->GetPathName());
	}
	delete pDlg;
}

//***************************************************************************************
