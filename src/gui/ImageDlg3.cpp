//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "Data.h"
#include "BusyCursor.h"

#include "ImageDlg3.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CImageDlg3, CDialog)

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

	ON_BN_CLICKED(IDC_INLAY_RADIO, OnInlayClicked)
	ON_BN_CLICKED(IDC_INLAY_RADIO1, OnInlay1Clicked)//Original
	ON_BN_CLICKED(IDC_INLAY_RADIO2, OnInlay2Clicked)//No Inlay
	ON_BN_CLICKED(IDC_INLAY_RADIO3, OnInlay3Clicked)//AUTO
	ON_BN_CLICKED(IDC_INLAY_RADIO4, OnInlay4Clicked)//Manual
													//Done

	ON_BN_CLICKED(IDC_SETINLAY_BTN, OnSetInalyDBtnClicked)//520 set inaly diameter


	ON_BN_CLICKED(IDC_PUPIL_CHECK, Repaint)
	ON_BN_CLICKED(IDC_LIMBUS_CHECK, Repaint)
	ON_BN_CLICKED(IDC_VERTEX_CHECK, Repaint)
	ON_BN_CLICKED(IDC_RRULER_CHECK, Repaint)
	ON_BN_CLICKED(IDC_LRULER_CHECK, Repaint)
	ON_BN_CLICKED(IDC_INLAY_CHECK, Repaint)

END_MESSAGE_MAP()

//***************************************************************************************
int CImageDlg3::m_wheel_n = 0;//520
							  //***************************************************************************************

CImageDlg3::CImageDlg3(CWnd* pParentWnd, CEyeImage* pImage, CEyeImage* pCTImage, BOOL WF, BOOL RADIAL_RULER) : CDialog(IDD_IMAGE_DLG3, pParentWnd)
{
	m_WF = WF;
	m_RADIAL_RULER = RADIAL_RULER;
	m_pImage = pImage;
	m_pCtImage = pCTImage;//
	m_last_wheel_n = 0;

	//m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));

	ASSERT(m_pImage);
}

//***************************************************************************************

CImageDlg3::~CImageDlg3()
{
	//DestroyAcceleratorTable(m_hAccel);
}

//***************************************************************************************

void CImageDlg3::DoDataExchange(CDataExchange* pDX)
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

	//520 Edit  disk Radios
	DDX_Control(pDX, IDC_INLAY_RADIO, m_InlayRadio);
	DDX_Control(pDX, IDC_INLAY_RADIO1, m_Inlay1Radio);
	DDX_Control(pDX, IDC_INLAY_RADIO2, m_Inlay2Radio);
	DDX_Control(pDX, IDC_INLAY_RADIO3, m_Inlay3Radio);
	DDX_Control(pDX, IDC_INLAY_RADIO4, m_Inlay4Radio);
	//Done

	//520
	DDX_Control(pDX, IDC_DIS_TEXT, m_DisText);

	DDX_Control(pDX, IDC_PUPIL_CHECK, m_PupilCheck);
	DDX_Control(pDX, IDC_LIMBUS_CHECK, m_LimbusCheck);
	DDX_Control(pDX, IDC_VERTEX_CHECK, m_VertexCheck);
	DDX_Control(pDX, IDC_RRULER_CHECK, m_RRulerCheck);
	DDX_Control(pDX, IDC_LRULER_CHECK, m_LRulerCheck);
	DDX_Control(pDX, IDC_INLAY_CHECK, m_InlayCheck);
	//Done

	// disk input 
	DDX_Control(pDX, IDC_DIS_TEXT2, m_DisText2);
	DDX_Control(pDX, IDC_DIS_TEXT3, m_DisText3);
	DDX_Control(pDX, IDC_EDIT_INALYD, m_InlayEdit);
	DDX_Control(pDX, IDC_SETINLAY_BTN, m_SetInlayBtn);
	DDX_Control(pDX, IDC_STATIC1, m_Split);

	DDX_Control(pDX, IDC_DIS_TEXT4, m_DisText4);
	DDX_Control(pDX, IDC_DIS_TEXT5, m_DisText5);
	DDX_Control(pDX, IDC_EDIT_INALYD2, m_InlayEdit2);

	DDX_Control(pDX, IDC_DIS_TEXT6, m_DisText6);
	//Done

	DDX_Control(pDX, IDC_STATIC_NOTE, m_LimbusNote);// 6.3.0 Fix the wrong placement and content of limbus note
	
}

//***************************************************************************************

BOOL CImageDlg3::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ShowTechInfo = FALSE;

	m_P = 1;
	m_L = 1;
	m_V = 1;
	m_R_R = 1;//520
	m_L_R = 1;//520
	m_I = 1;//520

			//Some attributs for the Inlay
	m_Last_x = 0;
	m_Last_y = 0;
	m_Last_I_R = -1;
	m_Last_I_I_R = -1;
	//Done

	m_InalyDrawed = FALSE;// The  disk is not drawed yet

	m_Image = *m_pImage;
	m_Ct_Image = *m_pCtImage;//

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
		m_InlayRadio.EnableWindow(FALSE);
	}
	//Done

	OnPupilRadioClicked();

	BOOL IsDBReadOnly = ::DB.IsReadOnly();
	m_OKButton.EnableWindow(!IsDBReadOnly);

	//6.2.1
	m_Limbus3Radio.ShowWindow(SW_HIDE);
	m_Limbus4Radio.ShowWindow(SW_HIDE);
	//6.2.1

	return FALSE;
}

//***************************************************************************************

void CImageDlg3::Resize()
{
	m_w = m_Image.m_w;
	m_h = m_Image.m_h;
	m_w_um = m_Image.m_w_um;
	m_h_um = m_Image.m_h_um;
	m_x_px_um = m_w / m_w_um;
	m_y_px_um = m_h / m_h_um;
	m_cx = 0.5 * m_w;
	m_cy = 0.5 * m_h - 1;

	//   //520
	// 	//a filled circle with a center cut-out, 50% transparent white, no outline
	////an annulus design 3.8 mm outer diameter and 1.6 mm inner diamet
	////int InDia_um  = intRound(1.6/0.002);
	//int OutDia_um = intRound(3.8/0.002);	
	//m_OutSideWidth = intRound(OutDia_um * m_x_px_um);
	//m_InsideWidth  = intRound(OutDia_um * m_x_px_um*0.7);
	////520

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
	wh = th + d + ih + d + bh + 17 * d;

	m_MemDC.CreateTrueColorDC(m_w, m_h);

	m_Rgn.CreateRectRgn(d, d, d + m_w, d + m_h);

	SetWindowPos(NULL, 0, 0, ww, wh, SWP_NOZORDER);
	CenterWindow();

	int t = intRound(0.005 * W);
	int dt1 = intRound(0.011 * W);
	int dt2 = intRound(0.015 * W);
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
	m_LimbusNote.SetWindowPos(NULL, iw + d + d + d, t - dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER); // 6.3.0 Fix the wrong placement and content of limbus note

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

	//520 Edit  disk Radios
	m_InlayRadio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Inlay1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Inlay2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Inlay3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Inlay4Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//Done

	if (m_RADIAL_RULER)
	{
		::SetRect(&m_InlayText1Rec, d, 2 * d + m_h + 5, 200, 5 * d + m_h + 5);
		::SetRect(&m_OtherText1Rec, d, 2 * d + m_h + 5, 500, 5 * d + m_h + 5);

		m_DisText6.SetWindowPos(NULL, d, intRound(6.5*d) + m_h + 5, 0, 0, SWP_NOSIZE);  //520;

		m_DisText.SetWindowPos(NULL, d, 2 * d + m_h + 5, 0, 0, SWP_NOSIZE); //Inlay inner Diameter	  

		m_DisText2.SetWindowPos(NULL, d + 210, 2 * d + m_h + 5, 0, 0, SWP_NOSIZE);  //520;	  
		m_InlayEdit.SetWindowPos(NULL, d + 325, 2 * d + m_h, 0, 0, SWP_NOSIZE);  //520
		m_DisText3.SetWindowPos(NULL, d + 385, 2 * d + m_h + 5, 0, 0, SWP_NOSIZE);  //520

		m_DisText4.SetWindowPos(NULL, d + 210, intRound(6.5*d) + m_h + 5, 0, 0, SWP_NOSIZE);  //520;	  
		m_InlayEdit2.SetWindowPos(NULL, d + 325, intRound(6.5*d) + m_h, 0, 0, SWP_NOSIZE);  //520
		m_DisText5.SetWindowPos(NULL, d + 385, intRound(6.5*d) + m_h + 5, 0, 0, SWP_NOSIZE);  //520

		m_SetInlayBtn.SetWindowPos(NULL, d + 550, intRound(6.5*d) + m_h + 2, 0, 0, SWP_NOSIZE);  //520
		m_Split.SetWindowPos(NULL, d, intRound(6.5*d) + m_h + 15, 468, 0, SWP_NOSIZE);

		m_DisText.SetWindowText(_T(""));//520;

		m_PupilCheck.SetWindowPos(NULL, d, intRound(10.5*d) + m_h + 15, 0, 0, SWP_NOSIZE);
		m_PupilCheck.SetCheck(TRUE);
		m_PupilCheck.EnableWindow(FALSE);

		m_LimbusCheck.SetWindowPos(NULL, d + dt3, intRound(10.5*d) + m_h + 15, 0, 0, SWP_NOSIZE);
		m_VertexCheck.SetWindowPos(NULL, d + 2 * dt3, intRound(10.5*d) + m_h + 15, 0, 0, SWP_NOSIZE);
		m_RRulerCheck.SetWindowPos(NULL, d + 3 * dt3, intRound(10.5*d) + m_h + 15, 0, 0, SWP_NOSIZE);

		m_LRulerCheck.SetWindowPos(NULL, d, intRound(13.5*d) + m_h + 15, 0, 0, SWP_NOSIZE);
		m_InlayCheck.SetWindowPos(NULL, d + 2 * dt3, intRound(13.5*d) + m_h + 15, 0, 0, SWP_NOSIZE);
	}
	else
	{
		m_PupilCheck.ShowWindow(SW_HIDE);
		m_LimbusCheck.ShowWindow(SW_HIDE);
		m_VertexCheck.ShowWindow(SW_HIDE);
		m_RRulerCheck.ShowWindow(SW_HIDE);
		m_LRulerCheck.ShowWindow(SW_HIDE);
		m_InlayCheck.ShowWindow(SW_HIDE);
	}

	GetClientRect(&Rect);
	cw = Rect.right - Rect.left;
	ch = Rect.bottom - Rect.top;

	int bl = intRound(0.5 * cw - 0.5 * d) - bw;
	int bt = intRound(ch - d - bh);
	m_CancelButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);
	bl = intRound(0.5 * cw + 0.5 * d);
	m_OKButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);

	HideInlayInfo();
}

//***************************************************************************************

void CImageDlg3::OnCtrlD()
{
	m_ShowTechInfo = !m_ShowTechInfo;
	Repaint();
}

//***************************************************************************************

void CImageDlg3::Repaint()
{
	DrawEye(m_MemDC);
	DrawPupil(m_MemDC);
	DrawLimbus(m_MemDC);
	DrawVertex(m_MemDC);
	DrawRuler(m_MemDC);//520
	DrawLRuler(m_MemDC);//520
	DrawInlay(m_MemDC);//520

	InvalidateRgn(&m_Rgn, FALSE);
}

//***************************************************************************************

void CImageDlg3::OnOK()
{
	*m_pImage = m_Image;
	
	CDialog::OnOK();
}

//***************************************************************************************

void CImageDlg3::OnPaint()
{
	CPaintDC PaintDC(this);
	PaintDC.BitBlt(m_l, m_t, m_w, m_h, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CImageDlg3::OnLButtonUp(uint nFlags, CPoint Point)
{
	m_wheel_n = m_last_wheel_n = 0;//520

	if (Point.x < m_l || Point.x >= m_l + m_w || Point.y < m_t || Point.y >= m_t + m_h) return;

	m_Last_x = 0;
	m_Last_y = 0;
}

//***************************************************************************************
void CImageDlg3::OnLButtonDown(uint nFlags, CPoint Point)
{
	if (Point.x < m_l || Point.x >= m_l + m_w || Point.y < m_t || Point.y >= m_t + m_h) return;

	real_t x = (Point.x - m_l) - m_cx;
	real_t y = m_cy - (Point.y - m_t);

	m_x_um[0] = x / m_x_px_um;
	m_y_um[0] = y / m_y_px_um;
	m_x_um[1] = m_x_um[0];
	m_y_um[1] = m_y_um[0];

	if (m_PupilRadio.GetCheck() && m_Pupil4Radio.GetCheck())
	{
		if (m_Image.m_pu_r_mean_um >= 800)
		{
			real_t disX = m_x_um[1] - m_Image.m_pu_x0_um;
			real_t disY = m_y_um[1] - m_Image.m_pu_y0_um;

			real_t dis = sqrt(disX*disX + disY*disY);

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
			real_t disX = m_x_um[1] - m_Image.m_li_x0_um;
			real_t disY = m_y_um[1] - m_Image.m_li_y0_um;

			real_t dis = sqrt(disX*disX + disY*disY);

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
			real_t disX = m_x_um[1] - m_Image.m_Ra_x_um;
			real_t disY = m_y_um[1] - m_Image.m_Ra_y_um;

			real_t dis = sqrt(disX*disX + disY*disY);

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
		real_t disX1 = m_x_um[1] - m_Image.m_LRa_x0_um;
		real_t disY1 = m_y_um[1] - m_Image.m_LRa_y0_um;

		real_t disX2 = m_x_um[1] - m_Image.m_LRa_x1_um;
		real_t disY2 = m_y_um[1] - m_Image.m_LRa_y1_um;

		real_t dis1 = sqrt(disX1*disX1 + disY1*disY1);
		real_t dis2 = sqrt(disX2*disX2 + disY2*disY2);


		//
		int x0 = intRound(m_cx + m_Image.m_LRa_x0_um * m_x_px_um);
		int y0 = intRound(m_cy - m_Image.m_LRa_y0_um * m_y_px_um);
		int x1 = intRound(m_cx + m_Image.m_LRa_x1_um * m_x_px_um);
		int y1 = intRound(m_cy - m_Image.m_LRa_y1_um * m_y_px_um);
		real_t xum, yum;
		if (x0 < x1)
		{
			xum = m_Image.m_LRa_x0_um + intRound((real_t)fabs(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um) / 2.0);
			if (m_Image.m_LRa_y0_um < m_Image.m_LRa_y1_um)	yum = m_Image.m_LRa_y0_um + intRound((real_t)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
			else yum = m_Image.m_LRa_y1_um + intRound((real_t)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
		}
		else
		{
			xum = m_Image.m_LRa_x1_um + intRound((real_t)fabs(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um) / 2.0);
			if (m_Image.m_LRa_y0_um < m_Image.m_LRa_y1_um)	yum = m_Image.m_LRa_y0_um + intRound((real_t)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
			else yum = m_Image.m_LRa_y1_um + intRound((real_t)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
		}

		real_t disX3 = m_x_um[1] - xum;
		real_t disY3 = m_y_um[1] - yum;

		real_t dis3 = sqrt(disX3*disY3 + disY3*disY3);
		//real dis3 = sqrt(disX*disX + disY*disY);

		if (dis1 <= 800)
		{
			m_Image.m_LRa_x0_um += disX1;
			m_Image.m_LRa_y0_um += disY1;
			real_t midx = m_Image.m_LRa_x0_um;
			real_t midy = m_Image.m_LRa_y0_um;

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

	else if (m_InlayRadio.GetCheck() && m_Inlay4Radio.GetCheck())
	{
		m_Last_x = m_x_um[0];
		m_Last_y = m_y_um[0];
	}
}

//***************************************************************************************

void CImageDlg3::OnMouseMove(uint nFlags, CPoint Point)
{
	if (Point.x < m_l || Point.x >= m_l + m_w || Point.y < m_t || Point.y >= m_t + m_h) return;

	real_t x = (Point.x - m_l) - m_cx;
	real_t y = m_cy - (Point.y - m_t);

	m_x_um[1] = x / m_x_px_um;
	m_y_um[1] = y / m_y_px_um;

	real_t dis = 1000;
	real_t disX, disY;
	real_t MoveDis = 0;

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
		real_t D = hyp(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um, m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um);
		if (D >= 1600)
		{
			int x0 = intRound(m_cx + m_Image.m_LRa_x0_um * m_x_px_um);
			int y0 = intRound(m_cy - m_Image.m_LRa_y0_um * m_y_px_um);
			int x1 = intRound(m_cx + m_Image.m_LRa_x1_um * m_x_px_um);
			int y1 = intRound(m_cy - m_Image.m_LRa_y1_um * m_y_px_um);
			real_t xum, yum;
			if (x0 < x1)
			{
				xum = m_Image.m_LRa_x0_um + intRound((real_t)fabs(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um) / 2.0);
				if (m_Image.m_LRa_y0_um < m_Image.m_LRa_y1_um)	yum = m_Image.m_LRa_y0_um + intRound((real_t)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
				else yum = m_Image.m_LRa_y1_um + intRound((real_t)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
			}
			else
			{
				xum = m_Image.m_LRa_x1_um + intRound((real_t)fabs(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um) / 2.0);
				if (m_Image.m_LRa_y0_um < m_Image.m_LRa_y1_um)	yum = m_Image.m_LRa_y0_um + intRound((real_t)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
				else yum = m_Image.m_LRa_y1_um + intRound((real_t)fabs(m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um) / 2.0);
			}

			disX = m_x_um[1] - xum;
			disY = m_y_um[1] - yum;

			dis = sqrt(disX*disX + disY*disY);
		}
	}

	else if (m_InlayRadio.GetCheck() && m_Inlay4Radio.GetCheck())
	{
		dis = 0;

		disX = m_x_um[1] - m_Last_x;
		disY = m_y_um[1] - m_Last_y;
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
				real_t r_um = 0.5 * hyp(m_x_um[0] - m_x_um[1], m_y_um[0] - m_y_um[1]);
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
				real_t r_um = 0.5 * hyp(m_x_um[0] - m_x_um[1], m_y_um[0] - m_y_um[1]);
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

		else if (m_InlayRadio.GetCheck() && m_Inlay4Radio.GetCheck())
		{
			if (m_Last_x != 0)
			{
				m_Last_x = m_x_um[1];
				m_Last_y = m_y_um[1];
				m_Image.m_In_x_um += disX;
				m_Image.m_In_y_um += disY;

				Repaint();
			}
		}
	}
}

//***************************************************************************************

void CImageDlg3::OnPupilRadioClicked()
{
	DisableLimbusRadios();
	DisableVertexRadios();
	DisableRadialRulerRadios();//520
	DisableLinearRulerRadios();//520
	DisableInlayRadios();//520

	m_DisText.MoveWindow(&m_OtherText1Rec, 1);

	if (m_RADIAL_RULER)
	{
		m_Last_I_R = -1;
		m_Last_I_I_R = -1;

		HideInlayInfo();

		m_PupilCheck.SetCheck(TRUE);
		m_PupilCheck.EnableWindow(FALSE);

		m_LimbusCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);
		m_InlayCheck.SetCheck(FALSE);

		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
		m_InlayCheck.EnableWindow(TRUE);
	}

	m_PupilRadio.SetCheck(1);
	m_Pupil1Radio.EnableWindow(TRUE);
	m_Pupil2Radio.EnableWindow(TRUE);
	if (m_WF) m_Pupil3Radio.EnableWindow(TRUE);
	m_Pupil4Radio.EnableWindow(TRUE);

	if (m_P == 1) OnPupil1RadioClicked();
	else if (m_P == 2) OnPupil2RadioClicked();
	else if (m_P == 3) OnPupil3RadioClicked();
	else if (m_P == 4) OnPupil4RadioClicked();
}

void CImageDlg3::OnPupil1RadioClicked()
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

void CImageDlg3::OnPupil2RadioClicked()
{
	m_Image.ClearPupil();

	m_Pupil1Radio.SetCheck(0);
	m_Pupil2Radio.SetCheck(1);
	m_Pupil3Radio.SetCheck(0);
	m_Pupil4Radio.SetCheck(0);
	m_P = 2;
	Repaint();
}

void CImageDlg3::OnPupil3RadioClicked()
{
	CBusyCursor Cursor;

	m_Image.FindPupil();

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

void CImageDlg3::OnPupil4RadioClicked()
{
	m_Pupil1Radio.SetCheck(0);
	m_Pupil2Radio.SetCheck(0);
	m_Pupil3Radio.SetCheck(0);
	m_Pupil4Radio.SetCheck(1);
	m_P = 4;
	Repaint();
}

//***************************************************************************************

void CImageDlg3::OnLimbusRadioClicked()
{
	DisablePupilRadios();
	DisableVertexRadios();
	DisableRadialRulerRadios();//520
	DisableLinearRulerRadios();//520
	DisableInlayRadios();//520

	m_DisText.MoveWindow(&m_OtherText1Rec, 1);
	if (m_RADIAL_RULER)
	{
		m_Last_I_R = -1;
		m_Last_I_I_R = -1;

		HideInlayInfo();

		m_LimbusCheck.SetCheck(TRUE);
		m_LimbusCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);
		m_InlayCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
		m_InlayCheck.EnableWindow(TRUE);
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

void CImageDlg3::OnLimbus1RadioClicked()
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

void CImageDlg3::OnLimbus2RadioClicked()
{
	m_Image.ClearLimbus();

	m_Limbus1Radio.SetCheck(0);
	m_Limbus2Radio.SetCheck(1);
	m_Limbus3Radio.SetCheck(0);
	m_Limbus4Radio.SetCheck(0);
	m_L = 2;
	Repaint();
}

void CImageDlg3::OnLimbus3RadioClicked()
{
	CBusyCursor Cursor;

	//m_Image.FindLimbus();

	////[5.1.1], only the WF need improve the comparison
	if (!m_WF)
	{
		m_Image.FindLimbus();
	}
	else
	{
		//m_Image.FindLimbus();

		double xdis = m_Ct_Image.m_li_x0_um - m_Ct_Image.m_ve_x_um;
		double ydis = m_Ct_Image.m_li_y0_um - m_Ct_Image.m_ve_y_um;

		double Adjust_x0_um = m_Ct_Image.m_ve_x_um + xdis;
		double Adjust_y0_um = m_Ct_Image.m_ve_x_um + ydis;

		if (m_Image.m_li_r_mean_um != m_Ct_Image.m_li_r_mean_um
			|| m_Image.m_li_r_min_um != m_Ct_Image.m_li_r_min_um
			|| m_Image.m_li_r_max_um != m_Ct_Image.m_li_r_max_um)
		{
			m_Image.m_li_r_mean_um = m_Ct_Image.m_li_r_mean_um;
			m_Image.m_li_r_min_um = m_Ct_Image.m_li_r_min_um;
			m_Image.m_li_r_max_um = m_Ct_Image.m_li_r_max_um;

			for (int a = 0; a < 360; a++)
			{
				m_Image.m_li_r_um[a] = m_Ct_Image.m_li_r_um[a];
			}

			m_Image.m_li_x0_um = Adjust_x0_um;
			m_Image.m_li_y0_um = Adjust_y0_um;

			m_Image.m_li_ok = m_Ct_Image.m_li_ok;
		}
		else
		{
			if (Adjust_x0_um != m_Image.m_li_x0_um || Adjust_y0_um != m_Image.m_li_y0_um)
			{
				m_Image.m_li_x0_um = Adjust_x0_um;
				m_Image.m_li_y0_um = Adjust_y0_um;
			}
		}
		//ABC
	}


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

void CImageDlg3::OnLimbus4RadioClicked()
{
	m_Limbus1Radio.SetCheck(0);
	m_Limbus2Radio.SetCheck(0);
	m_Limbus3Radio.SetCheck(0);
	m_Limbus4Radio.SetCheck(1);
	m_L = 4;
	Repaint();
}

//***************************************************************************************

void CImageDlg3::OnVertexRadioClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableRadialRulerRadios();//520
	DisableLinearRulerRadios();//520
	DisableInlayRadios();//520

	if (m_RADIAL_RULER)
	{
		m_Last_I_R = -1;
		m_Last_I_I_R = -1;

		HideInlayInfo();

		m_VertexCheck.SetCheck(TRUE);
		m_VertexCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_LimbusCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);
		m_InlayCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
		m_InlayCheck.EnableWindow(TRUE);
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

void CImageDlg3::OnVertex1RadioClicked()
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

void CImageDlg3::OnVertex2RadioClicked()
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

void CImageDlg3::OnVertex3RadioClicked()
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

void CImageDlg3::OnVertex4RadioClicked()
{
	m_V = 4;
	Repaint();
}

//***************************************************************************************

//520
void CImageDlg3::OnRadialRulerClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableVertexRadios();
	DisableLinearRulerRadios();//520
	DisableInlayRadios();

	m_DisText.MoveWindow(&m_OtherText1Rec, 1);

	if (m_RADIAL_RULER)
	{
		m_Last_I_R = -1;
		m_Last_I_I_R = -1;

		HideInlayInfo();

		m_RRulerCheck.SetCheck(TRUE);
		m_RRulerCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_LimbusCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);
		m_InlayCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
		m_InlayCheck.EnableWindow(TRUE);
	}

	if (m_RADIAL_RULER)
	{
		m_RRulerCheck.SetCheck(TRUE);
		m_RRulerCheck.EnableWindow(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
		m_InlayCheck.EnableWindow(TRUE);
	}

	m_RadialRulerRadio.SetCheck(1);
	m_RadialRuler1Radio.EnableWindow(TRUE);
	m_RadialRuler2Radio.EnableWindow(TRUE);
	m_RadialRuler3Radio.EnableWindow(TRUE);

	if (m_R_R == 1) OnRadialRuler1Clicked();
	else if (m_R_R == 2) OnRadialRuler2Clicked();
	else if (m_R_R == 3) OnRadialRuler3Clicked();
}

void CImageDlg3::OnRadialRuler1Clicked()
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

void CImageDlg3::OnRadialRuler2Clicked()
{
	m_Image.ClearRuler();

	m_RadialRuler1Radio.SetCheck(0);
	m_RadialRuler2Radio.SetCheck(1);
	m_RadialRuler3Radio.SetCheck(0);

	m_R_R = 2;
	Repaint();
}

void CImageDlg3::OnRadialRuler3Clicked()
{
	m_RadialRuler1Radio.SetCheck(0);
	m_RadialRuler2Radio.SetCheck(0);
	m_RadialRuler3Radio.SetCheck(1);

	m_R_R = 3;
	Repaint();
}


//***************************************************************************************

//520
void CImageDlg3::OnLinearRulerClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableVertexRadios();
	DisableInlayRadios();
	DisableRadialRulerRadios();

	m_DisText.MoveWindow(&m_OtherText1Rec, 1);

	if (m_RADIAL_RULER)
	{
		m_Last_I_R = -1;
		m_Last_I_I_R = -1;

		HideInlayInfo();

		m_LRulerCheck.SetCheck(TRUE);
		m_LRulerCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_LimbusCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);
		m_InlayCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
		m_InlayCheck.EnableWindow(TRUE);
	}

	m_LinearRulerRadio.SetCheck(1);
	m_LinearRuler1Radio.EnableWindow(TRUE);
	m_LinearRuler2Radio.EnableWindow(TRUE);
	m_LinearRuler3Radio.EnableWindow(TRUE);

	if (m_L_R == 1) OnLinearRuler1Clicked();
	else if (m_L_R == 2) OnLinearRuler2Clicked();
	else if (m_L_R == 3) OnLinearRuler3Clicked();
}

void CImageDlg3::OnLinearRuler1Clicked()
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

void CImageDlg3::OnLinearRuler2Clicked()
{
	m_Image.ClearLRuler();

	m_LinearRuler1Radio.SetCheck(0);
	m_LinearRuler2Radio.SetCheck(1);
	m_LinearRuler3Radio.SetCheck(0);

	m_L_R = 2;
	Repaint();
}

void CImageDlg3::OnLinearRuler3Clicked()
{
	m_LinearRuler1Radio.SetCheck(0);
	m_LinearRuler2Radio.SetCheck(0);
	m_LinearRuler3Radio.SetCheck(1);

	m_L_R = 3;
	Repaint();
}

//***************************************************************************************

void CImageDlg3::OnInlayClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableVertexRadios();
	DisableRadialRulerRadios();
	DisableLinearRulerRadios();

	m_DisText.MoveWindow(&m_InlayText1Rec, 1);

	if (m_RADIAL_RULER)
	{
		m_InalyDrawed = FALSE;
		HideInlayInfo();

		m_InlayCheck.SetCheck(TRUE);
		m_InlayCheck.EnableWindow(FALSE);

		m_PupilCheck.SetCheck(FALSE);
		m_LimbusCheck.SetCheck(FALSE);
		m_VertexCheck.SetCheck(FALSE);
		m_RRulerCheck.SetCheck(FALSE);
		m_LRulerCheck.SetCheck(FALSE);

		m_PupilCheck.EnableWindow(TRUE);
		m_LimbusCheck.EnableWindow(TRUE);
		m_VertexCheck.EnableWindow(TRUE);
		m_RRulerCheck.EnableWindow(TRUE);
		m_LRulerCheck.EnableWindow(TRUE);
	}

	m_InlayRadio.SetCheck(1);
	m_Inlay1Radio.EnableWindow(TRUE);
	m_Inlay2Radio.EnableWindow(TRUE);
	m_Inlay3Radio.EnableWindow(TRUE);
	m_Inlay4Radio.EnableWindow(TRUE);

	if (m_I == 1) OnInlay1Clicked();
	else if (m_I == 2) OnInlay2Clicked();
	else if (m_I == 3) OnInlay3Clicked();
	else if (m_I == 4) OnInlay4Clicked();
}

void CImageDlg3::OnInlay1Clicked()//Original
{
	if (m_pImage->m_In_ok == TRUE)
	{
		m_Image.m_In_ok = TRUE;
		m_Image.m_In_x_um = m_pImage->m_In_x_um;
		m_Image.m_In_y_um = m_pImage->m_In_y_um;
		m_Image.m_In_r_um = m_pImage->m_In_r_um;
		m_Image.m_In_In_r_um = m_pImage->m_In_In_r_um;

		m_Inlay1Radio.SetCheck(1);
		m_Inlay2Radio.SetCheck(0);
		m_Inlay3Radio.SetCheck(0);
		m_Inlay4Radio.SetCheck(0);

		m_I = 1;
	}
	else
	{
		m_Inlay1Radio.EnableWindow(FALSE);
		OnInlay2Clicked();
	}
	m_InalyDrawed = FALSE;
	HideInlayInfo();

	Repaint();
}

void CImageDlg3::OnInlay2Clicked()//No inlay
{
	m_Image.m_In_ok = FALSE;
	m_Image.m_In_x_um = 0.0;
	m_Image.m_In_y_um = 0.0;
	m_Image.m_In_r_um = 0.0;
	m_Image.m_In_In_r_um = 0.0;

	m_Inlay1Radio.SetCheck(0);
	m_Inlay2Radio.SetCheck(1);
	m_Inlay3Radio.SetCheck(0);
	m_Inlay4Radio.SetCheck(0);

	m_I = 2;

	m_InalyDrawed = FALSE;
	HideInlayInfo();

	Repaint();
}

void CImageDlg3::OnInlay3Clicked()//Auto
{
	if (m_Image.m_ve_ok) //m_pImage are comes from lat save info, m_Image is maped to current info
	{
		m_Image.m_In_ok = TRUE;
		m_Image.m_In_x_um = m_Image.m_ve_x_um;
		m_Image.m_In_y_um = m_Image.m_ve_y_um;
		m_Image.m_In_r_um = intRound(2.0 / 0.002);
		m_Image.m_In_In_r_um = 0;

		//530 For Presbia
		if (m_isPresbia)
		{
			m_Image.m_In_r_um = intRound(3.2 / 0.002);
			m_Image.m_In_In_r_um = intRound(0.51 / 0.002);
		}
		//530 Done

		m_Inlay1Radio.SetCheck(0);
		m_Inlay2Radio.SetCheck(0);
		m_Inlay3Radio.SetCheck(1);
		m_Inlay4Radio.SetCheck(0);

		m_I = 3;
	}
	else
	{
		m_Image.m_In_ok = TRUE;
		m_Image.m_In_x_um = 0;
		m_Image.m_In_y_um = 0;
		m_Image.m_In_r_um = intRound(2.0 / 0.002);
		m_Image.m_In_In_r_um = 0;

		//530 For Presbia
		if (m_isPresbia)
		{
			m_Image.m_In_r_um = intRound(3.2 / 0.002);
			m_Image.m_In_In_r_um = intRound(0.51 / 0.002);
		}
		//530 Done

		m_Inlay1Radio.SetCheck(0);
		m_Inlay2Radio.SetCheck(0);
		m_Inlay3Radio.SetCheck(1);
		m_Inlay4Radio.SetCheck(0);

		m_I = 3;
	}

	m_InalyDrawed = FALSE;
	HideInlayInfo();

	Repaint();
}

void CImageDlg3::OnInlay4Clicked()//Manual
{
	if (!m_Image.m_In_ok)
	{
		if (m_Image.m_ve_ok) //m_pImage are comes from lat save info, m_Image is maped to current info
		{
			m_Image.m_In_ok = TRUE;
			m_Image.m_In_x_um = m_Image.m_ve_x_um;
			m_Image.m_In_y_um = m_Image.m_ve_y_um;
			m_Image.m_In_r_um = intRound(2.0 / 0.002);
			m_Image.m_In_In_r_um = 0;
		}
		else
		{
			m_Image.m_In_ok = TRUE;
			m_Image.m_In_x_um = 0;
			m_Image.m_In_y_um = 0;
			m_Image.m_In_r_um = intRound(2.0 / 0.002);
			m_Image.m_In_In_r_um = 0;
		}
		m_Inlay1Radio.SetCheck(0);
		m_Inlay2Radio.SetCheck(0);
		m_Inlay3Radio.SetCheck(1);
		m_Inlay4Radio.SetCheck(0);

		m_I = 3;
	}
	else
	{
		m_Inlay1Radio.SetCheck(0);
		m_Inlay2Radio.SetCheck(0);
		m_Inlay3Radio.SetCheck(0);
		m_Inlay4Radio.SetCheck(1);

		m_I = 4;

		m_InalyDrawed = FALSE;
		HideInlayInfo();

		//Set the inner diameter range
		CString s, InlayV;
		m_InlayEdit.GetWindowText(InlayV);
		real_t val;
		if (!InlayV.IsEmpty() && IsDigital(InlayV))
		{
			G_As = InlayV;
			val = (real_t)atof(G_As) - 1;
		}
		else
		{
			val = m_Image.m_In_r_um * 2 / 1000 - 1;
		}

		if (val == 1 || val == 2 || val == 3 || val == 4 || val == 5
			|| val == 6 || val == 7 || val == 8)
		{
			s.Format(_T("mm (0 - %.0fmm)"), val);
		}
		else  s.Format(_T("mm (0 - %.3fmm)"), val);
		m_DisText5.SetWindowText(s);
		//Done

		Repaint();
	}
}
//520

//***************************************************************************************

//520
void CImageDlg3::OnSetInalyDBtnClicked()
{
	CString InlayV, InlaySV;
	m_InlayEdit.GetWindowText(InlayV);
	m_InlayEdit2.GetWindowText(InlaySV);

	if (InlayV.IsEmpty() && InlaySV.IsEmpty())
	{
		::Info("Please input the disk diameter.");
		m_InlayEdit.SetFocus();
		return;
	}

	if (!IsDigital(InlayV))
	{
		::Info("Please input the correct disk diameter (1 - 9 mm).");
		m_InlayEdit.SetWindowText(_T(""));
		m_InlayEdit.SetFocus();
		return;
	}

	real_t val;
	if (!InlayV.IsEmpty())
	{
		G_As = InlayV;
		val = (real_t)atof(G_As);

		if (val < 1 || val > 9 && val != 0)
		{
			::Info("Please input the correct disk diameter (1 - 9 mm).");
			m_InlayEdit.SetWindowText(_T(""));
			m_InlayEdit.SetFocus();
			return;
		}
	}

	//Inner Diameter
	CString s;
	if (!InlayV.IsEmpty()) 	m_InnerD_Ran = val - 1;
	else                    m_InnerD_Ran = m_Image.m_In_r_um * 2 / 1000 - 1;

	real_t val2 = 0;
	if (m_InnerD_Ran == 1 || m_InnerD_Ran == 2 || m_InnerD_Ran == 3 || m_InnerD_Ran == 4 || m_InnerD_Ran == 5
		|| m_InnerD_Ran == 6 || m_InnerD_Ran == 7 || m_InnerD_Ran == 8)
	{
		s.Format(_T("mm (0 - %.0fmm)"), m_InnerD_Ran);
	}
	else s.Format(_T("mm (0 - %.3fmm)"), m_InnerD_Ran);

	if (m_InnerD_Ran == 0)
	{
		s = "mm (0 mm)";
		m_InlayEdit2.SetWindowText(_T("0"));
		m_InlayEdit2.EnableWindow(FALSE);
		m_Image.m_In_r_um = val / 0.002;
		m_Image.m_In_In_r_um = 0;
		m_DisText5.SetWindowText(s);
	}
	else
	{
		m_InlayEdit2.EnableWindow(TRUE);
		m_DisText5.SetWindowText(s);

		InlaySV.Replace(_T(" "), _T(""));
		if (InlaySV != "")
		{
			if (!IsDigital(InlaySV))
			{
				s.Format(_T("Please input the correct disk inner diameter (0 - %.3f mm)."), m_InnerD_Ran);
				::Info(s);
				m_InlayEdit2.SetWindowText(_T(""));
				m_InlayEdit2.SetFocus();
				return;
			}

			G_As = InlaySV;
			val2 = atof(G_As);

			if (val2 < -0.0001 || val2 > m_InnerD_Ran + 0.0001)
			{
				s.Format(_T("Please input the correct disk inner diameter (0 - %.3f mm)."), m_InnerD_Ran);
				::Info(s);
				m_InlayEdit2.SetWindowText(_T(""));
				m_InlayEdit2.SetFocus();
				return;
			}
		}
		else
		{
			val2 = m_Image.m_In_In_r_um*0.002;
			if (val2 + 1 > val) val2 = val - 1;
			if (val2 < 0)      val2 = 0;
		}
		//Done


		if (!InlayV.IsEmpty()) m_Image.m_In_r_um = val / 0.002;
		m_Image.m_In_In_r_um = val2 / 0.002;
	}

	m_InalyDrawed = FALSE;

	m_InlayEdit.SetWindowText(_T(""));
	m_InlayEdit2.SetWindowText(_T(""));

	Repaint();
}

BOOL CImageDlg3::IsDigital(CString strItem)
{
	for (int i = 0; i<strItem.GetLength(); i++)
	{
		// check char
		if (!isdigit(strItem.GetAt(i)) && strItem.GetAt(i) != '.')
			return FALSE;
	}

	if (strItem.Find('.') != strItem.ReverseFind('.'))
		return FALSE;
	return TRUE;
}

//Done

//***************************************************************************************

void CImageDlg3::DrawEye(CMDC& MemDC)
{
	memcpy(MemDC.m_RGBData, m_Image.m_RGBData.GetMem(), m_Image.m_RGBData.GetSize());
}

//***************************************************************************************

void CImageDlg3::DrawPupil(CMDC& MemDC)
{
	if (m_PupilRadio.GetCheck() != 1 && !m_PupilCheck.GetCheck())
	{
		if (m_VertexRadio.GetCheck() == 1)
		{
			m_DisText.SetWindowText(_T(""));
		}
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
			real_t r_um = m_Image.m_pu_r_mean_um + 3;
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
			real_t r_um = m_Image.m_pu_r_mean_um - 3;
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

		// центр зрачка
		int x0 = intRound(m_cx + m_Image.m_pu_x0_um * m_x_px_um);
		int y0 = intRound(m_cy - m_Image.m_pu_y0_um * m_y_px_um);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, GREEN);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, GREEN);
		// граница зрачка
		for (int a = 0; a < 360; a++) {
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

void CImageDlg3::DrawLimbus(CMDC& MemDC)
{
	if (m_LimbusRadio.GetCheck() != 1 && !m_LimbusCheck.GetCheck())
	{
		if (m_VertexRadio.GetCheck() == 1)
		{
			m_DisText.SetWindowText(_T(""));
		}
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
			real_t r_um = m_Image.m_li_r_mean_um + 3;
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
			real_t r_um = m_Image.m_li_r_mean_um - 3;
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

void CImageDlg3::DrawVertex(CMDC& MemDC)
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
void CImageDlg3::DrawRuler(CMDC& MemDC)
{
	if (m_RadialRulerRadio.GetCheck() != 1 && !m_RRulerCheck.GetCheck())
	{
		if (m_VertexRadio.GetCheck() == 1)
		{
			m_DisText.SetWindowText(_T(""));
		}
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

			real_t r_um = m_Image.m_Ra_r_um + 350.0;
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
	else
	{
		BOOL setText = TRUE;
		if (m_Image.m_In_ok == TRUE)
		{
			if (m_Last_I_R == m_Image.m_In_r_um) setText = FALSE;
		}
		if (setText) m_DisText.SetWindowText(_T(""));
	}
}

//520

//***************************************************************************************
//520
void CImageDlg3::DrawLRuler(CMDC& MemDC)
{
	if (m_LinearRulerRadio.GetCheck() != 1 && !m_LRulerCheck.GetCheck())
	{
		if (m_VertexRadio.GetCheck() == 1)
		{
			m_DisText.SetWindowText(_T(""));
		}
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
		real_t dis = hyp(m_Image.m_LRa_x0_um - m_Image.m_LRa_x1_um, m_Image.m_LRa_y0_um - m_Image.m_LRa_y1_um);

		CString s;
		s.Format(_T("(D) = (%.3fmm)"), 0.001*dis);

		if (dis > 800)
		{
			int xm, ym;
			if (x0 < x1)
			{
				xm = x0 + intRound((real_t)abs(x0 - x1) / 2.0);
				if (y0 < y1)	ym = y0 + intRound((real_t)abs(y0 - y1) / 2.0);
				else ym = y1 + intRound((real_t)abs(y0 - y1) / 2.0);
			}
			else
			{
				xm = x1 + intRound((real_t)abs(x0 - x1) / 2.0);
				if (y0 < y1)	ym = y0 + intRound((real_t)abs(y0 - y1) / 2.0);
				else ym = y1 + intRound((real_t)abs(y0 - y1) / 2.0);
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
	else
	{
		BOOL setText = TRUE;
		if (m_Image.m_In_ok == TRUE)
		{
			if (m_Last_I_R == m_Image.m_In_r_um) setText = FALSE;
		}
		if (setText) m_DisText.SetWindowText(_T(""));
	}
}

//520
//***************************************************************************************
void CImageDlg3::DrawInlay(CMDC& MemDC)
{
	if (m_InlayRadio.GetCheck() != 1 && !m_InlayCheck.GetCheck())
	{
		if (m_VertexRadio.GetCheck() == 1)
		{
			m_DisText.SetWindowText(_T(""));
		}
		return;
	}

	{
		//The center cross
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);
	}

	if (m_Image.m_In_ok == TRUE && m_Image.m_In_r_um == 0)
		m_Image.m_In_ok = FALSE;

	if (m_Image.m_In_ok == TRUE)
	{
		int x0, y0, r0, r1;

		//520
		if (0.002*m_Image.m_In_In_r_um < 0) m_Image.m_In_In_r_um = 0;

		if (m_wheel_n != m_last_wheel_n   && m_I == 4)
		{
			m_InlayEdit.SetWindowText(_T(""));
			m_InlayEdit2.SetWindowText(_T(""));
			m_InlayEdit2.EnableWindow(TRUE);
			m_InalyDrawed = FALSE;
			BOOL ZoomOutSide = TRUE;

			real_t disX = m_x_um[1] - m_Image.m_In_x_um;
			real_t disY = m_y_um[1] - m_Image.m_In_y_um;
			real_t dis = sqrt(disX*disX + disY*disY);

			if (dis < m_Image.m_In_r_um) ZoomOutSide = FALSE;

			if (m_wheel_n > m_last_wheel_n)
			{
				if (ZoomOutSide) m_Image.m_In_r_um += 10;
				else
				{
					m_Image.m_In_In_r_um += 10;
					if (m_Image.m_In_In_r_um > m_Image.m_In_r_um - 500)
						m_Image.m_In_In_r_um = m_Image.m_In_r_um - 500;
				}
			}
			else if (m_wheel_n < m_last_wheel_n)
			{
				if (ZoomOutSide)
				{
					m_Image.m_In_r_um -= 10;
					if (m_Image.m_In_r_um < m_Image.m_In_In_r_um + 500)
						m_Image.m_In_r_um = m_Image.m_In_In_r_um + 500;
				}
				else
				{
					m_Image.m_In_In_r_um -= 10;
					if (m_Image.m_In_In_r_um < 0) m_Image.m_In_In_r_um = 0;
				}
			}

			CString s;
			real_t val = m_Image.m_In_r_um * 2 / 1000 - 1;
			if (val == 0)
			{
				s = "mm (0mm)";
				m_InlayEdit2.EnableWindow(FALSE);
			}
			else
			{
				if (val == 1 || val == 2 || val == 3 || val == 4 || val == 5
					|| val == 6 || val == 7 || val == 8)
				{
					s.Format(_T("mm (0 - %.0fmm)"), val);
				}
				else s.Format(_T("mm (0 - %.3fmm)"), val);
			}
			m_DisText5.SetWindowText(s);
		}

		if (m_I == 4) m_last_wheel_n = m_wheel_n;

		if (m_Image.m_In_r_um < 500) m_Image.m_In_r_um = 500;
		if (m_Image.m_In_r_um > 4500) m_Image.m_In_r_um = 4500;


		x0 = intRound(m_cx + m_Image.m_In_x_um * m_x_px_um);
		y0 = intRound(m_cy - m_Image.m_In_y_um * m_y_px_um);
		r0 = intRound(m_Image.m_In_r_um * m_x_px_um);
		r1 = intRound(m_Image.m_In_In_r_um * m_x_px_um);

		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, YELLOW);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, YELLOW);

		//a filled circle with a center cut-out, 50% transparent white, no outline
		//an annulus design 3.8 mm outer diameter and 1.6 mm inner diamet
		m_OutSideWidth = r0;
		m_InsideWidth = intRound(r1*0.7);

		CString s, s2;
		s.Format(_T("(D) = (%.3fmm)"), 0.002*m_Image.m_In_r_um);
		s2.Format(_T("(Inner D) = (%.3fmm)"), 0.002*m_Image.m_In_In_r_um);

		int OutSideLeftX = x0 - m_OutSideWidth - 1;
		int OutSideRightX = x0 + m_OutSideWidth + 1;
		int OutSideTopY = y0 - m_OutSideWidth - 1;
		int OutSideBotmY = y0 + m_OutSideWidth + 1;

		int InsideLeftX = x0 - m_InsideWidth + 1;
		int InsideRightX = x0 + m_InsideWidth - 1;
		int InsideTopY = y0 - m_InsideWidth + 1;
		int InsideBotmY = y0 + m_InsideWidth - 1;

		int t = 0;

		for (int i = OutSideLeftX; i <= OutSideRightX; i++)
		{
			for (int j = OutSideTopY; j <= OutSideBotmY; j++)
			{
				if (m_InalyDrawed)
				{
					if (m_InlayPos[t] == 1)
					{
						m_MemDC.SetPixel(i, j, 250, 250, 200, 128);
					}
				}
				else
				{
					if ((i >= InsideLeftX && i <= InsideRightX && j >= InsideTopY && j <= InsideBotmY))
					{
						m_InlayPos[t] = 0;
					}
					else
					{
						real_t disX = (real_t)(i - x0);
						real_t disY = (real_t)(j - y0);

						real_t dis = sqrt(disX*disX + disY*disY);

						if (dis <= r0 && dis >= r1)
						{
							m_InlayPos[t] = 1;
						}
						else m_InlayPos[t] = 0;
					}

					if (m_InlayPos[t] == 1)
					{
						m_MemDC.SetPixel(i, j, 250, 250, 200, 128);
					}

				}
				t++;
			}
		}
		m_InalyDrawed = TRUE;


		//Draw the dashed edge
		int tE = 0;
		for (int a = 0; a < 360; a++)
		{
			int b = a == 359 ? 0 : a + 1;

			int xa = x0 + intRound(r0 * COS[a]);
			int ya = y0 - intRound(r0 * SIN[a]);
			int xb = x0 + intRound(r0 * COS[b]);
			int yb = y0 - intRound(r0 * SIN[b]);

			int x2a = x0 + intRound(r1 * COS[a]);
			int y2a = y0 - intRound(r1 * SIN[a]);
			int x2b = x0 + intRound(r1 * COS[b]);
			int y2b = y0 - intRound(r1 * SIN[b]);

			tE++;
			if (tE == 10)
				tE = 0;
			if (tE <= 5)
			{
				MemDC.DrawLine(xa, ya, xb, yb, 1, BLACK);
				MemDC.DrawLine(x2a, y2a, x2b, y2b, 1, BLACK);
			}
			else
			{
				MemDC.DrawLine(xa, ya, xb, yb, 1, WHITE);
				MemDC.DrawLine(x2a, y2a, x2b, y2b, 1, WHITE);
			}
		}
		//Done


		if (m_InlayRadio.GetCheck() == 1 && m_I != 2)
		{
			m_DisText.ShowWindow(SW_SHOW);
			m_DisText6.ShowWindow(SW_SHOW);
			if (m_I == 4)
			{
				m_DisText2.ShowWindow(SW_SHOW);
				m_DisText3.ShowWindow(SW_SHOW);
				m_InlayEdit.ShowWindow(SW_SHOW);
				m_DisText4.ShowWindow(SW_SHOW);
				m_DisText5.ShowWindow(SW_SHOW);
				m_InlayEdit2.ShowWindow(SW_SHOW);
				m_SetInlayBtn.ShowWindow(SW_SHOW);
			}

			//if(m_Last_I_R != m_Image.m_In_r_um)
			{
				s = "Disk : " + s;
				m_DisText.SetWindowText(s);
				m_DisText6.SetWindowText(s2);
			}

			//if(m_Last_I_I_R != m_Image.m_In_In_r_um)
			{
				m_DisText6.SetWindowText(s2);
			}
		}

		m_Last_I_R = m_Image.m_In_r_um;
		m_Last_I_I_R = m_Image.m_In_In_r_um;
	}
	else
	{
		m_DisText.SetWindowText(_T(""));
	}
}

//*******************************************************************************

void CImageDlg3::DisablePupilRadios()
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

void CImageDlg3::DisableLimbusRadios()
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

void CImageDlg3::DisableVertexRadios()
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
void CImageDlg3::DisableRadialRulerRadios()
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
void CImageDlg3::DisableLinearRulerRadios()
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

//520
void CImageDlg3::DisableInlayRadios()
{
	m_InlayRadio.SetCheck(0);
	m_Inlay1Radio.SetCheck(0);
	m_Inlay2Radio.SetCheck(0);
	m_Inlay3Radio.SetCheck(0);
	m_Inlay4Radio.SetCheck(0);

	m_Inlay1Radio.EnableWindow(FALSE);
	m_Inlay2Radio.EnableWindow(FALSE);
	m_Inlay3Radio.EnableWindow(FALSE);
	m_Inlay4Radio.EnableWindow(FALSE);
}
//520
//***************************************************************************************
//520
BOOL CImageDlg3::OnMouseWheel(uint nFlags, short zDelta, CPoint Point)
{
	if ((m_PupilRadio.GetCheck() && m_P == 4)
		|| (m_LimbusRadio.GetCheck() && m_L == 4)
		|| (m_RadialRulerRadio.GetCheck() && m_R_R == 3)
		|| (m_InlayRadio.GetCheck() && m_I == 4)
		)
	{

		m_wheel_n += zDelta / WHEEL_DELTA;
		if (m_wheel_n < -200) {
			m_wheel_n = -1;
			m_last_wheel_n = 0;
			return FALSE;
		}
		else if (m_wheel_n > 200) {
			m_wheel_n = 1;
			m_last_wheel_n = 0;
			return FALSE;
		}

		Repaint();

		return CWnd::OnMouseWheel(nFlags, zDelta, Point);
	}
	else return FALSE;
}
//520
//***************************************************************************************

BOOL CImageDlg3::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		CEdit* ptr1 = (CEdit*)GetDlgItem(IDC_EDIT_INALYD);
		CEdit* ptr2 = (CEdit*)GetDlgItem(IDC_EDIT_INALYD2);
		CRect rect1, rect2;
		ptr1->GetWindowRect(&rect1);
		ptr2->GetWindowRect(&rect2);
		if (rect1.PtInRect(pMsg->pt))
		{
			m_InlayEdit.SetWindowText(_T(""));
			Repaint();
		}

		if (rect2.PtInRect(pMsg->pt))
		{
			CString D, s;

			m_InlayEdit.GetWindowText(D);
			m_InlayEdit2.SetWindowText(_T(""));
			if (!D.IsEmpty())
			{
				if (!IsDigital(D)) m_InnerD_Ran = 8;
				else
				{
					G_As = D;
					m_InnerD_Ran = (real_t)atof(G_As) - 1;
					if (m_InnerD_Ran > 8) m_InnerD_Ran = 8;
				}
			}
			else
			{
				m_InnerD_Ran = m_Image.m_In_r_um * 2 / 1000 - 1;
			}

			if (m_InnerD_Ran == 0)
			{
				s = "mm (0 mm)";
				m_InlayEdit2.SetWindowText(_T("0"));
				m_InlayEdit2.EnableWindow(FALSE);
			}
			else
			{
				if (m_InnerD_Ran == 1 || m_InnerD_Ran == 2 || m_InnerD_Ran == 3 || m_InnerD_Ran == 4 || m_InnerD_Ran == 5
					|| m_InnerD_Ran == 6 || m_InnerD_Ran == 7 || m_InnerD_Ran == 8)
				{
					s.Format(_T("mm (0 - %.0fmm)"), m_InnerD_Ran);
				}
				else s.Format(_T("mm (0 - %.3fmm)"), m_InnerD_Ran);
				m_InlayEdit2.EnableWindow(TRUE);
				m_InlayEdit2.SetFocus();
			}

			m_DisText5.SetWindowText(s);
			Repaint();
		}
	}

	return FALSE;
}

//**********************************************************************************

void  CImageDlg3::HideInlayInfo()
{
	m_DisText2.ShowWindow(SW_HIDE);
	m_DisText3.ShowWindow(SW_HIDE);
	m_InlayEdit.ShowWindow(SW_HIDE);
	m_DisText4.ShowWindow(SW_HIDE);
	m_DisText5.ShowWindow(SW_HIDE);
	m_DisText6.ShowWindow(SW_HIDE);
	m_InlayEdit2.ShowWindow(SW_HIDE);
	m_SetInlayBtn.ShowWindow(SW_HIDE);
}

//   //[5.1.1],improve the comprison and then find limbus( improve the accuracy)
//   //Create the array for the IOL eye image info	

////calculate the new limbus position
//CEyeImage* TestImage = new CEyeImage();
//TestImage->m_h = m_Image.m_h;
//TestImage->m_w = m_Image.m_w;
//TestImage->m_h_um = m_Image.m_h_um;
//TestImage->m_w_um = m_Image.m_w_um;

////uchar* pData = LoadEye.m_RGBData;
//TestImage->m_RGBData.Create(m_Image.m_h, (int)m_Image.m_w*3, m_Image.m_RGBData.GetMem());
////TestImage->SaveIntoFile("C://cTestbbb.bmp");

//TestImage->ImproveComprison();

////TestImage->SaveIntoFile("C://abc.bmp");

//TestImage->FindLimbus();		

//m_Image.m_li_ok = TestImage->m_li_ok;
//m_Image.m_li_r_max_um = TestImage->m_li_r_max_um;
//m_Image.m_li_r_mean_um = TestImage->m_li_r_mean_um;
//m_Image.m_li_r_min_um = TestImage->m_li_r_min_um;
//for(int i = 0; i < 360; i++)	m_Image.m_li_r_um[i] =  TestImage->m_li_r_um[i];
//m_Image.m_li_x0_um = TestImage->m_li_x0_um;
//m_Image.m_li_y0_um = TestImage->m_li_y0_um;

//delete TestImage;

//m_Image.FindLimbus();	
//for(int i = 0; i < 360; i++)	m_Image.m_li_r_um[i] =  TestImage->m_li_r_um[i];
//m_Image.m_li_x0_um = TestImage->m_li_x0_um;
//m_Image.m_li_y0_um = TestImage->m_li_y0_um;  
//[5.1.1]        