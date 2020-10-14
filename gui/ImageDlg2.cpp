//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ImageDlg2.h"
#include "Data.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CImageDlg2, CDialog)

	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()//520
	ON_WM_MOUSEWHEEL()//520

	ON_COMMAND(IDK_CTRL_D, OnCtrlD)

	ON_BN_CLICKED(IDC_PUPIL_RADIO, OnPupilRadioClicked)
	ON_BN_CLICKED(IDC_PUPIL1_RADIO, OnPupil1RadioClicked)
	ON_BN_CLICKED(IDC_PUPIL2_RADIO, OnPupil2RadioClicked)
	ON_BN_CLICKED(IDC_PUPIL3_RADIO, OnPupil3RadioClicked)
	ON_BN_CLICKED(IDC_PUPIL4_RADIO, OnPupil4RadioClicked)

	ON_BN_CLICKED(IDC_LIMBUS_RADIO, OnLimbusRadioClicked)
	ON_BN_CLICKED(IDC_LIMBUS1_RADIO, OnLimbus1RadioClicked)
	ON_BN_CLICKED(IDC_LIMBUS2_RADIO, OnLimbus2RadioClicked)
	ON_BN_CLICKED(IDC_LIMBUS3_RADIO, OnLimbus3RadioClicked)
	ON_BN_CLICKED(IDC_LIMBUS4_RADIO, OnLimbus4RadioClicked)

	ON_BN_CLICKED(IDC_VERTEX_RADIO, OnVertexRadioClicked)
	ON_BN_CLICKED(IDC_VERTEX1_RADIO, OnVertex1RadioClicked)
	ON_BN_CLICKED(IDC_VERTEX2_RADIO, OnVertex2RadioClicked)
	ON_BN_CLICKED(IDC_VERTEX3_RADIO, OnVertex3RadioClicked)
	ON_BN_CLICKED(IDC_VERTEX4_RADIO, OnVertex4RadioClicked)

	//[520]
	ON_BN_CLICKED(IDC_RADIAL_RULER, OnRadialRulerClicked)
	ON_BN_CLICKED(IDC_RADIAL_RULER1, OnRadialRuler1Clicked)
	ON_BN_CLICKED(IDC_RADIAL_RULER2, OnRadialRuler2Clicked)
	ON_BN_CLICKED(IDC_RADIAL_RULER3, OnRadialRuler3Clicked)

	ON_BN_CLICKED(IDC_LINEAR_RULER, OnLinearRulerClicked)
	ON_BN_CLICKED(IDC_LINEAR_RULER1, OnLinearRuler1Clicked)
	ON_BN_CLICKED(IDC_LINEAR_RULER2, OnLinearRuler2Clicked)
	ON_BN_CLICKED(IDC_LINEAR_RULER3, OnLinearRuler3Clicked)

	//Done

	ON_BN_CLICKED(IDC_PUPIL_CHECK, Repaint)
	ON_BN_CLICKED(IDC_LIMBUS_CHECK, Repaint)
	ON_BN_CLICKED(IDC_VERTEX_CHECK, Repaint)
	ON_BN_CLICKED(IDC_RRULER_CHECK, Repaint)
	ON_BN_CLICKED(IDC_LRULER_CHECK, Repaint)
	ON_BN_CLICKED(IDC_INLAY_CHECK, Repaint)

END_MESSAGE_MAP()

//***************************************************************************************
int CImageDlg2::m_wheel_n = 0;//520
							  //***************************************************************************************

CImageDlg2::CImageDlg2(CWnd* pParentWnd, CEyeImage* pImage, BOOL Gray, BOOL RADIAL_RULER) : CDialog(IDD_IMAGE_DLG2, pParentWnd)
{
	m_Gray = Gray;
	m_RADIAL_RULER = RADIAL_RULER;
	m_pImage = pImage;
	m_last_wheel_n = 0;

	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));

	ASSERT(m_pImage);
}

//***************************************************************************************

CImageDlg2::~CImageDlg2()
{
	DestroyAcceleratorTable(m_hAccel);
}

//***************************************************************************************

BOOL CImageDlg2::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam != VK_ESCAPE) {
			if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

void CImageDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);

	DDX_Control(pDX, IDC_PUPIL_RADIO, m_PupilRadio);
	DDX_Control(pDX, IDC_PUPIL1_RADIO, m_Pupil1Radio);
	DDX_Control(pDX, IDC_PUPIL2_RADIO, m_Pupil2Radio);
	DDX_Control(pDX, IDC_PUPIL3_RADIO, m_Pupil3Radio);
	DDX_Control(pDX, IDC_PUPIL4_RADIO, m_Pupil4Radio);

	DDX_Control(pDX, IDC_LIMBUS_RADIO, m_LimbusRadio);
	DDX_Control(pDX, IDC_LIMBUS1_RADIO, m_Limbus1Radio);
	DDX_Control(pDX, IDC_LIMBUS2_RADIO, m_Limbus2Radio);
	DDX_Control(pDX, IDC_LIMBUS3_RADIO, m_Limbus3Radio);
	DDX_Control(pDX, IDC_LIMBUS4_RADIO, m_Limbus4Radio);

	DDX_Control(pDX, IDC_VERTEX_RADIO, m_VertexRadio);
	DDX_Control(pDX, IDC_VERTEX1_RADIO, m_Vertex1Radio);
	DDX_Control(pDX, IDC_VERTEX2_RADIO, m_Vertex2Radio);
	DDX_Control(pDX, IDC_VERTEX3_RADIO, m_Vertex3Radio);
	DDX_Control(pDX, IDC_VERTEX4_RADIO, m_Vertex4Radio);

	//520 Edit Radial Ruler Radios
	DDX_Control(pDX, IDC_RADIAL_RULER, m_RadialRulerRadio);
	DDX_Control(pDX, IDC_RADIAL_RULER1, m_RadialRuler1Radio);
	DDX_Control(pDX, IDC_RADIAL_RULER2, m_RadialRuler2Radio);
	DDX_Control(pDX, IDC_RADIAL_RULER3, m_RadialRuler3Radio);
	//Done

	//520 Edit Radial Ruler Radios
	DDX_Control(pDX, IDC_LINEAR_RULER, m_LinearRulerRadio);
	DDX_Control(pDX, IDC_LINEAR_RULER1, m_LinearRuler1Radio);
	DDX_Control(pDX, IDC_LINEAR_RULER2, m_LinearRuler2Radio);
	DDX_Control(pDX, IDC_LINEAR_RULER3, m_LinearRuler3Radio);
	//Done

	//520
	DDX_Control(pDX, IDC_DIS_TEXT, m_DisText);
	DDX_Control(pDX, IDC_STATIC1, m_Split);
	DDX_Control(pDX, IDC_PUPIL_CHECK, m_PupilCheck);
	DDX_Control(pDX, IDC_LIMBUS_CHECK, m_LimbusCheck);
	DDX_Control(pDX, IDC_VERTEX_CHECK, m_VertexCheck);
	DDX_Control(pDX, IDC_RRULER_CHECK, m_RRulerCheck);
	DDX_Control(pDX, IDC_LRULER_CHECK, m_LRulerCheck);
	//Done

	//530
	DDX_Control(pDX, IDC_INLAY_RADIO, m_InlayRadio);
	DDX_Control(pDX, IDC_INLAY_RADIO1, m_Inlay1Radio);
	DDX_Control(pDX, IDC_INLAY_RADIO2, m_Inlay2Radio);
	DDX_Control(pDX, IDC_INLAY_RADIO3, m_Inlay3Radio);
	DDX_Control(pDX, IDC_INLAY_RADIO4, m_Inlay4Radio);
	//530
}

//***************************************************************************************

BOOL CImageDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ShowTechInfo = FALSE;

	m_P = 1;
	m_L = 1;
	m_V = 1;
	m_R_R = 1;//520
	m_L_R = 1;//520


	m_Image = *m_pImage;

	m_x_um[0] = 0.0;
	m_y_um[0] = 0.0;
	m_x_um[1] = 0.0;
	m_y_um[1] = 0.0;

	Resize();

	//[520]
	if (!m_RADIAL_RULER)
	{
		m_RadialRulerRadio.EnableWindow(FALSE);
		m_LinearRulerRadio.EnableWindow(FALSE);
	}
	//Done

	if (m_Gray)
	{
		m_VertexRadio.EnableWindow(FALSE);
	}

	OnPupilRadioClicked();

	BOOL IsDBReadOnly = ::DB.IsReadOnly();
	m_OKButton.EnableWindow(!IsDBReadOnly);

	return FALSE;
}

//***************************************************************************************

void CImageDlg2::Resize()
{
	m_w = m_Image.m_w;
	m_h = m_Image.m_h;
	m_w_um = m_Image.m_w_um;
	m_h_um = m_Image.m_h_um;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h - 1;


	RECT Rect;
	GetWindowRect(&Rect);
	int ww = Rect.right - Rect.left;
	int wh = Rect.bottom - Rect.top;
	GetClientRect(&Rect);
	int cw = Rect.right - Rect.left;
	int ch = Rect.bottom - Rect.top;
	int tw = ww - cw;
	int th = wh - ch;

	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int H = ::GetSystemMetrics(SM_CYSCREEN);
	int d = intRound(0.005 * W);
	int bw = intRound(0.075 * W);
	int bh = intRound(0.033 * H);//intRound(0.033 * H);

	m_l = m_t = d;

	int iw = __max(640, m_w);
	int ih = __max(480, m_h);

	ww = tw + d + iw + d + bw + 3 * d;
	wh = th + d + ih + d + bh + 13 * d;

	m_MemDC.CreateTrueColorDC(m_w, m_h);

	m_Rgn.CreateRectRgn(d, d, d + m_w, d + m_h);

	SetWindowPos(NULL, 0, 0, ww, wh, SWP_NOZORDER);
	CenterWindow();

	int t = intRound(0.005 * W);
	int dt1 = intRound(0.012 * W);
	int dt2 = intRound(0.014 * W);
	int dt3 = 145;

	m_PupilRadio.SetWindowPos(NULL, iw + d + d, t, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Pupil1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Pupil2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Pupil3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Pupil4Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_LimbusRadio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Limbus1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Limbus2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Limbus3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Limbus4Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_VertexRadio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Vertex1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Vertex2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Vertex3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Vertex4Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	//520 Edit Radial Ruler Radios
	m_RadialRulerRadio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_RadialRuler1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_RadialRuler2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_RadialRuler3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//Done

	//520 Edit Linear Ruler Radios
	m_LinearRulerRadio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_LinearRuler1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_LinearRuler2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_LinearRuler3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//Done


	m_DisText.SetWindowPos(NULL, d, 2 * d + m_h + 5, 0, 0, SWP_NOSIZE);  //520;
	m_DisText.SetWindowText(_T(""));//520;

	if (m_RADIAL_RULER)
	{
		m_Split.SetWindowPos(NULL, d, 2 * d + m_h + 15, 468, 0, SWP_NOSIZE);
		m_PupilCheck.SetWindowPos(NULL, d, 6 * d + m_h + 15, 0, 0, SWP_NOSIZE);
		m_PupilCheck.SetCheck(TRUE);
		m_PupilCheck.EnableWindow(FALSE);

		m_LimbusCheck.SetWindowPos(NULL, d + dt3, 6 * d + m_h + 15, 0, 0, SWP_NOSIZE);
		m_VertexCheck.SetWindowPos(NULL, d + 2 * dt3, 6 * d + m_h + 15, 0, 0, SWP_NOSIZE);
		m_RRulerCheck.SetWindowPos(NULL, d + 3 * dt3, 6 * d + m_h + 15, 0, 0, SWP_NOSIZE);

		m_LRulerCheck.SetWindowPos(NULL, d, 9 * d + m_h + 15, 0, 0, SWP_NOSIZE);
	}
	else
	{
		m_PupilCheck.ShowWindow(SW_HIDE);
		m_LimbusCheck.ShowWindow(SW_HIDE);
		m_VertexCheck.ShowWindow(SW_HIDE);
		m_RRulerCheck.ShowWindow(SW_HIDE);
		m_LRulerCheck.ShowWindow(SW_HIDE);
	}

	GetClientRect(&Rect);
	cw = Rect.right - Rect.left;
	ch = Rect.bottom - Rect.top;

	int bl = intRound(0.5 * cw - 0.5 * d) - bw;
	int bt = intRound(ch - d - bh);
	m_CancelButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);
	bl = intRound(0.5 * cw + 0.5 * d);
	m_OKButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);
}

//***************************************************************************************

void CImageDlg2::OnCtrlD()
{
	m_ShowTechInfo = !m_ShowTechInfo;
	Repaint();
}

//***************************************************************************************

void CImageDlg2::Repaint()
{
	DrawEye(m_MemDC);
	DrawPupil(m_MemDC);
	DrawLimbus(m_MemDC);
	DrawVertex(m_MemDC);
	DrawRuler(m_MemDC);//520
	DrawLRuler(m_MemDC);//520

	InvalidateRgn(&m_Rgn, FALSE);
}

//***************************************************************************************

void CImageDlg2::OnOK()
{
	*m_pImage = m_Image;

	CDialog::OnOK();
}

//***************************************************************************************

void CImageDlg2::OnPaint()
{
	CPaintDC PaintDC(this);
	PaintDC.BitBlt(m_l, m_t, m_w, m_h, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CImageDlg2::OnLButtonUp(uint nFlags, CPoint Point)
{
	m_wheel_n = m_last_wheel_n = 0;//520

	if (Point.x < m_l || Point.x >= m_l + m_w || Point.y < m_t || Point.y >= m_t + m_h) return;
}

//***************************************************************************************
void CImageDlg2::OnLButtonDown(uint nFlags, CPoint Point)
{
	if (Point.x < m_l || Point.x >= m_l + m_w || Point.y < m_t || Point.y >= m_t + m_h) return;

	real x = (Point.x - m_l) - m_cx;
	real y = m_cy - (Point.y - m_t);

	m_x_um[0] = x / m_x_px_um;
	m_y_um[0] = y / m_y_px_um;
	m_x_um[1] = m_x_um[0];
	m_y_um[1] = m_y_um[0];

	if (m_PupilRadio.GetCheck() && m_Pupil4Radio.GetCheck())
	{
		if (m_Image.m_pu_r_mean_um >= 800)
		{
			real disX = m_x_um[1] - m_Image.m_pu_x0_um;
			real disY = m_y_um[1] - m_Image.m_pu_y0_um;

			real dis = sqrt(disX*disX + disY*disY);

			if (dis <= 800)
			{
				m_Image.m_pu_x0_um += disX;
				m_Image.m_pu_y0_um += disY;
			}
			else
			{
				//m_P_Drawed = FALSE;	   

				m_Image.m_pu_ok = TRUE;
				m_Image.m_pu_x0_um = m_x_um[0];
				m_Image.m_pu_y0_um = m_y_um[0];
				for (int a = 0; a < 360; a++) {
					m_Image.m_pu_r_um[a] = 0.0;
				}
				m_Image.m_pu_r_min_um = 0.0;
				m_Image.m_pu_r_max_um = 0.0;
				m_Image.m_pu_r_mean_um = 0.0;
			}
		}
		else
		{
			//m_P_Drawed = FALSE;	 

			m_Image.m_pu_ok = TRUE;
			m_Image.m_pu_x0_um = m_x_um[0];
			m_Image.m_pu_y0_um = m_y_um[0];
			for (int a = 0; a < 360; a++) {
				m_Image.m_pu_r_um[a] = 0.0;
			}
			m_Image.m_pu_r_min_um = 0.0;
			m_Image.m_pu_r_max_um = 0.0;
			m_Image.m_pu_r_mean_um = 0.0;
		}

		Repaint();
	}

	else if (m_LimbusRadio.GetCheck() && m_Limbus4Radio.GetCheck())
	{
		if (m_Image.m_li_r_mean_um >= 800)
		{
			real disX = m_x_um[1] - m_Image.m_li_x0_um;
			real disY = m_y_um[1] - m_Image.m_li_y0_um;

			real dis = sqrt(disX*disX + disY*disY);

			if (dis <= 800)
			{
				m_Image.m_li_x0_um += disX;
				m_Image.m_li_y0_um += disY;
			}
			else
			{
				//m_L_Drawed = FALSE;	   

				m_Image.m_li_ok = TRUE;
				m_Image.m_li_x0_um = m_x_um[0];
				m_Image.m_li_y0_um = m_y_um[0];
				for (int a = 0; a < 360; a++) {
					m_Image.m_li_r_um[a] = 0.0;
				}
				m_Image.m_li_r_min_um = 0.0;
				m_Image.m_li_r_max_um = 0.0;
				m_Image.m_li_r_mean_um = 0.0;
			}
		}
		else
		{
			m_Image.m_li_ok = TRUE;
			m_Image.m_li_x0_um = m_x_um[0];
			m_Image.m_li_y0_um = m_y_um[0];
			for (int a = 0; a < 360; a++) {
				m_Image.m_li_r_um[a] = 0.0;
			}
			m_Image.m_li_r_min_um = 0.0;
			m_Image.m_li_r_max_um = 0.0;
			m_Image.m_li_r_mean_um = 0.0;
		}

		Repaint();
	}

	else if (m_VertexRadio.GetCheck() && m_Vertex4Radio.GetCheck()) {

		m_Image.m_ve_ok = TRUE;
		m_Image.m_ve_x_um = x / m_x_px_um;
		m_Image.m_ve_y_um = y / m_y_px_um;

		Repaint();
	}

	//520
	else if (m_RadialRulerRadio.GetCheck() && m_RadialRuler3Radio.GetCheck())
	{
		if (m_Image.m_Ra_r_um >= 800)
		{
			real disX = m_x_um[1] - m_Image.m_Ra_x_um;
			real disY = m_y_um[1] - m_Image.m_Ra_y_um;

			real dis = sqrt(disX*disX + disY*disY);

			if (dis <= 800)
			{
				m_Image.m_Ra_x_um += disX;
				m_Image.m_Ra_y_um += disY;
			}
			else
			{
				//m_RR_Drawed = FALSE;	   

				m_Image.m_Ra_ok = TRUE;
				m_Image.m_Ra_x_um = m_x_um[0];
				m_Image.m_Ra_y_um = m_y_um[0];
				m_Image.m_Ra_r_um = 0.0;
			}
		}
		else
		{
			//m_RR_Drawed = FALSE;

			m_Image.m_Ra_ok = TRUE;
			m_Image.m_Ra_x_um = m_x_um[0];
			m_Image.m_Ra_y_um = m_y_um[0];
			m_Image.m_Ra_r_um = 0.0;
		}
		Repaint();
	}

	else if (m_LinearRulerRadio.GetCheck() && m_LinearRuler3Radio.GetCheck())
	{
		real disX1 = m_x_um[1] - m_Image.m_LRa_x0_um;
		real disY1 = m_y_um[1] - m_Image.m_LRa_y0_um;

		real disX2 = m_x_um[1] - m_Image.m_LRa_x1_um;
		real disY2 = m_y_um[1] - m_Image.m_LRa_y1_um;

		real dis1 = sqrt(disX1*disX1 + disY1*disY1);
		real dis2 = sqrt(disX2*disX2 + disY2*disY2);


		//
		int x0 = intRound(m_cx + m_Image.m_LRa_x0_um * m_x_px_um);
		int y0 = intRound(m_cy - m_Image.m_LRa_y0_um * m_y_px_um);
		int x1 = intRound(m_cx + m_Image.m_LRa_x1_um * m_x_px_um);
		int y1 = intRound(m_cy - m_Image.m_LRa_y1_um * m_y_px_um);
		real xum, yum;
		if (x0 < x1)
		{
			xum = m_Image.m_LRa_x0_um + intRound((real)fabs(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um) / 2.0);
			if (m_Image.m_LRa_y0_um < m_Image.m_LRa_y1_um)	yum = m_Image.m_LRa_y0_um + intRound((real)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
			else yum = m_Image.m_LRa_y1_um + intRound((real)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
		}
		else
		{
			xum = m_Image.m_LRa_x1_um + intRound((real)fabs(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um) / 2.0);
			if (m_Image.m_LRa_y0_um < m_Image.m_LRa_y1_um)	yum = m_Image.m_LRa_y0_um + intRound((real)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
			else yum = m_Image.m_LRa_y1_um + intRound((real)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
		}

		real disX3 = m_x_um[1] - xum;
		real disY3 = m_y_um[1] - yum;

		real dis3 = sqrt(disX3*disY3 + disY3*disY3);
		//real dis3 = sqrt(disX*disX + disY*disY);

		if (dis1 <= 800)
		{
			m_Image.m_LRa_x0_um += disX1;
			m_Image.m_LRa_y0_um += disY1;
			real midx = m_Image.m_LRa_x0_um;
			real midy = m_Image.m_LRa_y0_um;

			m_Image.m_LRa_x0_um = m_Image.m_LRa_x1_um;
			m_Image.m_LRa_y0_um = m_Image.m_LRa_y1_um;

			m_Image.m_LRa_x1_um = midx;
			m_Image.m_LRa_y1_um = midy;
		}
		else if (dis2 <= 800)
		{
			m_Image.m_LRa_x1_um += disX2;
			m_Image.m_LRa_y1_um += disY2;
		}
		else if (dis3 <= 800)
		{
			m_Image.m_LRa_x0_um += disX3;
			m_Image.m_LRa_y0_um += disY3;
			m_Image.m_LRa_x1_um += disX3;
			m_Image.m_LRa_y1_um += disY3;
		}
		else
		{
			m_Image.m_LRa_ok = TRUE;
			m_Image.m_LRa_x0_um = m_x_um[0];
			m_Image.m_LRa_y0_um = m_y_um[0];
			m_Image.m_LRa_x1_um = m_x_um[0];
			m_Image.m_LRa_y1_um = m_y_um[0];
		}

		Repaint();
	}

}

//***************************************************************************************

void CImageDlg2::OnMouseMove(uint nFlags, CPoint Point)
{
	if (Point.x < m_l || Point.x >= m_l + m_w || Point.y < m_t || Point.y >= m_t + m_h) return;

	real x = (Point.x - m_l) - m_cx;
	real y = m_cy - (Point.y - m_t);

	m_x_um[1] = x / m_x_px_um;
	m_y_um[1] = y / m_y_px_um;

	real dis = 1000;
	real disX, disY;
	real MoveDis = 0;

	if (m_PupilRadio.GetCheck() && m_Pupil4Radio.GetCheck() && m_Image.m_pu_r_mean_um >= 800)
	{
		disX = m_x_um[1] - m_Image.m_pu_x0_um;
		disY = m_y_um[1] - m_Image.m_pu_y0_um;

		dis = sqrt(disX*disX + disY*disY);
	}
	else if (m_LimbusRadio.GetCheck() && m_Limbus4Radio.GetCheck() && m_Image.m_li_r_mean_um >= 800)
	{
		disX = m_x_um[1] - m_Image.m_li_x0_um;
		disY = m_y_um[1] - m_Image.m_li_y0_um;

		dis = sqrt(disX*disX + disY*disY);
	}
	else if (m_RadialRulerRadio.GetCheck() && m_RadialRuler3Radio.GetCheck() && m_Image.m_Ra_r_um >= 800)
	{
		disX = m_x_um[1] - m_Image.m_Ra_x_um;
		disY = m_y_um[1] - m_Image.m_Ra_y_um;

		dis = sqrt(disX*disX + disY*disY);
	}

	else if (m_LinearRulerRadio.GetCheck() && m_LinearRuler3Radio.GetCheck())
	{
		real D = hyp(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um, m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um);
		if (D >= 1600)
		{
			int x0 = intRound(m_cx + m_Image.m_LRa_x0_um * m_x_px_um);
			int y0 = intRound(m_cy - m_Image.m_LRa_y0_um * m_y_px_um);
			int x1 = intRound(m_cx + m_Image.m_LRa_x1_um * m_x_px_um);
			int y1 = intRound(m_cy - m_Image.m_LRa_y1_um * m_y_px_um);
			real xum, yum;
			if (x0 < x1)
			{
				xum = m_Image.m_LRa_x0_um + intRound((real)fabs(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um) / 2.0);
				if (m_Image.m_LRa_y0_um < m_Image.m_LRa_y1_um)	yum = m_Image.m_LRa_y0_um + intRound((real)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
				else yum = m_Image.m_LRa_y1_um + intRound((real)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
			}
			else
			{
				xum = m_Image.m_LRa_x1_um + intRound((real)fabs(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um) / 2.0);
				if (m_Image.m_LRa_y0_um < m_Image.m_LRa_y1_um)	yum = m_Image.m_LRa_y0_um + intRound((real)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
				else yum = m_Image.m_LRa_y1_um + intRound((real)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
			}

			disX = m_x_um[1] - xum;
			disY = m_y_um[1] - yum;

			dis = sqrt(disX*disX + disY*disY);
		}
	}


	if (dis <= 800) ::SetCursor(::LoadCursor(NULL, IDC_HAND));
	else ::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	if ((nFlags & MK_LBUTTON))
	{
		if (m_PupilRadio.GetCheck() && m_Pupil4Radio.GetCheck())
		{
			if (dis <= 800)
			{
				m_Image.m_pu_x0_um += disX;
				m_Image.m_pu_y0_um += disY;
			}
			else
			{
				m_Image.m_pu_x0_um = 0.5 * (m_x_um[0] + m_x_um[1]);
				m_Image.m_pu_y0_um = 0.5 * (m_y_um[0] + m_y_um[1]);
				real r_um = 0.5 * hyp(m_x_um[0] - m_x_um[1], m_y_um[0] - m_y_um[1]);
				for (int a = 0; a < 360; a++)
				{
					m_Image.m_pu_r_um[a] = r_um;
				}
				m_Image.m_pu_r_min_um = r_um;
				m_Image.m_pu_r_max_um = r_um;
				m_Image.m_pu_r_mean_um = r_um;
			}

			Repaint();
		}

		else if (m_LimbusRadio.GetCheck() && m_Limbus4Radio.GetCheck())
		{
			if (dis <= 800)
			{
				m_Image.m_li_x0_um += disX;
				m_Image.m_li_y0_um += disY;
			}
			else
			{
				m_Image.m_li_x0_um = 0.5 * (m_x_um[0] + m_x_um[1]);
				m_Image.m_li_y0_um = 0.5 * (m_y_um[0] + m_y_um[1]);
				real r_um = 0.5 * hyp(m_x_um[0] - m_x_um[1], m_y_um[0] - m_y_um[1]);
				for (int a = 0; a < 360; a++) {
					m_Image.m_li_r_um[a] = r_um;
				}
				m_Image.m_li_r_min_um = r_um;
				m_Image.m_li_r_max_um = r_um;
				m_Image.m_li_r_mean_um = r_um;
			}

			Repaint();
		}

		else if (m_VertexRadio.GetCheck() && m_Vertex4Radio.GetCheck())
		{
			m_Image.m_ve_x_um = x / m_x_px_um;
			m_Image.m_ve_y_um = y / m_y_px_um;

			Repaint();
		}

		else if (m_RadialRulerRadio.GetCheck() && m_RadialRuler3Radio.GetCheck())
		{
			if (dis <= 800)
			{
				m_Image.m_Ra_x_um += disX;
				m_Image.m_Ra_y_um += disY;
			}
			else
			{
				m_Image.m_Ra_x_um = 0.5 * (m_x_um[0] + m_x_um[1]);
				m_Image.m_Ra_y_um = 0.5 * (m_y_um[0] + m_y_um[1]);
				m_Image.m_Ra_r_um = 0.5 * hyp(m_x_um[0] - m_x_um[1], m_y_um[0] - m_y_um[1]);
			}

			Repaint();
		}

		else if (m_LinearRulerRadio.GetCheck() && m_LinearRuler3Radio.GetCheck())
		{
			if (dis <= 800)
			{
				m_Image.m_LRa_x0_um += disX;
				m_Image.m_LRa_y0_um += disY;
				m_Image.m_LRa_x1_um += disX;
				m_Image.m_LRa_y1_um += disY;
			}
			else
			{
				m_Image.m_LRa_x1_um = m_x_um[1];
				m_Image.m_LRa_y1_um = m_y_um[1];
			}

			Repaint();
		}

	}
}

//***************************************************************************************

void CImageDlg2::OnPupilRadioClicked()
{
	DisableLimbusRadios();
	DisableVertexRadios();
	DisableRadialRulerRadios();//520
	DisableLinearRulerRadios();//520

	if (m_RADIAL_RULER)
	{
		m_PupilCheck.SetCheck(TRUE);
		m_PupilCheck.EnableWindow(FALSE);

		m_LimbusCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);

		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
	}

	m_PupilRadio.SetCheck(1);
	m_Pupil1Radio.EnableWindow(TRUE);
	m_Pupil2Radio.EnableWindow(TRUE);
	if (!m_Gray) m_Pupil3Radio.EnableWindow(TRUE);
	m_Pupil4Radio.EnableWindow(TRUE);

	if (m_P == 1) OnPupil1RadioClicked();
	else if (m_P == 2) OnPupil2RadioClicked();
	else if (m_P == 3) OnPupil3RadioClicked();
	else if (m_P == 4) OnPupil4RadioClicked();
}

void CImageDlg2::OnPupil1RadioClicked()
{
	if (m_pImage->m_pu_ok) {

		m_Image.m_pu_ok = TRUE;
		m_Image.m_pu_x0_um = m_pImage->m_pu_x0_um;
		m_Image.m_pu_y0_um = m_pImage->m_pu_y0_um;
		for (int a = 0; a < 360; a++) {
			m_Image.m_pu_r_um[a] = m_pImage->m_pu_r_um[a];
		}
		m_Image.m_pu_r_min_um = m_pImage->m_pu_r_min_um;
		m_Image.m_pu_r_max_um = m_pImage->m_pu_r_max_um;
		m_Image.m_pu_r_mean_um = m_pImage->m_pu_r_mean_um;

		m_Pupil1Radio.SetCheck(1);
		m_Pupil2Radio.SetCheck(0);
		m_Pupil3Radio.SetCheck(0);
		m_Pupil4Radio.SetCheck(0);
		m_P = 1;
		Repaint();
	}

	else {
		m_Pupil1Radio.EnableWindow(FALSE);
		OnPupil2RadioClicked();
	}
}

void CImageDlg2::OnPupil2RadioClicked()
{
	m_Image.ClearPupil();

	m_Pupil1Radio.SetCheck(0);
	m_Pupil2Radio.SetCheck(1);
	m_Pupil3Radio.SetCheck(0);
	m_Pupil4Radio.SetCheck(0);
	m_P = 2;
	Repaint();
}

void CImageDlg2::OnPupil3RadioClicked()
{
	CBusyCursor Cursor;

	m_Image.FindPupil();

	if (m_Image.m_ve_ok)
	{
		real puveDisum = Distance(m_Image.m_pu_x0_um, m_Image.m_pu_y0_um, m_Image.m_ve_x_um, m_Image.m_ve_y_um);

		if (puveDisum > 1000)
		{
			m_Image.ClearPupil();
			m_Image.m_pu_ok = FindColorPupil(&m_Image);
		}
	}

	if (m_Image.m_pu_ok) {
		m_Pupil1Radio.SetCheck(0);
		m_Pupil2Radio.SetCheck(0);
		m_Pupil3Radio.SetCheck(1);
		m_Pupil4Radio.SetCheck(0);
		m_P = 3;
		Repaint();
	}
	else {
		::Info("Unable to find pupil.");
		OnPupil2RadioClicked();
	}
}

void CImageDlg2::OnPupil4RadioClicked()
{
	m_Pupil1Radio.SetCheck(0);
	m_Pupil2Radio.SetCheck(0);
	m_Pupil3Radio.SetCheck(0);
	m_Pupil4Radio.SetCheck(1);
	m_P = 4;
	Repaint();
}

//***************************************************************************************

void CImageDlg2::OnLimbusRadioClicked()
{
	DisablePupilRadios();
	DisableVertexRadios();
	DisableRadialRulerRadios();//520
	DisableLinearRulerRadios();//520

	if (m_RADIAL_RULER)
	{
		m_LimbusCheck.SetCheck(TRUE);
		m_LimbusCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
	}

	m_LimbusRadio.SetCheck(1);
	m_Limbus1Radio.EnableWindow(TRUE);
	m_Limbus2Radio.EnableWindow(TRUE);
	m_Limbus3Radio.EnableWindow(TRUE);
	m_Limbus4Radio.EnableWindow(TRUE);

	if (m_L == 1) OnLimbus1RadioClicked();
	else if (m_L == 2) OnLimbus2RadioClicked();
	else if (m_L == 3) OnLimbus3RadioClicked();
	else if (m_L == 4) OnLimbus4RadioClicked();
}

void CImageDlg2::OnLimbus1RadioClicked()
{
	if (m_pImage->m_li_ok) {

		m_Image.m_li_ok = TRUE;
		m_Image.m_li_x0_um = m_pImage->m_li_x0_um;
		m_Image.m_li_y0_um = m_pImage->m_li_y0_um;
		for (int a = 0; a < 360; a++) {
			m_Image.m_li_r_um[a] = m_pImage->m_li_r_um[a];
		}
		m_Image.m_li_r_min_um = m_pImage->m_li_r_min_um;
		m_Image.m_li_r_max_um = m_pImage->m_li_r_max_um;
		m_Image.m_li_r_mean_um = m_pImage->m_li_r_mean_um;

		m_Limbus1Radio.SetCheck(1);
		m_Limbus2Radio.SetCheck(0);
		m_Limbus3Radio.SetCheck(0);
		m_Limbus4Radio.SetCheck(0);
		m_L = 1;
		Repaint();
	}

	else {
		m_Limbus1Radio.EnableWindow(FALSE);
		OnLimbus2RadioClicked();
	}
}

void CImageDlg2::OnLimbus2RadioClicked()
{
	m_Image.ClearLimbus();

	m_Limbus1Radio.SetCheck(0);
	m_Limbus2Radio.SetCheck(1);
	m_Limbus3Radio.SetCheck(0);
	m_Limbus4Radio.SetCheck(0);
	m_L = 2;
	Repaint();
}

void CImageDlg2::OnLimbus3RadioClicked()
{
	CBusyCursor Cursor;

	//m_Image.FindLimbus();

	////[5.1.1], only the WF need improve the comparison
	m_Image.FindLimbus();

	if (m_Image.m_li_ok) {
		m_Limbus1Radio.SetCheck(0);
		m_Limbus2Radio.SetCheck(0);
		m_Limbus3Radio.SetCheck(1);
		m_Limbus4Radio.SetCheck(0);
		m_L = 3;
		Repaint();
	}
	else {
		::Info("Unable to find limbus.");
		OnLimbus2RadioClicked();
	}
}

void CImageDlg2::OnLimbus4RadioClicked()
{
	m_Limbus1Radio.SetCheck(0);
	m_Limbus2Radio.SetCheck(0);
	m_Limbus3Radio.SetCheck(0);
	m_Limbus4Radio.SetCheck(1);
	m_L = 4;
	Repaint();
}

//***************************************************************************************

void CImageDlg2::OnVertexRadioClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableRadialRulerRadios();//520
	DisableLinearRulerRadios();//520

	if (m_RADIAL_RULER)
	{
		m_VertexCheck.SetCheck(TRUE);
		m_VertexCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_LimbusCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
	}

	m_VertexRadio.SetCheck(1);
	m_Vertex1Radio.EnableWindow(TRUE);
	m_Vertex2Radio.EnableWindow(TRUE);
	m_Vertex3Radio.EnableWindow(TRUE);
	m_Vertex4Radio.EnableWindow(TRUE);

	if (m_V == 1) OnVertex1RadioClicked();
	else if (m_V == 2) OnVertex2RadioClicked();
	else if (m_V == 3) OnVertex3RadioClicked();
	else if (m_V == 4) OnVertex4RadioClicked();
}

void CImageDlg2::OnVertex1RadioClicked()
{
	if (m_pImage->m_ve_ok) {
		m_Image.m_ve_ok = TRUE;
		m_Image.m_ve_x_um = m_pImage->m_ve_x_um;
		m_Image.m_ve_y_um = m_pImage->m_ve_y_um;
		m_Vertex1Radio.SetCheck(1);
		m_Vertex2Radio.SetCheck(0);
		m_Vertex3Radio.SetCheck(0);
		m_Vertex4Radio.SetCheck(0);
		m_V = 1;
		Repaint();
	}
	else {
		m_Vertex1Radio.EnableWindow(FALSE);
		OnVertex2RadioClicked();
	}
}

void CImageDlg2::OnVertex2RadioClicked()
{
	m_Image.m_ve_ok = FALSE;
	m_Image.m_ve_x_um = 0.0;
	m_Image.m_ve_y_um = 0.0;
	m_Vertex1Radio.SetCheck(0);
	m_Vertex2Radio.SetCheck(1);
	m_Vertex3Radio.SetCheck(0);
	m_Vertex4Radio.SetCheck(0);
	m_V = 2;
	Repaint();
}

void CImageDlg2::OnVertex3RadioClicked()
{
	CBusyCursor Cursor;

	//long t = clock(); for (int i = 0; i < 10; i++) {
	m_Image.FindWFVertex();
	//} CString s; s.Format(_T("%u", clock() - t); ::Info(s);

	if (m_Image.m_ve_ok) {
		m_Vertex1Radio.SetCheck(0);
		m_Vertex2Radio.SetCheck(0);
		m_Vertex3Radio.SetCheck(1);
		m_Vertex4Radio.SetCheck(0);
		m_V = 3;
		Repaint();
	}

	else {
		::Info("Unable to find vertex.");
		OnVertex2RadioClicked();
	}
}

void CImageDlg2::OnVertex4RadioClicked()
{
	m_V = 4;
	Repaint();
}

//***************************************************************************************

//520
void CImageDlg2::OnRadialRulerClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableVertexRadios();
	DisableLinearRulerRadios();//520

	if (m_RADIAL_RULER)
	{
		m_RRulerCheck.SetCheck(TRUE);
		m_RRulerCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_LimbusCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
	}

	if (m_RADIAL_RULER)
	{
		m_RRulerCheck.SetCheck(TRUE);
		m_RRulerCheck.EnableWindow(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
	}

	m_RadialRulerRadio.SetCheck(1);
	m_RadialRuler1Radio.EnableWindow(TRUE);
	m_RadialRuler2Radio.EnableWindow(TRUE);
	m_RadialRuler3Radio.EnableWindow(TRUE);

	if (m_R_R == 1) OnRadialRuler1Clicked();
	else if (m_R_R == 2) OnRadialRuler2Clicked();
	else if (m_R_R == 3) OnRadialRuler3Clicked();
}

void CImageDlg2::OnRadialRuler1Clicked()
{
	if (m_pImage->m_Ra_ok == TRUE) {

		m_Image.m_Ra_ok = TRUE;
		m_Image.m_Ra_x_um = m_pImage->m_Ra_x_um;
		m_Image.m_Ra_y_um = m_pImage->m_Ra_y_um;

		m_Image.m_Ra_r_um = m_pImage->m_Ra_r_um;

		m_RadialRuler1Radio.SetCheck(1);
		m_RadialRuler2Radio.SetCheck(0);
		m_RadialRuler3Radio.SetCheck(0);

		m_R_R = 1;
		Repaint();
	}

	else {
		m_RadialRuler1Radio.EnableWindow(FALSE);
		OnRadialRuler2Clicked();
	}
}

void CImageDlg2::OnRadialRuler2Clicked()
{
	m_Image.ClearRuler();

	m_RadialRuler1Radio.SetCheck(0);
	m_RadialRuler2Radio.SetCheck(1);
	m_RadialRuler3Radio.SetCheck(0);

	m_R_R = 2;
	Repaint();
}

void CImageDlg2::OnRadialRuler3Clicked()
{
	m_RadialRuler1Radio.SetCheck(0);
	m_RadialRuler2Radio.SetCheck(0);
	m_RadialRuler3Radio.SetCheck(1);

	m_R_R = 3;
	Repaint();
}


//***************************************************************************************

//520
void CImageDlg2::OnLinearRulerClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableVertexRadios();
	DisableRadialRulerRadios();

	if (m_RADIAL_RULER)
	{
		m_LRulerCheck.SetCheck(TRUE);
		m_LRulerCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_LimbusCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
	}

	m_LinearRulerRadio.SetCheck(1);
	m_LinearRuler1Radio.EnableWindow(TRUE);
	m_LinearRuler2Radio.EnableWindow(TRUE);
	m_LinearRuler3Radio.EnableWindow(TRUE);

	if (m_L_R == 1) OnLinearRuler1Clicked();
	else if (m_L_R == 2) OnLinearRuler2Clicked();
	else if (m_L_R == 3) OnLinearRuler3Clicked();
}

void CImageDlg2::OnLinearRuler1Clicked()
{
	if (m_pImage->m_LRa_ok == TRUE)
	{
		m_Image.m_LRa_ok = TRUE;
		m_Image.m_LRa_x0_um = m_pImage->m_LRa_x0_um;
		m_Image.m_LRa_y0_um = m_pImage->m_LRa_y0_um;
		m_Image.m_LRa_x1_um = m_pImage->m_LRa_x1_um;
		m_Image.m_LRa_y1_um = m_pImage->m_LRa_y1_um;


		m_LinearRuler1Radio.SetCheck(1);
		m_LinearRuler2Radio.SetCheck(0);
		m_LinearRuler3Radio.SetCheck(0);

		m_L_R = 1;
		Repaint();
	}

	else
	{
		m_LinearRuler1Radio.EnableWindow(FALSE);
		OnLinearRuler2Clicked();
	}
}

void CImageDlg2::OnLinearRuler2Clicked()
{
	m_Image.ClearLRuler();

	m_LinearRuler1Radio.SetCheck(0);
	m_LinearRuler2Radio.SetCheck(1);
	m_LinearRuler3Radio.SetCheck(0);

	m_L_R = 2;
	Repaint();
}

void CImageDlg2::OnLinearRuler3Clicked()
{
	m_LinearRuler1Radio.SetCheck(0);
	m_LinearRuler2Radio.SetCheck(0);
	m_LinearRuler3Radio.SetCheck(1);

	m_L_R = 3;
	Repaint();
}

//***************************************************************************************

void CImageDlg2::DrawEye(CMDC& MemDC)
{
	memcpy(MemDC.m_RGBData, m_Image.m_RGBData.GetMem(), m_Image.m_RGBData.GetSize());
}

//***************************************************************************************

void CImageDlg2::DrawPupil(CMDC& MemDC)
{
	if (m_PupilRadio.GetCheck() != 1 && !m_PupilCheck.GetCheck())
	{
		if (m_LinearRulerRadio.GetCheck() != 1 && m_RadialRulerRadio.GetCheck() != 1 && m_LimbusRadio.GetCheck() != 1) m_DisText.SetWindowText(_T(""));
		return;
	}

	{
		// центр изображения
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);
	}

	if (m_ShowTechInfo)
	{
		// 10 миллиметровая окружность
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		MemDC.DrawEllipse(x0, y0, intRound(5000.0 * m_x_px_um), intRound(5000.0 * m_y_px_um), 1, ORANGE, NOCOLOR);

		// гистограмма
		if (m_Image.m_hist_max > 0.0) {
			for (int i = 0; i < 255; i++) {
				int h1 = intRound(400.0 * m_Image.m_hist[i] / m_Image.m_hist_max);
				int h2 = intRound(400.0 * m_Image.m_hist[i + 1] / m_Image.m_hist_max);
				MemDC.DrawLine(i, m_h - 10 - h1, i + 1, m_h - 10 - h2, 1, RED);
			}
			MemDC.DrawLine(m_Image.m_pu_thr, m_h - 10, m_Image.m_pu_thr, m_h - 410, 1, YELLOW);
		}

		// узлы
		int X = m_Image.m_table.GetNumCols();
		int Y = m_Image.m_table.GetNumRows();
		int Y2 = Y >> 1;
		int X2 = X >> 1;
		for (int y = 0; y < Y; y++) {
			for (int x = 0; x < X; x++) {
				int xx = intRound(m_cx + m_x_px_um * (x - X2) * m_Image.m_pu_d_um);
				int yy = intRound(m_cy - m_y_px_um * (y - Y2) * m_Image.m_pu_d_um);
				int g = m_Image.m_table(y, x);
				if (g == 3) MemDC.SetPixel(xx, yy, 0, 255, 0); // оболочка
				else if (g == 2) MemDC.SetPixel(xx, yy, 0, 0, 255); // починенная наибольшая группа
				else if (g == 1) MemDC.SetPixel(xx, yy, 255, 0, 255); // ниже порога
			}
		}

	}

	if (m_Image.m_pu_ok)
	{
		//520
		if (m_wheel_n > m_last_wheel_n && m_P == 4)
		{
			real r_um = m_Image.m_pu_r_mean_um + 3;
			for (int a = 0; a < 360; a++)
			{
				m_Image.m_pu_r_um[a] = r_um;
			}
			m_Image.m_pu_r_min_um = r_um;
			m_Image.m_pu_r_max_um = r_um;
			m_Image.m_pu_r_mean_um = r_um;
		}
		else if (m_wheel_n < m_last_wheel_n && m_P == 4)
		{
			real r_um = m_Image.m_pu_r_mean_um - 3;
			for (int a = 0; a < 360; a++)
			{
				m_Image.m_pu_r_um[a] = r_um;
			}
			m_Image.m_pu_r_min_um = r_um;
			m_Image.m_pu_r_max_um = r_um;
			m_Image.m_pu_r_mean_um = r_um;
		}

		if (m_P == 4) m_last_wheel_n = m_wheel_n;
		//520

		//
		int x0 = intRound(m_cx + m_Image.m_pu_x0_um * m_x_px_um);
		int y0 = intRound(m_cy - m_Image.m_pu_y0_um * m_y_px_um);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, GREEN);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, GREEN);
		//
		for (int a = 0; a < 360; a++)
		{
			int b = a == 359 ? 0 : a + 1;
			if (m_Image.m_pu_r_um[a] == INVALID_VALUE) continue;
			if (m_Image.m_pu_r_um[b] == INVALID_VALUE) continue;
			int xa = intRound(m_cx + (m_Image.m_pu_x0_um + m_Image.m_pu_r_um[a] * COS[a]) * m_x_px_um);
			int ya = intRound(m_cy - (m_Image.m_pu_y0_um + m_Image.m_pu_r_um[a] * SIN[a]) * m_y_px_um);
			int xb = intRound(m_cx + (m_Image.m_pu_x0_um + m_Image.m_pu_r_um[b] * COS[b]) * m_x_px_um);
			int yb = intRound(m_cy - (m_Image.m_pu_y0_um + m_Image.m_pu_r_um[b] * SIN[b]) * m_y_px_um);
			MemDC.DrawLine(xa, ya, xb, yb, 1, GREEN);
		}

		if (m_PupilRadio.GetCheck() == 1)
		{
			CString s;
			s.Format(_T("(D) = (%0.3fmm)"), m_Image.m_pu_r_mean_um*0.002);
			s = "Pupil: " + s;
			m_DisText.SetWindowText(s);
		}
	}
	else m_DisText.SetWindowText(_T(""));
}

//***************************************************************************************

void CImageDlg2::DrawLimbus(CMDC& MemDC)
{
	if (m_LimbusRadio.GetCheck() != 1 && !m_LimbusCheck.GetCheck())
	{
		if (m_LinearRulerRadio.GetCheck() != 1 && m_RadialRulerRadio.GetCheck() != 1 && m_PupilRadio.GetCheck() != 1) m_DisText.SetWindowText(_T(""));
		return;
	}

	{
		// центр изображения
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);
	}

	if (m_ShowTechInfo) {
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		// 9 миллиметровая окружность
		MemDC.DrawEllipse(x0, y0, intRound(4500.0 * m_x_px_um), intRound(4500.0 * m_y_px_um), 1, ORANGE, NOCOLOR);
		// 14 миллиметровая окружность
		MemDC.DrawEllipse(x0, y0, intRound(7000.0 * m_x_px_um), intRound(7000.0 * m_y_px_um), 1, ORANGE, NOCOLOR);
	}

	if (m_Image.m_li_ok)
	{
		//520
		if (m_wheel_n > m_last_wheel_n && m_L == 4)
		{
			real r_um = m_Image.m_li_r_mean_um + 3;
			for (int a = 0; a < 360; a++)
			{
				m_Image.m_li_r_um[a] = r_um;
			}
			m_Image.m_li_r_min_um = r_um;
			m_Image.m_li_r_max_um = r_um;
			m_Image.m_li_r_mean_um = r_um;
		}
		else if (m_wheel_n < m_last_wheel_n && m_L == 4)
		{
			real r_um = m_Image.m_li_r_mean_um - 3;
			for (int a = 0; a < 360; a++)
			{
				m_Image.m_li_r_um[a] = r_um;
			}
			m_Image.m_li_r_min_um = r_um;
			m_Image.m_li_r_max_um = r_um;
			m_Image.m_li_r_mean_um = r_um;
		}

		if (m_L == 4) m_last_wheel_n = m_wheel_n;
		//520

		// центр лимбуса
		int x0 = intRound(m_cx + m_Image.m_li_x0_um * m_x_px_um);
		int y0 = intRound(m_cy - m_Image.m_li_y0_um * m_y_px_um);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, BLUE);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, BLUE);
		// граница лимбуса
		for (int a = 0; a < 360; a++) {
			int b = a == 359 ? 0 : a + 1;
			if (m_Image.m_li_r_um[a] == INVALID_VALUE) continue;
			if (m_Image.m_li_r_um[b] == INVALID_VALUE) continue;
			int xa = intRound(m_cx + (m_Image.m_li_x0_um + m_Image.m_li_r_um[a] * COS[a]) * m_x_px_um);
			int ya = intRound(m_cy - (m_Image.m_li_y0_um + m_Image.m_li_r_um[a] * SIN[a]) * m_y_px_um);
			int xb = intRound(m_cx + (m_Image.m_li_x0_um + m_Image.m_li_r_um[b] * COS[b]) * m_x_px_um);
			int yb = intRound(m_cy - (m_Image.m_li_y0_um + m_Image.m_li_r_um[b] * SIN[b]) * m_y_px_um);
			MemDC.DrawLine(xa, ya, xb, yb, 1, BLUE);
		}

		if (m_LimbusRadio.GetCheck() == 1)
		{
			CString s;
			s.Format(_T("(D) = (%0.3fmm)"), m_Image.m_li_r_mean_um*0.002);
			s = "Limbus: " + s;
			m_DisText.SetWindowText(s);
		}
	}
	else m_DisText.SetWindowText(_T(""));
}

//***************************************************************************************

void CImageDlg2::DrawVertex(CMDC& MemDC)
{
	if (m_VertexRadio.GetCheck() != 1 && !m_VertexCheck.GetCheck()) return;

	{
		// центр изображения
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);
	}

	if (m_Image.m_ve_ok) {

		int x0 = intRound(m_cx + m_Image.m_ve_x_um * m_x_px_um);
		int y0 = intRound(m_cy - m_Image.m_ve_y_um * m_y_px_um);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, RED);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, RED);
	}

}
//***************************************************************************************
//520
void CImageDlg2::DrawRuler(CMDC& MemDC)
{
	if (m_RadialRulerRadio.GetCheck() != 1 && !m_RRulerCheck.GetCheck())
	{
		if (m_LinearRulerRadio.GetCheck() != 1 && m_PupilRadio.GetCheck() != 1 && m_LimbusRadio.GetCheck() != 1) m_DisText.SetWindowText(_T(""));
		return;
	}

	{
		//The center cross
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);
	}

	if (m_Image.m_Ra_ok == TRUE)
	{
		//520
		if (m_wheel_n > m_last_wheel_n  && m_R_R == 3)
		{
			m_Image.m_Ra_r_um = m_Image.m_Ra_r_um + 3;
		}
		else if (m_wheel_n < m_last_wheel_n  && m_R_R == 3)
		{
			m_Image.m_Ra_r_um = m_Image.m_Ra_r_um - 3;
		}

		if (m_R_R == 3) m_last_wheel_n = m_wheel_n;
		//520


		// центр зрачка
		int x0 = intRound(m_cx + m_Image.m_Ra_x_um * m_x_px_um);
		int y0 = intRound(m_cy - m_Image.m_Ra_y_um * m_y_px_um);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, YELLOW);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, YELLOW);
		// граница зрачка
		for (int a = 0; a < 360; a++) {
			int b = a == 359 ? 0 : a + 1;
			int xa = intRound(m_cx + (m_Image.m_Ra_x_um + m_Image.m_Ra_r_um * COS[a]) * m_x_px_um);
			int ya = intRound(m_cy - (m_Image.m_Ra_y_um + m_Image.m_Ra_r_um * SIN[a]) * m_y_px_um);
			int xb = intRound(m_cx + (m_Image.m_Ra_x_um + m_Image.m_Ra_r_um * COS[b]) * m_x_px_um);
			int yb = intRound(m_cy - (m_Image.m_Ra_y_um + m_Image.m_Ra_r_um * SIN[b]) * m_y_px_um);
			MemDC.DrawLine(xa, ya, xb, yb, 1, YELLOW);
		}

		//Draw the radius of the ruler
		CString s;
		s.Format(_T("(D) = (%0.3fmm)"), m_Image.m_Ra_r_um*0.002);

		if (m_Image.m_Ra_r_um >= 800)
		{
			int FontSize = intRound(0.035 * m_h);

			real r_um = m_Image.m_Ra_r_um + 350.0;
			int x = intRound(m_cx + (m_Image.m_Ra_x_um + r_um * COS[135]) * m_x_px_um);
			int y = intRound(m_cy - (m_Image.m_Ra_y_um + r_um * SIN[135]) * m_y_px_um);
			MemDC.WriteRotatedText(s, x, y, 45, FontSize, 400, "Arial", YELLOW, NOCOLOR);
		}
		if (m_RadialRulerRadio.GetCheck() == 1)
		{
			s = "Radial Ruler: " + s;
			m_DisText.SetWindowText(s);
		}
	}
	else m_DisText.SetWindowText(_T(""));
}

//520

//***************************************************************************************
//520
void CImageDlg2::DrawLRuler(CMDC& MemDC)
{
	if (m_LinearRulerRadio.GetCheck() != 1 && !m_LRulerCheck.GetCheck())
	{
		if (m_RadialRulerRadio.GetCheck() != 1 && m_PupilRadio.GetCheck() != 1 && m_LimbusRadio.GetCheck() != 1) m_DisText.SetWindowText(_T(""));
		return;
	}

	{
		//The center cross
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);
	}

	if (m_Image.m_LRa_ok == TRUE)
	{
		int x0 = intRound(m_cx + m_Image.m_LRa_x0_um * m_x_px_um);
		int y0 = intRound(m_cy - m_Image.m_LRa_y0_um * m_y_px_um);
		int x1 = intRound(m_cx + m_Image.m_LRa_x1_um * m_x_px_um);
		int y1 = intRound(m_cy - m_Image.m_LRa_y1_um * m_y_px_um);

		COLORREF LRulerColor = RGB(255, 128, 0);
		MemDC.DrawCircle(x0, y0, 2, 0, LRulerColor, LRulerColor);
		MemDC.DrawCircle(x1, y1, 2, 0, LRulerColor, LRulerColor);
		MemDC.DrawLine(x0, y0, x1, y1, 2, LRulerColor);

		//
		real dis = hyp(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um, m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um);

		CString s;
		s.Format(_T("(D) = (%.3fmm)"), 0.001*dis);

		if (dis > 800)
		{
			int xm, ym;
			if (x0 < x1)
			{
				xm = x0 + intRound((real)abs(x0 - x1) / 2.0);
				if (y0 < y1)	ym = y0 + intRound((real)abs(y0 - y1) / 2.0);
				else ym = y1 + intRound((real)abs(y0 - y1) / 2.0);
			}
			else
			{
				xm = x1 + intRound((real)abs(x0 - x1) / 2.0);
				if (y0 < y1)	ym = y0 + intRound((real)abs(y0 - y1) / 2.0);
				else ym = y1 + intRound((real)abs(y0 - y1) / 2.0);
			}

			MemDC.DrawLine(xm - 5, ym, xm + 6, ym, 1, LRulerColor);
			MemDC.DrawLine(xm, ym - 5, xm, ym + 6, 1, LRulerColor);

			int angle = intRound(atan2(double(x1 - x0), double(y1 - y0)) * 180 / 3.1415926) - 90;//

			int angle2 = angle;

			if (angle2 < 0) angle2 = 360 + angle2;

			if (x1 < x0) angle += 180;

			int xp = intRound(xm + 20 * SIN[angle2]);
			int yp = intRound(ym + 20 * COS[angle2]);

			int FontSize = intRound(0.035 * m_h);
			MemDC.WriteRotatedText(s, xp, yp, angle, FontSize, 400, "Arial", LRulerColor, NOCOLOR);
		}
		if (m_LinearRulerRadio.GetCheck() == 1)
		{
			s = "Linear Ruler: " + s;
			m_DisText.SetWindowText(s);
		}
	}
	else m_DisText.SetWindowText(_T(""));
}

//520
//***************************************************************************************

void CImageDlg2::DisablePupilRadios()
{
	m_PupilRadio.SetCheck(0);
	m_Pupil1Radio.SetCheck(0);
	m_Pupil2Radio.SetCheck(0);
	m_Pupil3Radio.SetCheck(0);
	m_Pupil4Radio.SetCheck(0);
	m_Pupil1Radio.EnableWindow(FALSE);
	m_Pupil2Radio.EnableWindow(FALSE);
	m_Pupil3Radio.EnableWindow(FALSE);
	m_Pupil4Radio.EnableWindow(FALSE);
}

void CImageDlg2::DisableLimbusRadios()
{
	m_LimbusRadio.SetCheck(0);
	m_Limbus1Radio.SetCheck(0);
	m_Limbus2Radio.SetCheck(0);
	m_Limbus3Radio.SetCheck(0);
	m_Limbus4Radio.SetCheck(0);
	m_Limbus1Radio.EnableWindow(FALSE);
	m_Limbus2Radio.EnableWindow(FALSE);
	m_Limbus3Radio.EnableWindow(FALSE);
	m_Limbus4Radio.EnableWindow(FALSE);
}

void CImageDlg2::DisableVertexRadios()
{
	m_VertexRadio.SetCheck(0);
	m_Vertex1Radio.SetCheck(0);
	m_Vertex2Radio.SetCheck(0);
	m_Vertex3Radio.SetCheck(0);
	m_Vertex4Radio.SetCheck(0);
	m_Vertex1Radio.EnableWindow(FALSE);
	m_Vertex2Radio.EnableWindow(FALSE);
	m_Vertex3Radio.EnableWindow(FALSE);
	m_Vertex4Radio.EnableWindow(FALSE);
}

//520
void CImageDlg2::DisableRadialRulerRadios()
{
	m_RadialRulerRadio.SetCheck(0);
	m_RadialRuler1Radio.SetCheck(0);
	m_RadialRuler2Radio.SetCheck(0);
	m_RadialRuler3Radio.SetCheck(0);

	m_RadialRuler1Radio.EnableWindow(FALSE);
	m_RadialRuler2Radio.EnableWindow(FALSE);
	m_RadialRuler3Radio.EnableWindow(FALSE);
}
//520

//520
void CImageDlg2::DisableLinearRulerRadios()
{
	m_LinearRulerRadio.SetCheck(0);
	m_LinearRuler1Radio.SetCheck(0);
	m_LinearRuler2Radio.SetCheck(0);
	m_LinearRuler3Radio.SetCheck(0);

	m_LinearRuler1Radio.EnableWindow(FALSE);
	m_LinearRuler2Radio.EnableWindow(FALSE);
	m_LinearRuler3Radio.EnableWindow(FALSE);
}
//520


//***************************************************************************************
//520
BOOL CImageDlg2::OnMouseWheel(uint nFlags, short zDelta, CPoint Point)
{
	if ((m_PupilRadio.GetCheck() && m_P == 4) || (m_LimbusRadio.GetCheck() && m_L == 4) || (m_RadialRulerRadio.GetCheck() && m_R_R == 3))
	{
		m_wheel_n += zDelta / WHEEL_DELTA;
		if (m_wheel_n < -500) {
			m_wheel_n = -500;
			return FALSE;
		}
		else if (m_wheel_n > 500) {
			m_wheel_n = 500;
			return FALSE;
		}

		Repaint();

		return CWnd::OnMouseWheel(nFlags, zDelta, Point);
	}
	else return FALSE;
}
//520
//***************************************************************************************
BOOL CImageDlg2::FindColorPupil(CEyeImage* OriImage)
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

double CImageDlg2::Gaussian(int x, int y, double de)
{
	double value = 0;
	double pai = 3.1415926;
	double e = 2.718281828;

	value = (-1)*((x*x + y*y) / (2 * de*de));
	value = (1 / (2 * pai*de*de))*pow(e, value);

	return value;
}

//***************************************************************************************

void CImageDlg2::SafeInteRelease(int **ppT, int Row)
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

//***************************************************************************************

HSI CImageDlg2::ChangeRGBtoHSI(int R, int G, int B)
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


