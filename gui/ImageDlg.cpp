//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ImageDlg.h"
#include "Data.h"
#include "BusyCursor.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CImageDlg, CDialog)

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

	ON_BN_CLICKED(IDC_RINGS_RADIO, OnRingsRadioClicked)
	ON_BN_CLICKED(IDC_RINGS1_RADIO, OnRings1RadioClicked)
	ON_BN_CLICKED(IDC_RINGS2_RADIO, OnRings2RadioClicked)
	ON_BN_CLICKED(IDC_RINGS3_RADIO, OnRings3RadioClicked)
	ON_BN_CLICKED(IDC_RINGS4_RADIO, OnRings4RadioClicked)


END_MESSAGE_MAP()

//***************************************************************************************
int CImageDlg::m_wheel_n = 0;//520
							 //***************************************************************************************

CImageDlg::CImageDlg(CWnd* pParentWnd, CEyeImage* pImage, BOOL WF, BOOL RADIAL_RULER) : CDialog(IDD_IMAGE_DLG, pParentWnd)
{
	m_WF = WF;
	m_RADIAL_RULER = RADIAL_RULER;
	m_pImage = pImage;
	m_last_wheel_n = 0;

	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));

	ASSERT(m_pImage);
}

//***************************************************************************************

CImageDlg::~CImageDlg()
{
	DestroyAcceleratorTable(m_hAccel);
}

//***************************************************************************************

BOOL CImageDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam != VK_ESCAPE) {
			if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

void CImageDlg::DoDataExchange(CDataExchange* pDX)
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

	DDX_Control(pDX, IDC_RINGS_RADIO, m_RingsRadio);
	DDX_Control(pDX, IDC_RINGS1_RADIO, m_Rings1Radio);
	DDX_Control(pDX, IDC_RINGS2_RADIO, m_Rings2Radio);
	DDX_Control(pDX, IDC_RINGS3_RADIO, m_Rings3Radio);
	DDX_Control(pDX, IDC_RINGS4_RADIO, m_Rings4Radio);

	DDX_Control(pDX, IDC_VERTEX_RADIO, m_VertexRadio);
	DDX_Control(pDX, IDC_VERTEX1_RADIO, m_Vertex1Radio);
	DDX_Control(pDX, IDC_VERTEX2_RADIO, m_Vertex2Radio);
	DDX_Control(pDX, IDC_VERTEX3_RADIO, m_Vertex3Radio);
	DDX_Control(pDX, IDC_VERTEX4_RADIO, m_Vertex4Radio);

	DDX_Control(pDX, IDC_DIS_TEXT, m_DisText);//520
}

//***************************************************************************************

BOOL CImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ShowTechInfo = FALSE;

	m_P = 1;
	m_L = 1;
	m_R = 1;
	m_V = 1;


	m_Image = *m_pImage;

	m_x_um[0] = 0.0;
	m_y_um[0] = 0.0;
	m_x_um[1] = 0.0;
	m_y_um[1] = 0.0;

	Resize();

	OnPupilRadioClicked();

	BOOL IsDBReadOnly = ::DB.IsReadOnly();
	m_OKButton.EnableWindow(!IsDBReadOnly);

	return FALSE;
}

//***************************************************************************************

void CImageDlg::Resize()
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
	int dt2 = intRound(0.015 * W);
	int dt3 = 145;

	m_PupilRadio.SetWindowPos(NULL, iw + d + d, t, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Pupil1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Pupil2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Pupil3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Pupil4Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	if (!m_WF)
	{
		m_LimbusRadio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Limbus1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Limbus2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Limbus3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Limbus4Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_RingsRadio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Rings1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Rings2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Rings3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Rings4Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	m_VertexRadio.SetWindowPos(NULL, iw + d + d, t += dt2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Vertex1Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Vertex2Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Vertex3Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_Vertex4Radio.SetWindowPos(NULL, iw + d + d + d, t += dt1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_DisText.SetWindowPos(NULL, d, 2 * d + m_h + 5, 0, 0, SWP_NOSIZE);  //520;

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

void CImageDlg::OnCtrlD()
{
	m_ShowTechInfo = !m_ShowTechInfo;
	Repaint();
}

//***************************************************************************************

void CImageDlg::Repaint()
{
	DrawEye(m_MemDC);
	DrawPupil(m_MemDC);
	DrawLimbus(m_MemDC);
	DrawRings(m_MemDC);
	DrawVertex(m_MemDC);

	InvalidateRgn(&m_Rgn, FALSE);
}

//***************************************************************************************

void CImageDlg::OnOK()
{
	*m_pImage = m_Image;

	CDialog::OnOK();
}

//***************************************************************************************

void CImageDlg::OnPaint()
{
	CPaintDC PaintDC(this);
	PaintDC.BitBlt(m_l, m_t, m_w, m_h, &m_MemDC, 0, 0, SRCCOPY);
}

//***************************************************************************************

void CImageDlg::OnLButtonUp(uint nFlags, CPoint Point)
{
	m_wheel_n = m_last_wheel_n = 0;//520

	if (Point.x < m_l || Point.x >= m_l + m_w || Point.y < m_t || Point.y >= m_t + m_h) return;
}

//***************************************************************************************
void CImageDlg::OnLButtonDown(uint nFlags, CPoint Point)
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
}

//***************************************************************************************

void CImageDlg::OnMouseMove(uint nFlags, CPoint Point)
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
	}
}

//***************************************************************************************

void CImageDlg::OnPupilRadioClicked()
{
	DisableLimbusRadios();
	DisableRingsRadios();
	DisableVertexRadios();

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

void CImageDlg::OnPupil1RadioClicked()
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

void CImageDlg::OnPupil2RadioClicked()
{
	m_Image.ClearPupil();

	m_Pupil1Radio.SetCheck(0);
	m_Pupil2Radio.SetCheck(1);
	m_Pupil3Radio.SetCheck(0);
	m_Pupil4Radio.SetCheck(0);
	m_P = 2;
	Repaint();
}

void CImageDlg::OnPupil3RadioClicked()
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

void CImageDlg::OnPupil4RadioClicked()
{
	m_Pupil1Radio.SetCheck(0);
	m_Pupil2Radio.SetCheck(0);
	m_Pupil3Radio.SetCheck(0);
	m_Pupil4Radio.SetCheck(1);
	m_P = 4;
	Repaint();
}

//***************************************************************************************

void CImageDlg::OnLimbusRadioClicked()
{
	DisablePupilRadios();
	DisableRingsRadios();
	DisableVertexRadios();

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

void CImageDlg::OnLimbus1RadioClicked()
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

void CImageDlg::OnLimbus2RadioClicked()
{
	m_Image.ClearLimbus();

	m_Limbus1Radio.SetCheck(0);
	m_Limbus2Radio.SetCheck(1);
	m_Limbus3Radio.SetCheck(0);
	m_Limbus4Radio.SetCheck(0);
	m_L = 2;
	Repaint();
}

void CImageDlg::OnLimbus3RadioClicked()
{
	CBusyCursor Cursor;

	//m_Image.FindLimbus();

	////[5.1.1], only the WF need improve the comparison
	if (!m_WF)
		m_Image.FindLimbus();
	else
	{
		//[5.1.1],improve the comprison and then find limbus( improve the accuracy)
		//Create the array for the IOL eye image info	

		//calculate the new limbus position
		CEyeImage* TestImage = new CEyeImage();
		TestImage->m_h = m_Image.m_h;
		TestImage->m_w = m_Image.m_w;
		TestImage->m_h_um = m_Image.m_h_um;
		TestImage->m_w_um = m_Image.m_w_um;

		//uchar* pData = LoadEye.m_RGBData;
		TestImage->m_RGBData.Create(m_Image.m_h, (int)m_Image.m_w * 3, m_Image.m_RGBData.GetMem());
		//TestImage->SaveIntoFile("C:\\cTestbbb.jpg");

		TestImage->ImproveComprison();

		//TestImage->SaveIntoFile("C:\\cTestaaa.jpg");

		TestImage->FindLimbus();

		m_Image.m_li_ok = TestImage->m_li_ok;
		m_Image.m_li_r_max_um = TestImage->m_li_r_max_um;
		m_Image.m_li_r_mean_um = TestImage->m_li_r_mean_um;
		m_Image.m_li_r_min_um = TestImage->m_li_r_min_um;
		for (int i = 0; i < 360; i++)	m_Image.m_li_r_um[i] = TestImage->m_li_r_um[i];
		m_Image.m_li_x0_um = TestImage->m_li_x0_um;
		m_Image.m_li_y0_um = TestImage->m_li_y0_um;

		delete TestImage;
	}
	//[5.1.1]        

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

void CImageDlg::OnLimbus4RadioClicked()
{
	m_Limbus1Radio.SetCheck(0);
	m_Limbus2Radio.SetCheck(0);
	m_Limbus3Radio.SetCheck(0);
	m_Limbus4Radio.SetCheck(1);
	m_L = 4;
	Repaint();
}

//***************************************************************************************

void CImageDlg::OnRingsRadioClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableVertexRadios();

	m_RingsRadio.SetCheck(1);
	m_Rings1Radio.EnableWindow(TRUE);
	m_Rings2Radio.EnableWindow(TRUE);
	m_Rings3Radio.EnableWindow(TRUE);

	if (m_R == 1) OnRings1RadioClicked();
	else if (m_R == 2) OnRings2RadioClicked();
	else if (m_R == 3) OnRings3RadioClicked();
	else if (m_R == 4) OnRings4RadioClicked();
}

void CImageDlg::OnRings1RadioClicked()
{
	if (m_pImage->m_ri_ok) {

		m_Image.m_ve_ok = TRUE;
		m_Image.m_ri_ok = TRUE;
		m_Image.m_ve_x_um = m_pImage->m_ve_x_um;
		m_Image.m_ve_y_um = m_pImage->m_ve_y_um;
		memcpy(m_Image.m_ri_r_um, m_pImage->m_ri_r_um, sizeof(m_Image.m_ri_r_um));

		m_Rings1Radio.SetCheck(1);
		m_Rings2Radio.SetCheck(0);
		m_Rings3Radio.SetCheck(0);
		m_Rings4Radio.SetCheck(0);
		m_R = 1;
		Repaint();
	}
	else {
		m_Rings1Radio.EnableWindow(FALSE);
		OnRings2RadioClicked();
	}
}

void CImageDlg::OnRings2RadioClicked()
{
	m_Image.ClearVertexAndRings();

	m_Rings1Radio.SetCheck(0);
	m_Rings2Radio.SetCheck(1);
	m_Rings3Radio.SetCheck(0);
	m_Rings4Radio.SetCheck(0);
	m_R = 2;
	Repaint();
}

void CImageDlg::OnRings3RadioClicked()
{
	CBusyCursor Cursor;

	m_Image.FindVertexAndRings();

	if (m_Image.m_ri_ok) {
		m_Rings1Radio.SetCheck(0);
		m_Rings2Radio.SetCheck(0);
		m_Rings3Radio.SetCheck(1);
		m_Rings4Radio.SetCheck(0);
		m_R = 3;
		Repaint();
	}

	else {
		::Info("Unable to find rings.");
		OnRings2RadioClicked();
	}
}

void CImageDlg::OnRings4RadioClicked()
{
	m_R = 4;
	Repaint();
}

//***************************************************************************************

void CImageDlg::OnVertexRadioClicked()
{
	DisablePupilRadios();
	DisableLimbusRadios();
	DisableRingsRadios();

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

void CImageDlg::OnVertex1RadioClicked()
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

void CImageDlg::OnVertex2RadioClicked()
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

void CImageDlg::OnVertex3RadioClicked()
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

void CImageDlg::OnVertex4RadioClicked()
{
	m_V = 4;
	Repaint();
}

//***************************************************************************************

void CImageDlg::DrawEye(CMDC& MemDC)
{
	memcpy(MemDC.m_RGBData, m_Image.m_RGBData.GetMem(), m_Image.m_RGBData.GetSize());
}

//***************************************************************************************

void CImageDlg::DrawPupil(CMDC& MemDC)
{
	if (m_PupilRadio.GetCheck() != 1)
	{
		if (m_RingsRadio.GetCheck() == 1 || m_VertexRadio.GetCheck() == 1)
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

void CImageDlg::DrawLimbus(CMDC& MemDC)
{
	if (m_LimbusRadio.GetCheck() != 1)
	{
		if (m_RingsRadio.GetCheck() == 1 || m_VertexRadio.GetCheck() == 1)
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

void CImageDlg::DrawRings(CMDC& MemDC)
{
	if (m_RingsRadio.GetCheck() != 1) return;

	{
		// центр изображения
		int x0 = intRound(m_cx);
		int y0 = intRound(m_cy);
		MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, WHITE);
		MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, WHITE);
	}

	if (m_ShowTechInfo) {

		if (m_Image.m_ve0_ok) {

			int x0 = intRound(m_cx);
			int y0 = intRound(m_cy);

			// гистограмма
			if (m_Image.m_hist_max > 0.0) {
				for (int i = 0; i < 255; i++) {
					int h1 = intRound(400.0 * m_Image.m_hist[i] / m_Image.m_hist_max);
					int h2 = intRound(400.0 * m_Image.m_hist[i + 1] / m_Image.m_hist_max);
					MemDC.DrawLine(i, m_h - 10 - h1, i + 1, m_h - 10 - h2, 1, RED);
				}
				MemDC.DrawLine(m_Image.m_ve0_thr, m_h - 10, m_Image.m_ve0_thr, m_h - 410, 1, YELLOW);
			}

			// узлы
			int X = m_Image.m_table.GetNumCols();
			int Y = m_Image.m_table.GetNumRows();
			int Y2 = Y >> 1;
			int X2 = X >> 1;
			for (int y = 0; y < Y; y++) {
				int yy = y - Y2;
				for (int x = 0; x < X; x++) {
					int xx = x - X2;
					int g = m_Image.m_table(y, x);
					if (g == 3) MemDC.SetPixel(x0 + xx, y0 - yy, 0, 255, 0); // оболочка
					else if (g == 2) MemDC.SetPixel(x0 + xx, y0 - yy, 0, 160, 0); // наибольшая группа
					else if (g == 1) MemDC.SetPixel(x0 + xx, y0 - yy, 0, 80, 0); // ниже порога
				}
			}
		}
	}

	else {

		if (m_Image.m_ri_ok) {

			int x0 = intRound(m_cx + m_Image.m_ve_x_um * m_x_px_um);
			int y0 = intRound(m_cy - m_Image.m_ve_y_um * m_y_px_um);
			MemDC.DrawLine(x0 - 5, y0, x0 + 6, y0, 1, RED);
			MemDC.DrawLine(x0, y0 - 5, x0, y0 + 6, 1, RED);

			for (int r = 0; r < m_Image.m_NumRings; r++) {
				for (int a = 0; a < 360; a++) {
					int b = a == 359 ? 0 : a + 1;
					if (m_Image.m_ri_r_um[r][a] == INVALID_VALUE) continue;
					if (m_Image.m_ri_r_um[r][b] == INVALID_VALUE) continue;
					int xa = intRound(m_cx + (m_Image.m_ve_x_um + m_Image.m_ri_r_um[r][a] * COS[a]) * m_x_px_um);
					int ya = intRound(m_cy - (m_Image.m_ve_y_um + m_Image.m_ri_r_um[r][a] * SIN[a]) * m_y_px_um);
					int xb = intRound(m_cx + (m_Image.m_ve_x_um + m_Image.m_ri_r_um[r][b] * COS[b]) * m_x_px_um);
					int yb = intRound(m_cy - (m_Image.m_ve_y_um + m_Image.m_ri_r_um[r][b] * SIN[b]) * m_y_px_um);
					MemDC.DrawLine(xa, ya, xb, yb, 1, r % 2 ? YELLOW : RED);
				}
			}
		}
	}
}

//***************************************************************************************

void CImageDlg::DrawVertex(CMDC& MemDC)
{
	if (m_VertexRadio.GetCheck() != 1) return;

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

void CImageDlg::DisablePupilRadios()
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

void CImageDlg::DisableLimbusRadios()
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

void CImageDlg::DisableRingsRadios()
{
	m_RingsRadio.SetCheck(0);
	m_Rings1Radio.SetCheck(0);
	m_Rings2Radio.SetCheck(0);
	m_Rings3Radio.SetCheck(0);
	m_Rings4Radio.SetCheck(0);
	m_Rings1Radio.EnableWindow(FALSE);
	m_Rings2Radio.EnableWindow(FALSE);
	m_Rings3Radio.EnableWindow(FALSE);
	m_Rings4Radio.EnableWindow(FALSE);
}

void CImageDlg::DisableVertexRadios()
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

//***************************************************************************************
//520
BOOL CImageDlg::OnMouseWheel(uint nFlags, short zDelta, CPoint Point)
{
	if ((m_PupilRadio.GetCheck() && m_P == 4)
		|| (m_LimbusRadio.GetCheck() && m_L == 4)
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