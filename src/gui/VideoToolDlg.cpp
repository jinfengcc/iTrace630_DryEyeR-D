//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "VideoToolDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CVideoToolDlg, CDialog)

	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CAL_START_VIDEO, OnStartVideoButtonClicked)
	ON_BN_CLICKED(IDC_CAL_ABORT_VIDEO, OnAbortVideoButtonClicked)
	ON_MESSAGE(WM_THREAD_UPDATE, OnUpdate)
	ON_MESSAGE(WM_THREAD_FINISH, OnFinish)

END_MESSAGE_MAP()

//***************************************************************************************

CVideoToolDlg::CVideoToolDlg(CHW* pHW, CWnd* pParentWnd) :
	CDialog(IDD_VIDEO_TOOL_DLG, pParentWnd)
{
	m_pHW = pHW;
	m_pHW->m_pCurrentVideoSettings = &m_pHW->m_Calibration.WFVideoSettings;
	m_pLiveVideo = new CLiveVideo(m_pHW);
	m_pLiveVideo->m_pVideoWnd = this;
}

//***************************************************************************************

CVideoToolDlg::~CVideoToolDlg()
{
	delete m_pLiveVideo;
}

//***************************************************************************************

void CVideoToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_CAL_START_VIDEO, m_StartVideoButton);
	DDX_Control(pDX, IDC_CAL_ABORT_VIDEO, m_AbortVideoButton);
	DDX_Control(pDX, IDC_CAL_VIDEO_INFRARED_LEDS_POWER_LEVEL_EDIT, m_InfraredLEDsPowerLevelEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_BRIGHTNESS_EDIT, m_BrightnessEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_CONTRAST_EDIT, m_ContrastEdit);
	DDX_Control(pDX, IDC_CAL_VIDEO_INFRARED_LEDS_POWER_LEVEL_SLIDER, m_InfraredLEDsPowerLevelSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_BRIGHTNESS_SLIDER, m_BrightnessSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_CONTRAST_SLIDER, m_ContrastSlider);
	DDX_Control(pDX, IDC_CAL_VIDEO_PUPIL_CHECK, m_PupilCheck);
	DDX_Control(pDX, IDC_CAL_VIDEO_TARGET_CHECK, m_TargetCheck);//[cjf***03282012]
}

//***************************************************************************************

BOOL CVideoToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_w = CHW::m_VideoWidth;
	m_h = CHW::m_VideoHeight;
	m_w_um = m_pHW->m_Calibration.VideoWidthMicrons;
	m_h_um = m_pHW->m_Calibration.VideoHeightMicrons;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h - 1;

	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	m_BackupInfraredLEDsPowerLevel = pVideoSettings->InfraredLEDsPowerLevel;
	m_BackupBrightness = pVideoSettings->Brightness;
	m_BackupContrast = pVideoSettings->Contrast;

	m_InfraredLEDsPowerLevelSlider.SetRange(0, 100, FALSE);
	m_BrightnessSlider.SetRange(0, 255, FALSE);
	m_ContrastSlider.SetRange(0, 255, FALSE);

	m_InfraredLEDsPowerLevelSlider.SetPos(pVideoSettings->InfraredLEDsPowerLevel);
	m_BrightnessSlider.SetPos(pVideoSettings->Brightness);
	m_ContrastSlider.SetPos(pVideoSettings->Contrast);

	CString s;

	s.Format(_T("%i"), pVideoSettings->InfraredLEDsPowerLevel);
	m_InfraredLEDsPowerLevelEdit.SetWindowText(s);

	s.Format(_T("%i"), pVideoSettings->Brightness);
	m_BrightnessEdit.SetWindowText(s);

	s.Format(_T("%i"), pVideoSettings->Contrast);
	m_ContrastEdit.SetWindowText(s);

	RECT Rect = { 10, 10, 10 + CHW::m_VideoWidth, 10 + CHW::m_VideoHeight };
	m_VideoWnd.CreateWnd(Rect, this);

	return TRUE;
}

//***************************************************************************************

void CVideoToolDlg::OnCancel()
{
	if (!m_CancelButton.IsWindowEnabled()) return;

	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;

	pVideoSettings->InfraredLEDsPowerLevel = m_BackupInfraredLEDsPowerLevel;
	pVideoSettings->Brightness = m_BackupBrightness;
	pVideoSettings->Contrast = m_BackupContrast;

	CDialog::OnCancel();
}

//***************************************************************************************

void CVideoToolDlg::OnOK()
{
	CDialog::OnOK();
}

//***************************************************************************************

void CVideoToolDlg::OnHScroll(uint nSBCode, uint nPos, CScrollBar* pScrollBar)
{
	VIDEO_SETTINGS* pVideoSettings = m_pHW->m_pCurrentVideoSettings;
	CString s;

	switch (pScrollBar->GetDlgCtrlID()) {
	case IDC_CAL_VIDEO_INFRARED_LEDS_POWER_LEVEL_SLIDER:
		pVideoSettings->InfraredLEDsPowerLevel = m_InfraredLEDsPowerLevelSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->InfraredLEDsPowerLevel);
		m_InfraredLEDsPowerLevelEdit.SetWindowText(s);
		m_pLiveVideo->ChangeInfraredLEDsPowerLevel();
		break;
	case IDC_CAL_VIDEO_BRIGHTNESS_SLIDER:
		pVideoSettings->Brightness = m_BrightnessSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->Brightness);
		m_BrightnessEdit.SetWindowText(s);
		m_pLiveVideo->ChangeBrightness();
		break;
	case IDC_CAL_VIDEO_CONTRAST_SLIDER:
		pVideoSettings->Contrast = m_ContrastSlider.GetPos();
		s.Format(_T("%i"), pVideoSettings->Contrast);
		m_ContrastEdit.SetWindowText(s);
		m_pLiveVideo->ChangeContrast();
		break;
	}
}

//***************************************************************************************

void CVideoToolDlg::OnStartVideoButtonClicked()
{
	if (!m_pLiveVideo->Start()) return;

	m_OKButton.EnableWindow(FALSE);
	m_CancelButton.EnableWindow(FALSE);
	m_StartVideoButton.EnableWindow(FALSE);
	m_AbortVideoButton.EnableWindow(TRUE);
}

//***************************************************************************************

void CVideoToolDlg::OnAbortVideoButtonClicked()
{
	m_pLiveVideo->Abort();
}

//***************************************************************************************

LRESULT CVideoToolDlg::OnFinish(WPARAM wParam, LPARAM lParam)
{
	m_pLiveVideo->Finish();

	m_OKButton.EnableWindow(TRUE);
	m_CancelButton.EnableWindow(TRUE);
	m_StartVideoButton.EnableWindow(TRUE);
	m_AbortVideoButton.EnableWindow(FALSE);

	return 0;
}

//***************************************************************************************

LRESULT CVideoToolDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	CString s;
	RECT Rect;
	int fs = 16;
	CMFont Font(fs, 600, "Arial");

	uchar* pRGBData = m_pHW->GetRGBData();
	memcpy(m_VideoWnd.m_MemDC.m_RGBData, pRGBData, CHW::m_VideoSize);

	if (m_PupilCheck.GetCheck()) {
		m_Image.m_w = CHW::m_VideoWidth;
		m_Image.m_h = CHW::m_VideoHeight;
		m_Image.m_w_um = m_pHW->m_Calibration.VideoWidthMicrons;
		m_Image.m_h_um = m_pHW->m_Calibration.VideoHeightMicrons;
		m_Image.m_RGBData.Attach(CHW::m_VideoHeight, CHW::m_VideoLineWidth, pRGBData);
		m_Image.FindPupil();
		if (m_Image.m_pu_ok) {
			CPen Pen(PS_SOLID, 1, GREEN);
			CPen* pPen = m_VideoWnd.m_MemDC.SelectObject(&Pen);
			int x = intRound(m_cx + (m_Image.m_pu_x0_um + m_Image.m_pu_r_um[0] * COS[0]) * m_x_px_um);
			int y = intRound(m_cy - (m_Image.m_pu_y0_um + m_Image.m_pu_r_um[0] * SIN[0]) * m_y_px_um);
			m_VideoWnd.m_MemDC.MoveTo(x, y);
			for (int i = 1; i < 360; i++) {
				x = intRound(m_cx + (m_Image.m_pu_x0_um + m_Image.m_pu_r_um[i] * COS[i]) * m_x_px_um);
				y = intRound(m_cy - (m_Image.m_pu_y0_um + m_Image.m_pu_r_um[i] * SIN[i]) * m_y_px_um);
				m_VideoWnd.m_MemDC.LineTo(x, y);
			}
			x = intRound(m_cx + (m_Image.m_pu_x0_um + m_Image.m_pu_r_um[0] * COS[0]) * m_x_px_um);
			y = intRound(m_cy - (m_Image.m_pu_y0_um + m_Image.m_pu_r_um[0] * SIN[0]) * m_y_px_um);
			m_VideoWnd.m_MemDC.LineTo(x, y);
			m_VideoWnd.m_MemDC.SelectObject(pPen);
			x = intRound(m_cx + m_Image.m_pu_x0_um * m_x_px_um);
			y = intRound(m_cy - m_Image.m_pu_y0_um * m_y_px_um);
			m_VideoWnd.m_MemDC.DrawLine(intRound(m_cx), intRound(m_cy), x, y, 1, GREEN);
		}
	}

	::SetRect(&Rect, 0, m_h - fs - fs, m_w - 5, m_h - fs);
	s.Format(_T("%u"), m_pLiveVideo->m_NumFramesReceived);
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	::SetRect(&Rect, 0, m_h - fs, m_w - 5, m_h);
	s.Format(_T("%.1f ms"), m_pLiveVideo->m_AverageCycleTime);
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	//add a test target for camera aliginment[cjf***032820012]
	if (m_TargetCheck.GetCheck()) {
		int icx = intRound(m_cx);
		int icy = intRound(m_cy);
		for (int i = 1; i <= 4; i++) {
			int rx = intRound(i * 1000.0 * m_x_px_um);
			int ry = intRound(i * 1000.0 * m_y_px_um);
			m_VideoWnd.m_MemDC.DrawEllipse(icx, icy, rx, ry, 1, 0x009f9f9f, NOCOLOR);
		}
		{
			int dx = intRound(4000.0 * m_x_px_um);
			int dy = intRound(4000.0 * m_y_px_um);
			m_VideoWnd.m_MemDC.DrawLine(icx - dx, icy, icx + dx + 1, icy, 1, 0x009f9f9f);
			m_VideoWnd.m_MemDC.DrawLine(icx, icy - dy, icx, icy + dy + 1, 1, 0x009f9f9f);
		}
	}
	//add a test target for camera aliginment[cjf***032820012] done

	m_VideoWnd.Invalidate(FALSE);
	m_VideoWnd.UpdateWindow();

	return 0;
}

//***************************************************************************************
