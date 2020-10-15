//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "RingsToolDlg.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CRingsToolDlg, CDialog)

	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CAL_EXAM1_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_EXAM2_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_EXAM3_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_EXAM4_RADIO, OnRadioClicked)
	ON_BN_CLICKED(IDC_CAL_START_EXAM, OnStartExamButtonClicked)
	ON_BN_CLICKED(IDC_CAL_ACQUIRE_EXAM, OnAcquireExamButtonClicked)
	ON_BN_CLICKED(IDC_CAL_ABORT_EXAM, OnAbortExamButtonClicked)
	ON_MESSAGE(WM_THREAD_UPDATE, OnUpdate)
	ON_MESSAGE(WM_THREAD_FINISH, OnFinish)

	//
	ON_BN_CLICKED(IDC_CAL_BALL1_RADIO, OnBallRadioClicked)
	ON_BN_CLICKED(IDC_CAL_BALL2_RADIO, OnBallRadioClicked)
	ON_BN_CLICKED(IDC_CAL_BALL3_RADIO, OnBallRadioClicked)
	ON_BN_CLICKED(IDC_CAL_BALL4_RADIO, OnBallRadioClicked)

	ON_BN_CLICKED(IDC_CAL_SAVE2, OnCalSaveBtnClicked)
	//

END_MESSAGE_MAP()

//***************************************************************************************

CRingsToolDlg::CRingsToolDlg(CCTAcquisition* pCTAcquisition, int b, CWnd* pParentWnd) :
	CDialog(IDD_RINGS_TOOL_DLG, pParentWnd)
{
	m_pCTAcquisition = pCTAcquisition;
	m_pCTAcquisition->m_pVideoWnd = this;
	m_b = b;
}

//***************************************************************************************

void CRingsToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_CAL_EXAM1_RADIO, m_Exam1Radio);
	DDX_Control(pDX, IDC_CAL_EXAM2_RADIO, m_Exam2Radio);
	DDX_Control(pDX, IDC_CAL_EXAM3_RADIO, m_Exam3Radio);
	DDX_Control(pDX, IDC_CAL_EXAM4_RADIO, m_Exam4Radio);
	DDX_Control(pDX, IDC_CAL_START_EXAM, m_StartExamButton);
	DDX_Control(pDX, IDC_CAL_ACQUIRE_EXAM, m_AcquireExamButton);
	DDX_Control(pDX, IDC_CAL_ABORT_EXAM, m_AbortExamButton);

	//
	DDX_Control(pDX, IDC_CAL_BALL1_RADIO, m_Ball1Radio);
	DDX_Control(pDX, IDC_CAL_BALL2_RADIO, m_Ball2Radio);
	DDX_Control(pDX, IDC_CAL_BALL3_RADIO, m_Ball3Radio);
	DDX_Control(pDX, IDC_CAL_BALL4_RADIO, m_Ball4Radio);
	DDX_Control(pDX, IDC_CAL_BALL1_EDIT, m_Ball1Edit);
	DDX_Control(pDX, IDC_CAL_BALL2_EDIT, m_Ball2Edit);
	DDX_Control(pDX, IDC_CAL_BALL3_EDIT, m_Ball3Edit);
	DDX_Control(pDX, IDC_CAL_BALL4_EDIT, m_Ball4Edit);

	DDX_Control(pDX, IDC_CAL_SAVE2, m_CalSaveButton);
	//
}

//***************************************************************************************

BOOL CRingsToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pCTAcquisition->m_TriLaserShutoffOn = FALSE;

	CALIBRATION* pCalibration = &m_pCTAcquisition->m_pHW->m_Calibration;
	VIDEO_SETTINGS* pVideoSettings = &pCalibration->CTVideoSettings;

	m_BackupVideoBrightness = pVideoSettings->Brightness;
	m_BackupVideoContrast = pVideoSettings->Contrast;

	m_Exam1Radio.SetCheck(1);

	m_w = CHW::m_VideoWidth;
	m_h = CHW::m_VideoHeight;
	m_w_um = pCalibration->VideoWidthMicrons;
	m_h_um = pCalibration->VideoHeightMicrons;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h - 1;

	RECT Rect = { 10, 10, 10 + m_w, 10 + m_h };
	m_Video1Wnd.CreateWnd(Rect, this);

	::SetRect(&Rect, 10 + m_w + 10, 10 + m_h - 324, 10 + m_w + 10 + 324, 10 + m_h);
	m_Video2Wnd.CreateWnd(Rect, this);

	// Set Ball selection
	if (m_b == 0)
		m_Ball1Radio.SetCheck(1);
	else  if (m_b == 1)
		m_Ball2Radio.SetCheck(1);
	else  if (m_b == 2)
		m_Ball3Radio.SetCheck(1);
	else  if (m_b == 3)
		m_Ball4Radio.SetCheck(1);

	CString s;
	// Ball 1 radius
	s.Format(_T("%.0f"), m_pCTAcquisition->m_pHW->m_Calibration.BallsAx[0]);
	m_Ball1Edit.SetWindowText(s);
	// Ball 2 radius
	s.Format(_T("%.0f"), m_pCTAcquisition->m_pHW->m_Calibration.BallsAx[1]);
	m_Ball2Edit.SetWindowText(s);
	// Ball 3 radius
	s.Format(_T("%.0f"), m_pCTAcquisition->m_pHW->m_Calibration.BallsAx[2]);
	m_Ball3Edit.SetWindowText(s);
	// Ball 4 radius
	s.Format(_T("%.0f"), m_pCTAcquisition->m_pHW->m_Calibration.BallsAx[3]);
	m_Ball4Edit.SetWindowText(s);
	//

	OnRadioClicked();

	m_Start = FALSE;//[5.1.1]

	return TRUE;
}

//***************************************************************************************

void CRingsToolDlg::OnCancel()
{
	if (!m_CancelButton.IsWindowEnabled()) return;

	VIDEO_SETTINGS* pVideoSettings = &m_pCTAcquisition->m_pHW->m_Calibration.CTVideoSettings;
	pVideoSettings->Brightness = m_BackupVideoBrightness;
	pVideoSettings->Contrast = m_BackupVideoContrast;

	CDialog::OnCancel();
}

//***************************************************************************************

void CRingsToolDlg::OnOK()
{
	for (int i = 0; i < 4; i++) {
		if (!m_CTExam[i].m_Image.IsRingsDataComplete()) {
			::Warning("Rings data is incomplete in at least one exam.");
			return;
		}
	}

	CALIBRATION* pCalibration = &m_pCTAcquisition->m_pHW->m_Calibration;

	for (int r = 0; r < pCalibration->NumRings; r++) {
		for (int a = 0; a < 360; a++) {
			pCalibration->BallsRi[m_b][r][a] = 0.0;
			for (int e = 0; e < 4; e++) {
				pCalibration->BallsRi[m_b][r][a] += m_CTExam[e].m_Image.m_ri_r_um[r][a];
			}
			pCalibration->BallsRi[m_b][r][a] /= 4.0;
		}
	}

	for (int r = pCalibration->NumRings; r < 26; r++) {
		for (int a = 0; a < 360; a++) {
			pCalibration->BallsRi[m_b][r][a] = INVALID_VALUE;
		}
	}

	real_t D = 0.0;
	for (int r = 0; r < pCalibration->NumRings; r++) {
		for (int a = 0; a < 360; a++) {
			for (int e = 0; e < 4; e++) {
				D += sqr(m_CTExam[e].m_Image.m_ri_r_um[r][a] - pCalibration->BallsRi[m_b][r][a]);
			}
		}
	}
	D = sqrt(D) / (pCalibration->NumRings * 360.0 * 4.0);

	CString s;
	s.Format(_T("Standard deviation = %.3f µ"), D);
	::Info(s);

	CDialog::OnOK();
}

//***************************************************************************************

void CRingsToolDlg::OnHScroll(uint nSBCode, uint nPos, CScrollBar* pScrollBar)
{
	if (!m_Start) return;//[5.1.1]
	VIDEO_SETTINGS* pVideoSettings = &m_pCTAcquisition->m_pHW->m_Calibration.CTVideoSettings;
	CString s;
	switch (pScrollBar->GetDlgCtrlID()) {
	case IDC_CAL_VIDEO_BRIGHTNESS_SLIDER:
		pVideoSettings->Brightness = m_VideoBrightnessSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->Brightness);
		m_VideoBrightnessEdit.SetWindowText(s);
		m_pCTAcquisition->ChangeBrightness();
		break;
	case IDC_CAL_VIDEO_CONTRAST_SLIDER:
		pVideoSettings->Contrast = m_VideoContrastSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->Contrast);
		m_VideoContrastEdit.SetWindowText(s);
		m_pCTAcquisition->ChangeContrast();
		break;
	}
}

//***************************************************************************************

int CRingsToolDlg::GetSelectedRadio()
{
	if (m_Exam1Radio.GetCheck()) return 0;
	if (m_Exam2Radio.GetCheck()) return 1;
	if (m_Exam3Radio.GetCheck()) return 2;
	if (m_Exam4Radio.GetCheck()) return 3;
	return 0;
}

//***************************************************************************************

void CRingsToolDlg::OnRadioClicked()
{
	m_Video1Wnd.ClearMemDC();

	int e = GetSelectedRadio();

	CEyeImage* pImage = &m_CTExam[e].m_Image;

	uchar* pRGBData = pImage->m_RGBData.GetMem();
	if (pRGBData) {
		memcpy(m_Video1Wnd.m_MemDC.m_RGBData, pRGBData, CHW::m_VideoSize);
	}
	else {
		memset(m_Video1Wnd.m_MemDC.m_RGBData, 0, CHW::m_VideoSize);
	}

	int m = 1000 * ((int)(0.001 * 0.5 * __max(m_w_um, m_h_um)) + 1);
	for (int i = -m; i <= m; i += 1000) {
		int y = intRound(m_cy - i * m_y_px_um);
		m_Video1Wnd.m_MemDC.DrawHorzDottedLine(0, m_w - 1, y, 0x004f4f4f, NOCOLOR);
		int x = intRound(m_cx + i * m_x_px_um);
		m_Video1Wnd.m_MemDC.DrawVertDottedLine(x, 0, m_h - 1, 0x004f4f4f, NOCOLOR);
	}

	if (pImage->m_ve_ok) {
		int y0 = intRound(m_cy - pImage->m_ve_y_um * m_y_px_um);
		int x0 = intRound(m_cx + pImage->m_ve_x_um * m_x_px_um);
		m_Video1Wnd.m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, RED);
		m_Video1Wnd.m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, RED);
		for (int a = 0; a < 360; a++) {
			int b = a == 359 ? 0 : a + 1;
			real_t ra_um = pImage->m_ri_r_um[0][a]; if (ra_um == INVALID_VALUE) continue;
			real_t rb_um = pImage->m_ri_r_um[0][b]; if (rb_um == INVALID_VALUE) continue;
			int ya = intRound(m_cy - (pImage->m_ve_y_um + ra_um * SIN[a]) * m_y_px_um);
			int xa = intRound(m_cx + (pImage->m_ve_x_um + ra_um * COS[a]) * m_x_px_um);
			int yb = intRound(m_cy - (pImage->m_ve_y_um + rb_um * SIN[b]) * m_y_px_um);
			int xb = intRound(m_cx + (pImage->m_ve_x_um + rb_um * COS[b]) * m_x_px_um);
			m_Video1Wnd.m_MemDC.DrawLine(xa, ya, xb, yb, 1, RED);
		}
	}
	if (pImage->m_ri_ok) {
		for (int r = 0; r < pImage->m_NumRings; r++) {
			for (int a = 0; a < 360; a++) {
				int b = a == 359 ? 0 : a + 1;
				real_t ra_um = pImage->m_ri_r_um[r][a]; if (ra_um == INVALID_VALUE) continue;
				real_t rb_um = pImage->m_ri_r_um[r][b]; if (rb_um == INVALID_VALUE) continue;
				int ya = intRound(m_cy - (pImage->m_ve_y_um + ra_um * SIN[a]) * m_y_px_um);
				int xa = intRound(m_cx + (pImage->m_ve_x_um + ra_um * COS[a]) * m_x_px_um);
				int yb = intRound(m_cy - (pImage->m_ve_y_um + rb_um * SIN[b]) * m_y_px_um);
				int xb = intRound(m_cx + (pImage->m_ve_x_um + rb_um * COS[b]) * m_x_px_um);
				m_Video1Wnd.m_MemDC.DrawLine(xa, ya, xb, yb, 1, r % 2 ? YELLOW : RED);
			}
		}

	}

	if (!pImage->IsRingsDataComplete()) {
		RECT Rect = { 0, 5, m_Video1Wnd.m_w, 25 };
		CMFont Font(20, 600, "Arial");
		m_Video1Wnd.m_MemDC.WriteText("rings data is incomplete", Rect, Font, RED, 1);
	}

	m_Video1Wnd.Invalidate(FALSE);
}
//***************************************************************************************

//
void CRingsToolDlg::OnBallRadioClicked()
{
	if (m_Ball1Radio.GetCheck()) m_b = 0;
	else if (m_Ball2Radio.GetCheck()) m_b = 1;
	else if (m_Ball3Radio.GetCheck()) m_b = 2;
	else if (m_Ball4Radio.GetCheck()) m_b = 3;
}

//***************************************************************************************

// Save the cal values to Exporm
void CRingsToolDlg::OnCalSaveBtnClicked()
{
	CBusyCursor Cursor;

	CString s;

	// Ball 1 radius
	m_Ball1Edit.GetWindowText(s);
	G_As = s;
	m_pCTAcquisition->m_pHW->m_Calibration.BallsAx[0] = atof(G_As);

	// Ball 2 radius
	m_Ball2Edit.GetWindowText(s);
	G_As = s;
	m_pCTAcquisition->m_pHW->m_Calibration.BallsAx[1] = atof(G_As);

	// Ball 3 radius
	m_Ball3Edit.GetWindowText(s);
	G_As = s;
	m_pCTAcquisition->m_pHW->m_Calibration.BallsAx[2] = atof(G_As);

	// Ball 4 radius
	m_Ball4Edit.GetWindowText(s);
	G_As = s;
	m_pCTAcquisition->m_pHW->m_Calibration.BallsAx[3] = atof(G_As);

	m_pCTAcquisition->m_pHW->SaveCalibrationIntoFlash();
}
//***************************************************************************************

void CRingsToolDlg::OnStartExamButtonClicked()
{
	if (!m_pCTAcquisition->Start()) return;

	m_Start = TRUE;

	m_OKButton.EnableWindow(FALSE);
	m_CancelButton.EnableWindow(FALSE);
	m_Exam1Radio.EnableWindow(FALSE);
	m_Exam2Radio.EnableWindow(FALSE);
	m_Exam3Radio.EnableWindow(FALSE);
	m_Exam4Radio.EnableWindow(FALSE);
	m_StartExamButton.EnableWindow(FALSE);
	m_AcquireExamButton.EnableWindow(TRUE);
	m_AbortExamButton.EnableWindow(TRUE);
}

//***************************************************************************************

void CRingsToolDlg::OnAcquireExamButtonClicked()
{
	m_pCTAcquisition->m_CTAutoMode = FALSE;//From m_AutoMode to m_CTAutoMode
	m_pCTAcquisition->Acquire();
}

//***************************************************************************************

void CRingsToolDlg::OnAbortExamButtonClicked()
{
	m_pCTAcquisition->Abort();
	m_Start = FALSE;
}

//***************************************************************************************

LRESULT CRingsToolDlg::OnFinish(WPARAM wParam, LPARAM lParam)
{
	CBusyCursor Cursor;

	m_pCTAcquisition->Finish();

	m_OKButton.EnableWindow(TRUE);
	m_CancelButton.EnableWindow(TRUE);
	m_AcquireExamButton.EnableWindow(FALSE);
	m_AbortExamButton.EnableWindow(FALSE);
	m_Exam1Radio.EnableWindow(TRUE);
	m_Exam2Radio.EnableWindow(TRUE);
	m_Exam3Radio.EnableWindow(TRUE);
	m_Exam4Radio.EnableWindow(TRUE);
	m_StartExamButton.EnableWindow(TRUE);

	int e = GetSelectedRadio();

	m_CTExam[e].m_Image.Destroy();

	if (wParam == 1)
	{
		m_CTExam[e] = m_pCTAcquisition->m_CTExam;

		m_CTExam[e].m_Image.FindVertexAndRings();
	}

	OnRadioClicked();

	return 0;
}

//***************************************************************************************

LRESULT CRingsToolDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	memcpy(m_Video1Wnd.m_MemDC.m_RGBData, m_pCTAcquisition->m_pHW->GetRGBData(), CHW::m_VideoSize);

	CEyeImage* pImage = &m_pCTAcquisition->m_CTExam.m_Image;

	CString s;
	RECT Rect;
	int fs = 16;
	CMFont Font(fs, 400, "Arial");

	int icx = intRound(m_cx);
	int icy = intRound(m_cy);

	int X = pImage->m_table.GetNumCols();
	int Y = pImage->m_table.GetNumRows();
	int Y2 = Y >> 1;
	int X2 = X >> 1;

	COLORREF green = m_pCTAcquisition->m_ve0_ok ? 0x0000ff00 : 0x00007f00;
	COLORREF red = m_pCTAcquisition->m_la_ok ? 0x000000ff : 0x0000009f;

	// оболочка
	CNode* pNode = pImage->m_hull.MoveFirst();
	while (pNode)
	{
		m_Video1Wnd.m_MemDC.DrawCircle(icx + (pNode->m_x - X2), icy - (pNode->m_y - Y2), 1, 1, green, green);
		pNode = pImage->m_hull.MoveNext();
	}

	m_Video1Wnd.m_MemDC.DrawLine(icx - X2, icy - Y2, icx + X2 + 1, icy - Y2, 1, YELLOW);
	m_Video1Wnd.m_MemDC.DrawLine(icx - X2, icy, icx + X2 + 1, icy, 1, YELLOW);
	m_Video1Wnd.m_MemDC.DrawLine(icx - X2, icy + Y2, icx + X2 + 1, icy + Y2, 1, YELLOW);
	m_Video1Wnd.m_MemDC.DrawLine(icx - X2, icy - Y2, icx - X2, icy + Y2 + 1, 1, YELLOW);
	m_Video1Wnd.m_MemDC.DrawLine(icx, icy - Y2, icx, icy + Y2 + 1, 1, YELLOW);
	m_Video1Wnd.m_MemDC.DrawLine(icx + X2, icy - Y2, icx + X2, icy + Y2 + 1, 1, YELLOW);

	if (pImage->m_ve0_ok)
	{
		m_Video1Wnd.m_MemDC.DrawLine(icx, icy, icx + pImage->m_ve0_x, icy - pImage->m_ve0_y, 3, green);
	}

	if (pImage->m_la_ok)
	{
		m_Video1Wnd.m_MemDC.DrawEllipse(icx + pImage->m_la_x, icy, 10, 5, 1, red, red);
	}

	{
		real_t hmax = -1000000000.0;

		for (int i = 0; i < 256; i++)
		{
			if (hmax < pImage->m_hist[i]) hmax = pImage->m_hist[i];
		}

		if (hmax != 0)
		{
			for (int i = 0; i < 255; i++)
			{
				int h1 = intRound(400.0 * pImage->m_hist[i] / hmax);
				int h2 = intRound(400.0 * pImage->m_hist[i + 1] / hmax);
				m_Video1Wnd.m_MemDC.DrawLine(i, m_h - 10 - h1, i + 1, m_h - 10 - h2, 1, RED);
			}
			m_Video1Wnd.m_MemDC.DrawLine(pImage->m_ve0_thr, m_h - 10, pImage->m_ve0_thr, m_h - 410, 1, YELLOW);
		}
	}

	::SetRect(&Rect, m_w - 100, m_h - 4 * fs, m_w - 5, m_h - 3 * fs);
	s.Format(_T("%i"), m_pCTAcquisition->m_TriangulationSensorReading);
	m_Video1Wnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	::SetRect(&Rect, m_w - 100, m_h - 3 * fs, m_w - 5, m_h - 2 * fs);
	s.Format(_T("%i"), pImage->m_la_v);
	m_Video1Wnd.m_MemDC.WriteText(s, Rect, Font, pImage->m_la_v >= m_pCTAcquisition->m_pHW->m_Calibration.LaserIntensityThreshold ? GREEN : RED, 2);

	::SetRect(&Rect, m_w - 100, m_h - 2 * fs, m_w - 5, m_h - fs);
	s.Format(_T("%i"), m_pCTAcquisition->m_NumFramesReceived);
	m_Video1Wnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	::SetRect(&Rect, m_w - 100, m_h - fs, m_w - 5, m_h);
	s.Format(_T("%.1f ms"), m_pCTAcquisition->m_AverageCycleTime);
	m_Video1Wnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	m_Video1Wnd.Invalidate(FALSE);
	m_Video1Wnd.UpdateWindow();

	m_Video2Wnd.m_MemDC.StretchBlt(0, 0, X << 2, Y << 2, &m_Video1Wnd.m_MemDC, icx - X2, icy - Y2, X, Y, SRCCOPY);
	m_Video2Wnd.Invalidate(FALSE);
	m_Video2Wnd.UpdateWindow();

	return 0;
}

//***************************************************************************************
