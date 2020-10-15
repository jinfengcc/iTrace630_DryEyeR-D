// averaged wf exam save
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SelectImgDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSelectImgDlg, CDialog)

	ON_BN_CLICKED(IDC_VIDEO_LEFT_BTN, OnLeftButtonClicked)
	ON_BN_CLICKED(IDC_VIDEO_RIGHT_BTN, OnRightButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CSelectImgDlg::CSelectImgDlg(CWnd* pParentWnd, int type) :
	CDialog(IDD_SELECT_IMG_DLG, pParentWnd)//type : 0 :OD; 1 :OS
{
	if (type == 0 || type == 1)
	{
		m_AverageExam = (type == 0 ? ::OD_AverageExam : ::OS_AverageExam);

		for (int i = 0; i < m_AverageExam; i++)
		{
			if (type == 0) m_SelectExamsID[i] = ::OD_SelectExamsID[i];
			else          m_SelectExamsID[i] = ::OS_SelectExamsID[i];
		}

		for (int i = 0; i < m_AverageExam; i++)
		{
			m_pExam[i] = new CWFExam;
			if (!::DB.LoadWFExam(m_SelectExamsID[i], m_pExam[i]))
			{
				::Error("Failed to load exams.");
				return;
			}
		}
	}
	else//for compare display
	{
		m_AverageExam = (type == 3 ? ::OD_AverageExam : type == 4 ? ::OD_AverageExam2
			: type == 5 ? ::OS_AverageExam : OS_AverageExam2);

		for (int i = 0; i < m_AverageExam; i++)
		{
			if (type == 3)       m_SelectExamsID[i] = ::OD_SelectExamsID[i];
			else if (type == 4)  m_SelectExamsID[i] = ::OD_SelectExamsID2[i];
			else if (type == 5)  m_SelectExamsID[i] = ::OS_SelectExamsID[i];
			else if (type == 6)  m_SelectExamsID[i] = ::OS_SelectExamsID2[i];
		}

		for (int i = 0; i < m_AverageExam; i++)
		{
			m_pExam[i] = new CWFExam;
			if (!::DB.LoadWFExam(m_SelectExamsID[i], m_pExam[i]))
			{
				::Error("Failed to load exams.");
				return;
			}
		}
	}
}

//***************************************************************************************

CSelectImgDlg::~CSelectImgDlg()
{

}


//***************************************************************************************

BOOL CSelectImgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Left and right arrow
	RECT Rect;
	this->GetWindowRect(&Rect);
	int W = Rect.right - Rect.left;
	int H = Rect.bottom - Rect.top;

	real_t ratio = real_t(m_pExam[0]->m_Image.m_h) / real_t(m_pExam[0]->m_Image.m_w);
	real_t w = 0.5*W;
	int  h = intRound(w*ratio);

	int  Btm = intRound(0.10*H) + h;
	int  BtnTop = Btm - intRound(0.1*H);

	::SetRect(&Rect, intRound(0.1*W), BtnTop, intRound(0.2*W), Btm);
	m_LeftButton.Create(_T(""), _T(""), 0, Rect, this, IDC_VIDEO_LEFT_BTN);
	m_LeftButton.SetImage(W == 1920 ? IDR_LEFT_ARROW_3 : W == 1600 ? IDR_LEFT_ARROW_2 : IDR_LEFT_ARROW_1);
	m_LeftButton.SetBk(FALSE);
	m_LeftButton.SetEnabled(FALSE);
	m_LeftButton.SetTip("Previous eye image");
	m_LeftButton.ShowWindow(SW_SHOW);

	::SetRect(&Rect, intRound(0.8*W), BtnTop, intRound(0.9*W), Btm);
	m_RightButton.Create(_T(""), _T(""), 0, Rect, this, IDC_VIDEO_RIGHT_BTN);
	m_RightButton.SetImage(W == 1920 ? IDR_RIGHT_ARROW_3 : W == 1600 ? IDR_RIGHT_ARROW_2 : IDR_RIGHT_ARROW_1);
	m_RightButton.SetBk(FALSE);
	m_RightButton.SetEnabled(TRUE);
	m_RightButton.SetTip("Next eye image");
	m_RightButton.ShowWindow(SW_SHOW);
	//Done

	//Eye image
	m_CurEye = 0;

	RECT Rect1 = { intRound(0.25*W), intRound(0.10*H) , intRound(0.75*W), Btm };
	m_EyeImageWnd.CreateWnd(Rect1, this);

	if (m_pExam[0]->m_Image.m_w != 0)
	{
		real_t ratioW = real_t(m_pExam[0]->m_Image.m_w) / real_t(m_EyeImageWnd.m_w);

		for (int y = 0; y < m_EyeImageWnd.m_h; y++)
		{
			for (int x = 0; x < m_EyeImageWnd.m_w; x++)
			{
				int InputI = intRound(x*ratioW);
				int InputJ = intRound(y*ratioW);

				if (InputI >= m_pExam[0]->m_Image.m_w) InputI = m_pExam[0]->m_Image.m_w - 1;
				if (InputJ >= m_pExam[0]->m_Image.m_h) InputJ = m_pExam[0]->m_Image.m_h - 1;

				int R = m_pExam[0]->m_Image.GetRAt(InputI, InputJ);

				m_EyeImageWnd.m_MemDC.SetPixel(x, m_EyeImageWnd.m_h - y - 1, R, R, R);
			}
		}
	}
	else //under some situations there is no image inside the exam
	{
		m_EyeImageWnd.ClearMemDC();
		RECT ThisRect = { 2, 2, m_EyeImageWnd.m_w - 2, m_EyeImageWnd.m_h - 2 };
		m_EyeImageWnd.m_MemDC.DrawRectangle(ThisRect, RED, NOCOLOR);
	}

	m_EyeImageWnd.Invalidate(FALSE);
	m_EyeImageWnd.ShowWindow(SW_SHOW);
	//Done

	return FALSE;
}

//***************************************************************************************

void CSelectImgDlg::OnLeftButtonClicked()
{
	m_CurEye--;

	if (!m_RightButton.IsEnabled())
	{
		m_RightButton.SetEnabled(TRUE);
		m_RightButton.Repaint();
	}

	if (m_CurEye == 0)
	{
		m_LeftButton.SetEnabled(FALSE);
		m_LeftButton.Repaint();
	}

	//Change the image
	if (m_pExam[m_CurEye]->m_Image.m_w != 0)
	{
		real_t ratioW = real_t(m_pExam[m_CurEye]->m_Image.m_w) / real_t(m_EyeImageWnd.m_w);

		for (int y = 0; y < m_EyeImageWnd.m_h; y++)
		{
			for (int x = 0; x < m_EyeImageWnd.m_w; x++)
			{
				int InputI = intRound(x*ratioW);
				int InputJ = intRound(y*ratioW);

				if (InputI >= m_pExam[m_CurEye]->m_Image.m_w) InputI = m_pExam[m_CurEye]->m_Image.m_w - 1;
				if (InputJ >= m_pExam[m_CurEye]->m_Image.m_h) InputJ = m_pExam[m_CurEye]->m_Image.m_h - 1;


				int R = m_pExam[m_CurEye]->m_Image.GetRAt(InputI, InputJ);

				m_EyeImageWnd.m_MemDC.SetPixel(x, m_EyeImageWnd.m_h - y - 1, R, R, R);
			}
		}
	}

	else //under some situations there is no image inside the exam
	{
		m_EyeImageWnd.ClearMemDC();
		RECT ThisRect = { 2, 2, m_EyeImageWnd.m_w - 2, m_EyeImageWnd.m_h - 2 };
		m_EyeImageWnd.m_MemDC.DrawRectangle(ThisRect, RED, NOCOLOR);
	}

	m_EyeImageWnd.Invalidate(FALSE);
	m_EyeImageWnd.ShowWindow(SW_SHOW);
	//Done
}

//***************************************************************************************

void CSelectImgDlg::OnRightButtonClicked()
{
	m_CurEye++;

	if (!m_LeftButton.IsEnabled())
	{
		m_LeftButton.SetEnabled(TRUE);
		m_LeftButton.Repaint();
	}

	if (m_CurEye >= m_AverageExam - 1)
	{
		m_RightButton.SetEnabled(FALSE);
		m_RightButton.Repaint();
	}

	//Change the image
	if (m_pExam[m_CurEye]->m_Image.m_w != 0)
	{
		real_t ratioW = real_t(m_pExam[m_CurEye]->m_Image.m_w) / real_t(m_EyeImageWnd.m_w);

		for (int y = 0; y < m_EyeImageWnd.m_h; y++)
		{
			for (int x = 0; x < m_EyeImageWnd.m_w; x++)
			{
				int InputI = intRound(x*ratioW);
				int InputJ = intRound(y*ratioW);

				if (InputI >= m_pExam[m_CurEye]->m_Image.m_w) InputI = m_pExam[m_CurEye]->m_Image.m_w - 1;
				if (InputJ >= m_pExam[m_CurEye]->m_Image.m_h) InputJ = m_pExam[m_CurEye]->m_Image.m_h - 1;


				int R = m_pExam[m_CurEye]->m_Image.GetRAt(InputI, InputJ);

				m_EyeImageWnd.m_MemDC.SetPixel(x, m_EyeImageWnd.m_h - y - 1, R, R, R);
			}
		}
	}

	else //under some situations there is no image inside the exam
	{
		m_EyeImageWnd.ClearMemDC();
		RECT ThisRect = { 2, 2, m_EyeImageWnd.m_w - 2, m_EyeImageWnd.m_h - 2 };
		m_EyeImageWnd.m_MemDC.DrawRectangle(ThisRect, RED, NOCOLOR);
	}

	m_EyeImageWnd.Invalidate(FALSE);
	m_EyeImageWnd.ShowWindow(SW_SHOW);
	//Done
}

//***************************************************************************************
void CSelectImgDlg::OnOK()
{
	m_Sel_ID = m_pExam[m_CurEye]->m_Header.m_ExamID;

	for (int i = 0; i < m_AverageExam; i++)
	{
		SAFE_DELETE(m_pExam[i]);
	}
	CDialog::OnOK();
}

//***************************************************************************************
void CSelectImgDlg::OnCancel()
{
	for (int i = 0; i < m_AverageExam; i++)
	{
		SAFE_DELETE(m_pExam[i]);
	}
	CDialog::OnCancel();
}