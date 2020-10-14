//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "SelectTwoExamDlg.h"
#include "Data.h"
#include "Settings.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CSelectTwoExamDlg, CDialog)

	ON_WM_MOUSEMOVE()//new

	ON_NOTIFY(NM_CLICK, IDC_WFEXAMS_LIST, OnListClicked)
	ON_NOTIFY(NM_CLICK, IDC_CTEXAMS_LIST, OnListClicked)

	ON_NOTIFY(NM_RCLICK, IDC_WFEXAMS_LIST, OnListRClicked)
	ON_NOTIFY(NM_RCLICK, IDC_CTEXAMS_LIST, OnListRClicked)

	ON_NOTIFY(NM_DBLCLK, IDC_WFEXAMS_LIST, OnListDblClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_CTEXAMS_LIST, OnListDblClicked)

	ON_NOTIFY(LVN_HOTTRACK, IDC_WFEXAMS_LIST, OnExamsListHOVER)//new
	ON_NOTIFY(LVN_HOTTRACK, IDC_CTEXAMS_LIST, OnExamsListHOVER)//new

END_MESSAGE_MAP()

//***************************************************************************************

CSelectTwoExamDlg::CSelectTwoExamDlg(CWnd* pParentWnd, GUID& PatientID, int Eye, CMFont* pFont, int WFn, int CTn, real ScanSize[NUM_EXAMS_MAX], int NumImages[NUM_EXAMS_MAX], CString  FellowIDStr[NUM_EXAMS_MAX]) :
	CDialog(IDD_SELECT_TWO_EXAM_DLG, pParentWnd)
{
	::DB.LoadExamHeaders(PatientID);//Big Database Research since 03062019
	//::DB.LoadTxtExamHeaders(PatientID);//Big Database Research since 03062019

	m_Eye = Eye;
	m_pFont = pFont;
	m_WFExamID = GUID_NULL;
	m_CTExamID = GUID_NULL;

	m_WFn = WFn;
	m_CTn = CTn;

	m_Type = 0;
	if (m_WFn == 1)      m_Type = 1;//Only one fellow WF exam
	else if (m_CTn == 1) m_Type = 2;//Only one fellow CT exam

	for (int i = 0; i < NUM_EXAMS_MAX; i++)
	{
		m_FellowIDStr[i] = FellowIDStr[i];
		m_ScanSize[i] = ScanSize[i];
		m_NumImages[i] = NumImages[i];
	}
}

//***************************************************************************************

BOOL CSelectTwoExamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString odos = "OD";
	if (ISOD(m_Eye))
		SetWindowText(_T("Please select fellow OD WF & CT exams"));
	else {
		SetWindowText(_T("Please select fellow OS WF & CT exams"));
		odos = "OS";
	}

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
	lh = intRound(lh / 2);
	int TitlexPos = intRound(lw / 2) + 2 * d;

	SetWindowPos(NULL, 0, 0, dw, dh, SWP_NOZORDER);
	CenterWindow();

	m_EXAM_List_Left = d + intRound((W - dw) / 2);

	int DlgTop = intRound((H - dh) / 2);

	RECT Rect;

	CMDC DC;
	DC.CreateCompatibleDC(NULL);

	//Title
	::SetRect(&Rect, d, d, TitlexPos, 5 * d);
	m_textWF.Create(odos + " WF", 2, Rect, this, BLACK);
	m_textWF.ShowWindow(SW_SHOW);

	::SetRect(&Rect, d, d + lh, TitlexPos, 5 * d + lh);
	m_textCT.Create(odos + " CT", 2, Rect, this, BLACK);
	m_textCT.ShowWindow(SW_SHOW);
	//Done

	int cw[12];

	DC.MeasureRect("00     ", Rect, *m_pFont);
	m_EXAM_List_ColW0 = cw[0] = Rect.right - Rect.left + 3;


	DC.MeasureRect("0000-00-00  ", Rect, *m_pFont);
	cw[1] = Rect.right - Rect.left + 3;

	DC.MeasureRect("00:00:00  ", Rect, *m_pFont);
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

	DC.MeasureRect("0.00 mm  ", Rect, *m_pFont);
	cw[10] = Rect.right - Rect.left + 3;

	int sw = intRound(0.020 * W); // scrollbar width ???

	cw[11] = lw - sw - (cw[0] + cw[1] + cw[2] + cw[3] + cw[4] + cw[5] + cw[6] + cw[7] + cw[8] + cw[9] + cw[10]);

	//WF list
	::SetRect(&Rect, d, 5 * d, d + lw, lh);

	m_WFExamHeadersList.SetFont(m_pFont);

	m_WFExamHeadersList.CreateWnd(WS_VISIBLE | LVS_SINGLESEL | LVS_NOSORTHEADER, Rect, this, IDC_WFEXAMS_LIST);

	m_WFExamHeadersList.InsertColumn(0, _T("#"), intRound(cw[0]));
	m_WFExamHeadersList.InsertColumn(1, _T("Date"), intRound(cw[1]));
	m_WFExamHeadersList.InsertColumn(2, _T("Time"), intRound(cw[2]));
	m_WFExamHeadersList.InsertColumn(3, _T("Ver"), intRound(cw[3]));
	m_WFExamHeadersList.InsertColumn(4, _T("Mode"), intRound(cw[4]));
	m_WFExamHeadersList.InsertColumn(5, _T("Clinic"), intRound(cw[5]));
	m_WFExamHeadersList.InsertColumn(6, _T("Physician"), intRound(cw[6]));
	m_WFExamHeadersList.InsertColumn(7, _T("Operator"), intRound(cw[7]));
	m_WFExamHeadersList.InsertColumn(8, _T("Preop"), intRound(cw[8]));
	m_WFExamHeadersList.InsertColumn(9, _T("Srs"), intRound(cw[9]));
	m_WFExamHeadersList.InsertColumn(10, _T("Scan"), intRound(cw[10]));
	m_WFExamHeadersList.InsertColumn(11, _T("Note"), intRound(cw[11]));

	//CT list
	cw[11] += cw[10];
	cw[10] = 0;

	::SetRect(&Rect, d, 5 * d + lh, d + lw, 2 * lh);

	m_EXAM_CTListTop = 5 * d + lh;

	m_CTExamHeadersList.SetFont(m_pFont);

	m_CTExamHeadersList.CreateWnd(WS_VISIBLE | LVS_SINGLESEL | LVS_NOSORTHEADER, Rect, this, IDC_CTEXAMS_LIST);

	m_CTExamHeadersList.InsertColumn(0, _T("#"), intRound(cw[0]));
	m_CTExamHeadersList.InsertColumn(1, _T("Date"), intRound(cw[1]));
	m_CTExamHeadersList.InsertColumn(2, _T("Time"), intRound(cw[2]));
	m_CTExamHeadersList.InsertColumn(3, _T("Ver"), intRound(cw[3]));
	m_CTExamHeadersList.InsertColumn(4, _T("Mode"), intRound(cw[4]));
	m_CTExamHeadersList.InsertColumn(5, _T("Clinic"), intRound(cw[5]));
	m_CTExamHeadersList.InsertColumn(6, _T("Physician"), intRound(cw[6]));
	m_CTExamHeadersList.InsertColumn(7, _T("Operator"), intRound(cw[7]));
	m_CTExamHeadersList.InsertColumn(8, _T("Preop"), intRound(cw[8]));
	m_CTExamHeadersList.InsertColumn(9, _T("Srs"), intRound(cw[9]));
	m_CTExamHeadersList.InsertColumn(10, _T("Scan"), intRound(cw[10]));
	m_CTExamHeadersList.InsertColumn(11, _T("Note"), intRound(cw[11]));
	//CT list Done

	CString s;

	int k1 = 0;
	int k2 = 0;

	for (int e = 0; e < ::DB.m_ExamHeaders.GetSize(); e++)
	{

		CExamHeader* pExamHeader = &::DB.m_ExamHeaders[e];

		if ((pExamHeader->m_Type == EXAM_TYPE_WF) && ((ISOD(pExamHeader->m_Eye) && ISOD(m_Eye)) || (ISOS(pExamHeader->m_Eye) && ISOS(m_Eye))))
		{

			// #
			if (m_NumImages[e] == 2) m_WFExamHeadersList.m_ColorNum[k1] = 1;
			else                    m_WFExamHeadersList.m_ColorNum[k1] = 0;

			s.Format(_T("%i"), pExamHeader->m_Number);
			s = s + m_FellowIDStr[e];
			m_WFExamHeadersList.InsertItem(k1, s);

			// date
			m_WFExamHeadersList.SetItemText(k1, 1, ::DateToStr(pExamHeader->m_Year, pExamHeader->m_Month, pExamHeader->m_Day));

			// time
			s.Format(_T("%02i:%02i:%02i"), pExamHeader->m_Hour, pExamHeader->m_Minute, pExamHeader->m_Second);
			m_WFExamHeadersList.SetItemText(k1, 2, s);

			// software version
			m_WFExamHeadersList.SetItemText(k1, 3, (LPCTSTR)pExamHeader->SoftwareVersion());

			// mode
			m_WFExamHeadersList.SetItemText(k1, 4, (LPCTSTR)pExamHeader->Mode());

			// clinic
			CClinic* pClinic = ::DB.GetClinic(pExamHeader->m_ClinicID);
			if (pClinic) m_WFExamHeadersList.SetItemText(k1, 5, pClinic->m_Name);

			// physician
			CPhysician* pPhysician = ::DB.GetPhysician(pExamHeader->m_PhysicianID);
			if (pPhysician) m_WFExamHeadersList.SetItemText(k1, 6, pPhysician->FullName());

			// operator
			COperator* pOperator = ::DB.GetOperator(pExamHeader->m_OperatorID);
			if (pOperator) m_WFExamHeadersList.SetItemText(k1, 7, pOperator->FullName());

			// preop
			m_WFExamHeadersList.SetItemText(k1, 8, (LPCTSTR)pExamHeader->Preop());

			// series number
			s.Format(_T("%i"), pExamHeader->m_SeriesNumber);
			m_WFExamHeadersList.SetItemText(k1, 9, s);

			//scan Size
			s.Format(_T("%.2f"), m_ScanSize[e] * 0.001);
			s = s + " mm";
			m_WFExamHeadersList.SetItemText(k1, 10, s);
			//scan Size

			// note
			m_WFExamHeadersList.SetItemText(k1, 11, pExamHeader->m_Note);//[5.1.1]
																		 //m_ExamHeadersList.SetItemText(k, 10, pExamHeader->m_Note);

			m_WFExamHeadersList.SetItemData(k1, e);

			k1++;
		}

		if ((pExamHeader->m_Type == EXAM_TYPE_CT) && ((ISOD(pExamHeader->m_Eye) && ISOD(m_Eye)) || (ISOS(pExamHeader->m_Eye) && ISOS(m_Eye))))
		{
			// #
			if (m_NumImages[e] == 2) m_CTExamHeadersList.m_ColorNum[k2] = 1;
			else                    m_CTExamHeadersList.m_ColorNum[k2] = 0;

			s.Format(_T("%i"), pExamHeader->m_Number);
			s = s + m_FellowIDStr[e];
			m_CTExamHeadersList.InsertItem(k2, s);

			// date
			m_CTExamHeadersList.SetItemText(k2, 1, ::DateToStr(pExamHeader->m_Year, pExamHeader->m_Month, pExamHeader->m_Day));

			// time
			s.Format(_T("%02i:%02i:%02i"), pExamHeader->m_Hour, pExamHeader->m_Minute, pExamHeader->m_Second);
			m_CTExamHeadersList.SetItemText(k2, 2, s);

			// software version
			m_CTExamHeadersList.SetItemText(k2, 3, (LPCTSTR)pExamHeader->SoftwareVersion());

			// mode
			m_CTExamHeadersList.SetItemText(k2, 4, (LPCTSTR)pExamHeader->Mode());

			// clinic
			CClinic* pClinic = ::DB.GetClinic(pExamHeader->m_ClinicID);
			if (pClinic) m_CTExamHeadersList.SetItemText(k2, 5, pClinic->m_Name);

			// physician
			CPhysician* pPhysician = ::DB.GetPhysician(pExamHeader->m_PhysicianID);
			if (pPhysician) m_CTExamHeadersList.SetItemText(k2, 6, pPhysician->FullName());

			// operator
			COperator* pOperator = ::DB.GetOperator(pExamHeader->m_OperatorID);
			if (pOperator) m_CTExamHeadersList.SetItemText(k2, 7, pOperator->FullName());

			// preop
			m_CTExamHeadersList.SetItemText(k2, 8, (LPCTSTR)pExamHeader->Preop());

			// series number
			s.Format(_T("%i"), pExamHeader->m_SeriesNumber);
			m_CTExamHeadersList.SetItemText(k2, 9, s);
			//[5.1.1]

			// note
			m_CTExamHeadersList.SetItemText(k2, 11, pExamHeader->m_Note);//[5.1.1]
																		 //m_ExamHeadersList.SetItemText(k, 10, pExamHeader->m_Note);

			m_CTExamHeadersList.SetItemData(k2, e);

			k2++;
		}
	}

	//Exam Numbers
	::SetRect(&Rect, TitlexPos, d, d + lw, 5 * d);
	s.Format(_T("%i"), k1);
	if (k1 > 1)  m_textWFNum.Create(s + " Exams", 1, Rect, this, BLACK);
	else        m_textWFNum.Create(s + " Exam", 1, Rect, this, BLACK);
	m_textWFNum.ShowWindow(SW_SHOW);

	::SetRect(&Rect, TitlexPos, d + lh, d + lw, 5 * d + lh);
	s.Format(_T("%i"), k2);
	if (k2 > 1)  m_textCTNum.Create(s + " Exams", 1, Rect, this, BLACK);
	else        m_textCTNum.Create(s + " Exam", 1, Rect, this, BLACK);
	m_textCTNum.ShowWindow(SW_SHOW);
	//Exam Numbers Done

	//Eye image preview
	RECT ImgInfoRect;
	::SetRect(&ImgInfoRect, 0, 0, 312, 234);
	m_ExamImgInfo.CreateWnd(ImgInfoRect, this);
	m_ExamImgInfo.ShowWindow(FALSE);
	//Eye image preview

	int bl = intRound(0.5 * dw - (0.5 * d + bw));
	int bt = (dh - th) - (d + bh);
	m_CancelButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);
	bl = intRound(0.5 * dw + 0.5 * d);
	m_OKButton.SetWindowPos(NULL, bl, bt, bw, bh, SWP_NOZORDER);

	m_WFExamHeadersList.SetFocus();

	if (m_Type == 1)
	{
		m_WFExamHeadersList.SetSelectionMark(0);
		m_WFExamHeadersList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	else if (m_Type == 2)
	{
		m_CTExamHeadersList.SetSelectionMark(0);
		m_CTExamHeadersList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}

	return FALSE;
}

//***************************************************************************************

void CSelectTwoExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

//***************************************************************************************

void CSelectTwoExamDlg::OnOK()
{
	m_ExamImgInfo.ShowWindow(FALSE);

	if (m_WFExamHeadersList.GetSelectionMark() == -1 && m_CTExamHeadersList.GetSelectionMark() == -1) return;

	else if (m_WFExamHeadersList.GetSelectionMark() != -1 && m_CTExamHeadersList.GetSelectionMark() == -1)
	{
		int Row = m_WFExamHeadersList.GetSelectionMark();
		int e = (int)m_WFExamHeadersList.GetItemData(Row);//the position of selected exam in the database
		if (m_FellowIDStr[e] != "")// There is a fellow exam exist
		{
			m_WFExamID = ::DB.m_ExamHeaders[e].m_ExamID;

			CWFExam* pWFExam = new CWFExam;
			if (!::DB.LoadWFExam(m_WFExamID, pWFExam))
			{
				delete pWFExam;
				::Error("Failed to load exam.");
				return;
			}
			m_CTExamID = pWFExam->m_Header.m_FellowExamID;
			delete pWFExam;
			CDialog::OnOK();
		}
		else
		{
			//::Info("Please select one WF and one CT exam, or double click one exem which own the fellow exam.");
			return;
		}
	}

	else if (m_CTExamHeadersList.GetSelectionMark() != -1 && m_WFExamHeadersList.GetSelectionMark() == -1)
	{
		int Row = m_CTExamHeadersList.GetSelectionMark();
		int e = (int)m_CTExamHeadersList.GetItemData(Row);//the position of selected exam in the database
		if (m_FellowIDStr[e] != "")// There is a fellow exam exist
		{
			m_CTExamID = ::DB.m_ExamHeaders[e].m_ExamID;

			CCTExam* pCTExam = new CCTExam;
			if (!::DB.LoadCTExam(m_CTExamID, pCTExam))
			{
				delete pCTExam;
				::Error("Failed to load exam.");
				return;
			}
			m_WFExamID = pCTExam->m_Header.m_FellowExamID;
			delete pCTExam;
			CDialog::OnOK();
		}
		else
		{
			//::Info("Please select one WF and one CT exam, or double click one exem which own the fellow exam.");
			return;
		}
	}

	else
	{
		CDialog::OnOK();

		int i = (int)m_WFExamHeadersList.GetItemData(m_WFExamHeadersList.GetSelectionMark());
		m_WFExamID = ::DB.m_ExamHeaders[i].m_ExamID;

		i = (int)m_CTExamHeadersList.GetItemData(m_CTExamHeadersList.GetSelectionMark());
		m_CTExamID = ::DB.m_ExamHeaders[i].m_ExamID;
	}
}

//***************************************************************************************

void CSelectTwoExamDlg::OnListDblClicked(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	m_ExamImgInfo.ShowWindow(FALSE);
	OnOK();
}

//***************************************************************************************
void CSelectTwoExamDlg::OnListRClicked(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	m_ExamImgInfo.ShowWindow(FALSE);

	*pRes = 0;

	//If there only one fellow WF or CT exam
	if (pNotifyStruct->hwndFrom == m_WFExamHeadersList.m_hWnd && m_Type == 1)
	{
		m_WFExamHeadersList.SetSelectionMark(0);
		m_WFExamHeadersList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		return;
	}
	else if (pNotifyStruct->hwndFrom == m_CTExamHeadersList.m_hWnd && m_Type == 2)
	{
		m_CTExamHeadersList.SetSelectionMark(0);
		m_CTExamHeadersList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		return;
	}
	//Done

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));

	if (pNotifyStruct->hwndFrom == m_WFExamHeadersList.m_hWnd)
	{
		m_WFExamHeadersList.ScreenToClient(&point);
		int Row = m_WFExamHeadersList.HitTest(point);

		if (m_WFExamHeadersList.GetSelectionMark() == Row)
		{
			if (m_CTExamHeadersList.m_FelRow != -1)
			{
				m_CTExamHeadersList.m_FelRow = -1;
				m_CTExamHeadersList.Invalidate(FALSE);
			}
			m_WFExamHeadersList.SetItemState(Row, 0, LVIS_SELECTED | LVIS_FOCUSED);
			m_WFExamHeadersList.SetSelectionMark(-1);

			if (m_CTExamHeadersList.GetSelectionMark() != -1)
			{
				pNotifyStruct->hwndFrom = m_CTExamHeadersList.m_hWnd;
				OnListClicked(pNotifyStruct, pRes);
			}
		}
	}
	else
	{
		m_CTExamHeadersList.ScreenToClient(&point);
		int Row = m_CTExamHeadersList.HitTest(point);
		m_CTExamHeadersList.SetItemState(Row, 0, LVIS_SELECTED | LVIS_FOCUSED);
		if (m_CTExamHeadersList.GetSelectionMark() == Row)
		{
			if (m_WFExamHeadersList.m_FelRow != -1)
			{
				m_WFExamHeadersList.m_FelRow = -1;
				m_WFExamHeadersList.Invalidate(FALSE);
			}
			m_CTExamHeadersList.SetSelectionMark(-1);

			if (m_WFExamHeadersList.GetSelectionMark() != -1)
			{
				pNotifyStruct->hwndFrom = m_WFExamHeadersList.m_hWnd;
				OnListClicked(pNotifyStruct, pRes);
			}
		}
	}
}

//***************************************************************************************
void CSelectTwoExamDlg::OnListClicked(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	m_ExamImgInfo.ShowWindow(FALSE);

	*pRes = 0;

	int WhichExam = 0;
	int Row = -1;
	int Row2 = -1;

	//Check the exem types and which row is selected
	// 1: WF; 2: CT
	if (pNotifyStruct->hwndFrom == m_WFExamHeadersList.m_hWnd)
	{
		WhichExam = 1;
		Row = m_WFExamHeadersList.GetSelectionMark();
		Row2 = m_CTExamHeadersList.GetSelectionMark();
	}
	else if (pNotifyStruct->hwndFrom == m_CTExamHeadersList.m_hWnd)
	{
		WhichExam = 2;
		Row = m_CTExamHeadersList.GetSelectionMark();
		Row2 = m_WFExamHeadersList.GetSelectionMark();
	}

	if (WhichExam == 1)
	{
		int e = (int)m_WFExamHeadersList.GetItemData(Row);//the position of selected exam in the database
		if (m_FellowIDStr[e] != "" && Row2 == -1)
		{
			GUID FellowExamID;

			GUID ExamID = ::DB.m_ExamHeaders[e].m_ExamID;

			CWFExam* pWFExam = new CWFExam;
			if (!::DB.LoadWFExam(ExamID, pWFExam))
			{
				delete pWFExam;
				::Error("Failed to load exam.");
				return;
			}
			FellowExamID = pWFExam->m_Header.m_FellowExamID;
			delete pWFExam;

			int n = m_CTExamHeadersList.GetItemCount();
			for (int i = 0; i < n; i++)
			{
				int e = m_CTExamHeadersList.GetItemData(i);
				if (::DB.m_ExamHeaders[e].m_ExamID == FellowExamID)
				{
					m_CTExamHeadersList.m_FelRow = i;
					//Scoll the fellow item to correct pos
					int nSel = m_CTExamHeadersList.m_FelRow;
					int nItem = m_CTExamHeadersList.GetTopIndex();
					CRect rc;
					m_CTExamHeadersList.GetItemRect(nItem, rc, LVIR_BOUNDS);
					CSize sz(0, (nSel - nItem)*rc.Height());
					m_CTExamHeadersList.Scroll(sz);
					//cjf
					break;
				}
			}
		}
		else m_CTExamHeadersList.m_FelRow = -1;
		m_CTExamHeadersList.Invalidate(FALSE);

		if (m_WFExamHeadersList.m_FelRow != -1)
		{
			m_WFExamHeadersList.m_FelRow = -1;
			m_WFExamHeadersList.Invalidate(FALSE);
		}
	}
	else
	{
		int e = (int)m_CTExamHeadersList.GetItemData(Row);//the position of selected exam in the database
		if (m_FellowIDStr[e] != ""  && Row2 == -1)
		{
			GUID FellowExamID;

			GUID ExamID = ::DB.m_ExamHeaders[e].m_ExamID;

			CCTExam* pCTExam = new CCTExam;
			if (!::DB.LoadCTExam(ExamID, pCTExam))
			{
				delete pCTExam;
				::Error("Failed to load exam.");
				return;
			}
			FellowExamID = pCTExam->m_Header.m_FellowExamID;
			delete pCTExam;

			int n = m_WFExamHeadersList.GetItemCount();
			for (int i = 0; i < n; i++)
			{
				int e = m_WFExamHeadersList.GetItemData(i);
				if (::DB.m_ExamHeaders[e].m_ExamID == FellowExamID)
				{
					m_WFExamHeadersList.m_FelRow = i;

					int nSel = m_WFExamHeadersList.m_FelRow;
					int nItem = m_WFExamHeadersList.GetTopIndex();
					CRect rc;
					m_WFExamHeadersList.GetItemRect(nItem, rc, LVIR_BOUNDS);
					CSize sz(0, (nSel - nItem)*rc.Height());
					m_WFExamHeadersList.Scroll(sz);

					break;
				}
			}
		}
		else m_WFExamHeadersList.m_FelRow = -1;
		m_WFExamHeadersList.Invalidate(FALSE);

		if (m_CTExamHeadersList.m_FelRow != -1)
		{
			m_CTExamHeadersList.m_FelRow = -1;
			m_CTExamHeadersList.Invalidate(FALSE);
		}
	}
	int a = 0;
}


//***************************************************************************************
void CSelectTwoExamDlg::OnExamsListHOVER(NMHDR* pNotifyStruct, LRESULT* pRes)
{
	CPoint point(GetCurrentMessage()->pt);

	int Row = -1;
	int WhichExam = -1;


	if (pNotifyStruct->hwndFrom == m_WFExamHeadersList.m_hWnd)
	{
		WhichExam = 1;

		int left = point.x - m_EXAM_List_Left;
		if (left > m_EXAM_List_ColW0)
		{
			m_ExamImgInfo.ShowWindow(FALSE);
			return;
		}

		m_WFExamHeadersList.ScreenToClient(&point);
		Row = m_WFExamHeadersList.HitTest(point);
	}
	else if (pNotifyStruct->hwndFrom == m_CTExamHeadersList.m_hWnd)
	{
		WhichExam = 2;

		int left = point.x - m_EXAM_List_Left;
		if (left > m_EXAM_List_ColW0)
		{
			m_ExamImgInfo.ShowWindow(FALSE);
			return;
		}

		m_CTExamHeadersList.ScreenToClient(&point);
		Row = m_CTExamHeadersList.HitTest(point);
	}

	if (Row == -1)
	{
		m_ExamImgInfo.ShowWindow(FALSE);
		return;
	}

	BOOL visible = m_ExamImgInfo.IsWindowVisible();

	if (m_ExamImgInfo.IsWindowVisible())
	{
		if (m_LastExam == WhichExam && m_LastRow == Row) return;
		else
		{
			m_LastExam = WhichExam;
			m_LastRow = Row;
		}
	}
	else
	{
		m_LastExam = WhichExam;
		m_LastRow = Row;
		m_ExamImgInfo.ShowWindow(FALSE);
	}


	int e = -1;

	int Infox, Infoy;

	if (WhichExam == 1)
	{
		e = m_WFExamHeadersList.GetItemData(Row);
		Infox = point.x + 20;
		Infoy = point.y + 60;
	}
	else if (WhichExam == 2)
	{
		e = m_CTExamHeadersList.GetItemData(Row);
		Infox = point.x + 20;
		Infoy = point.y - 254 + m_EXAM_CTListTop;
	}
	else { m_ExamImgInfo.ShowWindow(FALSE); return; }


	if (WhichExam == 1)
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

			real ratioW = real(pWFExam->m_Image.m_w) / real(m_ExamImgInfo.m_w);

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
			real ratioW = real(pCTExam->m_Image.m_w) / real(m_ExamImgInfo.m_w);

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
//***************************************************************************************

void CSelectTwoExamDlg::OnMouseMove(uint nFlags, CPoint Point)
{
	m_ExamImgInfo.ShowWindow(FALSE);
}
//***************************************************************************************