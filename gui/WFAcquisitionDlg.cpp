//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "WFAcquisitionDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CWFAcquisitionDlg, CAcquisitionDlg)

	ON_COMMAND(IDK_UP, OnUp)
	ON_COMMAND(IDK_DOWN, OnDown)
	ON_COMMAND(IDK_LEFT, OnLeft)
	ON_COMMAND(IDK_RIGHT, OnRight)
	ON_COMMAND(IDK_HOME, OnHome)
	ON_COMMAND(IDK_F1, OnF1)
	ON_COMMAND(IDK_F10, OnF10)

	ON_COMMAND(IDK_1, On1)//530
	ON_COMMAND(IDK_2, On2)//530
	ON_COMMAND(IDK_3, On3)//530
	ON_COMMAND(IDK_4, On4)//530
	ON_COMMAND(IDK_5, On5)//530
	ON_COMMAND(IDK_6, On6)//530
	ON_COMMAND(IDK_7, On7)//530
	ON_COMMAND(IDK_8, On8)//530
	ON_COMMAND(IDK_9, On9)//530
	ON_COMMAND(IDK_DOT, OnDot)//530
	ON_COMMAND(IDK_N1, OnN1)//530
	ON_COMMAND(IDK_N2, OnN2)//530
	ON_COMMAND(IDK_N3, OnN3)//530
	ON_COMMAND(IDK_N4, OnN4)//530
	ON_COMMAND(IDK_N5, OnN5)//530
	ON_COMMAND(IDK_N6, OnN6)//530
	ON_COMMAND(IDK_N7, OnN7)//530
	ON_COMMAND(IDK_N8, OnN8)//530
	ON_COMMAND(IDK_N9, OnN9)//530

END_MESSAGE_MAP()

//***************************************************************************************

CWFAcquisitionDlg::CWFAcquisitionDlg(CWnd* pParentWnd, CWFAcquisition* pWFAcquisition, BOOL DefaultScanModel) :
	CAcquisitionDlg(pParentWnd, pWFAcquisition)
{
	m_pWFAcquisition = pWFAcquisition;

	m_ScanRing = DefaultScanModel;//
}

//***************************************************************************************

BOOL CWFAcquisitionDlg::OnInitDialog()
{
	m_CT = FALSE;

	CAcquisitionDlg::OnInitDialog();

	RECT Rect = { 0, 0, m_w, m_h };
	CMFont Font(16, 600, "Arial");
	m_VideoWnd.m_MemDC.WriteText("Optometer initializing. Please wait...", Rect, Font, YELLOW, 1);

	m_pWFAcquisition->Start();

	return TRUE;
}

//***************************************************************************************

void CWFAcquisitionDlg::OnUp() { m_pWFAcquisition->NotifyIncScanSize(); }
void CWFAcquisitionDlg::OnDown() { m_pWFAcquisition->NotifyDecScanSize(); }
void CWFAcquisitionDlg::OnLeft() { m_pWFAcquisition->NotifyMoveTargetDn(); }
void CWFAcquisitionDlg::OnRight() { m_pWFAcquisition->NotifyMoveTargetUp(); }
void CWFAcquisitionDlg::OnHome() { m_pWFAcquisition->NotifyMoveTargetHome(); }
void CWFAcquisitionDlg::OnF1() { m_pWFAcquisition->NotifyChangeAlignmentMethod(); }
void CWFAcquisitionDlg::OnF10() { m_pWFAcquisition->NotifySwitchTargetLight(); }

void CWFAcquisitionDlg::OnDot()
{
	if (m_pWFAcquisition->m_LastNumStr != "")	 m_pWFAcquisition->m_LastNumStr = ".";
}

void CWFAcquisitionDlg::OnNum(int num)
{
	if (::PresbiaMode == 2) return;

	real    sSize;
	CString sStr;

	if (num == 1) { sSize = 500;	 sStr = "1"; }
	else if (num == 2) { sSize = 1000; sStr = "2"; }
	else if (num == 3) { sSize = 1500; sStr = "3"; }
	else if (num == 4) { sSize = 2000; sStr = "4"; }
	else if (num == 5) { sSize = 2500; sStr = "5"; }
	else if (num == 6) { sSize = 3000; sStr = "6"; }
	else if (num == 7) { sSize = 3500; sStr = "7"; }
	else if (num == 8) { sSize = 4000; sStr = "8"; }
	else if (num == 9) { sSize = 4500; sStr = "9"; }

	if (m_pWFAcquisition->m_LastNumStr != ".")
	{
		m_pWFAcquisition->m_NewScanSize = sSize;
	}
	else
	{
		m_pWFAcquisition->m_NewScanSize = (2 * m_pWFAcquisition->m_NewScanSize + num * 100) / 2;
	}

	m_pWFAcquisition->m_LastNumStr = sStr;

	if (::PresbiaMode == 1 && m_pWFAcquisition->m_NewScanSize < 1000)
		return;

	if (::PresbiaMode == 3 && m_pWFAcquisition->m_NewScanSize < 650)
		return;

	if (::PresbiaMode == 1 && m_pWFAcquisition->m_NewScanSize > 4000)
		return;

	if (::PresbiaMode == 3 && m_pWFAcquisition->m_NewScanSize > 2000)
		return;

	m_pWFAcquisition->NotifyChangeScanSize();
}
//***************************************************************************************

LRESULT CWFAcquisitionDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	CPaintDC PaintDC(this);
	CMDC DC;
	RECT Rect0 = { 5, 5, 100, 100 };
	DC.Attach(PaintDC.Detach());
	DC.FillSolidRect(&Rect0, BLACK);
	PaintDC.Attach(DC.Detach());
	InvalidateRect(&Rect0, FALSE);//530
								  //Invalidate(FALSE); 

	if (!m_WFGUICtrlWnd.m_ShowWindow)
	{
		m_WFGUICtrlWnd.ShowGUI();//530
								 //530
		if (m_pWFAcquisition->m_Combo)
		{
			m_WFGUICtrlWnd.HideAliMethod();
		}
	}
	//530

	memcpy(m_VideoWnd.m_MemDC.m_RGBData, m_pWFAcquisition->m_pHW->GetRGBData(), CHW::m_VideoSize);

	CEyeImage* pImage = &m_pWFAcquisition->m_WFExam.m_Image;

	//
	if (!m_ScanRing)
	{
		m_WFGUICtrlWnd.SetScanRing(FALSE);
		m_ScanRing = TRUE;
	}
	//

	//530 
	if (m_pWFAcquisition->m_WFAutoMode == 1)//
	{
		if (m_WFGUICtrlWnd.m_ShowAcqBtn)
		{
			m_WFGUICtrlWnd.m_ShowAcqBtn = FALSE;
			m_WFGUICtrlWnd.HideAcqBtnGUI();
		}
	}
	else
	{
		if (!m_WFGUICtrlWnd.m_ShowAcqBtn)
		{
			m_WFGUICtrlWnd.m_ShowAcqBtn = TRUE;
			m_WFGUICtrlWnd.ShowAcqBtnGUI();
		}
	}

	////
	if (::PresbiaMode == 1 && m_WFGUICtrl0Wnd.m_ShowWindow && ::WFScanModeChanged)
	{
		m_WFGUICtrl0Wnd.CrSliderInfo();

		m_WFGUICtrl0Wnd.SetPos(m_pWFAcquisition->m_WFExam.m_ScanDiameter*0.001);
		m_WFGUICtrl0Wnd.ShowGUI();

		::WFScanModeChanged = FALSE;
	}
	//

	//Presbia
	if (::PresbiaMode == 2)
	{
		m_WFGUICtrl0Wnd.HideGUI();
	}
	else
	{      //Presbia
		if (m_pWFAcquisition->m_WFAutoMode != 1 && !m_pWFAcquisition->m_Probing && m_pWFAcquisition->m_WFExam.m_ScanDiameter != 0)
		{
			if (m_pWFAcquisition->m_ChangeToPresbia256)
				m_WFGUICtrl0Wnd.m_ShowWindow = FALSE;

			if (m_WFGUICtrl0Wnd.m_ShowWindow)
			{
				if (m_pWFAcquisition->m_ChangeScanSize)
				{
					m_WFGUICtrl0Wnd.SetPos(m_pWFAcquisition->m_WFExam.m_ScanDiameter*0.001);
				}
				else
				{
					if (m_pWFAcquisition->m_WFExam.m_ScanDiameter != m_WFGUICtrl0Wnd.GetDiameter())
					{
						m_pWFAcquisition->m_WFExam.m_ScanDiameter = m_WFGUICtrl0Wnd.GetDiameter();

						if (::Normal64Mode) //
						{
							m_pWFAcquisition->m_user_r_max_um2 = 0.5 * m_pWFAcquisition->m_WFExam.m_ScanDiameter;
							m_pWFAcquisition->m_WFExam.CreateScanPattern(m_pWFAcquisition->m_user_r_max_um2);
						}
						else
						{
							m_pWFAcquisition->m_user_r_max_um = 0.5 * m_pWFAcquisition->m_WFExam.m_ScanDiameter;
							m_pWFAcquisition->m_WFExam.CreateScanPattern(m_pWFAcquisition->m_user_r_max_um);
						}
					}
				}
			}
			else
			{
				m_WFGUICtrl0Wnd.m_ShowWindow = TRUE;
				m_WFGUICtrl0Wnd.Clear();

				//Presbia
				m_WFGUICtrl0Wnd.CrSliderInfo();
				//Presbia

				m_WFGUICtrl0Wnd.SetPos(m_pWFAcquisition->m_WFExam.m_ScanDiameter*0.001);
				m_WFGUICtrl0Wnd.ShowGUI();
			}
		}
		else
		{
			if (m_WFGUICtrl0Wnd.m_ShowWindow)
			{
				m_WFGUICtrl0Wnd.HideGUI();
			}
		}
	}


	if (m_pWFAcquisition->m_ChangeAlMode)
	{
		m_WFGUICtrlWnd.m_AutoMode = m_pWFAcquisition->m_WFAutoMode;
		m_WFGUICtrlWnd.RepaintAuto();
	}
	else  m_pWFAcquisition->m_WFAutoMode = m_WFGUICtrlWnd.m_AutoMode;

	if (!m_pWFAcquisition->m_Combo)
	{
		if (m_pWFAcquisition->m_ChangeAlMethod)
		{
			m_WFGUICtrlWnd.m_PupilCen = (m_pWFAcquisition->m_WFExam.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER ? 1 : 0);
			m_WFGUICtrlWnd.RepaintPupilCen();
		}
		else  m_pWFAcquisition->m_WFExam.m_AlignmentMethod = m_WFGUICtrlWnd.m_PupilCen ? ALIGNMENT_PUPIL_CENTER : ALIGNMENT_4_IR_DOTS;
	}
	else
	{
		m_WFGUICtrlWnd.m_PupilCen = FALSE;
		m_pWFAcquisition->m_WFExam.m_AlignmentMethod = ALIGNMENT_4_IR_DOTS;
	}

	if (m_pWFAcquisition->m_ChangeIllumi)
	{
		m_WFGUICtrlWnd.m_TargetOn = m_pWFAcquisition->m_TargetLightOn;
		m_WFGUICtrlWnd.RepaintTarget();
	}
	else
	{
		if (m_pWFAcquisition->m_TargetLightOn != m_WFGUICtrlWnd.m_TargetOn)
		{
			m_pWFAcquisition->m_TargetLightOn = !m_pWFAcquisition->m_TargetLightOn;

			if (m_pWFAcquisition->m_TargetLightOn) {
				m_pWFAcquisition->m_pHW->TurnAccommodationTargetOn();
			}
			else {
				m_pWFAcquisition->m_pHW->TurnAccommodationTargetOff();
			}
		}

		if (m_pWFAcquisition->m_TargetLightOn == FALSE)
		{
			m_pWFAcquisition->m_TargetTime = -1;
		}
	}

	//CT Exam or color image Capture
	if (m_WFGUICtrlWnd.m_Acquire)
	{
		m_pWFAcquisition->Acquire();
		m_WFGUICtrlWnd.m_Acquire = FALSE;
	}
	//Done

	//530 Done

	int fs = 16;
	CMFont Font(fs, 600, "Arial");
	RECT Rect;
	CString s;

	int icx = intRound(m_cx);
	int icy = intRound(m_cy);

	// EYE
	int t = 5;
	::SetRect(&Rect, 10, t, 200, t + fs);
	m_VideoWnd.m_MemDC.WriteText("Eye", Rect, Font, YELLOW, 0);
	s = ISOD(m_pWFAcquisition->m_Eye) ? "OD" : "OS";
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	// SPHERE
	t += fs;
	::SetRect(&Rect, 10, t, 200, t + fs);
	m_VideoWnd.m_MemDC.WriteText("Sphere", Rect, Font, YELLOW, 0);
	real D = m_pWFAcquisition->m_WFExam.m_PrecalcSphere;
	s.Format(_T("%.2f D"), fabs(D));
	if (D < -0.01) s = "- " + s; else if (D > 0.01) s = "+ " + s;
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	// CYL
	t += fs;
	::SetRect(&Rect, 10, t, 200, t + fs);
	m_VideoWnd.m_MemDC.WriteText("Cylinder", Rect, Font, YELLOW, 0);
	D = m_pWFAcquisition->m_WFExam.m_PrecalcCylinder;
	s.Format(_T("%.2f D"), fabs(D));
	if (D < 0.01) s = "  " + s; else s = "+ " + s;
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	// TARGET
	t += fs;
	::SetRect(&Rect, 10, t, 200, t + fs);
	m_VideoWnd.m_MemDC.WriteText("Fixation target", Rect, Font, YELLOW, 0);
	D = m_pWFAcquisition->m_pHW->GetOptometerTargetPositionD();
	s.Format(_T("%.2f D"), fabs(D));
	if (D < -0.01) s = "- " + s; else if (D > 0.01) s = "+ " + s;
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	// FIXATION LIGHT
	t += fs;
	::SetRect(&Rect, 10, t, 200, t + fs);
	m_VideoWnd.m_MemDC.WriteText("Fixation light", Rect, Font, YELLOW, 0);

	//530
	if (m_WFGUICtrlWnd.m_TargetOn && ::Settings.m_AccommTargetLightTime != 60 && m_pWFAcquisition->m_TargetTime > 0)
	{
		RECT tmpRect;
		::SetRect(&tmpRect, 10, t, 180, t + fs);
		m_VideoWnd.m_MemDC.WriteText("On", tmpRect, Font, GREEN, 2);

		::SetRect(&tmpRect, 180, t, 200, t + fs);

		m_pWFAcquisition->m_TargetTime = intRound(m_pWFAcquisition->m_TimeLeft / 1000 - (60 - ::Settings.m_AccommTargetLightTime));
		s.Format(_T("%i"), m_pWFAcquisition->m_TargetTime);

		m_VideoWnd.m_MemDC.WriteText(s, tmpRect, Font, GREEN, 2);
	}
	else //530
	{
		m_VideoWnd.m_MemDC.WriteText(m_pWFAcquisition->m_TargetLightOn ? "On" : "Off", Rect, Font,
			m_pWFAcquisition->m_TargetLightOn ? GREEN : YELLOW, 2);
	}

	// PUPIL
	//Presbia
	if (::PresbiaMode == 2 || ::PresbiaMode == 3)
	{
	}
	else
	{   //Presbia
		t += fs;
		::SetRect(&Rect, 10, t, 200, t + fs);
		m_VideoWnd.m_MemDC.WriteText("Pupil", Rect, Font, GREEN, 0);
		if (pImage->m_pu_ok) {
			s.Format(_T("%.2f mm"), pImage->m_pu_r_mean_um * 0.002);
		}
		else {
			s = "-.-- mm";
		}
		m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, GREEN, 2);
	}

	// SCAN SIZE
	t += fs;
	if (m_pWFAcquisition->m_WFAutoMode != 1) {
		::SetRect(&Rect, 10, t, 200, t + fs);
		m_VideoWnd.m_MemDC.WriteText("Scan", Rect, Font, RED, 0);
		if (m_pWFAcquisition->m_WFExam.m_ScanDiameter<0) m_pWFAcquisition->m_WFExam.m_ScanDiameter = 0;
		s.Format(_T("%.2f mm"), m_pWFAcquisition->m_WFExam.m_ScanDiameter * 0.001);
		m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, RED, 2);
	}

	// MODE
	t = 5;
	::SetRect(&Rect, 0, t, m_w - 10, t + fs);
	m_VideoWnd.m_MemDC.WriteText(m_pWFAcquisition->m_Probing ? "Autofixation" : "Acquisition", Rect, Font, YELLOW, 2);

	t += fs;
	::SetRect(&Rect, 0, t, m_w - 10, t + fs);
	m_VideoWnd.m_MemDC.WriteText(m_pWFAcquisition->m_WFAutoMode == 1 ? "Auto" : "Manual", Rect, Font, YELLOW, 2);//

																												 // ALIGNMENT MODE
																												 //Presbia
	if (::PresbiaMode == 2 || ::PresbiaMode == 3)
	{
		int rx = intRound(1600.0 * m_x_px_um);
		int ry = intRound(1600.0 * m_y_px_um);
		m_VideoWnd.m_MemDC.DrawEllipse(icx, icy, rx, ry, 1, GREEN, NOCOLOR);

		rx = intRound(255.0 * m_x_px_um);
		ry = intRound(255.0 * m_y_px_um);
		m_VideoWnd.m_MemDC.DrawEllipse(icx, icy, rx, ry, 1, GREEN, NOCOLOR);
	}
	else//Presbia
	{
		t += fs;
		::SetRect(&Rect, 0, t, m_w - 10, t + fs);
		m_VideoWnd.m_MemDC.WriteText(m_pWFAcquisition->m_WFExam.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER ? "Pupil" : "4 IR Dots", Rect, Font, YELLOW, 2);
	}

	// TIME LEFT
	t += fs;
	::SetRect(&Rect, 0, t, m_w - 10, t + fs);
	s.Format(_T("%i"), m_pWFAcquisition->m_TimeLeft / 1000);
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, CYAN, 2);

	//Presbia
	if (::PresbiaMode == 2)
	{
	}
	else if (::PresbiaMode == 3)
	{
		int rx = intRound(m_pWFAcquisition->m_WFExam.m_ScanDiameter * m_x_px_um * 0.5);

		for (int a = 0; a < 360; a++)
		{
			int d = intRound((real)a / 5.0);

			if (d % 2 == 0)
			{
				int b = a == 359 ? 0 : a + 1;

				int xa = icx + intRound(m_pWFAcquisition->m_WFExam.m_ScanDiameter * 0.5 *  COS[a] * m_x_px_um);
				int ya = icy - intRound(m_pWFAcquisition->m_WFExam.m_ScanDiameter * 0.5 *  SIN[a] * m_y_px_um);
				int xb = icx + intRound(m_pWFAcquisition->m_WFExam.m_ScanDiameter * 0.5 *  COS[b] * m_x_px_um);
				int yb = icy - intRound(m_pWFAcquisition->m_WFExam.m_ScanDiameter * 0.5 *  SIN[b] * m_y_px_um);

				m_VideoWnd.m_MemDC.DrawLine(xa, ya, xb, yb, 1, 0x009f9f9f);
			}
		}
	}
	else
	{   //Presbia
		// GRID
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

		// POINTS
		//530 If user click 'show scan ring
		//Point or Rings
		if (m_pWFAcquisition->m_WFAutoMode != 1)
		{
			if (!m_pWFAcquisition->m_Probing)
			{
				if (m_WFGUICtrlWnd.m_ShowScanRing)
				{
					CScanPoint* pPoint = m_pWFAcquisition->m_WFExam.m_Points.MoveFirst();
					if (pPoint && pPoint->m_shot == 0)
					{
						int x = intRound(m_cx + pPoint->x_um() * m_x_px_um);
						int y = intRound(m_cy - pPoint->y_um() * m_y_px_um);

						int r = intRound(0.5 * m_pWFAcquisition->m_WFExam.m_ScanDiameter * m_x_px_um);
						m_VideoWnd.m_MemDC.DrawCircle(x, y, r, 1, RED, NOCOLOR);

						m_VideoWnd.m_MemDC.DrawCircle(x, y, 4, 1, RED, RED);
					}
				}
				else
				{
					CScanPoint* pPoint = m_pWFAcquisition->m_WFExam.m_Points.MoveFirst();
					while (pPoint)
					{
						if (pPoint->m_shot == 0)
						{
							int x = intRound(m_cx + pPoint->x_um() * m_x_px_um);
							int y = intRound(m_cy - pPoint->y_um() * m_y_px_um);
							m_VideoWnd.m_MemDC.DrawCircle(x, y, 4, 1, RED, RED);
						}
						pPoint = m_pWFAcquisition->m_WFExam.m_Points.MoveNext();
					}
				}
			}
		}
		//530 Done

		// PUPIL
		if (pImage->m_pu_ok) {
			CPen Pen(PS_SOLID, 3, GREEN);
			CPen* pPen = m_VideoWnd.m_MemDC.SelectObject(&Pen);
			int x = intRound(m_cx + (pImage->m_pu_x0_um + pImage->m_pu_r_um[0] * COS[0]) * m_x_px_um);
			int y = intRound(m_cy - (pImage->m_pu_y0_um + pImage->m_pu_r_um[0] * SIN[0]) * m_y_px_um);
			m_VideoWnd.m_MemDC.MoveTo(x, y);
			for (int i = 1; i < 360; i++) {
				x = intRound(m_cx + (pImage->m_pu_x0_um + pImage->m_pu_r_um[i] * COS[i]) * m_x_px_um);
				y = intRound(m_cy - (pImage->m_pu_y0_um + pImage->m_pu_r_um[i] * SIN[i]) * m_y_px_um);
				m_VideoWnd.m_MemDC.LineTo(x, y);
			}
			x = intRound(m_cx + (pImage->m_pu_x0_um + pImage->m_pu_r_um[0] * COS[0]) * m_x_px_um);
			y = intRound(m_cy - (pImage->m_pu_y0_um + pImage->m_pu_r_um[0] * SIN[0]) * m_y_px_um);
			m_VideoWnd.m_MemDC.LineTo(x, y);
			m_VideoWnd.m_MemDC.SelectObject(pPen);

			if (m_pWFAcquisition->m_WFExam.m_AlignmentMethod == ALIGNMENT_PUPIL_CENTER) {
				x = intRound(m_cx + pImage->m_pu_x0_um * m_x_px_um);
				y = intRound(m_cy - pImage->m_pu_y0_um * m_y_px_um);
				m_VideoWnd.m_MemDC.DrawLine(icx, icy, x, y, 3, GREEN);
			}
		}

		// VERTEX
		if (m_pWFAcquisition->m_WFExam.m_AlignmentMethod == ALIGNMENT_4_IR_DOTS && pImage->m_ve_ok) {

			int x = intRound(m_cx + pImage->m_ve_x_um * m_x_px_um);
			int y = intRound(m_cy - pImage->m_ve_y_um * m_y_px_um);
			//m_VideoWnd.m_MemDC.DrawLine(x - 10, y, x + 11, y, 3, RED);
			//m_VideoWnd.m_MemDC.DrawLine(x, y - 10, x, y + 11, 3, RED);
			m_VideoWnd.m_MemDC.DrawCircle(x, y, 2, 1, RED, RED);

			m_VideoWnd.m_MemDC.DrawLine(icx, icy, x, y, 3, RED);
		}
	}

	//531
	if (m_pWFAcquisition->m_ss != "-1")
	{
		RECT RectCo;
		::SetRect(&RectCo, icx - 25, icy - 25, icx + 25, icy + 25);

		CMFont FontCo(50, 600, "Arial");
		m_VideoWnd.m_MemDC.WriteText(m_pWFAcquisition->m_ss, RectCo, FontCo, BLUE, 1, -1);
	}
	//531

	if (m_ShowTechInfo) {

		real hmax = -1000000000.0;
		for (int i = 0; i < 256; i++) {
			if (hmax < pImage->m_hist[i]) hmax = pImage->m_hist[i];
		}
		if (hmax != 0) {
			for (int i = 0; i < 255; i++) {
				int h1 = intRound(400 * pImage->m_hist[i] / hmax);
				int h2 = intRound(400 * pImage->m_hist[i + 1] / hmax);
				m_VideoWnd.m_MemDC.DrawLine(i, m_h - 10 - h1, i + 1, m_h - 10 - h2, 1, RED);
			}
			m_VideoWnd.m_MemDC.DrawLine(pImage->m_pu_thr, m_h - 10, pImage->m_pu_thr, m_h - 410, 1, YELLOW);
		}

		::SetRect(&Rect, 0, m_h - fs - fs, m_w - 10, m_h - fs);
		s.Format(_T("%u"), m_pWFAcquisition->m_NumFramesReceived);
		m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

		::SetRect(&Rect, 0, m_h - fs, m_w - 10, m_h);
		s.Format(_T("%.1f ms"), m_pWFAcquisition->m_AverageCycleTime);
		m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);
	}

	m_VideoWnd.Invalidate(FALSE);
	m_VideoWnd.UpdateWindow();

	return 0;
}

//***************************************************************************************
