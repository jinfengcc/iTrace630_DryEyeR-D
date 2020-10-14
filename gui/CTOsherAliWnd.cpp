#include "StdAfx.h"
#include "Resource.h"
#include "CTOsherAliWnd.h"
#include "Settings.h"
#include "ImageDlg1.h"

#include "GlobalFunctions.h"//6.2.0 For read Chinese name

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CCTOsherAliWnd, CMemWnd)

	//ON_WM_LBUTTONDBLCLK()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_WM_RBUTTONUP()

	ON_BN_CLICKED(IDC_IMG_MOUSE_CONTROL_ITEM, OnImgMouseButtonClicked)
	ON_BN_CLICKED(IDC_RING_MOUSE_CONTROL_ITEM, OnRingMouseButtonClicked)

	ON_COMMAND(IDC_TYPE_CEYE_ITEM, OnTypeCEyeItemClicked)
	ON_COMMAND(IDC_TYPE_CAXM_ITEM, OnTypeCAxmItemClicked)
	ON_COMMAND(IDC_TYPE_CTNM_ITEM, OnTypeCTnmItemClicked)
	ON_COMMAND(IDC_TYPE_CRFM_ITEM, OnTypeCRfmItemClicked)
	ON_COMMAND(IDC_TYPE_CELM_ITEM, OnTypeCElmItemClicked)//z elevation
	ON_COMMAND(IDC_TYPE_CWFM_ITEM, OnTypeCWfmItemClicked)
	ON_COMMAND(IDC_TYPE_CRMS_ITEM, OnTypeCRmsItemClicked)
	ON_COMMAND(IDC_TYPE_CSKM_ITEM, OnTypeCSkmItemClicked)
	ON_COMMAND(IDC_TYPE_TLTR_ITEM, OnTypeSenllenItemClicked)// on show snellen E select

	ON_COMMAND(IDC_SHOW_COLOR_PUPIL_ITEM, OnShowPupilItemClicked)
	ON_COMMAND(IDC_SHOW_COLOR_LIMBUS_ITEM, OnShowLimbusItemClicked)
	ON_COMMAND(IDC_SHOW_COLOR_RRULER_ITEM, OnShowRRulerItemClicked)
	ON_COMMAND(IDC_SHOW_COLOR_LRULER_ITEM, OnShowLRulerItemClicked)
	ON_COMMAND(IDC_SHOW_COLOR_INLAY_ITEM, OnShowInlayItemClicked)//530


	ON_COMMAND(IDC_EDIT_LIMBUS_ITEM, OnEditLimbusItemClicked)

	ON_COMMAND(IDC_DEFAULT_GUI_ITEM, OnShowDefaultItemClicked)//521

END_MESSAGE_MAP()

//***************************************************************************************
//int CCTOsherAliWnd::m_wheel_n = 0;
//int CCTOsherAliWnd::m_wheel_n2 = 0;
//***************************************************************************************
void CCTOsherAliWnd::SafeInteRelease(int **ppT, int Row)
{
	if (ppT != NULL) 
	{
		for (int i = 0; i<Row; i++) 
		{
			if (ppT[i] != NULL) 
			{
				delete[](ppT[i]);
				ppT[i] = NULL;
			}
		}
		delete[](ppT);
		ppT = NULL;
	}
}

//*****************************************************************************************

double CCTOsherAliWnd::Gaussian(int x, int y, double de)
{
	double value = 0;
	double pai = 3.1415926;
	double e = 2.718281828;

	value = (-1)*((x*x + y*y) / (2 * de*de));
	value = (1 / (2 * pai*de*de))*pow(e, value);

	return value;
}

//*****************************************************************************************

HSI CCTOsherAliWnd::ChangeRGBtoHSI(int R, int G, int B)
{

	HSI res;
	res.I = (real)(R + G + B) / 3;

	//find the minixum of RGB for the S value
	real min = (real)R;
	if (min>G) min = G;
	if (min>B) min = B;
	//
	res.S = 1 - (min / res.I);

	//find the S
	real sita;
	real mid1 = 0.5*(2 * R - G - B);
	real mid2 = pow((R - G)*(R - G) + (R - B)*(G - B), 0.5) + 0.0000000001;
	mid1 = mid1 / mid2;
	sita = acos(mid1);

	if (B>G) sita = 2 * _Pi - sita;

	real angle = sita * 180 / _Pi;

	res.H = angle;
	//
	return res;
}


//*****************************************************************************************

CCTOsherAliWnd::CCTOsherAliWnd(CWnd* pParentWnd, RECT& Rect, CCTExam* pCTExam, int ThisMain)
{
	m_ThisMain = ThisMain;

	m_LensColor = 0x00007f9f;
	m_CaliperColor = MAGENTA;//RGB(230,215,30);//RGB(195,145,65);
	m_CaliperColor2 = CYAN;//RGB(255,128,0);
	m_AlignLine = FALSE;

	m_MoveCalipar = 0;
	m_MousePos = 0;

	m_pCTExam = pCTExam;
	m_wheel_n = 0;
	m_last_wheel_n = 0;
	m_wheel_n2 = 0;
	m_pMenu = new CXPMenu();

	m_ShowDymImg = TRUE;
	m_ShowOsherRing = FALSE;

	m_ShowLimbus = TRUE;
	m_ShowPupil = TRUE;
	m_ShowRRuler = TRUE;
	m_ShowLRuler = TRUE;
	m_ShowInlay = TRUE;//530

	m_a1 = 0;
	m_Ali_Rum1 = 0.0;
	m_a2 = 0;
	m_Ali_Rum2 = 0.0;

	CreateWnd(Rect, pParentWnd);

	m_ImgSize = m_h*m_w * 3;

	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h;

	for (int t = 0; t < 15; t++)
	{
		try {
			m_pZoomBackup[t] = new uchar[m_ImgSize];
			m_BackupYes[t] = FALSE;
			m_cxBackup[t] = m_cx;
			m_cyBackup[t] = m_cy;
		}
		catch (...) {
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


	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
	{
		m_pImage = &m_pCTExam->m_ColorImage;
		m_pImage->SwapImageInfo(&m_pCTExam->m_Image);

		if (!m_pImage->m_ve_ok)
		{
			m_pImage->FindWFVertex();
		}
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
				real puveDisum = Distance(m_pImage->m_pu_x0_um, m_pImage->m_pu_y0_um, m_pImage->m_ve_x_um, m_pImage->m_ve_y_um);

				if (puveDisum > 1000)
				{
					m_pImage->ClearPupil();
					m_pImage->m_pu_ok = FindColorPupil(m_pImage);
					int a = 0;
				}
			}
		}
		else
		{
			if (m_pImage->m_ve_ok)
			{
				real puveDisum = Distance(m_pImage->m_pu_x0_um, m_pImage->m_pu_y0_um, m_pImage->m_ve_x_um, m_pImage->m_ve_y_um);

				if (puveDisum > 1000)
				{
					m_pImage->ClearPupil();
					m_pImage->m_pu_ok = FindColorPupil(m_pImage);
					int a = 0;
				}
			}
		}
		//m_pImage->SaveIntoFile("C://1.jpg");

		////521, Recalculate the visual axis by useing ct gray image
		CEyeImage*  m_GrayImage;

		m_GrayImage = &m_pCTExam->m_Image;

		real xGrayDis = m_GrayImage->m_li_x0_um - m_GrayImage->m_ve_x_um;
		real yGrayDis = m_GrayImage->m_li_y0_um - m_GrayImage->m_ve_y_um;

		m_pImage->m_ve_x_um = m_pImage->m_li_x0_um - xGrayDis;
		m_pImage->m_ve_y_um = m_pImage->m_li_y0_um - yGrayDis;
		////Done
	}
	else
	{
		m_pImage = &m_pCTExam->m_Image;
	}

	//Create the pZoomImages
	CreateZoomImg();
	//

	real t = 0.02 * m_w;
	int h = intRound(t);
	int w = intRound(5 * t);
	int fs = intRound(0.02 * ::GetSystemMetrics(SM_CYSCREEN));

	::SetRect(&m_Ratio1Rect, m_w - 2 - w, m_h - 8 - 2 * h, m_w + 14 - w, m_h - 8 - h);
	m_ImgMouseCotrol.Create(_T(""), _T(""), 0, m_Ratio1Rect, this, IDC_IMG_MOUSE_CONTROL_ITEM);
	m_ImgMouseCotrol.SetImage(IDR_ACCEPT0);
	m_ImgMouseCotrol.SetBk(FALSE);
	m_ImgMouseCotrol.SetBkColor(BLACK);
	m_ImgMouseCotrol.ShowWindow(SW_SHOW);
	//m_ImgMouseCotrol.SetCheck(1);


	::SetRect(&m_Ratio2Rect, m_w - 2 - w, m_h - 8 - h, m_w + 14 - w, m_h - 8);
	m_RingMouseCotrol.Create(_T(""), _T(""), 0, m_Ratio2Rect, this, IDC_IMG_MOUSE_CONTROL_ITEM);
	m_RingMouseCotrol.SetImage(IDR_ACCEPT0);
	m_RingMouseCotrol.SetBk(FALSE);
	m_RingMouseCotrol.SetBkColor(BLACK);
	m_RingMouseCotrol.ShowWindow(SW_HIDE);
	//m_RingMouseCotrol.SetCheck(0);


	::SetRect(&m_Ratio1TextRect, m_w + 11 - w, m_h - 8 - 2 * h, m_w - 2, m_h - 8 - h);
	::SetRect(&m_Ratio2TextRect, m_w + 11 - w, m_h - 8 - h, m_w - 2, m_h - 8);

	::SetRect(&m_PNameTextRect, m_w - 5 * w, 3, m_w - 8, intRound(1.5*h));
	::SetRect(&m_pIDTextRect, m_w - 5 * w, intRound(1.5*h), m_w - 8, 3 * h);
	::SetRect(&m_pODOSTextRect, m_w - 2 - w, 3 * h, m_w - 2, intRound(4.5*h));//531

	::SetRect(&m_LRulerText1Rect, 8, m_h - 8 - 9 * h, 2 * w, m_h - 8 - 8 * h);
	::SetRect(&m_LRulerText2Rect, 8, m_h - 8 - 8 * h, 2 * w, m_h - 8 - 7 * h);
	::SetRect(&m_RRulerText1Rect, 8, m_h - 8 - 6 * h, 2 * w, m_h - 8 - 5 * h);
	::SetRect(&m_RRulerText2Rect, 8, m_h - 8 - 5 * h, 2 * w, m_h - 8 - 4 * h);

	//530
	::SetRect(&m_InlayText1Rect, 8, m_h - 8 - 3 * h, 2 * w, m_h - 8 - 2 * h);
	::SetRect(&m_InlayText2Rect, 8, m_h - 8 - 2 * h, 2 * w, m_h - 8 - 1 * h);
	::SetRect(&m_InlayText3Rect, 8, m_h - 8 - h, 2 * w, m_h - 8);
	//530

	Repaint();
	ShowWindow(SW_SHOW);
}

void CCTOsherAliWnd::RealseMem()
{
	for (int t = 0; t < 15; t++)
	{
		SAFE_DELETE_ARRAY(m_pZoomBackup[t]);
	}
}

//***************************************************************************************

void CCTOsherAliWnd::RepaintMemDC()
{
	ClearMemDC();
	Eye();
	AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2);
	AlignLine(m_a1, m_Ali_Rum1, 0);
	AlignLine(m_a2, m_Ali_Rum2, 1);
	Grid();
	Points();//Show the vertex(light reflexes)
	Limbus();
	Pupil();
	RadialRuler();
	LinearRuler();
	Inlay();//530
	Labels();
}

//***************************************************************************************

void CCTOsherAliWnd::CreateZoomImg()
{
	if (m_pImage == NULL) return;
	if (m_pImage->m_RGBData.GetMem() == NULL) return;

	m_pZoomImage2.m_w = m_w;
	m_pZoomImage2.m_h = m_h;
	m_pZoomImage2.m_RGBData.Create(m_h, m_w * 3, NULL);

	real x0_um = m_pImage->m_ve_x_um;
	real y0_um = m_pImage->m_ve_y_um;

	m_EyeWndTop = 1000;
	m_EyeWndLeft = 1000;
	m_EyeWndButtom = 0;
	m_EyeWndRight = 0;


	for (int y = 0; y < m_h; y++)
	{
		//real y_um = y0_um - (m_cy - y) / m_y_px_um;
		real y_um = -(m_cy - y) / m_y_px_um;
		for (int x = 0; x < m_w; x++)
		{
			//real x_um = x0_um + (x - m_cx) / m_x_px_um;
			real x_um = +(x - m_cx) / m_x_px_um;
			real r, g, b;
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

	/*m_cx = m_EyeWndTop + (round)((real)(m_EyeWndHeight)/2.0);
	m_cy = m_EyeWndLeft + (round)((real)(m_EyeWndWidth)/2.0);*/

	m_Adx = m_EyeWndLeft + (real)m_EyeWndWidth / 2.0 - m_cx;
	m_Ady = m_EyeWndTop + (real)m_EyeWndHeight / 2.0 - m_cy;
}

//***************************************************************************************
void CCTOsherAliWnd::Eye()
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
			real zoomRatio = 1;

			for (int i = 0; i<t; i++)
			{
				zoomRatio *= 1.05;
			}

			real actuaLMoveX = intRound((real)m_MovePoint.x / zoomRatio);
			real actualMoveY = intRound((real)m_MovePoint.y / zoomRatio);

			int  zoomWidth = intRound((real)m_EyeWndWidth / zoomRatio);
			int  zoomHeight = intRound((real)m_EyeWndHeight / zoomRatio);

			//Find the center pos
			int  zoomOutWidth = intRound((real)m_EyeWndWidth*zoomRatio);
			int  zoomOutHeight = intRound((real)m_EyeWndHeight*zoomRatio);

			int  zoomOutLeft, zoomOutTop;

			zoomOutLeft = m_EyeWndLeft - intRound((real)(zoomOutWidth - m_EyeWndWidth) / 2.0);
			zoomOutTop = m_EyeWndTop - intRound((real)(zoomOutHeight - m_EyeWndHeight) / 2.0);
			zoomOutLeft += m_MovePoint.x;
			zoomOutTop += m_MovePoint.y;
			//Done


			int  zoomLeft, zoomTop;

			zoomLeft = m_EyeWndLeft + intRound((real)(m_EyeWndWidth - zoomWidth) / 2.0);
			zoomTop = m_EyeWndTop + intRound((real)(m_EyeWndHeight - zoomHeight) / 2.0);

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

				m_cxBackup[t] = zoomOutLeft + intRound((real)(zoomOutWidth) / 2.0) - m_Adx;
				m_cyBackup[t] = zoomOutTop + intRound((real)(zoomOutHeight) / 2.0) - m_Ady;

				m_cx = m_cxBackup[t];
				m_cy = m_cyBackup[t];
			}
		}
	}
}

//***************************************************************************************

void CCTOsherAliWnd::Grid()
{
	real x_px_um = m_x_px_um;
	real y_px_um = m_y_px_um;

	COLORREF Color = m_Printing ? BLACK : WHITE;

	CString s;

	int FontSize = intRound(0.045 * m_h);
	int FontSize2 = intRound(0.025 * m_h);

	real r_um = 4000.0;

	real d_efX = 0;
	real d_efY = 0;

	if (m_pImage->m_ve_ok)
	{
		d_efX = m_pImage->m_ve_x_um* m_x_px_um;
		d_efY = m_pImage->m_ve_y_um* m_y_px_um;
	}

	int t = m_wheel_n;
	real  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	// circle
	int rx;
	int ry;
	/* int rx = intRound(r_um * m_x_px_um);
	int ry = intRound(r_um * m_y_px_um);
	m_MemDC.DrawEllipse(intRound(m_cx + m_defX), intRound(m_cy - m_defY), rx, ry, 3, Color, NOCOLOR);*/

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
		real r1_um = r_um + 100.0;
		int x1 = intRound(m_cx + d_efX*zoomRatio + x_px_um * r1_um * COS[a]);
		int y1 = intRound(m_cy - d_efY*zoomRatio - y_px_um * r1_um * SIN[a]);
		real r2_um = 0;

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
				real r3_um = r_um + 900.0;
				int x = intRound(m_cx + d_efX*zoomRatio + x_px_um * r3_um * COS[a]);
				int y = intRound(m_cy - d_efY*zoomRatio - y_px_um * r3_um * SIN[a]);
				s.Format(_T(" %i°"), sa);
				m_MemDC.WriteRotatedText(s, x, y, a - 90, FontSize, 400, "Arial", Color, NOCOLOR);
			}
			else
			{
				if (a % 15 == 0)
				{
					real r3_um = r_um + 900.0;
					int x = intRound(m_cx + d_efX*zoomRatio + x_px_um * r3_um * COS[a]);
					int y = intRound(m_cy - d_efY*zoomRatio - y_px_um * r3_um * SIN[a]);
					s.Format(_T(" %i°"), sa);
					m_MemDC.WriteRotatedText(s, x, y, a - 90, FontSize, 400, "Arial", Color, NOCOLOR);
				}
				else
				{
					real r3_um = r_um + 500.0;
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

void CCTOsherAliWnd::AlignLine(int a, real Ali_Rum, int Type)//0: First Line; 1:Second Line
{
	if (!m_AlignLine) return;

	real x_px_um = m_x_px_um;
	real y_px_um = m_y_px_um;

	real d_efX = 0;
	real d_efY = 0;

	if (m_pImage->m_ve_ok)
	{
		d_efX = m_pImage->m_ve_x_um* m_x_px_um;
		d_efY = m_pImage->m_ve_y_um* m_y_px_um;
	}

	int t = m_wheel_n;
	real  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	int x1 = intRound(m_cx + d_efX*zoomRatio);
	int y1 = intRound(m_cy - d_efY*zoomRatio);

	int x2 = intRound(m_cx + d_efX*zoomRatio + Ali_Rum *zoomRatio * COS[a] * x_px_um);
	int y2 = intRound(m_cy - d_efY*zoomRatio - Ali_Rum *zoomRatio * SIN[a] * y_px_um);

	int w = intRound(0.005 * m_h);


	COLORREF       UsedColor = m_CaliperColor;
	if (Type != 0)  UsedColor = m_CaliperColor2;

	m_MemDC.DrawLine(x1, y1, x2, y2, w, UsedColor);
	m_MemDC.DrawCircle(x2, y2, w, w, UsedColor, UsedColor);


	//Show the line distance
	//aaa
	//Done
}

//***************************************************************************************

void CCTOsherAliWnd::AlignLineDeg(int a1, int a2, real Ali_Rum1, real Ali_Rum2)
{
	if (!m_AlignLine) return;

	if (Ali_Rum1 == 0 || Ali_Rum2 == 0) return;

	int a01, a02;
	a01 = a1;
	a02 = a2;

	if (!(angular_distance_counterclockwise(a1, a2) < 180))
	{
		a01 = a2;
		a02 = a1;
	}

	real d_efX = 0;
	real d_efY = 0;

	if (m_pImage->m_ve_ok)
	{
		d_efX = m_pImage->m_ve_x_um* m_x_px_um;
		d_efY = m_pImage->m_ve_y_um* m_y_px_um;
	}

	int t = m_wheel_n;
	real  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	int  x = intRound(m_cx + d_efX*zoomRatio);
	int  y = intRound(m_cy - d_efY*zoomRatio);

	int  w = intRound(0.002 * m_h);
	int  r = intRound(0.9*(Ali_Rum1 < Ali_Rum2 ? Ali_Rum1 : Ali_Rum2)*zoomRatio* m_x_px_um);

	m_MemDC.DrawArc(x, y, r, w, a01, a02, BLACK);

	//Degree text
	int  ADis = SmallAng(a1, a2);
	CString s;
	s.Format(_T("%i°"), ADis);

	int amin = min(a01, a02);
	int amax = max(a01, a02);
	int a03 = (amin + amax) / 2;

	if (amax - amin > 180) { // ëåæàò ïî ðàçíûå ñòîðîíû íóëÿ
		a03 = (a03 + 180) % 360;
	}
	x = intRound(m_cx + d_efX*zoomRatio + (r + 15) * COS[a03]);
	y = intRound(m_cy - d_efY*zoomRatio - (r + 15) * SIN[a03]);

	int FontSize = intRound(0.020 * m_h);
	int o = CheckAngle((a03 - 90) - 1);

	m_MemDC.WriteRotatedText(s, x, y, o, FontSize, 400, "Arial", BLACK, NOCOLOR);
}

//***************************************************************************************
void CCTOsherAliWnd::Pupil()
{
	if (!m_ShowPupil) return;
	if (!m_pImage->m_pu_ok) return;
	/*{
	if(m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL)
	m_pImage->FindPupil();
	if (!m_pImage->m_pu_ok) return;
	}*/

	int x0, y0;

	int t = m_wheel_n;
	real  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	real x0R = +m_pImage->m_pu_x0_um * m_x_px_um;
	real y0R = -m_pImage->m_pu_y0_um * m_y_px_um;

	x0 = intRound(m_cx + x0R*zoomRatio);
	y0 = intRound(m_cy + y0R*zoomRatio);

	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, GREEN);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, GREEN);

	for (int a = 0; a < 360; a++)
	{
		int b = a == 359 ? 0 : a + 1;

		if (m_pImage->m_pu_r_um[a] == INVALID_VALUE) continue;
		if (m_pImage->m_pu_r_um[b] == INVALID_VALUE) continue;

		real xa0 = +(m_pImage->m_pu_x0_um + m_pImage->m_pu_r_um[a] * COS[a])* m_x_px_um;
		real ya0 = -(m_pImage->m_pu_y0_um + m_pImage->m_pu_r_um[a] * SIN[a])* m_y_px_um;
		real xb0 = +(m_pImage->m_pu_x0_um + m_pImage->m_pu_r_um[b] * COS[b])* m_x_px_um;
		real yb0 = -(m_pImage->m_pu_y0_um + m_pImage->m_pu_r_um[b] * SIN[b])* m_y_px_um;

		int xa = intRound(m_cx + xa0*zoomRatio);
		int ya = intRound(m_cy + ya0*zoomRatio);
		int xb = intRound(m_cx + xb0*zoomRatio);
		int yb = intRound(m_cy + yb0*zoomRatio);

		m_MemDC.DrawLine(xa, ya, xb, yb, 3, GREEN);
	}
}

//***************************************************************************************

void CCTOsherAliWnd::RadialRuler()
{
	if (!m_ShowRRuler) return;

	if (m_pImage->m_Ra_ok != TRUE)  return;

	int x0, y0;

	int t = m_wheel_n;
	real  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	real x0R = +m_pImage->m_Ra_x_um * m_x_px_um;
	real y0R = -m_pImage->m_Ra_y_um * m_y_px_um;

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

void CCTOsherAliWnd::LinearRuler()
{
	if (!m_ShowLRuler) return;

	if (m_pImage->m_LRa_ok != TRUE) return;
	//
	int t = m_wheel_n;
	real  zoomRatio = 1.0;
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
//530
void CCTOsherAliWnd::Inlay()
{
	if (!m_ShowInlay) return;

	if (m_pImage->m_In_ok != TRUE) return;
	//
	int t = m_wheel_n;
	real  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	COLORREF LRulerColor = RGB(255, 128, 0);

	int  x0 = intRound(m_cx + (m_pImage->m_In_x_um) * m_x_px_um * zoomRatio);
	int  y0 = intRound(m_cy - (m_pImage->m_In_y_um) * m_y_px_um * zoomRatio);

	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);

	int OutSideWidth = intRound(m_pImage->m_In_r_um * m_x_px_um * zoomRatio);
	if (m_pImage->m_In_In_r_um  < 0) m_pImage->m_In_In_r_um = 0;
	int InsideWidth = intRound(m_pImage->m_In_In_r_um * m_x_px_um * zoomRatio);

	int OutSideLeftX = x0 - OutSideWidth - 1;
	int OutSideRightX = x0 + OutSideWidth + 1;
	int OutSideTopY = y0 - OutSideWidth - 1;
	int OutSideBotmY = y0 + OutSideWidth + 1;


	for (int i = OutSideLeftX; i <= OutSideRightX; i++)
	{
		for (int j = OutSideTopY; j <= OutSideBotmY; j++)
		{

			real disX = (real)(i - x0);
			real disY = (real)(j - y0);

			real dis = sqrt(disX*disX + disY*disY);

			if (dis <= OutSideWidth && dis >= InsideWidth)
			{
				COLORREF C = m_MemDC.GetPixel(i, j);
				int bR = GetRValue(C);
				int bG = GetGValue(C);
				int bB = GetBValue(C);
				int Brightness = intRound(0.3*bR + 0.59*bG + 0.11*bB);

				int R = 250;
				int G = 250;
				int B = 200;

				if (Brightness > 200)
				{
					R = 220; G = 220; B = 170;
				}


				int A = 128;
				int bA = 127;
				R = (A * R + bA * bR) / 255;
				G = (A * G + bA * bG) / 255;
				B = (A * B + bA * bB) / 255;

				m_MemDC.SetPixelV(i, j, RGB(R, G, B));
			}

			//if(dis <= OutSideWidth && dis >= InsideWidth)
			//	m_MemDC.SetPixel(i, j, 250, 250, 200, 128);

		}
	}

	//Draw the dashed edge
	int tE = 0;
	for (int a = 0; a < 360; a++)
	{
		int b = a == 359 ? 0 : a + 1;

		int xa = x0 + intRound(OutSideWidth * COS[a]);
		int ya = y0 - intRound(OutSideWidth * SIN[a]);
		int xb = x0 + intRound(OutSideWidth * COS[b]);
		int yb = y0 - intRound(OutSideWidth * SIN[b]);

		int x2a = x0 + intRound(InsideWidth * COS[a]);
		int y2a = y0 - intRound(InsideWidth * SIN[a]);
		int x2b = x0 + intRound(InsideWidth * COS[b]);
		int y2b = y0 - intRound(InsideWidth * SIN[b]);

		tE++;
		if (tE == 10)
			tE = 0;
		if (tE <= 5)
		{
			m_MemDC.DrawLine(xa, ya, xb, yb, 1, BLACK);
			m_MemDC.DrawLine(x2a, y2a, x2b, y2b, 1, BLACK);
		}
		else
		{
			m_MemDC.DrawLine(xa, ya, xb, yb, 1, WHITE);
			m_MemDC.DrawLine(x2a, y2a, x2b, y2b, 1, WHITE);
		}
	}
	//Done

	//Draw four lines and txt length
	//  1
	//2   0
	//  3
	//x1,y1 are 4 points in the  disk out-edge;
	//x2,y2 are 4 points in the limbua edge
	//
	if (m_pImage->m_li_ok)
	{
		int x1[4], y1[4], x2[4], y2[4];

		x1[0] = OutSideRightX;
		y1[0] = y0;

		x1[1] = x0;
		y1[1] = OutSideBotmY;

		x1[2] = OutSideLeftX;
		y1[2] = y0;

		x1[3] = x0;
		y1[3] = OutSideTopY;

		//
		for (int i = 0; i < 4; i++) { x2[i] = -1000; y2[i] = -1000; }

		int minXT = 10000;
		int minXB = 10000;
		int minYR = 10000;
		int minYL = 10000;
		for (int a = 0; a < 360; a++)
		{
			int b = a == 359 ? 0 : a + 1;

			int xa = intRound(m_cx + (m_pImage->m_li_x0_um + m_pImage->m_li_r_mean_um * COS[a]) * m_x_px_um * zoomRatio);
			int ya = intRound(m_cy - (m_pImage->m_li_y0_um + m_pImage->m_li_r_mean_um * SIN[a]) * m_y_px_um * zoomRatio);
			int xb = intRound(m_cx + (m_pImage->m_li_x0_um + m_pImage->m_li_r_mean_um * COS[b]) * m_x_px_um * zoomRatio);
			int yb = intRound(m_cy - (m_pImage->m_li_y0_um + m_pImage->m_li_r_mean_um * SIN[b]) * m_y_px_um * zoomRatio);

			m_MemDC.DrawLine(xa, ya, xb, yb, 3, GRAY);

			int yDis = abs(ya - y0);
			int xDis = abs(xa - x0);

			if (yDis < minYR && ((a > 270 && a < 360) || (a >= 0 && a < 90)))
			{
				minYR = yDis;
				if (xa > x1[0])
				{
					x2[0] = xa;
					y2[0] = y0;
				}
			}

			if (yDis < minYL && a > 90 && a < 270)
			{
				minYL = yDis;
				if (xa < x1[2])
				{
					x2[2] = xa;
					y2[2] = y0;
				}
			}

			if (xDis < minXB && a > 180 && a < 360)
			{
				minXB = xDis;
				if (ya > y1[1])
				{
					x2[1] = x0;
					y2[1] = ya;
				}

			}

			if (xDis < minXT && a > 0 && a < 180)
			{
				minXT = xDis;
				if (ya < y1[3])
				{
					x2[3] = x0;
					y2[3] = ya;
				}
			}
		}

		//RECT rect1,rect2,rect3,rect4;
		RECT rect1 = { x1[0], y1[0] - 4, x2[0], y1[0] + 4 };
		RECT rect2 = { x1[1] - 4, y1[1], x1[1] + 4, y2[1] };
		RECT rect3 = { x1[2], y1[2] - 4, x2[2], y1[2] + 4 };
		RECT rect4 = { x1[3] - 4, y1[3], x1[3] + 4, y2[3] };

		if (y2[0] != -1000 && x2[0] != -1000)
			m_MemDC.DrawRectangle(rect1, 0x00505050, 0x00505050);
		if (y2[1] != -1000 && x2[1] != -1000)
			m_MemDC.DrawRectangle(rect2, 0x00505050, 0x00505050);
		if (y2[2] != -1000 && x2[2] != -1000)
			m_MemDC.DrawRectangle(rect3, 0x00505050, 0x00505050);
		if (y2[3] != -1000 && x2[3] != -1000)
			m_MemDC.DrawRectangle(rect4, 0x00505050, 0x00505050);


		//Show the length of four lines
		CString sDis1, sDis2, sDis3, sDis4;

		real xD = (real)x1[0] - (real)x2[0];
		real yD = (real)y1[0] - (real)y2[0];
		real dis1 = (sqrt(xD*xD + yD*yD) / m_x_px_um)*0.001;
		sDis1.Format(_T("%.3fmm"), dis1);

		xD = (real)x1[1] - (real)x2[1];
		yD = (real)y1[1] - (real)y2[1];
		real dis2 = (sqrt(xD*xD + yD*yD) / m_x_px_um)*0.001;
		sDis2.Format(_T("%.3fmm"), dis2);

		xD = (real)x1[2] - (real)x2[2];
		yD = (real)y1[2] - (real)y2[2];
		real dis3 = (sqrt(xD*xD + yD*yD) / m_x_px_um)*0.001;
		sDis3.Format(_T("%.3fmm"), dis3);

		xD = (real)x1[3] - (real)x2[3];
		yD = (real)y1[3] - (real)y2[3];
		real dis4 = (sqrt(xD*xD + yD*yD) / m_x_px_um)*0.001;
		sDis4.Format(_T("%.3fmm"), dis4);

		int FontSize = intRound(0.02 * m_h);
		CMFont Font(FontSize, 400, "Arial");
		RECT rect1Txt = { x1[0], y1[0] + 30, x2[0], y1[0] + 10 };
		m_MemDC.WriteText(sDis1, rect1Txt, Font, BLACK, 1, NOCOLOR);

		int yStart = y1[1] + (int)((real)(y2[1] - y1[1]) / 2.0);
		m_MemDC.WriteRotatedText(sDis2, x1[1] + 15, yStart, 90, FontSize, 300, "Arial", BLACK, NOCOLOR);

		RECT rect3Txt = { x1[2], y1[2] + 30, x2[2], y1[2] + 10 };
		m_MemDC.WriteText(sDis3, rect3Txt, Font, BLACK, 1, NOCOLOR);

		yStart = y1[3] + (int)((real)(y2[3] - y1[3]) / 2.0);
		m_MemDC.WriteRotatedText(sDis4, x1[3] + 15, yStart, 90, FontSize, 300, "Arial", BLACK, NOCOLOR);
	}
	//Done
}
//530
//***************************************************************************************

void CCTOsherAliWnd::Limbus()
{
	if (!m_ShowLimbus) return;
	if (!m_pImage->m_li_ok) return;

	int x0, y0;

	int t = m_wheel_n;
	real  zoomRatio = 1.0;
	for (int i = 0; i<t; i++) { zoomRatio *= 1.05; }

	real x0R = +m_pImage->m_li_x0_um * m_x_px_um;
	real y0R = -m_pImage->m_li_y0_um * m_y_px_um;

	x0 = intRound(m_cx + x0R*zoomRatio);
	y0 = intRound(m_cy + y0R*zoomRatio);

	m_MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 3, BLUE);
	m_MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 3, BLUE);

	for (int a = 0; a < 360; a++)
	{
		int b = a == 359 ? 0 : a + 1;

		real xa0 = +(m_pImage->m_li_x0_um + m_pImage->m_li_r_mean_um * COS[a])* m_x_px_um;
		real ya0 = -(m_pImage->m_li_y0_um + m_pImage->m_li_r_mean_um * SIN[a])* m_y_px_um;
		real xb0 = +(m_pImage->m_li_x0_um + m_pImage->m_li_r_mean_um * COS[b])* m_x_px_um;
		real yb0 = -(m_pImage->m_li_y0_um + m_pImage->m_li_r_mean_um * SIN[b])* m_y_px_um;

		int xa = intRound(m_cx + xa0*zoomRatio);
		int ya = intRound(m_cy + ya0*zoomRatio);
		int xb = intRound(m_cx + xb0*zoomRatio);
		int yb = intRound(m_cy + yb0*zoomRatio);

		m_MemDC.DrawLine(xa, ya, xb, yb, 3, BLUE);
	}
}

//***************************************************************************************
void CCTOsherAliWnd::Points()
{
	int x0, y0;
	real d_efX = m_pImage->m_ve_x_um* m_x_px_um;
	real d_efY = m_pImage->m_ve_y_um* m_y_px_um;

	int t = m_wheel_n;
	real  zoomRatio = 1.0;
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

void CCTOsherAliWnd::Labels()
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
	//531
	OD = m_pCTExam->m_Header.IsOD() ? "OD" : "OS";
	m_MemDC.WriteText(OD, m_pODOSTextRect, LabelsFont2, WHITE, 1, BLACK);//531

																		 //The linear ruler and radial ruler
	CString s1, s2;

	if (m_ShowLRuler && m_pImage->m_LRa_ok == TRUE)
	{
		COLORREF LRulerColor = RGB(255, 128, 0);
		real dis = hyp(m_pImage->m_LRa_x0_um - m_pImage->m_LRa_x1_um, m_pImage->m_LRa_y0_um - m_pImage->m_LRa_y1_um);
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

	//530
	if (m_ShowInlay && m_pImage->m_In_ok == TRUE)
	{
		CString s3;

		s1 = "DISK";
		s2.Format(_T("(D) = (%0.3fmm)"), m_pImage->m_In_r_um*0.002);
		s3.Format(_T("(Inner D) = (%0.3fmm)"), m_pImage->m_In_In_r_um*0.002);

		m_MemDC.WriteText(s1, m_InlayText1Rect, LabelsFont, WHITE, 0, NOCOLOR);
		m_MemDC.WriteText(s2, m_InlayText2Rect, LabelsFont, WHITE, 0, NOCOLOR);
		m_MemDC.WriteText(s3, m_InlayText3Rect, LabelsFont, WHITE, 0, NOCOLOR);
	}
	//530
}

//***************************************************************************************

void CCTOsherAliWnd::OnShow(BOOL* check)
{
	if (!check) return;

	*check = !*check;
}

//***************************************************************************************

//void CCTOsherAliWnd::OnLButtonDblClk(uint nFlags, CPoint Point)
//{
//  GetParent()->PostMessage(WM_NOTIFY_LBTN_DBCLK, (WPARAM)0, (LPARAM)0);    
//}

//***************************************************************************************
void  CCTOsherAliWnd::OnMouseWheel(uint nFlags, short zDelta, CPoint Point)
{
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
	AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2);
	AlignLine(m_a1, m_Ali_Rum1, 0);
	AlignLine(m_a2, m_Ali_Rum2, 1);
	Grid();
	Points();
	Limbus();
	Pupil();
	RadialRuler();
	LinearRuler();
	Inlay();//530
	Labels();

	Invalidate(FALSE);
	UpdateWindow();

	return;
}

//***************************************************************************************
void CCTOsherAliWnd::OnLButtonDown(uint nFlags, CPoint Point)
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

	if (m_ShowOsherRing) OnMouseMove(nFlags, Point);
}

//***************************************************************************************

void CCTOsherAliWnd::OnLButtonUp(uint nFlags, CPoint Point)
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
	else if (m_ShowOsherRing)
	{
		m_MousePos = 0;
		m_MoveCalipar = 0;
	}
}

//***************************************************************************************

void CCTOsherAliWnd::OnMouseMove(uint nFlags, CPoint Point)
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
		//
		if (m_MovePoint.x == 0 && m_MovePoint.y == 0)
		{
		}
		else
		{
			for (int i = 1; i<15; i++) m_BackupYes[i] = FALSE;
			ClearMemDC();
			Eye();
			AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2);
			AlignLine(m_a1, m_Ali_Rum1, 0);
			AlignLine(m_a2, m_Ali_Rum2, 1);
			Grid();
			Points();
			Limbus();
			Pupil();
			RadialRuler();
			LinearRuler();
			Inlay();//530
			Labels();
			Invalidate(FALSE);
			UpdateWindow();
		}
	}

	if (m_ShowOsherRing)
	{
		if (nFlags & MK_LBUTTON)
		{
			m_AlignLine = TRUE;

			real x_px_um = m_x_px_um;
			real y_px_um = m_y_px_um;

			real x_um = (Point.x - m_cx) / x_px_um;
			real y_um = (m_cy - Point.y) / y_px_um;
			real a_rd = angle(y_um, x_um);
			int a = intRound(_180_Pi * a_rd) % 360; if (a < 0) a += 360;
			real Ali_Rum = sqrt(x_um*x_um + y_um*y_um);
			real d2 = sqr(x_um) + sqr(y_um);


			int t = m_wheel_n;
			real  zoomRatio = 1.0;
			for (int i = 0; i<t; i++) { zoomRatio *= 1.05; } Ali_Rum = Ali_Rum / zoomRatio;

			//if(a > 90 && a < 270) a = (a+180)%360;

			if (m_Ali_Rum1 == 0 && m_Ali_Rum2 == 0)
			{
				m_a1 = a;
				m_Ali_Rum1 = Ali_Rum;
			}
			else if (m_Ali_Rum1 == 0 && m_Ali_Rum2 != 0)
			{
				m_a1 = a;
				m_Ali_Rum1 = Ali_Rum;
			}
			else if (m_Ali_Rum1 != 0 && m_Ali_Rum2 == 0)
			{
				m_a2 = a;
				m_Ali_Rum2 = Ali_Rum;
			}
			else if (m_Ali_Rum1 != 0 && m_Ali_Rum2 != 0)
			{
				//m_MoveCalipar = ( abs(a - m_a1) < abs(a - m_a2) ? 1 : 2 );
				int disA1 = SmallAng(a, m_a1);
				int disA2 = SmallAng(a, m_a2);
				int disA = disA1 - disA2;
				m_MoveCalipar = (disA < 0 ? 1 : 2);

				if (abs(disA) < 10)
				{
					real disR1 = fabs(Ali_Rum - m_Ali_Rum1);
					real disR2 = fabs(Ali_Rum - m_Ali_Rum2);

					if (disR1 < disR2)
					{
						m_MoveCalipar = 1;
					}
					else
					{
						m_MoveCalipar = 2;
					}
				}


				if (m_MoveCalipar == 1)
				{
					m_a1 = a;
					m_Ali_Rum1 = Ali_Rum;
				}
				else
				{
					m_a2 = a;
					m_Ali_Rum2 = Ali_Rum;
				}
			}

			ClearMemDC();
			Eye();
			AlignLineDeg(m_a1, m_a2, m_Ali_Rum1, m_Ali_Rum2);
			AlignLine(m_a1, m_Ali_Rum1, 0);
			AlignLine(m_a2, m_Ali_Rum2, 1);
			Grid();
			Points();
			Limbus();
			Pupil();
			RadialRuler();
			LinearRuler();
			Inlay();//530
			Labels();

			Invalidate(FALSE);
			UpdateWindow();
		}
	}
}

//***************************************************************************************

void CCTOsherAliWnd::CreateMenu()
{
	//if(::Licensing.IsWorkstation())
	{
		m_pMenu->AppendItem(IDC_TYPE_CEYE_ITEM, FALSE, FALSE, "Eye Image", "");
		m_pMenu->AppendItem(IDC_TYPE_CAXM_ITEM, FALSE, FALSE, "Axial Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CTNM_ITEM, FALSE, FALSE, "Local ROC Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CRFM_ITEM, FALSE, FALSE, "Refractive Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CELM_ITEM, FALSE, FALSE, "Z Elevation Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CWFM_ITEM, FALSE, FALSE, "Wavefront Map", "");
		m_pMenu->AppendItem(IDC_TYPE_CRMS_ITEM, FALSE, FALSE, "Root Mean Square", "");
		m_pMenu->AppendItem(IDC_TYPE_CSKM_ITEM, FALSE, FALSE, "Keratometry Map", "");
		m_pMenu->AppendItem(IDC_TYPE_TLTR_ITEM, FALSE, FALSE, "Visual Acuity Chart", "");//

		m_pMenu->AppendItem(IDC_TYPE_OSHER_ALIGNMENT, TRUE, FALSE, "Osher Iris Imaging", "");

		m_pMenu->AppendSeparator();
	}

	m_pMenu->AppendItem(IDC_SHOW_COLOR_LIMBUS_ITEM, FALSE, m_ShowLimbus, "Show Limbus", "");
	m_pMenu->AppendItem(IDC_SHOW_COLOR_PUPIL_ITEM, FALSE, m_ShowPupil, "Show Pupil", "");
	m_pMenu->AppendItem(IDC_SHOW_COLOR_RRULER_ITEM, FALSE, m_ShowRRuler, "Show Radial Ruler", "");
	m_pMenu->AppendItem(IDC_SHOW_COLOR_LRULER_ITEM, FALSE, m_ShowLRuler, "Show Linear Ruler", "");
	m_pMenu->AppendItem(IDC_SHOW_COLOR_INLAY_ITEM, FALSE, m_ShowInlay, "Show Disk", "");//530

	m_pMenu->AppendSeparator();

	//530
	m_pMenu->AppendItem(IDC_EDIT_LIMBUS_ITEM, FALSE, FALSE, "Edit pupil,limbus,vertex,ruler and disk", "");
	//530

	//521 
	//if (m_Screen != 13 && m_Screen != 27)//not custom
	if (m_isCombo)
	{
		if (!m_ThisMain)
		{			
			if (m_Screen != 46 && m_Screen != 15)
			{
				m_pMenu->AppendSeparator();
				m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
			}
		}
	}
	else
	{
		m_pMenu->AppendSeparator();
		m_pMenu->AppendItem(IDC_DEFAULT_GUI_ITEM, FALSE, FALSE, "Return to default", "");
	}
	//521
}


void CCTOsherAliWnd::OnRButtonUp(uint nFlags, CPoint Point)
{
	SAFE_DELETE(m_pMenu);

	m_pMenu = new CXPMenu();

	CreateMenu();

	int l = (int)((uint)Point.x & 0x0000ffff);
	int t = (int)((uint)Point.y & 0x0000ffff);

	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, l, t, this);
}
//***************************************************************************************

//***************************************************************************************
// distance from a1 to a2 counterclockwise ª [0, 360)
int CCTOsherAliWnd::angular_distance_counterclockwise(int a1, int a2)
{
	int da = (a2 - a1) % 360;
	if (da < 0) da += 360;
	return da;
}

//***************************************************************************************
// distance between a1 and a2 ª [0, 180)
int CCTOsherAliWnd::angular_distance(int a1, int a2)
{
	int da = angular_distance_counterclockwise(a1, a2);
	if (da > 180) da = 360 - da;
	return da;
}


//***************************************************************************************

BOOL CCTOsherAliWnd::OverLapFun(int a0, int a1, int b0, int b1)
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
int  CCTOsherAliWnd::SmallAng(int a0, int a1)
{
	int dis = abs(a0 - a1);
	if (dis > 180) dis = 360 - dis;

	return dis;

	/*int a2 = a1+180;
	if(a2>= 360) a2 = a2%360;

	if(a0 > a1 && a0 > a2)
	{
	a0 = a0 + 180;
	if(a0 >= 360) a0 = a0% 360;
	}

	if(a0 < a1 && a0 < a2)
	{
	a0 = a0 + 180;
	if(a0 >= 360) a0 = a0% 360;
	}

	return abs(a0- a1) < abs(a0-a2)? abs(a0-a1) : abs(a0 -a2);	*/
}

//***************************************************************************************
void CCTOsherAliWnd::FindClearLimbus(CEyeImage* OriImage, real LastLimbuX, real LastLimbuY, real LastLimbuR)
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

	OriImage->m_li_ok = TestImage->m_li_ok;
	OriImage->m_li_r_max_um = TestImage->m_li_r_max_um;
	OriImage->m_li_r_mean_um = TestImage->m_li_r_mean_um;
	OriImage->m_li_r_min_um = TestImage->m_li_r_min_um;
	OriImage->m_li_x0_um = TestImage->m_li_x0_um;
	OriImage->m_li_y0_um = TestImage->m_li_y0_um;

	delete TestImage;
}


//***************************************************************************************

BOOL CCTOsherAliWnd::FindColorPupil(CEyeImage* OriImage)
{
	CEyeImage* TestImage = new CEyeImage();
	int h_h = TestImage->m_h = OriImage->m_h;
	int w_w = TestImage->m_w = OriImage->m_w;
	TestImage->m_h_um = OriImage->m_h_um;
	TestImage->m_w_um = OriImage->m_w_um;

	TestImage->m_RGBData.Create(OriImage->m_h, (int)OriImage->m_w * 3, OriImage->m_RGBData.GetMem());

	if (TestImage->m_RGBData.GetMem() == NULL) return FALSE;

	int ** m_pixels = (int**)calloc(w_w, sizeof(int));
	real** m_g = (real**)calloc(4, sizeof(real));
	int ** m_BlurPixels = (int**)calloc(w_w, sizeof(int));
	int ** m_gxy = (int**)calloc(w_w, sizeof(int));
	int ** m_gx = (int**)calloc(w_w, sizeof(int));
	int ** m_gy = (int**)calloc(w_w, sizeof(int));
	int ** m_dis = (int**)calloc(w_w, sizeof(int));
	int ** m_degree = (int**)calloc(w_w, sizeof(int));
	int ** m_gR = (int**)calloc(w_w, sizeof(int));

	for (int i = 0; i<w_w; i++)
	{
		if (i<4)  m_g[i] = (real*)calloc(4, sizeof(real));
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


	real de = 1.4;


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
	real Mid = 0;
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
	real fmean = 0;
	int t = 0;
	double mid = 180 / 3.1415926;

	for (int h = SRegion_StY + 1; h < SRegion_EndY - 1; h++)
	{
		for (int v = SRegion_StX + 1; v < SRegion_EndX - 1; v++)
		{
			m_gx[v][h] = (m_BlurPixels[v - 1][h - 1] * (-1) + m_BlurPixels[v + 1][h + 1] * 1 + m_BlurPixels[v - 1][h] * (-2) + m_BlurPixels[v + 1][h] * 2 + m_BlurPixels[v - 1][h + 1] * (-1) + m_BlurPixels[v + 1][h + 1] * 1);
			m_gy[v][h] = (m_BlurPixels[v - 1][h - 1] * (1) + m_BlurPixels[v][h - 1] * 2 + m_BlurPixels[v + 1][h + 1] * 1 + m_BlurPixels[v - 1][h + 1] * (-1) + m_BlurPixels[v][h + 1] * (-2) + m_BlurPixels[v + 1][h + 1] * (-1));

			m_gxy[v][h] = intRound(sqrt((real)m_gx[v][h] * m_gx[v][h] + (real)m_gy[v][h] * m_gy[v][h]));

			fmean += (real)m_gxy[v][h];
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
				m_dis[v][h] = intRound((real)m_gxy[v][h] / (real)maxGradient * 255.0);
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
void CCTOsherAliWnd::OnType(int Type)
{
	GetParent()->PostMessage(WM_NOTIFY_OSHER_ALG_BACK, (WPARAM)Type, (LPARAM)m_d);
}


//***************************************************************************************

void CCTOsherAliWnd::OnImgMouseButtonClicked()
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

void CCTOsherAliWnd::OnRingMouseButtonClicked()
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

void CCTOsherAliWnd::OnEditLimbusItemClicked()
{
	BOOL GrayImg = TRUE;

	if (m_pCTExam->m_ColorImage.m_RGBData.GetMem() != NULL) GrayImg = FALSE;

	CImageDlg1* pDlg = new CImageDlg1(this, m_pImage, FALSE, GrayImg, TRUE);


	if (pDlg->DoModal() == IDOK) {

		Repaint();

		::DB.SaveCTExam(m_pCTExam);
	}

	delete pDlg;
}

//***************************************************************************************
//521
void CCTOsherAliWnd::OnShowDefaultItemClicked()
{
	OnType(-1);
}
//521
//***************************************************************************************