//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SelectExamDlg.h"
#include "Data.h"
#include "Settings.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSelectExamDlg, CDialog)

	ON_WM_MOUSEMOVE()

	ON_NOTIFY(NM_DBLCLK, IDC_EXAMS_LIST, OnExamsListDblClicked)

	ON_NOTIFY(LVN_HOTTRACK, IDC_EXAMS_LIST, OnExamsListHOVER)

END_MESSAGE_MAP()

//***************************************************************************************

CSelectExamDlg::CSelectExamDlg(CWnd* pParentWnd, GUID& PatientID, int ExamOrder, int Type, int Eye, CMFont* pFont, int NumImages[NUM_EXAMS_MAX]) :
	CDialog(IDD_SELECT_EXAM_DLG, pParentWnd)
{
	::DB.LoadExamHeaders(PatientID);//Big Database Research since 03062019
	//::DB.LoadTxtExamHeaders(PatientID);//Big Database Research since 03062019

	m_Type = Type;
	m_Eye = Eye;
	m_pFont = pFont;
	m_ExamID = GUID_NULL;
	for (int i = 0; i < NUM_EXAMS_MAX; i++)
	{
		m_NumImages[i] = NumImages[i];
	}

	m_ExamOrder = ExamOrder;
}

//***************************************************************************************

BOOL CSelectExamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	CString sExamOrder = "";
	if (m_ExamOrder != 0)
	{
		CString ExamOrderStr = "";
		ExamOrderStr.Format(_T("%i"), m_ExamOrder);
		sExamOrder = " (Current selected is #" + ExamOrderStr + ")";
	}
	//

	if (m_Type == EXAM_TYPE_WF)
	{
		// For Depth of Focus Comparison selection
		if (ISBOTH(m_Eye))
		{
			SetWindowText(_T("Please select a second WF exam") + sExamOrder);
		}
		else // For Depth of Focus Comparison selection Done
		{
			if (ISOD(m_Eye) && !ISOS(m_Eye)) SetWindowText(_T("Please select a second OD WF exam") + sExamOrder);
			else if (!ISOD(m_Eye) && ISOS(m_Eye)) SetWindowText(_T("Please select a second OS WF exam") + sExamOrder);
		}
	}
	else
	{
		if (ISOD(m_Eye)) SetWindowText(_T("Please select a second OD CT exam") + sExamOrder);
		else SetWindowText(_T("Please select a second OS CT exam") + sExamOrder);
	}
	//520

	RECT Rect1, Rect2;
	GetWindowRect(&Rect1);
	GetClientRect(&Rect2);
	int tw = (Rect1.right - Rect1.left) - (Rect2.right - Rect2.left);
	int th = (Rect1.bottom - Rect1.top) - (Rect2.bottom - Rect2.top);

	int W = ::GetSystemMetrics(SM_CXSCREEN);
	int H = ::GetSystemMetrics(SM_CYSCREEN);

	int d = intRound(0.005 * W);

	int bw = intRound(0.065 * W);           // button width
	int bh = intRound(0.033 * H);           // button height

	int dw = intRound(0.700 * W);           // dialog width
	int dh = intRound(0.800 * H);           // dialog height

	int lw = dw - (tw + d + d);          // list width
	int lh = dh - (th + d + d + bh + d); // list height

	SetWindowPos(NULL, 0, 0, dw, dh, SWP_NOZORDER);
	CenterWindow();

	m_EXAM_List_Left = d + intRound((W - dw) / 2);//520
	m_EXAM_List_Hight = dh;//520

	RECT Rect;

	CMDC DC;
	DC.CreateCompatibleDC(NULL);

	//int cw[11];
	int cw[13];//[5.1.1]

			   //[5.1.1]
	DC.MeasureRect("0000  ", Rect, *m_pFont);
	m_EXAM_List_ColW0 = cw[0] = Rect.right - Rect.left + 3;//520 'm_EXAM_List_ColW0 = '

	DC.MeasureRect("0000-00-00   ", Rect, *m_pFont);
	cw[1] = Rect.right - Rect.left + 3;

	DC.MeasureRect("00:00:00   ", Rect, *m_pFont);
	cw[2] = Rect.right - Rect.left + 3;

	cw[3] = 3;

	cw[4] = 3;

	cw[5] = 3;

	cw[6] = 3;

	cw[7] = 3;

	DC.MeasureRect("Preop  ", Rect, *m_pFont);
	cw[8] = Rect.right - Rect.left + 3;

	DC.MeasureRect("Srs  ", Rect, *m_pFont);
	cw[9] = Rect.right - Rect.left + 3;

	if (m_Type == EXAM_TYPE_WF)
	{
		DC.MeasureRect("0.00 mm  ", Rect, *m_pFont);
		cw[10] = Rect.right - Rect.left + 3;
	}
	else cw[10] = 0;
	//[5.1.1]

	//
	DC.MeasureRect("OD/OS  ", Rect, *m_pFont);
	cw[11] = Rect.right - Rect.left + 3;
	//

	int sw = intRound(0.020 * W); // scrollbar width ???
								  //cw[10] = lw - sw - (cw[0] + cw[1] + cw[2] + cw[3] + cw[4] + cw[5] + cw[6] + cw[7] + cw[8] + cw[9]);

	cw[12] = lw - sw - (cw[0] + cw[1] + cw[2] + cw[3] + cw[4] + cw[5] + cw[6] + cw[7] + cw[8] + cw[9] + cw[10] + cw[11]);

	::SetRect(&Rect, d, d, d + lw, d + lh);

	m_ExamHeadersList.SetFont(m_pFont);

	if (m_Type == EXAM_TYPE_WF)//allow use 'ctrl' for muti exam selection
		m_ExamHeadersList.CreateWnd(WS_VISIBLE | LVS_NOSORTHEADER, Rect, this, IDC_EXAMS_LIST);
	else
		m_ExamHeadersList.CreateWnd(WS_VISIBLE | LVS_SINGLESEL | LVS_NOSORTHEADER, Rect, this, IDC_EXAMS_LIST);

	m_ExamHeadersList.InsertColumn(0, _T("#"), intRound(cw[0]));
	m_ExamHeadersList.InsertColumn(1, _T("Date"), intRound(cw[1]));
	m_ExamHeadersList.InsertColumn(2, _T("Time"), intRound(cw[2]));
	m_ExamHeadersList.InsertColumn(3, _T("Ver"), intRound(cw[3]));
	m_ExamHeadersList.InsertColumn(4, _T("Mode"), intRound(cw[4]));
	m_ExamHeadersList.InsertColumn(5, _T("Clinic"), intRound(cw[5]));
	m_ExamHeadersList.InsertColumn(6, _T("Physician"), intRound(cw[6]));
	m_ExamHeadersList.InsertColumn(7, _T("Operator"), intRound(cw[7]));
	m_ExamHeadersList.InsertColumn(8, _T("Preop"), intRound(cw[8]));
	m_ExamHeadersList.InsertColumn(9, _T("Srs"), intRound(cw[9]));
	m_ExamHeadersList.InsertColumn(10, _T("Scan"), intRound(cw[10]));//[5.1.1]
	m_ExamHeadersList.InsertColumn(11, _T("OD/OS"), intRound(cw[11]));//
	m_ExamHeadersList.InsertColumn(12, _T("Note"), intRound(cw[12]));//[5.1.1]
																	 /*m_ExamHeadersList.InsertColumn(10, "Note"     , intRound(cw[10]));*/

	CString s;

	int k = 0;

	for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
	{

		CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

		if ((pExamHeader->m_Type == m_Type) && ((ISOD(pExamHeader->m_Eye) && ISOD(m_Eye)) || (ISOS(pExamHeader->m_Eye) && ISOS(m_Eye)) || ISBOTH(m_Eye)))
		{

			/* if(m_NumImages[e] == 2) m_ExamHeadersList.m_ColorNum[k] = 1;
			else                    m_ExamHeadersList.m_ColorNum[k] = 0;*/

			BOOL FellowExamExist = FALSE;
			GUID FellowExamID = GUID_NULL;
			real_t scanSize = 0;
			int  NumImages = 1;//[5.2]

			if (pExamHeader->m_Type == EXAM_TYPE_WF)
				FellowExamID = ::DB.LoadFellowID(pExamHeader->m_ExamID, scanSize, NumImages, EXAM_TYPE_WF);

			else if (pExamHeader->m_Type == EXAM_TYPE_CT)
				FellowExamID = ::DB.LoadFellowID(pExamHeader->m_ExamID, scanSize, NumImages, EXAM_TYPE_CT);

			if (FellowExamID != GUID_NULL && ::DB.ExamExist(FellowExamID)) FellowExamExist = TRUE;

			// #
			s.Format(_T("%i"), pExamHeader->m_Number);
			if (FellowExamExist) 	s = s + " .";
			m_ExamHeadersList.InsertItem(k, s);

			if (pExamHeader->m_Type == EXAM_TYPE_CT)
			{
				int t = m_ExamHeadersList.GetItemCount();
				if (NumImages == 2) m_ExamHeadersList.m_ColorNum[t - 1] = 1;
				else m_ExamHeadersList.m_ColorNum[t - 1] = 0;
			}

			// date
			m_ExamHeadersList.SetItemText(k, 1, ::DateToStr(pExamHeader->m_Year, pExamHeader->m_Month, pExamHeader->m_Day));

			// time
			s.Format(_T("%02i:%02i:%02i"), pExamHeader->m_Hour, pExamHeader->m_Minute, pExamHeader->m_Second);
			m_ExamHeadersList.SetItemText(k, 2, s);

			// software version
			m_ExamHeadersList.SetItemText(k, 3, LPCTSTR(pExamHeader->SoftwareVersion()));

			// mode
			m_ExamHeadersList.SetItemText(k, 4, LPCTSTR(pExamHeader->Mode()));

			// clinic
			CClinic* pClinic = ::DB.GetClinic(pExamHeader->m_ClinicID);
			if (pClinic) m_ExamHeadersList.SetItemText(k, 5, pClinic->m_Name);

			// physician
			CPhysician* pPhysician = ::DB.GetPhysician(pExamHeader->m_PhysicianID);
			if (pPhysician) m_ExamHeadersList.SetItemText(k, 6, pPhysician->FullName());

			// operator
			COperator* pOperator = ::DB.GetOperator(pExamHeader->m_OperatorID);
			if (pOperator) m_ExamHeadersList.SetItemText(k, 7, pOperator->FullName());

			// preop
			m_ExamHeadersList.SetItemText(k, 8, (LPCTSTR)pExamHeader->Preop());

			// series number
			s.Format(_T("%i"), pExamHeader->m_SeriesNumber);
			m_ExamHeadersList.SetItemText(k, 9, s);

			//[5.1.1]
			if (m_Type == EXAM_TYPE_WF)
			{
				CWFExam *CurWFExam = new CWFExam;
				::DB.LoadWFExam(pExamHeader->m_ExamID, CurWFExam);
				s.Format(_T("%.2f"), CurWFExam->m_ScanDiameter * 0.001);
				s = s + " mm";
				//m_ScanDiameter[e] = CurWFExam->m_ScanDiameter;
				delete CurWFExam;
			}
			else s = "";
			m_ExamHeadersList.SetItemText(k, 10, s);
			//[5.1.1]


			// For Depth of Focus Comparison, shows eye type(OD and OS) in the list
			if (m_Type == EXAM_TYPE_WF)
			{
				if (ISOD(pExamHeader->m_Eye)) s = "OD";
				else s = "OS";

				m_ExamHeadersList.SetItemText(k, 11, s);
			}
			// For Depth of Focus Comparison, shows eye type(OD and OS) in the list Done

			// note
			m_ExamHeadersList.SetItemText(k, 12, pExamHeader->m_Note);//[5.1.1]
																	  //m_ExamHeadersList.SetItemText(k, 10, pExamHeader->m_Note);



			m_ExamHeadersList.SetItemData(k, e);

			k++;
		}
	}

	int bl = intRound(0.5 * dw - (0.5 * d + bw));
	int bt = (dh - th) - (d + bh);
	m_CancelButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);
	bl = intRound(0.5 * dw + 0.5 * d);
	m_OKButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);

	//520 Eye image preview
	RECT ImgInfoRect;
	::SetRect(&ImgInfoRect, 0, 0, 312, 234);
	m_ExamImgInfo.CreateWnd(ImgInfoRect, this);
	m_ExamImgInfo.ShowWindow(FALSE);
	//520 Eye image preview

	m_ExamHeadersList.SetFocus();

	return FALSE;
}

//***************************************************************************************

void CSelectExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

//***************************************************************************************

void CSelectExamDlg::OnExamsListDblClicked(NMHDR* pNMHDR, LRESULT* pRes)
{
	*pRes = 0;

	m_ExamImgInfo.ShowWindow(FALSE);
	OnOK();
}

//***************************************************************************************

void CSelectExamDlg::OnOK()
{
	m_ExamImgInfo.ShowWindow(FALSE);

	if (m_ExamHeadersList.GetSelectionMark() == -1) return;

	CDialog::OnOK();


	
	m_SelNum = 1;
	int n = m_ExamHeadersList.GetSelectedCount();//*****************
	if (n > 1)
	{
		if (m_Type == EXAM_TYPE_WF)
		{
			for (int i = 0; i < NUM_EXAMS_MAX; i++) m_ExamIDs[i] = GUID_NULL;

			POSITION Pos = m_ExamHeadersList.GetFirstSelectedItemPosition();
			int t = 0;
			while (Pos)
			{
				int p = (int)m_ExamHeadersList.GetItemData(m_ExamHeadersList.GetNextSelectedItem(Pos));
				m_ExamIDs[t] = ::DB.m_ExamHeaders[p].m_ExamID;
				t++;
			}
			m_ExamID = ::DB.m_ExamHeaders[t - 1].m_ExamID;
			m_SelNum = t;
		}
		else
		{
			::Info("Please select only on CT exam");
			return;
		}
	}
	else
	{
		//Done
		int i = (int)m_ExamHeadersList.GetItemData(m_ExamHeadersList.GetSelectionMark());
		m_ExamID = ::DB.m_ExamHeaders[i].m_ExamID;
	}
}
//***************************************************************************************
//520
void CSelectExamDlg::OnExamsListHOVER(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	CPoint point(GetCurrentMessage()->pt);

	int left = point.x - m_EXAM_List_Left;
	if (left > m_EXAM_List_ColW0)
	{
		m_ExamImgInfo.ShowWindow(FALSE);
		return;
	}

	m_ExamHeadersList.ScreenToClient(&point);
	int Row = m_ExamHeadersList.HitTest(point);

	if (Row == -1) { m_ExamImgInfo.ShowWindow(FALSE); return; }

	BOOL visible = m_ExamImgInfo.IsWindowVisible();

	if (m_ExamImgInfo.IsWindowVisible())
	{
		if (m_LastRow == Row) return;
		else
		{
			m_LastRow = Row;
		}
	}
	else
	{
		m_LastRow = Row;
		m_ExamImgInfo.ShowWindow(FALSE);
	}


	int e = m_ExamHeadersList.GetItemData(Row);
	int Infox = point.x + 20;
	int Infoy = point.y + 40;

	if (point.y > intRound(0.5*m_EXAM_List_Hight))
	{
		Infoy = point.y - 254;
	}

	if (m_Type == 1)
	{
		CWFExam* pWFExam = new CWFExam;
		if (!::DB.LoadWFExam(::DB.m_ExamHeaders[e].m_ExamID, pWFExam))
		{
			m_ExamImgInfo.ShowWindow(FALSE);
			delete pWFExam;
			return;
		}
		else
		{
			m_ExamImgInfo.MoveWindow(Infox, Infoy, 312, 234, 1);

			if (pWFExam->m_Image.m_RGBData.GetMem() == NULL)
			{
				m_ExamImgInfo.ShowWindow(FALSE);
				delete pWFExam;
				return;
			}

			real_t ratioW = real_t(pWFExam->m_Image.m_w) / real_t(m_ExamImgInfo.m_w);

			for (int y = 0; y < m_ExamImgInfo.m_h; y++)
			{
				for (int x = 0; x < m_ExamImgInfo.m_w; x++)
				{
					int InputI = intRound(x*ratioW);
					int InputJ = intRound(y*ratioW);

					if (InputI >= pWFExam->m_Image.m_w) InputI = pWFExam->m_Image.m_w - 1;
					if (InputJ >= pWFExam->m_Image.m_h) InputJ = pWFExam->m_Image.m_h - 1;


					int R = pWFExam->m_Image.GetRAt(InputI, InputJ);
					/*int G = pWFExam->m_Image.GetGAt(InputI,InputJ);
					int B = pWFExam->m_Image.GetBAt(InputI,InputJ);*/

					m_ExamImgInfo.m_MemDC.SetPixel(x, m_ExamImgInfo.m_h - y - 1, R, R, R);
				}
			}

			m_ExamImgInfo.ShowWindow(TRUE);
			m_ExamImgInfo.Invalidate(FALSE);
		}
		delete pWFExam;
	}
	else
	{
		CCTExam* pCTExam = new CCTExam;
		if (!::DB.LoadCTExam(::DB.m_ExamHeaders[e].m_ExamID, pCTExam))
		{
			m_ExamImgInfo.ShowWindow(FALSE);
			delete pCTExam;
			return;
		}
		else
		{
			m_ExamImgInfo.MoveWindow(Infox, Infoy, 312, 234, 1);

			if (pCTExam->m_Image.m_RGBData.GetMem() == NULL)
			{
				m_ExamImgInfo.ShowWindow(FALSE);
				delete pCTExam;
				return;
			}

			//m_ExamImgInfo.ClearMemDC();
			real_t ratioW = real_t(pCTExam->m_Image.m_w) / real_t(m_ExamImgInfo.m_w);

			for (int y = 0; y < m_ExamImgInfo.m_h; y++)
			{
				for (int x = 0; x < m_ExamImgInfo.m_w; x++)
				{
					int InputI = intRound(x*ratioW);
					int InputJ = intRound(y*ratioW);

					if (InputI >= pCTExam->m_Image.m_w) InputI = pCTExam->m_Image.m_w - 1;
					if (InputJ >= pCTExam->m_Image.m_h) InputJ = pCTExam->m_Image.m_h - 1;


					int R = pCTExam->m_Image.GetRAt(InputI, InputJ);
					/*int G = pCTExam->m_Image.GetGAt(InputI,InputJ);
					int B = pCTExam->m_Image.GetBAt(InputI,InputJ);*/

					m_ExamImgInfo.m_MemDC.SetPixel(x, m_ExamImgInfo.m_h - y - 1, R, R, R);
				}
			}

			m_ExamImgInfo.ShowWindow(TRUE);
			m_ExamImgInfo.Invalidate(FALSE);
		}
		delete pCTExam;
	}
}
//520

//***************************************************************************************

//520
void CSelectExamDlg::OnMouseMove(uint nFlags, CPoint Point)
{
	m_ExamImgInfo.ShowWindow(FALSE);
}
//520
//***************************************************************************************
