//***************************************************************************************

#include "StdAfx.h"
#include "CTAcquisitionDlg.h"
#include "Settings.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CCTAcquisitionDlg::CCTAcquisitionDlg(CWnd* pParentWnd, CCTAcquisition* pCTAcquisition) :
	CAcquisitionDlg(pParentWnd, pCTAcquisition)
{
	m_pCTAcquisition = pCTAcquisition;
}

//***************************************************************************************

BOOL CCTAcquisitionDlg::OnInitDialog()
{
	m_CT = TRUE;

	CAcquisitionDlg::OnInitDialog();

	m_pCTAcquisition->m_TriLaserShutoffOn = TRUE;
	m_pCTAcquisition->Start();

	return TRUE;
}

//***************************************************************************************

LRESULT CCTAcquisitionDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	////cjf 10222020 test for slow gain
 // if (wParam != 0) {
 //   memcpy(m_VideoWnd.m_MemDC.m_RGBData, m_pCTAcquisition->m_pHW->GetRGBData(), CHW::m_VideoSize);
 //   m_VideoWnd.Invalidate(FALSE);
 //   m_VideoWnd.UpdateWindow();

 //   return 0;
 // }
 // //cjf 10222020 test for slow gain

	memcpy(m_VideoWnd.m_MemDC.m_RGBData, m_pCTAcquisition->m_pHW->GetRGBData(), CHW::m_VideoSize);

	//530
	if (lParam == 0)
	{
		/* if(m_WFGUICtrlWnd.m_ShowWindow )
		{
		m_WFGUICtrlWnd.m_ShowWindow = FALSE;
		m_WFGUICtrlWnd.HideGUI();
		}
		if(m_WFGUICtrlWnd.m_ShowWindow )
		{
		if(m_WFGUICtrlWnd.m_WFORCT == 0)
		{
		m_WFGUICtrlWnd.m_WFORCT = 1;

		if(m_pCTAcquisition->m_AutoMode != m_WFGUICtrlWnd.m_AutoMode)
		{
		m_WFGUICtrlWnd.m_AutoMode = m_pCTAcquisition->m_AutoMode;
		m_WFGUICtrlWnd.RepaintAuto();
		}

		m_WFGUICtrlWnd.ShowGUI();
		}
		}
		else
		{
		m_WFGUICtrlWnd.m_WFORCT = 1;

		if(m_pCTAcquisition->m_AutoMode != m_WFGUICtrlWnd.m_AutoMode)
		{
		m_WFGUICtrlWnd.m_AutoMode = m_pCTAcquisition->m_AutoMode;
		m_WFGUICtrlWnd.RepaintAuto();
		}

		m_WFGUICtrlWnd.ShowGUI();
		}*/
	}

	if (lParam == 1)//Manual Color Image Capture
	{
		/* if(m_WFGUICtrlWnd.m_ShowWindow)
		{
		m_WFGUICtrlWnd.m_ShowWindow = FALSE;
		m_WFGUICtrlWnd.HideGUI();
		}*/

		if (!m_ControlWnd.m_ShowWindow)
		{
			m_ControlWnd.ShowGUI();
		}

		m_pCTAcquisition->m_WhiteLEDsPower = m_ControlWnd.GetWhiteLEDsPower();
    m_pCTAcquisition->m_Brightness     = m_ControlWnd.GetBrightness();
    m_pCTAcquisition->m_Contrast       = m_ControlWnd.GetContrast();
    m_pCTAcquisition->m_Hue            = m_ControlWnd.GetHue();
    m_pCTAcquisition->m_Saturation     = m_ControlWnd.GetSaturation();

		m_ControlWnd.ShowAcqBtnGUI();
	}
	else if (lParam == 0) //CT Capture
	{
		/*if( m_pCTAcquisition->m_AutoMode )
		{
		if(m_ExamCaptureWnd.m_ShowWindow)
		{
		m_ExamCaptureWnd.m_ShowWindow = FALSE;
		m_ExamCaptureWnd.HideGUI();
		}
		}
		else
		{
		if(!m_ExamCaptureWnd.m_ShowWindow)
		{
		m_ExamCaptureWnd.m_ShowWindow = TRUE;
		m_ExamCaptureWnd.ShowGUI();
		}
		}

		if(m_pCTAcquisition->m_ChangeAlMode)
		{
		m_WFGUICtrlWnd.m_AutoMode = m_pCTAcquisition->m_AutoMode;
		m_WFGUICtrlWnd.RepaintAuto();
		}
		else
		{
		if( m_pCTAcquisition->m_AutoMode != m_WFGUICtrlWnd.m_AutoMode)
		m_pCTAcquisition->m_AutoMode  = m_WFGUICtrlWnd.m_AutoMode;
		}*/
	}
	else if (lParam == 2)//Auto Color Image Capture
	{
		if (m_ControlWnd)
		{
  		m_ControlWnd.HideAcqBtnGUI();
		}

		/*if(m_WFGUICtrlWnd.m_ShowWindow)
		{
		m_WFGUICtrlWnd.m_ShowWindow = FALSE;
		m_WFGUICtrlWnd.HideGUI();
		}*/
	}


	if (lParam != 2)
	{
		//CT Exam or color image Capture
		if (m_ControlWnd.m_Acquire == TRUE)
		{
			m_pCTAcquisition->Acquire();
			m_ControlWnd.m_Acquire = FALSE;
		}
		//Done
	}
	//530

	CEyeImage* pImage = &m_pCTAcquisition->m_CTExam.m_Image;

	int fs = 16;
	CMFont Font(fs, 600, "Arial");
	RECT Rect;
	CString s;

	int icx = intRound(m_cx);
	int icy = intRound(m_cy);

	int X = pImage->m_table.GetNumCols();
	int Y = pImage->m_table.GetNumRows();
	int Y2 = Y >> 1;
	int X2 = X >> 1;

	// MODE
	int t = 5;
	::SetRect(&Rect, 0, t, m_w - 10, t + fs);
	m_VideoWnd.m_MemDC.WriteText(m_pCTAcquisition->m_CTAutoMode ? "Auto" : "Manual", Rect, Font, YELLOW, 2);

	// TIME LEFT
	t += fs;
	::SetRect(&Rect, 0, t, m_w - 10, t + fs);
	s.Format(_T("%i"), m_pCTAcquisition->m_TimeLeft / 1000);
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, CYAN, 2);

	// OD / OS
	::SetRect(&Rect, 10, 5, 200, 5 + fs);
	m_VideoWnd.m_MemDC.WriteText("Eye", Rect, Font, YELLOW, 0);
	s = ISOD(m_pCTAcquisition->m_Eye) ? "OD" : "OS";
	m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

	if (::NewSettings.m_Adjusting_CT)//[520]
	{
		/*::SetRect(&Rect, 0, m_h-2*fs, m_w, m_h-fs);
		m_VideoWnd.m_MemDC.WriteText("Please push button or 'SPACE' bar to capture the color image, or press 'ESC' to abort.", Rect, Font, YELLOW, 1);
		::SetRect(&Rect, 0, m_h-fs, m_w, m_h);
		m_VideoWnd.m_MemDC.WriteText("Push 'Ctrl + s' button to set the camera.", Rect, Font, YELLOW, 1);
		*/
		::SetRect(&Rect, 0, m_h - fs, m_w, m_h);
		m_VideoWnd.m_MemDC.WriteText("Please push button or 'SPACE' bar to capture the color image, or press 'ESC' to abort.", Rect, Font, YELLOW, 1);
	}
	else
	{
		::SetRect(&Rect, 0, m_h - fs, m_w, m_h);
		m_VideoWnd.m_MemDC.WriteText("CT acquisition must be made while moving the iTrace toward the patient.", Rect, Font, YELLOW, 1);
	}

	COLORREF green = m_pCTAcquisition->m_ve0_ok ? 0x0000ff00 : 0x00007f00;
	COLORREF red = m_pCTAcquisition->m_la_ok ? 0x000000ff : 0x0000009f;

	// ��������
	if (!::NewSettings.m_Adjusting_CT)//[520]
	{
		CNode* pNode = pImage->m_hull.MoveFirst();
		while (pNode) {
			m_VideoWnd.m_MemDC.DrawCircle(icx + (pNode->m_x - X2), icy - (pNode->m_y - Y2), 1, 1, green, green);
			pNode = pImage->m_hull.MoveNext();
		}
	}

	m_VideoWnd.m_MemDC.DrawLine(icx - X2, icy - Y2, icx + X2 + 1, icy - Y2, 1, YELLOW);
	m_VideoWnd.m_MemDC.DrawLine(icx - X2, icy, icx + X2 + 1, icy, 1, YELLOW);
	m_VideoWnd.m_MemDC.DrawLine(icx - X2, icy + Y2, icx + X2 + 1, icy + Y2, 1, YELLOW);
	m_VideoWnd.m_MemDC.DrawLine(icx - X2, icy - Y2, icx - X2, icy + Y2 + 1, 1, YELLOW);
	m_VideoWnd.m_MemDC.DrawLine(icx, icy - Y2, icx, icy + Y2 + 1, 1, YELLOW);
	m_VideoWnd.m_MemDC.DrawLine(icx + X2, icy - Y2, icx + X2, icy + Y2 + 1, 1, YELLOW);

	// ������� �� ������ �� ��������
	if (!::NewSettings.m_Adjusting_CT)//[520]
	{
		if (pImage->m_ve0_ok) {
			m_VideoWnd.m_MemDC.DrawLine(icx, icy, icx + pImage->m_ve0_x, icy - pImage->m_ve0_y, 3, green);
		}
	}

	// ������ ������ ������
	//if (pImage->m_la_ok)//
	if (!::NewSettings.m_Adjusting_CT)//[520]
	{
		if (pImage->m_Target_ok)//[5.1.1]	  
		{
			if (m_pCTAcquisition->m_ShowRedDot) //[cjf***04032012]
				m_VideoWnd.m_MemDC.DrawEllipse(icx + pImage->m_la_x, icy, 10, 5, 1, red, red);
			//[cjf***04112012]
			//if(pImage->m_la_x > 0 && pImage->m_la_RtoC_OK)
			if (pImage->m_la_x > -5 && pImage->m_Target_ok)
			{
				m_VideoWnd.m_MemDC.DrawLine(icx + pImage->m_la_x - 5, icy, icx + pImage->m_la_x + 5, icy, 3, red);
				m_VideoWnd.m_MemDC.DrawLine(icx + pImage->m_la_x, icy - 5, icx + pImage->m_la_x, icy + 5, 3, red);
			}
			//[cjf***04112012]
		}
	}

	//::SetRect(&Rect, 10, 5 + fs, 200, 5 + 9 * fs);
	//s.Format(_T("%d"), pImage->m_la_xmax );
	//m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, green, 2);




	if (m_pCTAcquisition->m_pHW->m_Calibration.ZAlignmentMethod == 1) {
		// triangulation sensor reading
		::SetRect(&Rect, 10, 5 + fs, 200, 5 + 2 * fs);
		m_VideoWnd.m_MemDC.WriteText("Sensor", Rect, Font, YELLOW, 0);
		s.Format(_T("%i"), m_pCTAcquisition->m_TriangulationSensorReading);
		m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);
	}

	if (m_ShowTechInfo) {

		real_t hmax = -1000000000.0;
		for (int i = 0; i < 256; i++) {
			if (hmax < pImage->m_hist[i]) hmax = pImage->m_hist[i];
		}
		if (hmax != 0.0) {
			for (int i = 0; i < 255; i++) {
				int h1 = intRound(400.0 * pImage->m_hist[i] / hmax);
				int h2 = intRound(400.0 * pImage->m_hist[i + 1] / hmax);
				m_VideoWnd.m_MemDC.DrawLine(i, m_h - 10 - h1, i + 1, m_h - 10 - h2, 1, RED);
			}
			m_VideoWnd.m_MemDC.DrawLine(pImage->m_ve0_thr, m_h - 10, pImage->m_ve0_thr, m_h - 410, 1, YELLOW);
		}

		::SetRect(&Rect, 0, m_h - fs - fs, m_w - 10, m_h - fs);
		s.Format(_T("%u"), m_pCTAcquisition->m_NumFramesReceived);
		m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);

		::SetRect(&Rect, 0, m_h - fs, m_w - 10, m_h);
		s.Format(_T("%.1f ms"), m_pCTAcquisition->m_AverageCycleTime);
		m_VideoWnd.m_MemDC.WriteText(s, Rect, Font, YELLOW, 2);
	}

	m_VideoWnd.Invalidate(FALSE);
	m_VideoWnd.UpdateWindow();

	return 0;
}

//***************************************************************************************
