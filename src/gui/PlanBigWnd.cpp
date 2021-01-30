#include "StdAfx.h"
#include "Resource.h"
#include "Settings.h"
#include "ImageDlg2.h"
#include "GlobalFunctions.h"//6.2.0 For read Chinese name

#include "PlanBigWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CPlanBigWnd, CMemWnd)

	//ON_WM_LBUTTONDBLCLK()

	ON_WM_CTLCOLOR()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_WM_RBUTTONUP()

	ON_BN_CLICKED(IDC_SAVE_SCREEN_BUTTON, OnSaveScreenButtonClicked)
	ON_BN_CLICKED(IDC_PRINT_SCREEN_BUTTON, OnPrintScreenButtonClicked)
	ON_BN_CLICKED(IDC_SHOW_MAP_ZALDIVAR_ITEM, OnZaldivarButtonClicked)
	ON_BN_CLICKED(IDC_CTZOOM_EXIT_BTN, OnExitButtonClicked)
	ON_BN_CLICKED(IDC_IMG_MOUSE_CONTROL_ITEM, OnImgMouseButtonClicked)
	ON_BN_CLICKED(IDC_RING_MOUSE_CONTROL_ITEM, OnRingMouseButtonClicked)

	ON_COMMAND(IDC_SHOW_COLOR_PUPIL_ITEM, OnShowPupilItemClicked)
	ON_COMMAND(IDC_SHOW_COLOR_LIMBUS_ITEM, OnShowLimbusItemClicked)
	ON_COMMAND(IDC_SHOW_COLOR_RRULER_ITEM, OnShowRRulerItemClicked)
	ON_COMMAND(IDC_SHOW_COLOR_LRULER_ITEM, OnShowLRulerItemClicked)

	ON_COMMAND(IDC_EDIT_LIMBUS_ITEM, OnEditLimbusItemClicked)

END_MESSAGE_MAP()

//***************************************************************************************
//int CPlanBigWnd::m_wheel_n = 0;
//int CPlanBigWnd::m_wheel_n2 = 0;
//***************************************************************************************
void CPlanBigWnd::SafeInteRelease(int **ppT, int Row)
{
	if (ppT != NULL) {
		for (int i = 0; i<Row; i++) {
			if (ppT[i] != NULL) {
				delete[](ppT[i]);
				ppT[i] = NULL;
			}
		}
		delete[](ppT);
		ppT = NULL;
	}
}

//*****************************************************************************************

double CPlanBigWnd::Gaussian(int x, int y, double de)
{
	double value = 0;
	double pai = 3.1415926;
	double e = 2.718281828;

	value = (-1)*((x*x + y*y) / (2 * de*de));
	value = (1 / (2 * pai*de*de))*pow(e, value);

	return value;
}

//*****************************************************************************************

HSI CPlanBigWnd::ChangeRGBtoHSI(int R, int G, int B)
{

	HSI res;
	res.I = (real_t)(R + G + B) / 3;

	//find the minixum of RGB for the S value
	real_t min = (real_t)R;
	if (min>G) min = G;
	if (min>B) min = B;
	//
	res.S = 1 - (min / res.I);

	//find the S
	real_t sita;
	real_t mid1 = 0.5*(2 * R - G - B);
	real_t mid2 = pow((R - G)*(R - G) + (R - B)*(G - B), 0.5) + 0.0000000001;
	mid1 = mid1 / mid2;
	sita = acos(mid1);

	if (B>G) sita = 2 * _Pi - sita;

	real_t angle = sita * 180 / _Pi;

	res.H = angle;
	//
	return res;
}

//*****************************************************************************************

CPlanBigWnd::CPlanBigWnd(CWnd* pParentWnd, RECT& Rect, CDicomInfo  DicomInfo, CCTExam* pCTExam)//Type 0: for planner toric; 1: for other CT functionalities(sum1 to sum4)
{
	m_LensColor = 0x00007f9f;
	m_CaliperColor = RGB(230, 180, 60);//RGB(230,215,30);//RGB(195,145,65);
	m_CaliperColor2 = RGB(255, 128, 0);
	m_OverLap = FALSE;
	m_AlignLine = (m_isCombo ? TRUE : FALSE);;

	m_DicomInfo = DicomInfo;
	m_MoveCalipar = 0;
	m_MousePos = 0;

	m_pCTExam = pCTExam;
	m_wheel_n = 0;
	m_last_wheel_n = 0;
	m_wheel_n2 = 0;
	m_pMenu = new CXPMenu();

	m_ShowDymImg = TRUE;
	m_ShowOsherRing = FALSE;
	m_ShowCaliper = (m_isCombo ? TRUE : FALSE);

	m_ShowLimbus = TRUE;
	m_ShowPupil = TRUE;
	m_ShowRRuler = TRUE;
	m_ShowLRuler = TRUE;

	if (m_isCombo && m_pCTExam->m_OpData.m_RefAxis[0] < 0)
	{
		m_a1 = m_pCTExam->m_OpData.m_IOLPlacementAxis; ;
		GetProAngs(m_pCTExam->m_OpData.m_RefAxis[0], m_a2, m_a3, m_a4);

		//6.2.0 Save the Length		
		m_Ali_Rum1 = m_pCTExam->m_OpData.m_RefLength[0];
		m_Ali_Rum2 = m_pCTExam->m_OpData.m_RefLength[1];
		m_Ali_Rum3 = m_pCTExam->m_OpData.m_RefLength[2];
		m_Ali_Rum4 = m_pCTExam->m_OpData.m_RefLength[3];
		//Done

		/*m_Ali_Rum1 = 5200.0;
		m_Ali_Rum2 = 5200.0;
		m_Ali_Rum3 = 5200.0;*/
		//m_Ali_Rum4 = (m_a4 == -1 ? 0 : 5200.0);
	}
	else
	{
		m_AlignLine = FALSE;
		m_a1 = -1;
		m_Ali_Rum1 = 0.0;
		m_a2 = -1;
		m_Ali_Rum2 = 0.0;
		m_a3 = -1;
		m_Ali_Rum3 = 0.0;
		m_a4 = -1;
		m_Ali_Rum4 = 0.0;
	}

	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
	{
		m_pImage = &m_pCTExam->m_ColorImage;
		m_pImage->SwapImageInfo(&m_pCTExam->m_Image);

		if (!m_pImage->m_ve_ok)	m_pImage->FindWFVertex();
		if (!m_pImage->m_li_ok)
		{
			m_pImage->FindLimbus();
			//FindClearLimbus(m_pImage,m_pCTExam->m_Image.m_li_x0_um, m_pCTExam->m_Image.m_li_y0_um, m_pCTExam->m_Image.m_li_r_mean_um);
		}
		if (m_pImage->m_pu_r_mean_um > 0) m_pImage->m_pu_ok = TRUE;
		if (!m_pImage->m_pu_ok)
		{
			m_pImage->FindPupil();

			//Judge wether the finding pupil is correct or not
			if (m_pImage->m_ve_ok)
			{
				real_t puveDisum = Distance(m_pImage->m_pu_x0_um, m_pImage->m_pu_y0_um, m_pImage->m_ve_x_um, m_pImage->m_ve_y_um);

				if (puveDisum > 1000)
				{
					m_pImage->ClearPupil();
					m_pImage->m_pu_ok = FindColorPupil(m_pImage);
				}
			}
		}
		else
		{
			if (m_pImage->m_ve_ok)
			{
				real_t puveDisum = Distance(m_pImage->m_pu_x0_um, m_pImage->m_pu_y0_um, m_pImage->m_ve_x_um, m_pImage->m_ve_y_um);

				if (puveDisum > 1000)
				{
					m_pImage->ClearPupil();
					m_pImage->m_pu_ok = FindColorPupil(m_pImage);
				}
			}
		}

		////521, Recalculate the visual axis by useing ct gray image
		CEyeImage*  m_GrayImage;

		m_GrayImage = &m_pCTExam->m_Image;

		real_t xGrayDis = m_GrayImage->m_li_x0_um - m_GrayImage->m_ve_x_um;
		real_t yGrayDis = m_GrayImage->m_li_y0_um - m_GrayImage->m_ve_y_um;

		m_pImage->m_ve_x_um = m_pImage->m_li_x0_um - xGrayDis;
		m_pImage->m_ve_y_um = m_pImage->m_li_y0_um - yGrayDis;
		////Done
	}
	else
	{
		m_pImage = &m_pCTExam->m_Image;
	}

	CreateWnd(Rect, pParentWnd);


	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int H = ::GetSystemMetrics(SM_CYSCREEN);
	int m_q = W >= 1920 ? 2 : (W >= 1600 ? 1 : 0);

	m_ImgSize = m_h*m_w * 3;

	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h;

	for (int t = 0; t < 15; t++)
	{
		try 
		{
			m_pZoomBackup[t] = new uchar[m_ImgSize];
			m_BackupYes[t] = FALSE;
			m_cxBackup[t] = m_cx;
			m_cyBackup[t] = m_cy;
		}
		catch (...)
		{
			m_pZoomBackup[t] = NULL;
			return;
		}
	}

	m_mouseCap = FALSE;
	m_mouseRealCap = FALSE;

	m_MovePoint.x = 0;
	m_MovePoint.y = 0;

	m_h_um = 12000.0;
	m_w_um = m_h_um * m_w / m_h;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;


	//Create the pZoomImages
	CreateZoomImg();
	//

	real_t t = 0.02 * m_w;
	int h = intRound(t);
	int w = intRound(5 * t);
	int smallW = intRound(w / 2) - 4;
	int fs = intRound(0.02 * ::GetSystemMetrics(SM_CYSCREEN));

	CMRgn Rgn;
	RECT Rect1;

	::SetRect(&Rect1, m_w - 2 - w, m_h - 2 - h, m_w - 2, m_h - 2);
	m_ExitButton.SetBkColor(NOCOLOR);
	m_ExitButton.SetText("Exit");
	m_ExitButton.SetFont(fs);
	m_ExitButton.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, Rect1, this, IDC_CTZOOM_EXIT_BTN);
	//Rgn.SetRectRgn(&Rect1);
	//   m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	::SetRect(&Rect1, m_w - 2 - w, m_h - 2 - 2 * h, m_w - 2, m_h - 2 - h);
	m_ZaldivarButton.SetBkColor(NOCOLOR);
	if (m_isCombo)  m_ZaldivarButton.SetText("Protractor");//531
	else           m_ZaldivarButton.SetText("Zaldivar Caliper");
	m_ZaldivarButton.SetFont(fs);
	m_ZaldivarButton.Create(_T(""), _T(""), WS_CHILD | WS_VISIBLE, Rect1, this, IDC_SHOW_MAP_ZALDIVAR_ITEM);
	//Rgn.SetRectRgn(&Rect1);
	//m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	::SetRect(&Rect1, m_w - 2 - w, m_h - 8 - 4 * h, m_w - 2 - w + smallW, m_h - 6 - 2 * h);
	m_SaveScreenButton.Create(_T(""), _T(""), WS_CHILD, Rect1, this, IDC_SAVE_SCREEN_BUTTON);
	m_SaveScreenButton.SetTip("Save Screen");
	m_SaveScreenButton.SetImage(m_q == 2 ? IDR_SAVE_SCREEN_3 : m_q == 1 ? IDR_SAVE_SCREEN_2 : IDR_SAVE_SCREEN_1);
	m_SaveScreenButton.SetBkColor(BLACK);
	m_SaveScreenButton.ShowWindow(SW_SHOW);
	//Rgn.SetRectRgn(&Rect1);
	//m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);

	::SetRect(&Rect1, m_w - 2 - smallW, m_h - 8 - 4 * h, m_w - 2, m_h - 6 - 2 * h);
	m_PrintScreenButton.Create(_T(""), _T(""), WS_CHILD, Rect1, this, IDC_PRINT_SCREEN_BUTTON);
	m_PrintScreenButton.SetTip("Print Screen");
	m_PrintScreenButton.SetImage(m_q == 2 ? IDR_PRINT_SCREEN_3 : m_q == 1 ? IDR_PRINT_SCREEN_2 : IDR_PRINT_SCREEN_1);
	m_PrintScreenButton.SetBkColor(BLACK);
	m_PrintScreenButton.ShowWindow(SW_SHOW);
	//Rgn.SetRectRgn(&Rect1);
	//m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);


	::SetRect(&m_IOLPlaceTextRect, m_w - 2 - intRound(1.35*w), m_h - 8 - 9 * h, m_w - 2, m_h - 8 - 8 * h);//531

	::SetRect(&m_Ratio1Rect, m_w - 2 - w, m_h - 8 - 7 * h, m_w + 14 - w, m_h - 8 - 6 * h);
	m_ImgMouseCotrol.Create(_T(""), _T(""), 0, m_Ratio1Rect, this, IDC_IMG_MOUSE_CONTROL_ITEM);
	m_ImgMouseCotrol.SetImage(IDR_ACCEPT0);
	m_ImgMouseCotrol.SetBk(FALSE);
	m_ImgMouseCotrol.SetBkColor(BLACK);
	m_ImgMouseCotrol.ShowWindow(SW_SHOW);
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);


	::SetRect(&m_Ratio2Rect, m_w - 2 - w, m_h - 8 - 6 * h, m_w + 14 - w, m_h - 8 - 5 * h);
	m_RingMouseCotrol.Create(_T(""), _T(""), 0, m_Ratio2Rect, this, IDC_IMG_MOUSE_CONTROL_ITEM);
	m_RingMouseCotrol.SetImage(IDR_ACCEPT0);
	m_RingMouseCotrol.SetBk(FALSE);
	m_RingMouseCotrol.SetBkColor(BLACK);
	m_RingMouseCotrol.ShowWindow(SW_HIDE);
	Rgn.SetRectRgn(&Rect1);
	m_Rgn.CombineRgn(&m_Rgn, &Rgn, RGN_DIFF);


	::SetRect(&m_Ratio1TextRect, m_w + 11 - w, m_h - 8 - 7 * h, m_w - 2, m_h - 8 - 6 * h);
	::SetRect(&m_Ratio2TextRect, m_w + 11 - w, m_h - 8 - 6 * h, m_w - 2, m_h - 8 - 5 * h);

	::SetRect(&m_PNameTextRect, m_w - 5 * w, 3, m_w - 8, intRound(1.5*h));
	::SetRect(&m_pIDTextRect, m_w - 5 * w, intRound(1.5*h), m_w - 8, 3 * h);
	::SetRect(&m_pODOSTextRect, m_w - 2 - w, 3 * h, m_w - 2, intRound(4.5*h));//531

	::SetRect(&m_LRulerText1Rect, 8, m_h - 8 - 5 * h, 2 * w, m_h - 8 - 4 * h);
	::SetRect(&m_LRulerText2Rect, 8, m_h - 8 - 4 * h, 2 * w, m_h - 8 - 3 * h);
	::SetRect(&m_RRulerText1Rect, 8, m_h - 8 - 2 * h, 2 * w, m_h - 8 - h);
	::SetRect(&m_RRulerText2Rect, 8, m_h - 8 - h, 2 * w, m_h - 8);

	Repaint();
	ShowWindow(SW_SHOW);
}

void CPlanBigWnd::RealseMem()
{
	for (int t = 0; t < 15; t++)
	{
		SAFE_DELETE_ARRAY(m_pZoomBackup[t]);
	}
}

//***************************************************************************************

void CPlanBigWnd::RepaintMemDC()
{
	ClearMemDC();
	Eye();
	Caliper();
	Grid();
	Limbus();
	Pupil();
	RadialRuler();
	LinearRuler();
	Labels();

	AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 1);
	AlignLineDeg(m_a1, m_a3, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 2);
	AlignLineDeg(m_a1, m_a4, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 3);

	AlignLine(m_a1, m_Ali_Rum1, 0, "0");
	AlignLine(m_a2, m_Ali_Rum2, 1, "1");
	AlignLine(m_a3, m_Ali_Rum3, 1, "2");
	AlignLine(m_a4, m_Ali_Rum4, 1, "3");

	AlignHoyaLine(m_a1, m_Ali_Rum1);
	AlignHoyaLine(m_a2, m_Ali_Rum2);

	Points();//Show the Pupil, limbus centers and vertex(light reflexes)
}

//***************************************************************************************

void CPlanBigWnd::CreateZoomImg()
{
	if (m_pImage == NULL) return;
	if (m_pImage->m_RGBData.GetMem() == NULL) return;

	m_pZoomImage2.m_w = m_w;
	m_pZoomImage2.m_h = m_h;
	m_pZoomImage2.m_RGBData.Create(m_h, m_w * 3, NULL);


	m_EyeWndTop = 1000;
	m_EyeWndLeft = 1000;
	m_EyeWndButtom = 0;
	m_EyeWndRight = 0;
	for (int y = 0; y < m_h; y++)
	{
		real_t y_um = -(m_cy - y) / m_y_px_um;
		for (int x = 0; x < m_w; x++)
		{
			real_t x_um = +(x - m_cx) / m_x_px_um;
			real_t r, g, b;
			if (m_pImage->GetRGBAtUm(x_um, y_um, &r, &g, &b))
			{

				m_MemDC.SetPixel(x, m_h - y - 1, (int)r, (int)g, (int)b);
				if (y < m_EyeWndTop)    m_EyeWndTop = y;
				if (x < m_EyeWndLeft)   m_EyeWndLeft = x;
				if (y > m_EyeWndButtom) m_EyeWndButtom = y;
				if (x > m_EyeWndRight)  m_EyeWndRight = x;
			}
		}
	}

	m_EyeWndButtom = m_h;
	m_EyeWndWidth = m_EyeWndRight - m_EyeWndLeft;
	m_EyeWndHeight = m_EyeWndButtom - m_EyeWndTop;

	memcpy(m_pZoomImage2.m_RGBData, m_MemDC.m_RGBData, m_ImgSize);

	m_Adx = m_EyeWndLeft + (real_t)m_EyeWndWidth / 2.0 - m_cx;
	m_Ady = m_EyeWndTop + (real_t)m_EyeWndHeight / 2.0 - m_cy;
}

//***************************************************************************************

void CPlanBigWnd::Eye()
{
	int t = m_wheel_n;

	if (t == 0)
	{
		memcpy(m_MemDC.m_RGBData, m_pZoomImage2.m_RGBData, m_ImgSize);
		m_mouseCap = FALSE;
		for (int i = 1; i<15; i++) m_BackupYes[i] = FALSE;
		memcpy(m_pZoomBackup[0], m_pZoomImage2.m_RGBData, m_ImgSize);

		m_cx = 0.5 * m_w;
		m_cy = 0.5 * m_h;
	}
	else
	{
		if (m_BackupYes[t])
		{
			memcpy(m_MemDC.m_RGBData, m_pZoomBackup[t], m_ImgSize);
			m_cx = m_cxBackup[t];
			m_cy = m_cyBackup[t];
		}
		else
		{
			real_t zoomRatio = 1;

			for (int i = 0; i<t; i++)
			{
				zoomRatio *= 1.05;
			}

			real_t actuaLMoveX = intRound((real_t)m_MovePoint.x / zoomRatio);
			real_t actualMoveY = intRound((real_t)m_MovePoint.y / zoomRatio);

			int  zoomWidth = intRound((real_t)m_EyeWndWidth / zoomRatio);
			int  zoomHeight = intRound((real_t)m_EyeWndHeight / zoomRatio);

			//Find the center pos
			int  zoomOutWidth = intRound((real_t)m_EyeWndWidth*zoomRatio);
			int  zoomOutHeight = intRound((real_t)m_EyeWndHeight*zoomRatio);

			int  zoomOutLeft, zoomOutTop;

			zoomOutLeft = m_EyeWndLeft - intRound((real_t)(zoomOutWidth - m_EyeWndWidth) / 2.0);
			zoomOutTop = m_EyeWndTop - intRound((real_t)(zoomOutHeight - m_EyeWndHeight) / 2.0);
			zoomOutLeft += m_MovePoint.x;
			zoomOutTop += m_MovePoint.y;
			//Done

			int  zoomLeft, zoomTop;

			zoomLeft = m_EyeWndLeft + intRound((real_t)(m_EyeWndWidth - zoomWidth) / 2.0);
			zoomTop = m_EyeWndTop + intRound((real_t)(m_EyeWndHeight - zoomHeight) / 2.0);

			zoomLeft -= (int)actuaLMoveX;
			zoomTop -= (int)actualMoveY;

			if (zoomLeft - m_EyeWndLeft < 0)
			{
				zoomLeft = m_EyeWndLeft;
				zoomOutLeft = m_EyeWndLeft;
			}
			if (zoomTop - m_EyeWndTop  < 0)
			{
				zoomTop = m_EyeWndTop;
				zoomOutTop = m_EyeWndTop;
			}

			if (zoomLeft + zoomWidth  > m_EyeWndWidth + m_EyeWndLeft)
			{
				zoomLeft = m_EyeWndWidth - zoomWidth + m_EyeWndLeft;
				//zoomOutLeft = zoomOutLeft - intRound((real)(zoomOutWidth  - m_EyeWndWidth)/2.0);
				zoomOutLeft = m_EyeWndLeft - zoomOutWidth + m_EyeWndWidth;
			}
			if (zoomTop + zoomHeight > m_EyeWndHeight + m_EyeWndTop)
			{
				zoomTop = m_EyeWndHeight - zoomHeight + m_EyeWndTop;
				zoomOutTop = m_EyeWndHeight - zoomOutHeight + m_EyeWndTop;
			}

			memcpy(m_MemDC.m_RGBData, m_pZoomImage2.m_RGBData, m_ImgSize);
			m_MemDC.StretchBlt(m_EyeWndLeft, m_EyeWndTop, m_EyeWndWidth, m_EyeWndHeight, &m_MemDC, zoomLeft, zoomTop, zoomWidth, zoomHeight, SRCCOPY);

			if (!m_BackupYes[t])
			{
				memcpy(m_pZoomBackup[t], m_MemDC.m_RGBData, m_ImgSize);
				m_BackupYes[t] = TRUE;

				m_cxBackup[t] = zoomOutLeft + intRound((real_t)(zoomOutWidth) / 2.0) - m_Adx;
				m_cyBackup[t] = zoomOutTop + intRound((real_t)(zoomOutHeight) / 2.0) - m_Ady;

				m_cx = m_cxBackup[t];
				m_cy = m_cyBackup[t];
			}
		}
	}
}

//***************************************************************************************

void CPlanBigWnd::Grid()
{
	real_t x_px_um = m_x_px_um;
	real_t y_px_um = m_y_px_um;

	COLORREF Color = m_Printing ? BLACK : WHITE;

	CString s;

	int FontSize = intRound(0.045 * m_h);
	int FontSize2 = intRound(0.025 * m_h);

	real_t r_um = 4000.0;

	real_t d_efX = 0;
	real_t d_efY = 0;

	if (m_pImage->m_ve_ok)
	{
		d_efX = m_pImage->m_ve_x_um* m_x_px_um;
		d_efY = m_pImage->m_ve_y_um* m_y_px_um;
	}

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	// circle
	int rx;
	int ry;

	if (m_ShowCaliper)
	{
		rx = intRound(r_um * m_x_px_um);
		ry = intRound(r_um * m_y_px_um);
		m_MemDC.DrawEllipse(intRound(m_cx + d_efX*zoomRatio), intRound(m_cy - d_efY*zoomRatio), rx, ry, 3, Color, NOCOLOR);
	}

	r_um += m_wheel_n2 * 100.0;

	// circle
	rx = intRound(r_um * m_x_px_um);
	ry = intRound(r_um * m_y_px_um);
	m_MemDC.DrawEllipse(intRound(m_cx + d_efX*zoomRatio), intRound(m_cy - d_efY*zoomRatio), rx, ry, 3, Color, NOCOLOR);

	// numbers
	int IncreaseDeg = 5;
	if (m_wheel_n2 >= 10) IncreaseDeg = 1;//[cjf***04202012]  

	int DegTextStep = 30;
	if (m_wheel_n2 >= 20) DegTextStep = 5;//[cjf***04202012]  

	for (int a = 0; a < 360; a += IncreaseDeg)
	{
		real_t r1_um = r_um + 100.0;
		int x1 = intRound(m_cx + d_efX*zoomRatio + x_px_um * r1_um * COS[a]);
		int y1 = intRound(m_cy - d_efY*zoomRatio - y_px_um * r1_um * SIN[a]);
		real_t r2_um = 0;

		if (IncreaseDeg == 1) r2_um = r_um + ((a % 15) == 0 ? 400.0 : 150.0);//[cjf***04202012]
		else r2_um = r_um + ((a % 15) == 0 ? 400.0 : 250.0);//[cjf***04202012]

		if (IncreaseDeg == 1 && (a % 5) == 0 && (a % 15) != 0)	r2_um = r_um + 250;//[cjf***04202012] 

		int x2 = intRound(m_cx + d_efX*zoomRatio + x_px_um * r2_um * COS[a]);
		int y2 = intRound(m_cy - d_efY*zoomRatio - y_px_um * r2_um * SIN[a]);
		int w = intRound(((a % 15) == 0 ? 0.006 : 0.003) * m_h);

		m_MemDC.DrawLine(x1, y1, x2, y2, w, Color);

		int sa = 0;
		if (a % DegTextStep == 0)
		{
			sa = a;
			if (::TempSettings.Temp_ScaleRang)
			{
				if (a > 180) sa = a - 180;
			}
			if (DegTextStep == 30)
			{
				real_t r3_um = r_um + 900.0;
				int x = intRound(m_cx + d_efX*zoomRatio + x_px_um * r3_um * COS[a]);
				int y = intRound(m_cy - d_efY*zoomRatio - y_px_um * r3_um * SIN[a]);
				s.Format(_T(" %i°"), sa);
				m_MemDC.WriteRotatedText(s, x, y, a - 90, FontSize, 400, "Arial", Color, NOCOLOR);
			}
			else
			{
				if (a % 15 == 0)
				{
					real_t r3_um = r_um + 900.0;
					int x = intRound(m_cx + d_efX*zoomRatio + x_px_um * r3_um * COS[a]);
					int y = intRound(m_cy - d_efY*zoomRatio - y_px_um * r3_um * SIN[a]);
					s.Format(_T(" %i°"), sa);
					m_MemDC.WriteRotatedText(s, x, y, a - 90, FontSize, 400, "Arial", Color, NOCOLOR);
				}
				else
				{
					real_t r3_um = r_um + 500.0;
					int x = intRound(m_cx + d_efX*zoomRatio + x_px_um * r3_um * COS[a]);
					int y = intRound(m_cy - d_efY*zoomRatio - y_px_um * r3_um * SIN[a]);
					s.Format(_T(" %i°"), sa);
					m_MemDC.WriteRotatedText(s, x, y, a - 90, FontSize2, 400, "Arial", Color, NOCOLOR);
				}
			}

		}
	}
}

//***************************************************************************************

void CPlanBigWnd::Caliper()
{
	if (!m_ShowCaliper) return;
	if (m_isCombo) return;//531

	real_t x_px_um = m_x_px_um;
	real_t y_px_um = m_y_px_um;

	CString s;

	int FontSize = intRound(0.045 * m_h);

	real_t d_efX = 0;
	real_t d_efY = 0;

	if (m_pImage->m_ve_ok)
	{
		d_efX = m_pImage->m_ve_x_um* m_x_px_um;
		d_efY = m_pImage->m_ve_y_um* m_y_px_um;
	}

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	int aw[2] = { m_pCTExam->m_OpData.m_RefAxis[0], m_pCTExam->m_OpData.m_RefAxis[1] };

	if (aw[0] == INVALID_VALUE) {
		if (aw[1] == INVALID_VALUE) return;
		aw[0] = aw[1];
	}
	else if (aw[1] == INVALID_VALUE) {
		aw[1] = aw[0];
	}
	aw[0] %= 360; if (aw[0] < 0) aw[0] += 360;
	aw[1] %= 360; if (aw[1] < 0) aw[1] += 360;

	for (int i = 0; i < 2; i++) {
		int x1 = intRound(m_cx + d_efX*zoomRatio + 4100.0 * COS[aw[i]] * x_px_um);
		int y1 = intRound(m_cy - d_efY*zoomRatio - 4100.0 * SIN[aw[i]] * y_px_um);
		int x2 = intRound(m_cx + d_efX*zoomRatio + 5400.0 * COS[aw[i]] * x_px_um);
		int y2 = intRound(m_cy - d_efY*zoomRatio - 5400.0 * SIN[aw[i]] * y_px_um);
		int w = intRound(0.01 * m_h);
		if (m_OverLap) m_MemDC.DrawLine(x1, y1, x2, y2, w, m_CaliperColor2);
		else m_MemDC.DrawLine(x1, y1, x2, y2, w, m_CaliperColor);
	}

	int ar[2];
	if (m_pCTExam->m_OpData.m_OpType == 0) {
		ar[0] = m_pCTExam->m_OpData.m_IOLPlacementAxis;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 1) {
		ar[0] = (m_pCTExam->m_OpData.m_InternalPostopAxis + 90) % 180;
	}
	else if (m_pCTExam->m_OpData.m_OpType == 2) {
		ar[0] = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis;
	}
	else return;
	ar[0] %= 180; if (ar[0] < 0) ar[0] += 180;
	ar[1] = ar[0] + 180;

	for (int i = 0; i < 2; i++) {
		int x1 = intRound(m_cx + d_efX*zoomRatio + 4100.0 * COS[ar[i]] * x_px_um);
		int y1 = intRound(m_cy - d_efY*zoomRatio - 4100.0 * SIN[ar[i]] * y_px_um);
		int x2 = intRound(m_cx + d_efX*zoomRatio + 5400.0 * COS[ar[i]] * x_px_um);
		int y2 = intRound(m_cy - d_efY*zoomRatio - 5400.0 * SIN[ar[i]] * y_px_um);
		int w = intRound(0.01 * m_h);
		m_MemDC.DrawLine(x1, y1, x2, y2, w, m_LensColor);
	}

	for (int i = 0; i < 2; i++) {

		int a1, a2;

		if (angular_distance_counterclockwise(aw[i], ar[i]) < 180) {
			a1 = aw[i];
			a2 = ar[i];
		}
		else {
			a1 = ar[i];
			a2 = aw[i];
		}

		int x = intRound(m_cx + d_efX*zoomRatio);
		int y = intRound(m_cy - d_efY*zoomRatio);
		int r = intRound(5500.0 * m_x_px_um);
		int w = intRound(0.01 * m_h);
		m_MemDC.DrawArc(x, y, r, w, a1, a2, m_CaliperColor);

		// текст
		int amin = std::min(aw[i], ar[i]);
		int amax = std::max(aw[i], ar[i]);
		int a0 = (amin + amax) / 2;
		if (amax - amin > 180) { // лежат по разные стороны нуля
			a0 = (a0 + 180) % 360;
		}
		x = intRound(m_cx + d_efX*zoomRatio + 6000.0 * COS[a0] * x_px_um);
		y = intRound(m_cy - d_efY*zoomRatio - 6000.0 * SIN[a0] * y_px_um);
		s.Format(_T("%i°"), angular_distance(aw[i], ar[i]));
		int o = CheckAngle((a0 - 90) - 1);
		m_MemDC.WriteRotatedText(s, x, y, o, FontSize, 400, "Arial", m_CaliperColor, NOCOLOR);
	}
}

//***************************************************************************************
//531
void CPlanBigWnd::AlignLineDeg(int a1, int a2, real_t Ali_Rum1, real_t Ali_Rum2, real_t Ali_Rum3, real_t Ali_Rum4, int Type)
{
	int oria1 = a1;
	int oria2 = a2;

	if (!m_AlignLine || a2 == -1 || !m_isCombo) return;

	if (Ali_Rum1 == 0 || Ali_Rum2 == 0) return;

	int aDis = abs(a1 - a2);

	if (aDis > 90)
	{
		int a1Test = (a1 + 180) % 360;

		if (a1Test > 270 && a2 < 90 && (a2 + 360 - a1Test) <= 90)
		{
			a1 = a1Test;
			oria1 = a1;
		}
		else
		{
			if (abs(a1Test - a2) > 90)
			{
				int amid = a2;
				a2 = a1;
				a1 = amid;
			}
			else
			{
				a1 = a1Test;
				oria1 = a1;
			}
		}
	}

	int a01, a02;
	a01 = a1;
	a02 = a2;

	if (!(angular_distance_counterclockwise(a1, a2) < 180))
	{
		a01 = a2;
		a02 = a1;
	}

	real_t d_efX = 0;
	real_t d_efY = 0;

	//if(m_pImage->m_ve_ok)
	{
		d_efX = m_pImage->m_ve_x_um* m_x_px_um;
		d_efY = m_pImage->m_ve_y_um* m_y_px_um;
	}

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	int  x = intRound(m_cx + d_efX*zoomRatio);
	int  y = intRound(m_cy - d_efY*zoomRatio);

	int  w = intRound(0.002 * m_h);
	real_t r_ratio = 0.9;
	switch (Type)
	{
	case 1: r_ratio = 0.9;
		break;
	case 2: r_ratio = 0.75;
		break;
	case 3: r_ratio = 0.6;
		break;
	}
	real_t minRum = Ali_Rum1 < Ali_Rum2 ? Ali_Rum1 : Ali_Rum2;

	if (Ali_Rum3 != 0) minRum = minRum < Ali_Rum3 ? minRum : Ali_Rum3;
	if (Ali_Rum4 != 0) minRum = minRum < Ali_Rum4 ? minRum : Ali_Rum4;

	int  r = intRound(r_ratio*(minRum)*zoomRatio* m_x_px_um);

	m_MemDC.DrawArc(x, y, r, w, a01, a02, MAGENTA);

	//Degree text
	int  ADis = SmallAng(a1, a2);
	CString s;
	s.Format(_T("%i°"), ADis);

	int amin = std::min(a01, a02);
	int amax = std::max(a01, a02);
	int a03 = (amin + amax) / 2;

	if (amax - amin > 180) { // лежат по разные стороны нуля
		a03 = (a03 + 180) % 360;
	}
	x = intRound(m_cx + d_efX*zoomRatio + (r + 15) * COS[a03]);
	y = intRound(m_cy - d_efY*zoomRatio - (r + 15) * SIN[a03]);

	int FontSize = intRound(0.03 * m_h);
	int o = CheckAngle((a03 - 90) - 1);

	m_MemDC.WriteRotatedText(s, x, y, o, FontSize, 400, "Arial", RED, NOCOLOR);

	//Another degree line and Degree
	if (m_a4 == -1)
	{
		x = intRound(m_cx + d_efX*zoomRatio);
		y = intRound(m_cy - d_efY*zoomRatio);
		oria1 = (oria1 + 180) % 360;
		a01 = oria1;
		a02 = oria2;

		if (!(angular_distance_counterclockwise(oria1, oria2) < 180))
		{
			a01 = oria2;
			a02 = oria1;
		}

		if (a01 > a02)
		{
			if (a01 > 180) a01 -= 360;
			else
			{
				a02 = oria2;
				a01 = oria1;
			}
		}

		t = 0;
		for (int i = a01; i <= a02; i = i + 5)
		{

			if (t == 0)
			{
				int stAngle = i + 5;
				if (stAngle > a02) stAngle = a02;
				m_MemDC.DrawArc(x, y, r, w, i, stAngle, BROWN);
			}

			if (t == 0) t = 1;
			else t = 0;
		}

		//Degree text
		ADis = SmallAng(oria1, oria2);
		s.Format(_T("%i°"), ADis);

		amin = std::min(a01, a02);
		amax = std::max(a01, a02);
		a03 = (amin + amax) / 2;

		if (amax - amin > 180) { // лежат по разные стороны нуля
			a03 = (a03 + 180) % 360;
		}
		if (a03 < 0) a03 += 360;
		x = intRound(m_cx + d_efX*zoomRatio + (r + 15) * COS[a03]);
		y = intRound(m_cy - d_efY*zoomRatio - (r + 15) * SIN[a03]);

		o = CheckAngle((a03 - 90) - 1);

		m_MemDC.WriteRotatedText(s, x, y, o, FontSize, 400, "Arial", RED, NOCOLOR);
	}
	//Done
}
//***************************************************************************************

void CPlanBigWnd::AlignHoyaLine(int a, real_t Ali_Rum)
{
	if (m_isCombo)       return;
	if (!m_ShowCaliper)  return;
	if (!m_AlignLine)    return;

	real_t x_px_um = m_x_px_um;
	real_t y_px_um = m_y_px_um;

	real_t d_efX = 0;
	real_t d_efY = 0;

	if (m_pImage->m_ve_ok)
	{
		d_efX = m_pImage->m_ve_x_um* m_x_px_um;
		d_efY = m_pImage->m_ve_y_um* m_y_px_um;
	}

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	int x1 = intRound(m_cx + d_efX*zoomRatio);
	int y1 = intRound(m_cy - d_efY*zoomRatio);
	int x2 = intRound(m_cx + d_efX*zoomRatio + Ali_Rum * zoomRatio * COS[a] * x_px_um);
	int y2 = intRound(m_cy - d_efY*zoomRatio - Ali_Rum * zoomRatio * SIN[a] * y_px_um);
	int w = intRound(0.005 * m_h);

	m_MemDC.DrawLine(x1, y1, x2, y2, w, m_CaliperColor);
	m_MemDC.DrawCircle(x2, y2, w, w, m_CaliperColor, m_CaliperColor);
}

//***************************************************************************************

void CPlanBigWnd::AlignLine(int a, real_t Ali_Rum, int Type, CString whichLine)//Type: 0: Base Line; 1:Protractor Line; | WhcichLine : 1, 2, 3
{
	if (a == -1 || !m_AlignLine || !m_isCombo || Ali_Rum == 0) return;

	real_t x_px_um = m_x_px_um;
	real_t y_px_um = m_y_px_um;

	real_t d_efX = 0;
	real_t d_efY = 0;

	//if(m_pImage->m_ve_ok)
	{
		d_efX = m_pImage->m_ve_x_um* m_x_px_um;
		d_efY = m_pImage->m_ve_y_um* m_y_px_um;
	}

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	int x1 = intRound(m_cx + d_efX*zoomRatio);
	int y1 = intRound(m_cy - d_efY*zoomRatio);

	if (Type == 0)
	{
		int a2 = (a + 180) % 360;
		x1 = intRound(m_cx + d_efX*zoomRatio + Ali_Rum *zoomRatio * COS[a2] * x_px_um);
		y1 = intRound(m_cy - d_efY*zoomRatio - Ali_Rum *zoomRatio * SIN[a2] * y_px_um);
	}

	int x2 = intRound(m_cx + d_efX*zoomRatio + Ali_Rum *zoomRatio * COS[a] * x_px_um);
	int y2 = intRound(m_cy - d_efY*zoomRatio - Ali_Rum *zoomRatio * SIN[a] * y_px_um);

	int w = intRound(0.005 * m_h);


	COLORREF       UsedColor = MAGENTA;
	if (Type != 0)  UsedColor = CYAN;

	m_MemDC.DrawLine(x1, y1, x2, y2, w, UsedColor);
	m_MemDC.DrawCircle(x2, y2, w, w, UsedColor, UsedColor);

	int fs = 25;
	if (Type != 0)
	{
		int angle = a;//

		int angle2 = angle;

		if (angle2 < 0) angle2 = 360 + angle2;

		if (x2 < x1) angle += 180;

		int x = intRound(x2 + 20 * SIN[angle2]);
		int y = intRound(y2 + 20 * COS[angle2]);

		m_MemDC.WriteRotatedText(whichLine, x, y, angle, fs, 400, "Arial", RED, NOCOLOR);
	}
	else m_MemDC.DrawCircle(x1, y1, w, w, UsedColor, UsedColor);
}

//***************************************************************************************

void CPlanBigWnd::Pupil()
{
	if (!m_ShowPupil) return;
	if (!m_pImage->m_pu_ok) return;

	int x0, y0;

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	real_t x0R = +m_pImage->m_pu_x0_um * m_x_px_um;
	real_t y0R = -m_pImage->m_pu_y0_um * m_y_px_um;

	x0 = intRound(m_cx + x0R*zoomRatio);
	y0 = intRound(m_cy + y0R*zoomRatio);

	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, GREEN);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, GREEN);

	for (int a = 0; a < 360; a++)
	{
		int b = a == 359 ? 0 : a + 1;

		if (m_pImage->m_pu_r_um[a] == INVALID_VALUE) continue;
		if (m_pImage->m_pu_r_um[b] == INVALID_VALUE) continue;

		real_t xa0 = +(m_pImage->m_pu_x0_um + m_pImage->m_pu_r_um[a] * COS[a])* m_x_px_um;
		real_t ya0 = -(m_pImage->m_pu_y0_um + m_pImage->m_pu_r_um[a] * SIN[a])* m_y_px_um;
		real_t xb0 = +(m_pImage->m_pu_x0_um + m_pImage->m_pu_r_um[b] * COS[b])* m_x_px_um;
		real_t yb0 = -(m_pImage->m_pu_y0_um + m_pImage->m_pu_r_um[b] * SIN[b])* m_y_px_um;

		int xa = intRound(m_cx + xa0*zoomRatio);
		int ya = intRound(m_cy + ya0*zoomRatio);
		int xb = intRound(m_cx + xb0*zoomRatio);
		int yb = intRound(m_cy + yb0*zoomRatio);

		m_MemDC.DrawLine(xa, ya, xb, yb, 3, GREEN);
	}
}

//***************************************************************************************

void CPlanBigWnd::RadialRuler()
{
	if (!m_ShowRRuler) return;

	if (m_pImage->m_Ra_ok != TRUE)  return;

	int x0, y0;

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	real_t x0R = +m_pImage->m_Ra_x_um * m_x_px_um;
	real_t y0R = -m_pImage->m_Ra_y_um * m_y_px_um;

	x0 = intRound(m_cx + x0R*zoomRatio);
	y0 = intRound(m_cy + y0R*zoomRatio);

	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, YELLOW);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, YELLOW);

	// 
	for (int a = 0; a < 360; a++)
	{
		int b = a == 359 ? 0 : a + 1;
		int xa = intRound(m_cx + (m_pImage->m_Ra_x_um + m_pImage->m_Ra_r_um * COS[a]) * m_x_px_um * zoomRatio);
		int ya = intRound(m_cy - (m_pImage->m_Ra_y_um + m_pImage->m_Ra_r_um * SIN[a]) * m_y_px_um * zoomRatio);
		int xb = intRound(m_cx + (m_pImage->m_Ra_x_um + m_pImage->m_Ra_r_um * COS[b]) * m_x_px_um * zoomRatio);
		int yb = intRound(m_cy - (m_pImage->m_Ra_y_um + m_pImage->m_Ra_r_um * SIN[b]) * m_y_px_um * zoomRatio);
		m_MemDC.DrawLine(xa, ya, xb, yb, 3, YELLOW);
	}
}

//***************************************************************************************

void CPlanBigWnd::LinearRuler()
{
	if (!m_ShowLRuler) return;

	if (m_pImage->m_LRa_ok != TRUE) return;
	//
	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	COLORREF LRulerColor = RGB(255, 128, 0);

	int x0 = intRound(m_cx + (m_pImage->m_LRa_x0_um) * m_x_px_um * zoomRatio);
	int y0 = intRound(m_cy - (m_pImage->m_LRa_y0_um) * m_y_px_um * zoomRatio);
	int x1 = intRound(m_cx + (m_pImage->m_LRa_x1_um) * m_x_px_um * zoomRatio);
	int y1 = intRound(m_cy - (m_pImage->m_LRa_y1_um) * m_y_px_um * zoomRatio);

	m_MemDC.DrawCircle(x0, y0, 2, 0, LRulerColor, LRulerColor);
	m_MemDC.DrawCircle(x1, y1, 2, 0, LRulerColor, LRulerColor);
	m_MemDC.DrawLine(x0, y0, x1, y1, 2, LRulerColor);
}

//***************************************************************************************

void CPlanBigWnd::Limbus()
{
	if (!m_ShowLimbus) return;
	if (!m_pImage->m_li_ok) return;

	int x0, y0;

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	real_t x0R = +m_pImage->m_li_x0_um * m_x_px_um;
	real_t y0R = -m_pImage->m_li_y0_um * m_y_px_um;

	x0 = intRound(m_cx + x0R*zoomRatio);
	y0 = intRound(m_cy + y0R*zoomRatio);

	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, BLUE);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, BLUE);

	for (int a = 0; a < 360; a++)
	{
		int b = a == 359 ? 0 : a + 1;

		real_t xa0 = +(m_pImage->m_li_x0_um + m_pImage->m_li_r_mean_um * COS[a])* m_x_px_um;
		real_t ya0 = -(m_pImage->m_li_y0_um + m_pImage->m_li_r_mean_um * SIN[a])* m_y_px_um;
		real_t xb0 = +(m_pImage->m_li_x0_um + m_pImage->m_li_r_mean_um * COS[b])* m_x_px_um;
		real_t yb0 = -(m_pImage->m_li_y0_um + m_pImage->m_li_r_mean_um * SIN[b])* m_y_px_um;

		int xa = intRound(m_cx + xa0*zoomRatio);
		int ya = intRound(m_cy + ya0*zoomRatio);
		int xb = intRound(m_cx + xb0*zoomRatio);
		int yb = intRound(m_cy + yb0*zoomRatio);

		m_MemDC.DrawLine(xa, ya, xb, yb, 3, BLUE);
	}
}

//***************************************************************************************

void CPlanBigWnd::Points()
{
	int x0, y0;
	real_t d_efX = m_pImage->m_ve_x_um* m_x_px_um;
	real_t d_efY = m_pImage->m_ve_y_um* m_y_px_um;

	int t = m_wheel_n;
	real_t  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	if (m_pImage->m_ve_ok)
	{
		x0 = intRound(m_cx + d_efX*zoomRatio);
		y0 = intRound(m_cy - d_efY*zoomRatio);

		m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, RED);
		m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, RED);
	}
}

//***************************************************************************************

void CPlanBigWnd::Labels()
{
	//int FontSize = intRound(0.045 * m_h);
	CMFont      LabelsFont, LabelsFont2;
	LabelsFont.Create(intRound(0.02 * m_h), 400, "Arial");
	LabelsFont2.Create(intRound(0.03 * m_h), 400, "Arial");

	if (m_ShowDymImg)
		m_MemDC.WriteText("  Dynamic Image", m_Ratio1TextRect, LabelsFont, RED, 0, BLACK);
	else
		m_MemDC.WriteText("  Dynamic Image", m_Ratio1TextRect, LabelsFont, WHITE, 0, BLACK);

	if (m_ShowOsherRing)
		m_MemDC.WriteText("  Dynamic Scale", m_Ratio2TextRect, LabelsFont, RED, 0, BLACK);
	else
		m_MemDC.WriteText("  Dynamic Scale", m_Ratio2TextRect, LabelsFont, WHITE, 0, BLACK);

	//The patient name and id
	CString PName, ID, OD;
	CPatient* pPatient = ::DB.GetPatient(m_pCTExam->m_Header.m_PatientID);

	PName = G_ReadChinese(pPatient->m_FirstName) + " " + G_ReadChinese(pPatient->m_MiddleName) + " " + G_ReadChinese(pPatient->m_LastName);

	ID = pPatient->m_CustomID;
	if (!ID.IsEmpty()) ID = "ID: " + ID;
	m_MemDC.WriteText(PName, m_PNameTextRect, LabelsFont2, WHITE, 2, BLACK);
	m_MemDC.WriteText(ID, m_pIDTextRect, LabelsFont2, WHITE, 2, BLACK);
	OD = m_pCTExam->m_Header.IsOD() ? "OD" : "OS";
	m_MemDC.WriteText(OD, m_pODOSTextRect, LabelsFont2, WHITE, 1, BLACK);

	//531 IOL Placement Axis	
	if (m_isCombo)
	{
		CString s;

		s.Format(_T("Axis of Placement = %i°"), m_pCTExam->m_OpData.m_IOLPlacementAxis);
		m_MemDC.DrawRectangle(m_IOLPlaceTextRect, WHITE, NOCOLOR);
		m_MemDC.WriteText(s, m_IOLPlaceTextRect, LabelsFont, WHITE, 1, NOCOLOR);
	}
	//531 Done

	//The linear ruler and radial ruler
	CString s1, s2;

	if (m_ShowLRuler && m_pImage->m_LRa_ok == TRUE)
	{
		COLORREF LRulerColor = RGB(255, 128, 0);
		real_t dis = hyp(m_pImage->m_LRa_x0_um - m_pImage->m_LRa_x1_um, m_pImage->m_LRa_y0_um - m_pImage->m_LRa_y1_um);
		if (dis > 0)
		{
			s1 = "Linear Ruler";
			s2.Format(_T("(D) = (%.3fmm)"), 0.001*dis);

			m_MemDC.WriteText(s1, m_LRulerText1Rect, LabelsFont, LRulerColor, 0, NOCOLOR);
			m_MemDC.WriteText(s2, m_LRulerText2Rect, LabelsFont, LRulerColor, 0, NOCOLOR);
		}
		else
		{
			m_pImage->m_LRa_ok = FALSE;
		}
	}

	if (m_pImage->m_Ra_ok == TRUE && m_pImage->m_Ra_r_um <= 0) m_pImage->m_Ra_ok = FALSE;

	if (m_ShowRRuler && m_pImage->m_Ra_ok == TRUE)
	{
		s1 = "Radial Ruler";
		s2.Format(_T("(D) = (%0.3fmm)"), m_pImage->m_Ra_r_um*0.002);

		m_MemDC.WriteText(s1, m_RRulerText1Rect, LabelsFont, YELLOW, 0, NOCOLOR);
		m_MemDC.WriteText(s2, m_RRulerText2Rect, LabelsFont, YELLOW, 0, NOCOLOR);
	}
}

//***************************************************************************************
void CPlanBigWnd::OnShow(BOOL* check)
{
	if (!check) return;

	*check = !*check;
}

//***************************************************************************************

//void CPlanBigWnd::OnLButtonDblClk(uint nFlags, CPoint Point)
//{
//  GetParent()->PostMessage(WM_NOTIFY_LBTN_DBCLK, (WPARAM)0, (LPARAM)0);    
//}

//***************************************************************************************

void  CPlanBigWnd::OnMouseWheel(uint nFlags, short zDelta, CPoint Point)
{
	if (Point.x <= m_EyeWndLeft || Point.x >= m_EyeWndRight || Point.y <= m_EyeWndTop || Point.y >= m_EyeWndButtom)
	{
		return;
	}

	if (m_ShowOsherRing)
	{
		m_wheel_n2 += zDelta / WHEEL_DELTA;
		int maxDegree = 30 + 2 * m_wheel_n;

		if (m_wheel_n2 < -30)      m_wheel_n2 = -30;
		else if (m_wheel_n2 > maxDegree)  m_wheel_n2 = maxDegree;
	}
	else
	{
		m_wheel_n += zDelta / WHEEL_DELTA;

		int maxDegree = 30 + 2 * m_wheel_n;
		if (m_wheel_n2 > maxDegree)  m_wheel_n2 = maxDegree;

		if (m_wheel_n - m_last_wheel_n > 1)
		{
			m_wheel_n = m_last_wheel_n + 1;
			m_last_wheel_n = m_wheel_n;
		}
		if (m_last_wheel_n - m_wheel_n > 1)
		{
			m_wheel_n = m_last_wheel_n - 1;
			m_last_wheel_n = m_wheel_n;
		}

		if (m_wheel_n <= 0) {
			m_wheel_n = 0;
		}
		else if (m_wheel_n >= 15) {
			m_wheel_n = 14;
		}
	}

	//RestoreMemDC();
	ClearMemDC();
	if (m_wheel_n != 0 && (!m_ShowOsherRing)) ::SetCursor(::LoadCursor(NULL, IDC_HAND));
	else ::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	if (m_wheel_n == 0)
	{
		m_MovePoint.x = 0;
		m_MovePoint.y = 0;
		for (int i = 1; i<15; i++) m_BackupYes[i] = FALSE;
	}

	Eye();
	Caliper();

	Grid();
	Limbus();
	Pupil();
	RadialRuler();
	LinearRuler();
	Labels();

	AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 1);
	AlignLineDeg(m_a1, m_a3, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 2);
	AlignLineDeg(m_a1, m_a4, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 3);

	AlignLine(m_a1, m_Ali_Rum1, 0, "0");
	AlignLine(m_a2, m_Ali_Rum2, 1, "1");
	AlignLine(m_a3, m_Ali_Rum3, 1, "2");
	AlignLine(m_a4, m_Ali_Rum4, 1, "3");

	AlignHoyaLine(m_a1, m_Ali_Rum1);
	AlignHoyaLine(m_a2, m_Ali_Rum2);

	Points();

	InvalidateRgn(&m_Rgn, FALSE);
	UpdateWindow();

	return;
}

//***************************************************************************************
void CPlanBigWnd::OnLButtonDown(uint nFlags, CPoint Point)
{
	if (Point.x >= m_Ratio1Rect.left && Point.x <= m_Ratio1TextRect.right && Point.y >= m_Ratio1Rect.top && Point.y <= m_Ratio1Rect.bottom)
	{
		OnImgMouseButtonClicked();
		return;
	}

	if (Point.x >= m_Ratio2Rect.left && Point.x <= m_Ratio1TextRect.right && Point.y >= m_Ratio2Rect.top && Point.y <= m_Ratio2Rect.bottom)
	{
		OnRingMouseButtonClicked();
		return;
	}


	if (Point.x <= m_EyeWndLeft || Point.x >= m_EyeWndRight || Point.y <= m_EyeWndTop || Point.y >= m_EyeWndButtom)
	{
		return;
	}
	else
	{
		m_CapturePoint = Point;
		if (m_wheel_n != 0)
		{
			m_mouseCap = TRUE;
			m_mouseRealCap = TRUE;
			if (m_ShowDymImg)  ::SetCursor(::LoadCursor(NULL, IDC_HAND));
			m_Movex = intRound(m_cx - 0.5*m_w);
			m_Movey = intRound(m_cy - 0.5*m_h);
		}
		else m_mouseCap = FALSE;
	}

	//OnMouseMove(nFlags, Point);
	if (m_ShowCaliper && m_ShowOsherRing)
	{
		m_MousePos = 0;
		m_MoveCalipar = 0;
	}

	if (m_ShowCaliper && m_ShowOsherRing) OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CPlanBigWnd::OnLButtonUp(uint nFlags, CPoint Point)
{
	m_mouseRealCap = FALSE;

	if (Point.x <= m_EyeWndLeft || Point.x >= m_EyeWndRight || (Point.y <= m_EyeWndTop || Point.y >= m_EyeWndButtom))
	{
		return;
	}

	if (!m_ShowOsherRing)
	{
		if (m_wheel_n != 0) ::SetCursor(::LoadCursor(NULL, IDC_HAND));
	}
	else if (m_ShowCaliper && m_ShowOsherRing)
	{
		m_MousePos = 0;
		m_MoveCalipar = 0;
	}
}

//***************************************************************************************

void CPlanBigWnd::OnMouseMove(uint nFlags, CPoint Point)
{
	if (Point.x <= m_EyeWndLeft || Point.x >= m_EyeWndRight || Point.y <= m_EyeWndTop || Point.y >= m_EyeWndButtom)
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		if (Point.x >= m_Ratio2Rect.left && Point.x <= m_Ratio1TextRect.right && Point.y >= m_Ratio2Rect.top && Point.y <= m_Ratio2Rect.bottom) return;
	}
	else
	{
		if (m_wheel_n != 0 && !m_ShowOsherRing) ::SetCursor(::LoadCursor(NULL, IDC_HAND));
		else ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	if (m_ShowDymImg && m_mouseCap && m_mouseRealCap && m_wheel_n != 0)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		m_MovePoint.x = Point.x - m_CapturePoint.x + m_Movex;
		m_MovePoint.y = Point.y - m_CapturePoint.y + m_Movey;

		if (m_MovePoint.x == 0 && m_MovePoint.y == 0)
		{
		}
		else
		{
			for (int i = 1; i<15; i++) m_BackupYes[i] = FALSE;
			ClearMemDC();
			Eye();
			Caliper();

			Grid();
			Limbus();
			Pupil();
			RadialRuler();
			LinearRuler();
			Labels();

			AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 1);
			AlignLineDeg(m_a1, m_a3, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 2);
			AlignLineDeg(m_a1, m_a4, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 3);

			AlignLine(m_a1, m_Ali_Rum1, 0, "0");
			AlignLine(m_a2, m_Ali_Rum2, 1, "1");
			AlignLine(m_a3, m_Ali_Rum3, 1, "2");
			AlignLine(m_a4, m_Ali_Rum4, 1, "3");

			AlignHoyaLine(m_a1, m_Ali_Rum1);
			AlignHoyaLine(m_a2, m_Ali_Rum2);


			Points();

			InvalidateRgn(&m_Rgn, FALSE);
			UpdateWindow();
		}
	}

	//531
	if (m_isCombo)
	{
		if (m_ShowOsherRing)
		{
			if (nFlags & MK_LBUTTON)
			{
				m_AlignLine = TRUE;

				real_t x_px_um = m_x_px_um;
				real_t y_px_um = m_y_px_um;

				real_t x_um = (Point.x - m_cx) / x_px_um;
				real_t y_um = (m_cy - Point.y) / y_px_um;
				real_t a_rd = angle(y_um, x_um);
				int a = intRound(_180_Pi * a_rd) % 360; if (a < 0) a += 360;
				real_t Ali_Rum = sqrt(x_um*x_um + y_um*y_um);

				real_t d2 = sqr(x_um) + sqr(y_um);

				int t = m_wheel_n;
				real_t  zoomRatio = 1.0;
				for (int i = 0; i<t; i++) { zoomRatio *= 1.05; } Ali_Rum = Ali_Rum / zoomRatio;

				//
				if (d2 <= sqr(800.0))
				{
					if (m_MoveCalipar == 2)
					{
						if (m_Ali_Rum3 != 0)
						{
							m_Ali_Rum2 = m_Ali_Rum3;
							m_a2 = m_a3;

							if (m_Ali_Rum4 != 0)
							{
								m_Ali_Rum3 = m_Ali_Rum4;
								m_a3 = m_a4;

								m_Ali_Rum4 = 0;
								m_a4 = -1;
							}
							else
							{
								m_Ali_Rum3 = 0;
								m_a3 = -1;
							}
						}
						else
						{
							m_Ali_Rum2 = 0;
							m_a2 = -1;
						}
					}
					else if (m_MoveCalipar == 3)
					{
						if (m_Ali_Rum4 != 0)
						{
							m_Ali_Rum3 = m_Ali_Rum4;
							m_a3 = m_a4;

							m_Ali_Rum4 = 0;
							m_a4 = -1;
						}
						else
						{
							m_Ali_Rum3 = 0;
							m_a3 = -1;
						}
					}
					else if (m_MoveCalipar == 4)
					{
						m_Ali_Rum4 = 0;
						m_a4 = -1;
					}

					m_MoveCalipar = 0;

					//Save the angles		
					if (m_a2 != -1 && m_a3 != -1)
					{
						if (m_a4 != -1)  m_pCTExam->m_OpData.m_RefAxis[0] = CreateRefAxis(m_a2, m_a3, m_a4);
						else            m_pCTExam->m_OpData.m_RefAxis[0] = CreateRefAxis(m_a2, m_a3, 370);
					}
					else m_pCTExam->m_OpData.m_RefAxis[0] = 100;
					//Done

					//6.2.0 Save the Length		
					m_pCTExam->m_OpData.m_RefLength[0] = m_Ali_Rum1;
					m_pCTExam->m_OpData.m_RefLength[1] = m_Ali_Rum2;
					m_pCTExam->m_OpData.m_RefLength[2] = m_Ali_Rum3;
					m_pCTExam->m_OpData.m_RefLength[3] = m_Ali_Rum4;
					//Done

					ClearMemDC();
					Eye();

					Grid();
					Limbus();
					Pupil();
					RadialRuler();
					LinearRuler();
					Labels();

					AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 1);
					AlignLineDeg(m_a1, m_a3, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 2);
					AlignLineDeg(m_a1, m_a4, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 3);

					AlignLine(m_a1, m_Ali_Rum1, 0, "0");
					AlignLine(m_a2, m_Ali_Rum2, 1, "1");
					AlignLine(m_a3, m_Ali_Rum3, 1, "2");
					AlignLine(m_a4, m_Ali_Rum4, 1, "3");

					Points();

					Invalidate(FALSE);
					UpdateWindow();
					return;
				}


				//
				if ((m_Ali_Rum1 != 0 || m_Ali_Rum2 != 0 || m_Ali_Rum3 != 0 || m_Ali_Rum4 != 0) && m_MoveCalipar == 0)
				{
					int disa1 = 1000;
					int disa2 = 1000;
					int disa3 = 1000;
					int disa4 = 1000;

					if (m_Ali_Rum1 != 0) disa1 = SmallAng(a, m_a1);
					if (m_Ali_Rum2 != 0) disa2 = SmallAng(a, m_a2);
					if (m_Ali_Rum3 != 0) disa3 = SmallAng(a, m_a3);
					if (m_Ali_Rum4 != 0) disa4 = SmallAng(a, m_a4);

					if (disa1 <= 5)
					{
						m_MoveCalipar = 1;
					}
					else if (disa2 <= 5)
					{
						m_MoveCalipar = 2;
					}
					else if (disa3 <= 5)
					{
						m_MoveCalipar = 3;
					}
					else if (disa4 <= 5)
					{
						m_MoveCalipar = 4;
					}
				}
				//


				if (m_Ali_Rum1 == 0)
				{
					m_a1 = m_pCTExam->m_OpData.m_IOLPlacementAxis;
					m_Ali_Rum1 = Ali_Rum;
					m_MoveCalipar = 1;
				}
				else if (m_Ali_Rum1 != 0 && m_Ali_Rum2 == 0 && m_MoveCalipar == 0)
				{
					m_a2 = a;
					m_Ali_Rum2 = Ali_Rum;
					m_MoveCalipar = 2;
				}
				else if (m_Ali_Rum1 != 0 && m_Ali_Rum2 != 0 && m_Ali_Rum3 == 0 && m_MoveCalipar == 0)
				{
					m_a3 = a;
					m_Ali_Rum3 = Ali_Rum;
					m_MoveCalipar = 3;
				}
				else if (m_Ali_Rum1 != 0 && m_Ali_Rum2 != 0 && m_Ali_Rum3 != 0 && m_Ali_Rum4 == 0 && m_MoveCalipar == 0)
				{
					m_a4 = a;
					m_Ali_Rum4 = Ali_Rum;
					m_MoveCalipar = 4;
				}
				else if (m_Ali_Rum1 != 0 && m_Ali_Rum2 != 0 && m_Ali_Rum3 != 0 && m_Ali_Rum4 != 0)
				{
					if (m_MoveCalipar == 0)
					{
						int disA1 = SmallAng(a, m_a1);
						int a2 = (m_a1 + 180) % 360;
						int disAC = SmallAng(a, a2);
						disA1 = (disA1 < disAC ? disA1 : disAC);

						int disA2 = SmallAng(a, m_a2);
						int disA3 = SmallAng(a, m_a3);
						int disA4 = SmallAng(a, m_a4);

						int minDisA1 = (disA1 < disA2 ? disA1 : disA2);
						int minDisA2 = (disA3 < disA4 ? disA3 : disA4);
						int minDisA = (minDisA1 < minDisA2 ? minDisA1 : minDisA2);

						int DragedLine1 = (disA1 < disA2 ? 1 : 2);
						int DragedLine2 = (disA3 < disA4 ? 3 : 4);
						int DragedLine = (minDisA1 < minDisA2 ? DragedLine1 : DragedLine2);

						m_MoveCalipar = (disA1 < disA2 ? 1 : 2);

						if (minDisA < 5)
						{
							if (DragedLine == 1) 	  m_MoveCalipar = 1;
							else if (DragedLine == 2)  m_MoveCalipar = 2;
							else if (DragedLine == 3)  m_MoveCalipar = 3;
							else if (DragedLine == 4)  m_MoveCalipar = 4;
						}
						else
						{
							m_MoveCalipar = 0;
							return;
						}
					}

					if (m_MoveCalipar == 1)
					{
						m_Ali_Rum1 = Ali_Rum;
					}
					else if (m_MoveCalipar == 2)
					{
						m_a2 = a;
						m_Ali_Rum2 = Ali_Rum;
					}
					else if (m_MoveCalipar == 3)
					{
						m_a3 = a;
						m_Ali_Rum3 = Ali_Rum;
					}
					else if (m_MoveCalipar == 4)
					{
						m_a4 = a;
						m_Ali_Rum4 = Ali_Rum;
					}
				}
				else
				{
					if (m_MoveCalipar == 1)
					{
						m_Ali_Rum1 = Ali_Rum;
					}
					else if (m_MoveCalipar == 2)
					{
						m_a2 = a;
						m_Ali_Rum2 = Ali_Rum;
					}
					else if (m_MoveCalipar == 3)
					{
						m_a3 = a;
						m_Ali_Rum3 = Ali_Rum;
					}
					else if (m_MoveCalipar == 4)
					{
						m_a4 = a;
						m_Ali_Rum4 = Ali_Rum;
					}
				}

				////Save the angles		
				if (m_a2 != -1 && m_a3 != -1)
				{
					if (m_a4 != -1)  m_pCTExam->m_OpData.m_RefAxis[0] = CreateRefAxis(m_a2, m_a3, m_a4);
					else           m_pCTExam->m_OpData.m_RefAxis[0] = CreateRefAxis(m_a2, m_a3, 370);
				}
				//Done

				//6.2.0 Save the Length		
				m_pCTExam->m_OpData.m_RefLength[0] = m_Ali_Rum1;
				m_pCTExam->m_OpData.m_RefLength[1] = m_Ali_Rum2;
				m_pCTExam->m_OpData.m_RefLength[2] = m_Ali_Rum3;
				m_pCTExam->m_OpData.m_RefLength[3] = m_Ali_Rum4;
				//Done

				ClearMemDC();
				Eye();

				Grid();
				Limbus();
				Pupil();
				RadialRuler();
				LinearRuler();
				Labels();

				AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 1);
				AlignLineDeg(m_a1, m_a3, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 2);
				AlignLineDeg(m_a1, m_a4, m_Ali_Rum1, m_Ali_Rum2, m_Ali_Rum3, m_Ali_Rum4, 3);

				AlignLine(m_a1, m_Ali_Rum1, 0, "0");
				AlignLine(m_a2, m_Ali_Rum2, 1, "1");
				AlignLine(m_a3, m_Ali_Rum3, 1, "2");
				AlignLine(m_a4, m_Ali_Rum4, 1, "3");

				Points();

				Invalidate(FALSE);
				UpdateWindow();
			}
		}
	}//531 Done
	else //WorkStation
	{
		if (m_ShowCaliper && m_ShowOsherRing)
		{
			if (nFlags & MK_LBUTTON)
			{
				BOOL Redraw = FALSE;

				real_t x_px_um = m_x_px_um;
				real_t y_px_um = m_y_px_um;

				real_t x_um = (Point.x - m_cx) / x_px_um;
				real_t y_um = (m_cy - Point.y) / y_px_um;
				real_t a_rd = angle(y_um, x_um);
				int a = intRound(_180_Pi * a_rd) % 360; if (a < 0) a += 360;
				real_t d2 = sqr(x_um) + sqr(y_um);

				if (sqr(4100.0) <= d2)
				{
					if (d2 <= sqr(5400.0)) m_AlignLine = FALSE;
					else                  m_AlignLine = TRUE;
					if (m_MousePos == 0 || m_MousePos == 3)
					{
						if (m_ShowCaliper)
						{
							m_MousePos = 3;

							////[cjf***05032012*005*]
							if (m_MoveCalipar == 0)
							{
								int disA1 = SmallAng(a, m_pCTExam->m_OpData.m_RefAxis[0]);
								int disA2 = SmallAng(a, m_pCTExam->m_OpData.m_RefAxis[1]);
								int disA = disA1 - disA2;
								m_MoveCalipar = (disA < 0 ? 1 : 2);

								//m_MoveCalipar = ( abs(a - m_pCTExam->m_OpData.m_RefAxis[0]) < abs(a - m_pCTExam->m_OpData.m_RefAxis[1]) ? 1 : 2 );			 
							}


							int ar[2];
							if (m_pCTExam->m_OpData.m_OpType == 0) {
								ar[0] = m_pCTExam->m_OpData.m_IOLPlacementAxis;
							}
							else if (m_pCTExam->m_OpData.m_OpType == 1) {
								ar[0] = (m_pCTExam->m_OpData.m_InternalPostopAxis + 90) % 180;
							}
							else if (m_pCTExam->m_OpData.m_OpType == 2) {
								ar[0] = m_pCTExam->m_OpData.m_ICL.m_PlacementAxis;
							}
							else return;
							ar[0] %= 180; if (ar[0] < 0) ar[0] += 180;
							ar[1] = ar[0] + 180;

							//judge there is overlap or not		 
							int br[2];
							if (m_MoveCalipar == 1)
							{
								br[0] = a;
								br[1] = m_pCTExam->m_OpData.m_RefAxis[1];
							}
							else
							{
								br[0] = m_pCTExam->m_OpData.m_RefAxis[0];
								br[1] = a;
							}

							m_OverLap = OverLapFun(ar[0], ar[1], br[0], br[1]);

							if (m_OverLap)// If it is overlap 
							{
								if (m_MoveCalipar == 1) m_pCTExam->m_OpData.m_RefAxis[1] = a;
								else m_pCTExam->m_OpData.m_RefAxis[0] = a;
							}

							m_pCTExam->m_OpData.m_RefAxis[m_MoveCalipar - 1] = a;
							//[cjf***05032012*005*]

							// m_pCTExam->m_OpData.m_RefAxis[x_um >= 0.0 ? 0 : 1] = a;
							Redraw = TRUE;
						}
					}
				}

				if (Redraw)
				{
					real_t Ali_Rum = sqrt(x_um*x_um + y_um*y_um);

					int t = m_wheel_n;
					real_t  zoomRatio = 1.0;
					for (int i = 0; i<t; i++) { zoomRatio *= 1.05; } Ali_Rum = Ali_Rum / zoomRatio;

					ClearMemDC();
					Eye();
					Caliper();

					m_a1 = m_pCTExam->m_OpData.m_RefAxis[0];
					m_a2 = m_pCTExam->m_OpData.m_RefAxis[1];

					if (m_MoveCalipar == 1)
					{
						m_Ali_Rum1 = Ali_Rum;
					}
					else
					{
						m_Ali_Rum2 = Ali_Rum;
					}

					AlignHoyaLine(m_a1, m_Ali_Rum1);
					AlignHoyaLine(m_a2, m_Ali_Rum2);

					Grid();
					Points();
					Limbus();
					Pupil();
					RadialRuler();
					LinearRuler();
					Labels();
					//Invalidate(FALSE);
					InvalidateRgn(&m_Rgn, FALSE);
					UpdateWindow();
				}
			}
		}
	}
}
//***************************************************************************************

//531 Transform the three protractor's angles to one int according to some rule

int  CPlanBigWnd::CreateRefAxis(int a0, int a1, int a2)
{
	CString s, s0, s1, s2;

	s0.Format(_T("%i"), a0);
	s1.Format(_T("%i"), a1);
	s2.Format(_T("%i"), a2);

	int len = s0.GetLength();

	if (len == 1) s0 = "00" + s0;
	else if (len == 2) s0 = "0" + s0;

	len = s1.GetLength();

	if (len == 1) s1 = "00" + s1;
	else if (len == 2) s1 = "0" + s1;

	len = s2.GetLength();

	if (len == 1) s2 = "00" + s2;
	else if (len == 2) s2 = "0" + s2;

	s = "-1" + s0 + s1 + s2;

	G_As = s;
	int val = atoi(G_As);

	return val;
}

//***************************************************************************************

//Transform one int value to the three protractor's angles according to some rule
void CPlanBigWnd::GetProAngs(int inputV, int &a0, int &a1, int &a2)
{
	CString inputS;
	inputS.Format(_T("%i"), inputV);
	G_As = inputS.Right(3);
	a2 = atoi(G_As);

	if (a2 == 370) a2 = -1;
	inputS.Truncate(8);
	G_As = inputS.Right(3);
	a1 = atoi(G_As);

	inputS.Truncate(5);
	G_As = inputS.Right(3);
	a0 = atoi(G_As);

	a0 = a0 % 360;
	a1 = a1 % 360;
	if (a2 != -1) a2 = a2 % 360;
}

//***************************************************************************************

void CPlanBigWnd::CreateMenu()
{
	m_pMenu->AppendItem(IDC_SHOW_COLOR_LIMBUS_ITEM, FALSE, m_ShowLimbus, "Show Limbus", "");
	m_pMenu->AppendItem(IDC_SHOW_COLOR_PUPIL_ITEM, FALSE, m_ShowPupil, "Show Pupil", "");
	m_pMenu->AppendItem(IDC_SHOW_COLOR_RRULER_ITEM, FALSE, m_ShowRRuler, "Show Radial Ruler", "");
	m_pMenu->AppendItem(IDC_SHOW_COLOR_LRULER_ITEM, FALSE, m_ShowLRuler, "Show Linear Ruler", "");

	m_pMenu->AppendSeparator();

	m_pMenu->AppendItem(IDC_EDIT_LIMBUS_ITEM, FALSE, FALSE, "Edit pupil, limbus, vertex and ruler", "");
	//m_pMenu->AppendItem(IDC_EDIT_LIMBUS_ITEM,   FALSE, FALSE, "Edit pupil,limbus,vertex,ruler and disk", "");  
}

//***************************************************************************************

void CPlanBigWnd::OnRButtonUp(uint nFlags, CPoint Point)
{
	SAFE_DELETE(m_pMenu);

	m_pMenu = new CXPMenu();

	CreateMenu();

	int l = (int)((uint)Point.x & 0x0000ffff);
	int t = (int)((uint)Point.y & 0x0000ffff);

	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, l, t, this);
}
//***************************************************************************************

// distance from a1 to a2 counterclockwise Є [0, 360)
int CPlanBigWnd::angular_distance_counterclockwise(int a1, int a2)
{
	int da = (a2 - a1) % 360;
	if (da < 0) da += 360;
	return da;
}

//***************************************************************************************

// distance between a1 and a2 Є [0, 180)
int CPlanBigWnd::angular_distance(int a1, int a2)
{
	int da = angular_distance_counterclockwise(a1, a2);
	if (da > 180) da = 360 - da;
	return da;
}

//***************************************************************************************

void CPlanBigWnd::OnImgMouseButtonClicked()
{
	if (!m_ShowDymImg)
	{
		OnShow(&m_ShowDymImg);
		if (m_ShowDymImg)
		{
			m_ShowOsherRing = FALSE;
			m_ImgMouseCotrol.ShowWindow(SW_SHOW);
			m_RingMouseCotrol.ShowWindow(SW_HIDE);
		}
		else
		{
			m_ShowOsherRing = TRUE;
			m_ImgMouseCotrol.ShowWindow(SW_HIDE);
			m_RingMouseCotrol.ShowWindow(SW_SHOW);
		}
		Repaint();
	}
}

//***************************************************************************************

void CPlanBigWnd::OnRingMouseButtonClicked()
{
	if (!m_ShowOsherRing)
	{
		OnShow(&m_ShowOsherRing);
		if (m_ShowOsherRing)
		{
			m_ShowDymImg = FALSE;
			m_ImgMouseCotrol.ShowWindow(SW_HIDE);
			m_RingMouseCotrol.ShowWindow(SW_SHOW);
		}
		else
		{
			m_ShowDymImg = TRUE;
			m_ImgMouseCotrol.ShowWindow(SW_SHOW);
			m_RingMouseCotrol.ShowWindow(SW_HIDE);
		}
		Repaint();
	}
}
//***************************************************************************************
void CPlanBigWnd::OnExitButtonClicked()
{
	GetParent()->PostMessage(WM_NOTIFY_LBTN_DBCLK, (WPARAM)0, (LPARAM)0);
}


//***************************************************************************************

void CPlanBigWnd::OnZaldivarButtonClicked()
{
	if (m_isCombo)
	{
		m_AlignLine = !m_AlignLine;
	}
	else
	{
		m_ShowCaliper = !m_ShowCaliper;
		::Settings.m_PlannerSettings.m_MapShowZaldivarCaliper = m_ShowCaliper;
	}
	Repaint();
}

//***************************************************************************************

void CPlanBigWnd::OnSaveScreenButtonClicked()
{
	this->SaveIntoFile(m_DicomInfo);
}

//***************************************************************************************

void CPlanBigWnd::OnPrintScreenButtonClicked()
{
	this->Print();
}

//***************************************************************************************

BOOL CPlanBigWnd::OverLapFun(int a0, int a1, int b0, int b1)
{
	int rangMin = 0;
	int rangMax = 0;

	b1 = ((b1 - a0)<0 ? 360 + (b1 - a0) : (b1 - a0));
	b0 = ((b0 - a0)<0 ? 360 + (b0 - a0) : (b0 - a0));
	a1 = ((a1 - a0)<0 ? 360 + (a1 - a0) : (a1 - a0));


	if (b0 > 180) {
		rangMin = b0;
		rangMax = 360;
	}
	else rangMax = b0;

	if (b1 <= rangMax && b1 >= rangMin) return TRUE;


	return FALSE;
}

//***************************************************************************************
int  CPlanBigWnd::SmallAng(int a0, int a1)
{
	int dis = abs(a0 - a1);
	if (dis > 180) dis = 360 - dis;

	return dis;
}


//***************************************************************************************
void CPlanBigWnd::FindClearLimbus(CEyeImage* OriImage, real_t LastLimbuX, real_t LastLimbuY, real_t LastLimbuR)
{
	CEyeImage* TestImage = new CEyeImage();
	int h = TestImage->m_h = OriImage->m_h;
	int w = TestImage->m_w = OriImage->m_w;
	TestImage->m_h_um = OriImage->m_h_um;
	TestImage->m_w_um = OriImage->m_w_um;

	TestImage->m_RGBData.Create(OriImage->m_h, (int)OriImage->m_w * 3, OriImage->m_RGBData.GetMem());

	if (TestImage->m_RGBData.GetMem() == NULL) return;

	int num = w*h;
	int AveG = 0;

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			int R0 = TestImage->GetRAt(i, j);
			int G0 = TestImage->GetGAt(i, j);
			int B0 = TestImage->GetBAt(i, j);
			int Gray0 = intRound(0.3*R0 + 0.59*G0 + 0.11*B0);
			AveG += Gray0;
		}
	}

	AveG = intRound(AveG / num);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			int R0 = TestImage->GetRAt(i, j);
			int G0 = TestImage->GetGAt(i, j);
			int B0 = TestImage->GetBAt(i, j);
			int Gray0 = intRound(0.3*R0 + 0.59*G0 + 0.11*B0);

			int NewGray = AveG + 2 * (Gray0 - AveG);

			if (NewGray > 250) NewGray = 250;
			if (NewGray < 0)  NewGray = 0;

			TestImage->SetRAt(i, j, NewGray);
			TestImage->SetGAt(i, j, NewGray);
			TestImage->SetBAt(i, j, NewGray);
		}
	}

	TestImage->FindLimbus2(LastLimbuX, LastLimbuY, LastLimbuR);
	//TestImage->FindLimbus();

	OriImage->m_li_ok = TestImage->m_li_ok;
	OriImage->m_li_r_max_um = TestImage->m_li_r_max_um;
	OriImage->m_li_r_mean_um = TestImage->m_li_r_mean_um;
	OriImage->m_li_r_min_um = TestImage->m_li_r_min_um;
	for (int i = 0; i < 360; i++)	OriImage->m_li_r_um[i] = TestImage->m_li_r_um[i];
	OriImage->m_li_x0_um = TestImage->m_li_x0_um;
	OriImage->m_li_y0_um = TestImage->m_li_y0_um;

	delete TestImage;
}

//***************************************************************************************

BOOL CPlanBigWnd::FindColorPupil(CEyeImage* OriImage)
{
	CEyeImage* TestImage = new CEyeImage();
	int h_h = TestImage->m_h = OriImage->m_h;
	int w_w = TestImage->m_w = OriImage->m_w;
	TestImage->m_h_um = OriImage->m_h_um;
	TestImage->m_w_um = OriImage->m_w_um;

	TestImage->m_RGBData.Create(OriImage->m_h, (int)OriImage->m_w * 3, OriImage->m_RGBData.GetMem());

	if (TestImage->m_RGBData.GetMem() == NULL) return FALSE;

	int ** m_pixels = (int**)calloc(w_w, sizeof(int));
	real_t** m_g = (real_t**)calloc(4, sizeof(real_t));
	int ** m_BlurPixels = (int**)calloc(w_w, sizeof(int));
	int ** m_gxy = (int**)calloc(w_w, sizeof(int));
	int ** m_gx = (int**)calloc(w_w, sizeof(int));
	int ** m_gy = (int**)calloc(w_w, sizeof(int));
	int ** m_dis = (int**)calloc(w_w, sizeof(int));
	int ** m_degree = (int**)calloc(w_w, sizeof(int));
	int ** m_gR = (int**)calloc(w_w, sizeof(int));

	for (int i = 0; i<w_w; i++)
	{
		if (i<4)  m_g[i] = (real_t*)calloc(4, sizeof(real_t));
		m_pixels[i] = (int*)calloc(h_h, sizeof(int));
		m_BlurPixels[i] = (int*)calloc(h_h, sizeof(int));

		m_gxy[i] = (int*)calloc(h_h, sizeof(int));
		m_gx[i] = (int*)calloc(h_h, sizeof(int));
		m_gy[i] = (int*)calloc(h_h, sizeof(int));
		m_dis[i] = (int*)calloc(h_h, sizeof(int));
		m_degree[i] = (int*)calloc(h_h, sizeof(int));

		m_gR[i] = (int*)calloc(h_h, sizeof(int));
	}


	//Set the Tracking Region
	int SRegion_StX = intRound(0.3*w_w);// Search region start x
	int SRegion_EndX = intRound(0.7*w_w);// Search region end x
	int SRegion_StY = intRound(0.3*h_h);// Search region start x
	int SRegion_EndY = intRound(0.7*h_h);// Search region end x	

	int StartR = intRound(0.05*w_w);
	int EndR = intRound(0.15*w_w);


	real_t de = 1.4;


	//*******************************************************************************
	//The arrays definition
	//*******************************************************************************

	int tt = 0;
	int ave = 0;
	for (int i = SRegion_StX + 1; i < SRegion_EndX; i++)
	{
		for (int j = SRegion_StY + 1; j < SRegion_EndY; j++)
		{
			int R = TestImage->m_RGBData(j, i * 3 + 2);
			int G = TestImage->m_RGBData(j, i * 3 + 1);
			int B = TestImage->m_RGBData(j, i * 3);

			ave += intRound(0.3*R + 0.59*G + 0.11*B);
			tt++;
		}
	}
	ave = ave / tt;

	int Gray = 0;
	int dif = 0;
	for (int i = SRegion_StX + 1; i < SRegion_EndX; i++)
	{
		for (int j = SRegion_StY + 1; j < SRegion_EndY; j++)
		{
			int R = TestImage->m_RGBData(j, i * 3 + 2);
			int G = TestImage->m_RGBData(j, i * 3 + 1);
			int B = TestImage->m_RGBData(j, i * 3);

			Gray = intRound(0.3*R + 0.59*G + 0.11*B);
			dif = abs(Gray - ave) + dif;
		}
	}
	dif = dif / tt;


	int R;
	int G;
	int B;

	for (int i = 0; i<w_w; i++)
	{
		for (int j = 0; j<h_h; j++)
		{
			//For Gaussian Blur		
			if (i < 4 && j < 4)
			{
				m_g[i][j] = Gaussian(i, j, de);
			}

			if (i > SRegion_StX && i < SRegion_EndX && j > SRegion_StY && j < SRegion_EndY)
			{
				R = TestImage->m_RGBData(j, i * 3 + 2);
				G = TestImage->m_RGBData(j, i * 3 + 1);
				B = TestImage->m_RGBData(j, i * 3);

				if (dif > 25)// Lighter eye has larger variance
				{
					m_pixels[i][j] = intRound(0.3*R + 0.59*G + 0.11*B);
					//m_pixels[i][j] = intRound(sqrt(((real)B*R + (real)B*G + (real)R*G)/3.0));
				}
				else//Darker eye use the color for judgement
				{
					HSI res = ChangeRGBtoHSI(R, G, B);//improve the w/b comparison of pupil for iTrace color image
					m_pixels[i][j] = intRound(res.H);
					if (0 < res.H && res.H <= 30)
					{
						m_pixels[i][j] = 10;
					}
					else if (30 < res.H && res.H <= 60)
					{
						m_pixels[i][j] = 40;
					}
					else if (60 < res.H && res.H <= 90)
					{
						m_pixels[i][j] = 70;
					}
					else if (90 < res.H && res.H <= 120)
					{
						m_pixels[i][j] = 100;
					}
					else if (120 < res.H && res.H <= 150)
					{
						m_pixels[i][j] = 130;
					}
					else if (150 < res.H && res.H <= 180)
					{
						m_pixels[i][j] = 160;
					}
					else if (180 < res.H && res.H <= 240)
					{
						m_pixels[i][j] = 190;
					}
					else if (240 < res.H && res.H <= 270)
					{
						m_pixels[i][j] = 220;
					}
					else if (270 < res.H && res.H <= 300)
					{
						m_pixels[i][j] = 250;
					}
					else if (300 < res.H && res.H <= 330)
					{
						m_pixels[i][j] = 40;
					}
					else if (330 < res.H && res.H <= 360)
					{
						m_pixels[i][j] = 10;
					}
				}
			}
			else m_pixels[i][j] = 0;

			m_BlurPixels[i][j] = 0;
			//Done


			//For Canney Edge
			m_gxy[i][j] = 0;
			m_gx[i][j] = 0;
			m_gy[i][j] = 0;
			m_degree[i][j] = 0;
			m_dis[i][j] = 0;
			m_gR[i][j] = 0;
			//Done
		}
	}
	//*******************************************************************************
	//arrays definition Done
	//*******************************************************************************





	//*******************************************************************************
	//First step is the Gassusian blur
	//*******************************************************************************	
	real_t Mid = 0;
	for (int h = SRegion_StY + 3; h < SRegion_EndY - 3; h++)
	{
		for (int v = SRegion_StX + 3; v < SRegion_EndX - 3; v++)
		{
			Mid = 0;

			//1
			int R0 = m_pixels[v - 3][h - 3];
			int R1 = m_pixels[v - 2][h - 3];
			int R2 = m_pixels[v - 1][h - 3];
			int R3 = m_pixels[v][h - 3];
			int R4 = m_pixels[v + 1][h - 3];
			int R5 = m_pixels[v + 2][h - 3];
			int R6 = m_pixels[v + 2][h - 3];

			Mid = (R0)*m_g[3][3] + (R1)*m_g[2][3] + (R2)*m_g[1][3] + (R3)*m_g[0][3] + (R4)*m_g[3][3] + (R5)*m_g[3][2] + (R6)*m_g[3][1];

			//2
			R0 = m_pixels[v - 3][h - 2];
			R1 = m_pixels[v - 2][h - 2];
			R2 = m_pixels[v - 1][h - 2];
			R3 = m_pixels[v][h - 2];
			R4 = m_pixels[v + 1][h - 2];
			R5 = m_pixels[v + 2][h - 2];
			R6 = m_pixels[v + 2][h - 2];

			Mid += (R0)*m_g[3][2] + (R1)*m_g[2][2] + (R2)*m_g[1][2] + (R3)*m_g[0][2] + (R4)*m_g[3][2] + (R5)*m_g[2][2] + (R6)*m_g[2][1];

			//3
			R0 = m_pixels[v - 3][h - 1];
			R1 = m_pixels[v - 2][h - 1];
			R2 = m_pixels[v - 1][h - 1];
			R3 = m_pixels[v][h - 1];
			R4 = m_pixels[v + 1][h - 1];
			R5 = m_pixels[v + 2][h - 1];
			R6 = m_pixels[v + 2][h - 1];

			Mid += (R0)*m_g[3][1] + (R1)*m_g[2][1] + (R2)*m_g[1][1] + (R3)*m_g[0][1] + (R4)*m_g[1][3] + (R5)*m_g[1][2] + (R6)*m_g[1][1];

			//4
			R0 = m_pixels[v - 3][h];
			R1 = m_pixels[v - 2][h];
			R2 = m_pixels[v - 1][h];
			R3 = m_pixels[v][h];
			R4 = m_pixels[v + 1][h];
			R5 = m_pixels[v + 2][h];
			R6 = m_pixels[v + 2][h];

			Mid += (R0)*m_g[3][0] + (R1)*m_g[2][0] + (R2)*m_g[1][0] + (R3)*m_g[0][0] + (R4)*m_g[0][3] + (R5)*m_g[0][2] + (R6)*m_g[0][1];

			//5
			R0 = m_pixels[v - 3][h + 1];
			R1 = m_pixels[v - 2][h + 1];
			R2 = m_pixels[v - 1][h + 1];
			R3 = m_pixels[v][h + 1];
			R4 = m_pixels[v + 1][h + 1];
			R5 = m_pixels[v + 2][h + 1];
			R6 = m_pixels[v + 2][h + 1];

			Mid += (R0)*m_g[3][1] + (R1)*m_g[2][1] + (R2)*m_g[1][1] + (R3)*m_g[0][1] + (R4)*m_g[1][3] + (R5)*m_g[1][2] + (R6)*m_g[1][1];

			//6
			R0 = m_pixels[v - 3][h + 2];
			R1 = m_pixels[v - 2][h + 2];
			R2 = m_pixels[v - 1][h + 2];
			R3 = m_pixels[v][h + 2];
			R4 = m_pixels[v + 1][h + 2];
			R5 = m_pixels[v + 2][h + 2];
			R6 = m_pixels[v + 2][h + 2];

			Mid += (R0)*m_g[3][2] + (R1)*m_g[2][2] + (R2)*m_g[1][2] + (R3)*m_g[0][2] + (R4)*m_g[3][2] + (R5)*m_g[2][2] + (R6)*m_g[2][1];

			//7
			R0 = m_pixels[v - 3][h + 3];
			R1 = m_pixels[v - 2][h + 3];
			R2 = m_pixels[v - 1][h + 3];
			R3 = m_pixels[v][h + 3];
			R4 = m_pixels[v + 1][h + 3];
			R5 = m_pixels[v + 2][h + 3];
			R6 = m_pixels[v + 2][h + 3];

			Mid += (R0)*m_g[3][3] + (R1)*m_g[2][3] + (R2)*m_g[1][3] + (R3)*m_g[0][3] + (R4)*m_g[3][3] + (R5)*m_g[3][2] + (R6)*m_g[3][1];

			m_BlurPixels[v][h] = intRound(Mid);
		}
	}
	//*******************************************************************************
	//Gassusian blur Done
	//*******************************************************************************




	//*******************************************************************************
	//Second step is Canney Edge detection
	//*******************************************************************************
	int Range = EndR - StartR;
	int maxGradient = 0;
	real_t fmean = 0;
	int t = 0;
	double mid = 180 / 3.1415926;

	for (int h = SRegion_StY + 1; h < SRegion_EndY - 1; h++)
	{
		for (int v = SRegion_StX + 1; v < SRegion_EndX - 1; v++)
		{
			m_gx[v][h] = (m_BlurPixels[v - 1][h - 1] * (-1) + m_BlurPixels[v + 1][h + 1] * 1 + m_BlurPixels[v - 1][h] * (-2) + m_BlurPixels[v + 1][h] * 2 + m_BlurPixels[v - 1][h + 1] * (-1) + m_BlurPixels[v + 1][h + 1] * 1);
			m_gy[v][h] = (m_BlurPixels[v - 1][h - 1] * (1) + m_BlurPixels[v][h - 1] * 2 + m_BlurPixels[v + 1][h + 1] * 1 + m_BlurPixels[v - 1][h + 1] * (-1) + m_BlurPixels[v][h + 1] * (-2) + m_BlurPixels[v + 1][h + 1] * (-1));

			m_gxy[v][h] = intRound(sqrt((real_t)m_gx[v][h] * m_gx[v][h] + (real_t)m_gy[v][h] * m_gy[v][h]));

			fmean += (real_t)m_gxy[v][h];
			t++;
			if (m_gxy[v][h]>maxGradient) maxGradient = m_gxy[v][h];

			if (m_gx[v][h] == 0)
			{
				m_degree[v][h] = ((m_gy[v][h] == 0) ? 0 : 90);
			}
			else
			{
				double div = (double)m_gy[v][h] / (double)m_gx[v][h];
				double ang = 0;

				if (div < 0)
				{
					ang = 180 - atan(-div) * mid;
				}
				else
				{
					ang = atan(div) * mid;
				}

				//(0 to 22.5 & 157.5 to 180 degrees) =>0
				//22.5 to 67.5 degrees =>45
				//67.5 to 112.5 degrees =>90
				//112.5 to 157.5 degrees =>135				
				if (ang < 22.5)        m_degree[v][h] = 0;
				else if (ang < 67.5)   m_degree[v][h] = 45;
				else if (ang < 112.5)  m_degree[v][h] = 90;
				else if (ang < 157.5)  m_degree[v][h] = 135;
				else ang = 0;
			}
		}
	}


	int leftPixel;
	int rightPixel;
	for (int h = SRegion_StY + 1; h < SRegion_EndY - 1; h++)
	{
		for (int v = SRegion_StX + 1; v < SRegion_EndX - 1; v++)
		{
			switch (m_degree[v][h])
			{
			case 0:
				leftPixel = m_gxy[v - 1][h];
				rightPixel = m_gxy[v + 1][h];
				break;
			case 45:
				leftPixel = m_gxy[v - 1][h + 1];
				rightPixel = m_gxy[v + 1][h - 1];
				break;
			case 90:
				leftPixel = m_gxy[v][h + 1];
				rightPixel = m_gxy[v][h - 1];
				break;
			case 135:
				leftPixel = m_gxy[v + 1][h + 1];
				rightPixel = m_gxy[v - 1][h - 1];
				break;
			}
			if (m_gxy[v][h] < leftPixel || m_gxy[v][h] < rightPixel)
			{
				m_dis[v][h] = 0;
			}
			else
			{
				m_dis[v][h] = intRound((real_t)m_gxy[v][h] / (real_t)maxGradient * 255.0);
			}
		}
	}

	fmean = fmean / t;
	fmean = fmean / maxGradient * 255;
	int highThreshold = intRound(fmean);
	int lowThreshold = intRound(0.4 * highThreshold);

	for (int y = SRegion_StY; y < SRegion_EndY; y++)
	{
		for (int x = SRegion_StX; x < SRegion_EndX; x++)
		{
			if (m_dis[x][y] != 0)
			{
				if (m_dis[x][y] < highThreshold)
				{
					if (m_dis[x][y] < lowThreshold)
					{
						m_dis[x][y] = 0;
					}
					else
					{
						if ((m_dis[x - 1][y]   < highThreshold) &&
							(m_dis[x + 1][y]   < highThreshold) &&
							(m_dis[x - 1][y - 1] < highThreshold) &&
							(m_dis[x][y - 1]   < highThreshold) &&
							(m_dis[x + 1][y - 1] < highThreshold) &&
							(m_dis[x - 1][y + 1] < highThreshold) &&
							(m_dis[x][y + 1]   < highThreshold) &&
							(m_dis[x + 1][y + 1] < highThreshold))
						{
							m_dis[x][y] = 0;
						}
					}
				}
			}
		}
	}
	//*******************************************************************************
	//Canney Edge detection Done
	//******************************************************************************* 






	//*******************************************************************************
	//Third step is hough circle detection
	//*******************************************************************************
	int EdgeX = 0;
	int EdgeY = 0;

	OnePoint CircleCen;
	CircleCen.x = 0;
	CircleCen.y = 0;

	int max = 0;
	int FindR = 0;

	for (int CircleR = StartR; CircleR <= EndR; CircleR++)
	{
		for (int h = SRegion_StY + 5; h < SRegion_EndY - 5; h++)
		{
			for (int v = SRegion_StX + 5; v < SRegion_EndX - 5; v++)
			{
				if (m_dis[v][h] != 0 && m_dis[v][h] != -3)
				{
					for (int r = 0; r< 360; r++)
					{
						EdgeX = v + ::MutiCOS[CircleR][r];
						EdgeY = h + ::MutiSIN[CircleR][r];

						if (EdgeX > SRegion_StX && EdgeX < SRegion_EndX && EdgeY > SRegion_StY && EdgeY < SRegion_EndY)
						{
							m_gR[EdgeX][EdgeY]++;
							m_gR[EdgeX - 1][EdgeY]++;
							m_gR[EdgeX + 1][EdgeY]++;
							m_gR[EdgeX - 1][EdgeY - 1]++;
							m_gR[EdgeX][EdgeY - 1]++;
							m_gR[EdgeX + 1][EdgeY - 1]++;
							m_gR[EdgeX - 1][EdgeY + 1]++;
							m_gR[EdgeX][EdgeY + 1]++;
							m_gR[EdgeX + 1][EdgeY + 1]++;
						}
					}
				}
			}
		}

		//Find the circle
		for (int h = 0; h<h_h; h++)
		{
			for (int v = 0; v<w_w; v++)
			{
				if (m_gR[v][h] != 0)
				{
					if (m_gR[v][h] > max)
					{
						max = m_gR[v][h];
						FindR = CircleR;
						CircleCen.x = v;
						CircleCen.y = h;
					}
					m_gR[v][h] = 0;
				}
			}
		}
	}

	if (max < 50)
	{
		return FALSE;
	}


	//Save the result  	
	{
		int R_um = intRound(FindR * TestImage->m_w_um / w_w);

		int cx = intRound(0.5*w_w);
		int cy = intRound(0.5*h_h);

		OriImage->m_pu_ok = TRUE;
		OriImage->m_pu_x0_um = (CircleCen.x - cx)*TestImage->m_w_um / w_w;
		OriImage->m_pu_y0_um = (CircleCen.y - cy)*TestImage->m_h_um / h_h;

		for (int a = 0; a < 360; a++)
		{
			OriImage->m_pu_r_um[a] = R_um;
		}

		OriImage->m_pu_r_min_um = R_um;
		OriImage->m_pu_r_max_um = R_um;
		OriImage->m_pu_r_mean_um = R_um;
	}

	//*******************************************************************************
	//hough circle detection Done
	//*******************************************************************************

	delete TestImage;

	//free memory	
	if (m_g != NULL) {
		for (int i = 0; i<3; i++) {
			if (m_g[i] != NULL) {
				delete[](m_g[i]);
				m_g[i] = NULL;
			}
		}
		delete[](m_g);
		m_g = NULL;
	}

	SafeInteRelease(m_pixels, w_w);
	SafeInteRelease(m_BlurPixels, w_w);
	SafeInteRelease(m_gxy, w_w);
	SafeInteRelease(m_gx, w_w);
	SafeInteRelease(m_gy, w_w);
	SafeInteRelease(m_dis, w_w);
	SafeInteRelease(m_degree, w_w);

	SafeInteRelease(m_gR, w_w);

	return TRUE;
}


//***************************************************************************************

HBRUSH   CPlanBigWnd::OnCtlColor(CDC*   pDC, CWnd*   pWnd, UINT   nCtlColor)
{
	if (pWnd->GetDlgCtrlID() == IDC_IMG_MOUSE_CONTROL_ITEM)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
		pDC->SetBkColor(RGB(255, 0, 255));

		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);//NULL_BRUSH
	}

	return   (HBRUSH)GetStockObject(NULL_BRUSH);
}

//***************************************************************************************

void CPlanBigWnd::OnEditLimbusItemClicked()
{
	BOOL GrayImg = TRUE;

	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL) GrayImg = FALSE;

	CImageDlg2* pDlg = new CImageDlg2(this, m_pImage, GrayImg, TRUE);
	//CImageDlg1* pDlg = new CImageDlg1(this, m_pImage, FALSE, GrayImg, TRUE);  //530 

	if (pDlg->DoModal() == IDOK) {

		Repaint();

		::DB.SaveCTExam(m_pCTExam);
	}

	delete pDlg;
}
