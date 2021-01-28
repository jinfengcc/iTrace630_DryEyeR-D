// averaged wf exam save
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ExamAveDlg.h"
#include "ClinicDlg.h"
#include "PhysicianDlg.h"
#include "OperatorDlg.h"
#include "Settings.h"
#include "PasswordDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CExamAveDlg, CDialog)

	ON_BN_CLICKED(IDC_NEW_CLINIC, OnNewClinicButtonClicked)
	ON_BN_CLICKED(IDC_EDIT_CLINIC, OnEditClinicButtonClicked)
	ON_BN_CLICKED(IDC_NEW_PHYSICIAN, OnNewPhysicianButtonClicked)
	ON_BN_CLICKED(IDC_EDIT_PHYSICIAN, OnEditPhysicianButtonClicked)
	ON_BN_CLICKED(IDC_NEW_OPERATOR, OnNewOperatorButtonClicked)
	ON_BN_CLICKED(IDC_EDIT_OPERATOR, OnEditOperatorButtonClicked)
	ON_BN_CLICKED(IDC_VIDEO_LEFT_BTN, OnLeftButtonClicked)
	ON_BN_CLICKED(IDC_VIDEO_RIGHT_BTN, OnRightButtonClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CExamAveDlg::CExamAveDlg(CWnd* pParentWnd, CWFExam* pExams[NUM_EXAMS_MAX], int SelNum) :
	CDialog(IDD_EXAM_AVE_DLG, pParentWnd)
{
	m_SelNum = SelNum;

	for (int i = 0; i < m_SelNum; i++)
	{
		m_pExam[i] = pExams[i];
		int a = 0;
	}

	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));
}

//***************************************************************************************

CExamAveDlg::~CExamAveDlg()
{
	DestroyAcceleratorTable(m_hAccel);
}

//***************************************************************************************

BOOL CExamAveDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 0x45 && GetKeyState(VK_CONTROL) < 0) {
		if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

void CExamAveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PRE_OP_RADIO, m_PreOpRadio);
	DDX_Control(pDX, IDC_POST_OP_RADIO, m_PostOpRadio);
	DDX_Control(pDX, IDC_UNSPECIFIED_RADIO, m_UnspecifiedRadio);
	DDX_Control(pDX, IDC_CLINICS_COMBOBOX, m_ClinicsComboBox);//1
	DDX_Control(pDX, IDC_PHYSICIANS_COMBOBOX, m_PhysiciansComboBox);//1
	DDX_Control(pDX, IDC_OPERATORS_COMBOBOX, m_OperatorsComboBox);//1
	DDX_Control(pDX, IDC_NOTE_EDIT, m_NoteEdit);//1  
}

//***************************************************************************************

BOOL CExamAveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (m_pExam[0]->m_Header.m_Preop)
	{
	case PREOP_YES:
		m_PreOpRadio.SetCheck(1);
		break;
	case PREOP_NO:
		m_PostOpRadio.SetCheck(1);
		break;
	default:
		m_UnspecifiedRadio.SetCheck(1);
	}

	if (m_pExam[0]->m_Header.m_Saved) {
		CreateClinicsList(m_pExam[0]->m_Header.m_ClinicID);
		CreatePhysiciansList(m_pExam[0]->m_Header.m_PhysicianID);
		CreateOperatorsList(m_pExam[0]->m_Header.m_OperatorID);
	}
	else {
		CreateClinicsList(::Settings.m_ClinicID);
		CreatePhysiciansList(::Settings.m_PhysicianID);
		CreateOperatorsList(::Settings.m_OperatorID);
	}

	CString s;

	m_NoteEdit.SetWindowText(_T("Averaged Exam"));
	m_NoteEdit.SetFocus();


	BOOL IsDBReadOnly = ::DB.IsReadOnly();
	GetDlgItem(IDOK)->EnableWindow(!IsDBReadOnly);
	GetDlgItem(IDC_NEW_CLINIC)->EnableWindow(!IsDBReadOnly);
	GetDlgItem(IDC_NEW_PHYSICIAN)->EnableWindow(!IsDBReadOnly);
	GetDlgItem(IDC_NEW_OPERATOR)->EnableWindow(!IsDBReadOnly);

	SetWindowText(_T("Averaged Exam"));

	::TempSettings.Temp_Deleted = 0;//[cjf***05082012]

									//Left and right arrow
	RECT Rect;
	this->GetWindowRect(&Rect);
	int W = Rect.right - Rect.left;
	int H = Rect.bottom - Rect.top;

	::SetRect(&Rect, intRound(0.1*W), intRound(0.8*H), intRound(0.2*W), intRound(0.85*H));
	m_LeftButton.Create(_T(""), _T(""), 0, Rect, this, IDC_VIDEO_LEFT_BTN);
	m_LeftButton.SetImage(W == 1920 ? IDR_LEFT_ARROW_3 : W == 1600 ? IDR_LEFT_ARROW_2 : IDR_LEFT_ARROW_1);
	m_LeftButton.SetBk(FALSE);
	m_LeftButton.SetEnabled(FALSE);
	m_LeftButton.SetTip("Previous eye image");
	m_LeftButton.ShowWindow(SW_SHOW);

	::SetRect(&Rect, intRound(0.8*W), intRound(0.8*H), intRound(0.9*W), intRound(0.85*H));
	m_RightButton.Create(_T(""), _T(""), 0, Rect, this, IDC_VIDEO_RIGHT_BTN);
	m_RightButton.SetImage(W == 1920 ? IDR_RIGHT_ARROW_3 : W == 1600 ? IDR_RIGHT_ARROW_2 : IDR_RIGHT_ARROW_1);
	m_RightButton.SetBk(FALSE);
	m_RightButton.SetEnabled(TRUE);
	m_RightButton.SetTip("Next eye image");
	m_RightButton.ShowWindow(SW_SHOW);
	//Done

	//Eye image
	m_CurEye = 0;

	RECT Rect0;
	GetDlgItem(IDC_STATIC6)->GetWindowRect(&Rect0);
	RECT Rect1 = { intRound(0.25*W), Rect0.bottom , intRound(0.75*W), intRound(0.85*H) };
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

void CExamAveDlg::OnNewClinicButtonClicked()
{
	::TempSettings.Temp_m_Edit = 0;//[cjf***05082012]
	CClinic NewClinic;
	CClinicDlg* pDlg = new CClinicDlg(this, &NewClinic);
	if (pDlg->DoModal() == IDOK) {
		::DB.SaveClinic(NewClinic);
		CreateClinicsList(NewClinic.m_ClinicID);
	}
	delete pDlg;
}

void CExamAveDlg::OnEditClinicButtonClicked()
{
	::TempSettings.Temp_m_Edit = 1;//[cjf***05082012]  
	int i = m_ClinicsComboBox.GetCurSel(); if (i < 1) return;
	CClinic EditClinic;
	EditClinic = ::DB.m_Clinics[i - 1];
	CClinicDlg* pDlg = new CClinicDlg(this, &EditClinic);
	if (pDlg->DoModal() == IDOK) {
		::DB.SaveClinic(EditClinic);
		CreateClinicsList(EditClinic.m_ClinicID);
	}

	if (::TempSettings.Temp_Deleted) CreateClinicsList(GUID_NULL); //[cjf***05082012]

	delete pDlg;
}

//***************************************************************************************

void CExamAveDlg::OnNewPhysicianButtonClicked()
{
	::TempSettings.Temp_m_Edit = 0;//[cjf***05082012]
	CPhysician NewPhysician;
	CPhysicianDlg* pDlg = new CPhysicianDlg(this, &NewPhysician);
	if (pDlg->DoModal() == IDOK) {
		::DB.SavePhysician(NewPhysician);
		CreatePhysiciansList(NewPhysician.m_PhysicianID);
	}
	delete pDlg;
}

void CExamAveDlg::OnEditPhysicianButtonClicked()
{
	::TempSettings.Temp_m_Edit = 1;//[cjf***05082012]
	int i = m_PhysiciansComboBox.GetCurSel(); if (i < 1) return;
	CPhysician EditPhysician;
	EditPhysician = ::DB.m_Physicians[i - 1];
	CPhysicianDlg* pDlg = new CPhysicianDlg(this, &EditPhysician);
	if (pDlg->DoModal() == IDOK) {
		::DB.SavePhysician(EditPhysician);
		CreatePhysiciansList(EditPhysician.m_PhysicianID);
	}
	delete pDlg;
}

//***************************************************************************************

void CExamAveDlg::OnNewOperatorButtonClicked()
{
	::TempSettings.Temp_m_Edit = 0;//[cjf***05082012]
	COperator NewOperator;
	COperatorDlg* pDlg = new COperatorDlg(this, &NewOperator);
	if (pDlg->DoModal() == IDOK) {
		::DB.SaveOperator(NewOperator);
		CreateOperatorsList(NewOperator.m_OperatorID);
	}
	delete pDlg;
}

void CExamAveDlg::OnEditOperatorButtonClicked()
{
	::TempSettings.Temp_m_Edit = 1;//[cjf***05082012]
	int i = m_OperatorsComboBox.GetCurSel(); if (i < 1) return;
	COperator EditOperator;
	EditOperator = ::DB.m_Operators[i - 1];
	COperatorDlg* pDlg = new COperatorDlg(this, &EditOperator);
	if (pDlg->DoModal() == IDOK) {
		::DB.SaveOperator(EditOperator);
		CreateOperatorsList(EditOperator.m_OperatorID);
	}
	delete pDlg;
}

//***************************************************************************************

void CExamAveDlg::OnOK()
{
	CWFExam* Ave_Exam = new CWFExam;

	//Create the ave Exam
	BOOL SOK = TRUE;
	Matrix<CWFExam*> pWFSeriesExams(m_SelNum);

	for (int s = 0; s < m_SelNum; s++)
	{
		CWFExam* loadExam = new CWFExam;

		if (::DB.LoadWFExam(m_pExam[s]->m_Header.m_ExamID, loadExam))
		{
			loadExam->Process();

			// for average exam 
			CString ExamId, LoadFileName;
			GUIDToStr(loadExam->m_Header.m_ExamID, ExamId);
			LoadFileName = MAIN_DIR + "\\Data\\" + ExamId;

			if (::PathFileExists(LoadFileName))
			{
				CAveExamInfo AveExamInfo;

				AveExamInfo.LoadFromFile(LoadFileName);
				for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
				{
					loadExam->m_WfSurface.m_c[z] = AveExamInfo.m_c[z];
				}
			}
			// for average exam Done

			pWFSeriesExams[s] = loadExam;
		}
		else
		{
			SAFE_DELETE(Ave_Exam);
			SOK = FALSE;
			break;
		}
	}

	if (SOK)
	{
		CAveExamInfo AveExamInfo;//601 06052015 for average exam 

		Ave_Exam->Average(pWFSeriesExams);

		//601 06052015 for average exam 
		for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
		{
			AveExamInfo.m_c[z] = Ave_Exam->m_WfSurface.m_c[z];
		}
		//601 06052015 for average exam 

		Ave_Exam->m_Header.m_Eye = (m_pExam[0]->m_Header.IsOD() ? EYE_OD : EYE_OS);

		::CoCreateGuid(&Ave_Exam->m_Header.m_ExamID);

		if (m_PreOpRadio.GetCheck()) Ave_Exam->m_Header.m_Preop = PREOP_YES;
		else if (m_PostOpRadio.GetCheck()) Ave_Exam->m_Header.m_Preop = PREOP_NO;
		else Ave_Exam->m_Header.m_Preop = PREOP_UNKNOWN;

		int i = m_ClinicsComboBox.GetCurSel();
		if (i < 1 || i > ::DB.m_Clinics.GetSize()) Ave_Exam->m_Header.m_ClinicID = GUID_NULL;
		else Ave_Exam->m_Header.m_ClinicID = ::DB.m_Clinics[i - 1].m_ClinicID;

		i = m_PhysiciansComboBox.GetCurSel();
		if (i < 1 || i > ::DB.m_Physicians.GetSize()) Ave_Exam->m_Header.m_PhysicianID = GUID_NULL;
		else Ave_Exam->m_Header.m_PhysicianID = ::DB.m_Physicians[i - 1].m_PhysicianID;

		i = m_OperatorsComboBox.GetCurSel();
		if (i < 1 || i > ::DB.m_Operators.GetSize()) Ave_Exam->m_Header.m_OperatorID = GUID_NULL;
		else Ave_Exam->m_Header.m_OperatorID = ::DB.m_Operators[i - 1].m_OperatorID;

		m_NoteEdit.GetWindowText(Ave_Exam->m_Header.m_Note);
		Ave_Exam->m_Header.m_Note.Remove((char)10);
		for (int i = 0; i < Ave_Exam->m_Header.m_Note.GetLength(); i++)
		{
			if (Ave_Exam->m_Header.m_Note.GetAt(i) == (char)13)
			{
				Ave_Exam->m_Header.m_Note.SetAt(i, ' ');
			}
		}

		Ave_Exam->m_Header.m_Mode = EXAM_MODE_MANUAL;

		//Eye Image
		Ave_Exam->m_Image = m_pExam[m_CurEye]->m_Image;
		//

		//531 Point
		Ave_Exam->m_Points = m_pExam[m_CurEye]->m_Points;
		Ave_Exam->m_DetectorType = DT_LINEAR_ARRAYS;
		//531

		::Settings.m_ClinicID = Ave_Exam->m_Header.m_ClinicID;
		::Settings.m_PhysicianID = Ave_Exam->m_Header.m_PhysicianID;
		::Settings.m_OperatorID = Ave_Exam->m_Header.m_OperatorID;

		//Create the ave Exam
		::DB.SaveWFExam(Ave_Exam);

		//601 06052015 for average exam 
		CString s, SavedFileName;
		GUIDToStr(Ave_Exam->m_Header.m_ExamID, s);
		SavedFileName = MAIN_DIR + "\\Data\\" + s;
		AveExamInfo.SaveIntoFile(SavedFileName);
		//601 06052015 for average exam 	
	}
	else
		::Info("Averaged exam creating is failed");

	SAFE_DELETE(Ave_Exam);
	//Done

	CDialog::OnOK();
}

//***************************************************************************************
void CExamAveDlg::CreateClinicsList(const GUID& ClinicID)
{
	::DB.LoadClinics();
	m_ClinicsComboBox.ResetContent();
	m_ClinicsComboBox.AddString(_T(""));
	int k = 0;
	int pos = 0;
	for (int i = 0; i < ::DB.m_Clinics.GetSize(); i++)
	{
		m_ClinicsComboBox.AddString(::DB.m_Clinics[i].m_Name);
		if (::DB.m_Clinics[i].m_ClinicID == ClinicID) k = i + 1;
		if (::DB.m_Clinics[i].m_ClinicID == ::Settings.m_ClinicID) pos = i + 1;
	}

	//[cjf***04182012], if no ClinicID match to it, set the default clinic as the last clinic
	if (k == 0) k = pos;
	m_ClinicsComboBox.SetCurSel(k);
}

//***************************************************************************************

void CExamAveDlg::CreatePhysiciansList(const GUID& PhysicianID)
{
	::DB.LoadPhysicians();
	m_PhysiciansComboBox.ResetContent();
	m_PhysiciansComboBox.AddString(_T(""));
	int k = 0;
	int pos = 0;
	for (int i = 0; i < ::DB.m_Physicians.GetSize(); i++) {
		m_PhysiciansComboBox.AddString(::DB.m_Physicians[i].FullName());
		if (::DB.m_Physicians[i].m_PhysicianID == PhysicianID) k = i + 1;
		if (::DB.m_Physicians[i].m_PhysicianID == ::Settings.m_PhysicianID) pos = i + 1;
	}

	//[cjf***04182012], if no PhysicianID match to it, set the default Physician as the last Physician
	if (k == 0)  k = pos;

	m_PhysiciansComboBox.SetCurSel(k);
}

//***************************************************************************************

void CExamAveDlg::CreateOperatorsList(const GUID& OperatorID)
{
	::DB.LoadOperators();
	m_OperatorsComboBox.ResetContent();
	m_OperatorsComboBox.AddString(_T(""));
	int k = 0;
	int pos = 0;
	for (int i = 0; i < ::DB.m_Operators.GetSize(); i++) {
		m_OperatorsComboBox.AddString(::DB.m_Operators[i].FullName());
		if (::DB.m_Operators[i].m_OperatorID == OperatorID) k = i + 1;
		if (::DB.m_Operators[i].m_OperatorID == ::Settings.m_OperatorID) pos = i + 1;
	}

	//[cjf***04182012], if no OperatorID match to it, set the default Operator as the last Operator
	if (k == 0)  k = pos;

	m_OperatorsComboBox.SetCurSel(k);
}

//***************************************************************************************

void CExamAveDlg::OnLeftButtonClicked()
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

void CExamAveDlg::OnRightButtonClicked()
{
	m_CurEye++;

	if (!m_LeftButton.IsEnabled())
	{
		m_LeftButton.SetEnabled(TRUE);
		m_LeftButton.Repaint();
	}

	if (m_CurEye >= m_SelNum - 1)
	{
		m_RightButton.SetEnabled(FALSE);
		m_RightButton.Repaint();
	}

	//Change the image
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