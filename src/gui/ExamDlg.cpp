//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ExamDlg.h"
#include "ClinicDlg.h"
#include "PhysicianDlg.h"
#include "OperatorDlg.h"
#include "Settings.h"
#include "PasswordDlg.h"
#include "GlobalFunctions.h"//6.2.0 For adding read chinese function

#include "TranExamDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CExamDlg, CDialog)

	ON_COMMAND(IDK_CTRL_E, OnCtrlE)
	ON_BN_CLICKED(IDC_NEW_CLINIC, OnNewClinicButtonClicked)
	ON_BN_CLICKED(IDC_EDIT_CLINIC, OnEditClinicButtonClicked)
	ON_BN_CLICKED(IDC_NEW_PHYSICIAN, OnNewPhysicianButtonClicked)
	ON_BN_CLICKED(IDC_EDIT_PHYSICIAN, OnEditPhysicianButtonClicked)
	ON_BN_CLICKED(IDC_NEW_OPERATOR, OnNewOperatorButtonClicked)
	ON_BN_CLICKED(IDC_EDIT_OPERATOR, OnEditOperatorButtonClicked)
	ON_BN_CLICKED(IDC_LENS_FLAT_CURVE_RADIO, OnWavetouchLensFlatCurveRadioClicked)
	ON_BN_CLICKED(IDC_LENS_STEEP_CURVE_RADIO, OnWavetouchLensSteepCurveRadioClicked)
	ON_BN_CLICKED(IDC_LENS_OTHER_CURVE_RADIO, OnWavetouchLensOtherCurveRadioClicked)

END_MESSAGE_MAP()

//***************************************************************************************

CExamDlg::CExamDlg(CWnd* pParentWnd, CExam* pExam) :
	CDialog(::Settings.m_ShowWavetouch && pExam->m_Header.m_Type == EXAM_TYPE_WF ? IDD_WF_EXAM_DLG : IDD_EXAM_DLG, pParentWnd)
{
	m_pExam = pExam;

	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));
}

//***************************************************************************************

CExamDlg::~CExamDlg()
{
	DestroyAcceleratorTable(m_hAccel);
}

//***************************************************************************************

BOOL CExamDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 0x45 && GetKeyState(VK_CONTROL) < 0) 
	{
		if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

void CExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PRE_OP_RADIO, m_PreOpRadio);
	DDX_Control(pDX, IDC_POST_OP_RADIO, m_PostOpRadio);
	DDX_Control(pDX, IDC_UNSPECIFIED_RADIO, m_UnspecifiedRadio);
	DDX_Control(pDX, IDC_SERIES_COMBOBOX, m_SeriesComboBox);
	DDX_Control(pDX, IDC_CLINICS_COMBOBOX, m_ClinicsComboBox);
	DDX_Control(pDX, IDC_PHYSICIANS_COMBOBOX, m_PhysiciansComboBox);
	DDX_Control(pDX, IDC_OPERATORS_COMBOBOX, m_OperatorsComboBox);
	DDX_Control(pDX, IDC_NOTE_EDIT, m_NoteEdit);

	if (::Settings.m_ShowWavetouch && m_pExam->m_Header.m_Type == EXAM_TYPE_WF) 
	{
		DDX_Control(pDX, IDC_LENS_POWER_EDIT, m_WavetouchLensPowerEdit);
		DDX_Control(pDX, IDC_LENS_FLAT_CURVE_RADIO, m_WavetouchLensFlatCurveRadio);
		DDX_Control(pDX, IDC_LENS_STEEP_CURVE_RADIO, m_WavetouchLensSteepCurveRadio);
		DDX_Control(pDX, IDC_LENS_OTHER_CURVE_RADIO, m_WavetouchLensOtherCurveRadio);
		DDX_Control(pDX, IDC_OTHER_EDIT, m_WavetouchOtherEdit);
		DDX_Control(pDX, IDC_WT_NOTE_EDIT, m_WavetouchNoteEdit);
		DDX_Control(pDX, IDC_SPH_EDIT, m_WavetouchSphEdit);
		DDX_Control(pDX, IDC_CYL_EDIT, m_WavetouchCylEdit);
		DDX_Control(pDX, IDC_AXIS_EDIT, m_WavetouchAxisEdit);
	}
}

//***************************************************************************************

BOOL CExamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// m_OverrideEyeCheck.SetCheck(m_pExam->m_Header.IsEyeManuallyOverriden());

	switch (m_pExam->m_Header.m_Preop)
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

	if (m_pExam->m_Header.m_Saved)
	{
		CreateClinicsList(m_pExam->m_Header.m_ClinicID);
		CreatePhysiciansList(m_pExam->m_Header.m_PhysicianID);
		CreateOperatorsList(m_pExam->m_Header.m_OperatorID);
	}
	else
	{
		CreateClinicsList(::Settings.m_ClinicID);
		CreatePhysiciansList(::Settings.m_PhysicianID);
		CreateOperatorsList(::Settings.m_OperatorID);
	}

	CString s;
	for (int i = 0; i <= 20; i++)
	{
		s.Format(_T("%i"), i);
		m_SeriesComboBox.AddString(s);
		if (m_pExam->m_Header.m_SeriesNumber == i)
		{
			m_SeriesComboBox.SetCurSel(i);
		}
	}

	m_NoteEdit.SetWindowText(G_ReadChinese(m_pExam->m_Header.m_Note));
	m_NoteEdit.SetFocus();

	if (::Settings.m_ShowWavetouch && m_pExam->m_Header.m_Type == EXAM_TYPE_WF)
	{

		CWFExam* pWFExam = (CWFExam*)m_pExam;
		CString s;

		if (pWFExam->m_WavetouchLensPower == INVALID_VALUE) s = "";
		else s.Format(_T("%.2f"), pWFExam->m_WavetouchLensPower);
		m_WavetouchLensPowerEdit.SetWindowText(s);

		switch (pWFExam->m_WavetouchLensBaseCurve)
		{
		case WAVETOUCH_LENS_BASECURVE_MEDIAN:
			m_WavetouchLensFlatCurveRadio.SetCheck(1);
			break;
		case WAVETOUCH_LENS_BASECURVE_STEEP:
			m_WavetouchLensSteepCurveRadio.SetCheck(1);
			break;
		default:
			m_WavetouchLensOtherCurveRadio.SetCheck(1);
			m_WavetouchOtherEdit.EnableWindow(TRUE);
		}

		m_WavetouchOtherEdit.SetWindowText(pWFExam->m_WavetouchOther);

		m_WavetouchNoteEdit.SetWindowText(pWFExam->m_WavetouchNote);

		if (pWFExam->m_WavetouchSph == INVALID_VALUE)
		{
			s = "";
		}
		else
		{
			s.Format(_T("%.2f"), pWFExam->m_WavetouchSph);
		}
		m_WavetouchSphEdit.SetWindowText(s);

		if (pWFExam->m_WavetouchCyl == INVALID_VALUE)
		{
			s = "";
		}
		else
		{
			s.Format(_T("%.2f"), pWFExam->m_WavetouchCyl);
		}
		m_WavetouchCylEdit.SetWindowText(s);

		if (pWFExam->m_WavetouchAxis == INVALID_VALUE)
		{
			s = "";
		}
		else
		{
			s.Format(_T("%i"), pWFExam->m_WavetouchAxis);
		}
		m_WavetouchAxisEdit.SetWindowText(s);
	}

	BOOL IsDBReadOnly = ::DB.IsReadOnly();
	GetDlgItem(IDOK)->EnableWindow(!IsDBReadOnly);
	GetDlgItem(IDC_NEW_CLINIC)->EnableWindow(!IsDBReadOnly);
	GetDlgItem(IDC_NEW_PHYSICIAN)->EnableWindow(!IsDBReadOnly);
	GetDlgItem(IDC_NEW_OPERATOR)->EnableWindow(!IsDBReadOnly);

	if (m_pExam->m_Header.m_ExamID == GUID_NULL) 
	{
		s = "";
	}
	else
	{
		::GUIDToStr(m_pExam->m_Header.m_ExamID, s);
	}
	SetWindowText(_T("Exam  ") + s);

	::TempSettings.Temp_Deleted = 0;

	return FALSE;
}

//***************************************************************************************

void CExamDlg::OnCtrlE()
{
	CPasswordDlg* pDlg = new CPasswordDlg(this);
	int Res = pDlg->DoModal();
	CString Password = pDlg->m_Password;
	delete pDlg;

	if (Res == IDOK && Password == "tracey") {

		// transfer exams from one patient to another
		CTranExamDlg* pTransExamDlg = new CTranExamDlg(this);
		int ThisRes = pTransExamDlg->DoModal();
		if (ThisRes == IDOK)
		{
			if (pTransExamDlg->m_Res == 0)
			{
				switch (m_pExam->m_Header.m_Eye)
				{
				case EYE_OD:
					if (::Question("Are you sure you want to override the hardware detected setting?", 0, 1))
					{
						m_pExam->m_Header.m_Eye = EYE_OD_OS;
						::Info("The selected exam has been transformed to OS sucesfully.");
						break;
					}
				case EYE_OS:
					if (::Question("Are you sure you want to override the hardware detected setting?", 0, 1))
					{
						m_pExam->m_Header.m_Eye = EYE_OS_OD;
						::Info("The selected exam has been transformed to OD sucesfully.");
					}
					break;
				case EYE_OD_OS:
					if (::Question("Are you sure you want to override the OD/OS setting again?", 0, 1))
					{
						m_pExam->m_Header.m_Eye = EYE_OS_OD;
						::Info("The selected exam has been transformed to OD sucesfully.");
					}
					break;
				case EYE_OS_OD:
					if (::Question("Are you sure you want to override the OD/OS setting again?", 0, 1))
					{
						m_pExam->m_Header.m_Eye = EYE_OD_OS;
						::Info("The selected exam has been transformed to OS sucesfully.");
						break;
					}
					break;
				}
			}
			else if (pTransExamDlg->m_Res == 1)
			{
				if (::Question("Are you sure you want to transform this exam to another patient?", 0, 1))
				{
					CPatient TranformPatient = ::DB.m_Patients[pTransExamDlg->m_P];

					m_pExam->m_Header.m_PatientID = TranformPatient.m_PatientID;
					::DB.SaveExam(m_pExam);

					::Info("The selected exam has been transformed to patient " + TranformPatient.FullName() + " sucesfully.");
				}
			}
		}
		delete pTransExamDlg;
		// 
	}

	//
	//The fellow id problem still need to be handled.
	//
}

//***************************************************************************************

void CExamDlg::OnNewClinicButtonClicked()
{
	::TempSettings.Temp_m_Edit = 0;
	CClinic NewClinic;
	CClinicDlg* pDlg = new CClinicDlg(this, &NewClinic);

	if (pDlg->DoModal() == IDOK)
	{
		::DB.SaveClinic(NewClinic);
		CreateClinicsList(NewClinic.m_ClinicID);
	}
	delete pDlg;
}

//***************************************************************************************

void CExamDlg::OnEditClinicButtonClicked()
{
	::TempSettings.Temp_m_Edit = 1;

	int i = m_ClinicsComboBox.GetCurSel(); if (i < 1) return;
	CClinic EditClinic;
	EditClinic = ::DB.m_Clinics[i - 1];
	CClinicDlg* pDlg = new CClinicDlg(this, &EditClinic);

	if (pDlg->DoModal() == IDOK) 
	{
		::DB.SaveClinic(EditClinic);
		CreateClinicsList(EditClinic.m_ClinicID);
	}

	if (::TempSettings.Temp_Deleted) CreateClinicsList(GUID_NULL);

	delete pDlg;
}

//***************************************************************************************

void CExamDlg::OnNewPhysicianButtonClicked()
{
	::TempSettings.Temp_m_Edit = 0;
	CPhysician NewPhysician;
	CPhysicianDlg* pDlg = new CPhysicianDlg(this, &NewPhysician);

	if (pDlg->DoModal() == IDOK)
	{
		::DB.SavePhysician(NewPhysician);
		CreatePhysiciansList(NewPhysician.m_PhysicianID);
	}
	delete pDlg;
}

//***************************************************************************************

void CExamDlg::OnEditPhysicianButtonClicked()
{
	::TempSettings.Temp_m_Edit = 1;

	int i = m_PhysiciansComboBox.GetCurSel(); if (i < 1) return;

	CPhysician EditPhysician;
	EditPhysician = ::DB.m_Physicians[i - 1];

	CPhysicianDlg* pDlg = new CPhysicianDlg(this, &EditPhysician);

	if (pDlg->DoModal() == IDOK) 
	{
		::DB.SavePhysician(EditPhysician);
		CreatePhysiciansList(EditPhysician.m_PhysicianID);
	}
	delete pDlg;
}

//***************************************************************************************

void CExamDlg::OnNewOperatorButtonClicked()
{
	::TempSettings.Temp_m_Edit = 0;
	COperator NewOperator;
	COperatorDlg* pDlg = new COperatorDlg(this, &NewOperator);

	if (pDlg->DoModal() == IDOK) 
	{
		::DB.SaveOperator(NewOperator);
		CreateOperatorsList(NewOperator.m_OperatorID);
	}
	delete pDlg;
}

//***************************************************************************************

void CExamDlg::OnEditOperatorButtonClicked()
{
	::TempSettings.Temp_m_Edit = 1;
	int i = m_OperatorsComboBox.GetCurSel(); if (i < 1) return;
	COperator EditOperator;
	EditOperator = ::DB.m_Operators[i - 1];
	COperatorDlg* pDlg = new COperatorDlg(this, &EditOperator);

	if (pDlg->DoModal() == IDOK) 
	{
		::DB.SaveOperator(EditOperator);
		CreateOperatorsList(EditOperator.m_OperatorID);
	}
	delete pDlg;
}

//***************************************************************************************

void CExamDlg::OnOK()
{
	if (m_PreOpRadio.GetCheck()) m_pExam->m_Header.m_Preop = PREOP_YES;
	else if (m_PostOpRadio.GetCheck()) m_pExam->m_Header.m_Preop = PREOP_NO;
	else m_pExam->m_Header.m_Preop = PREOP_UNKNOWN;

	int i = m_ClinicsComboBox.GetCurSel();
	if (i < 1 || i > ::DB.m_Clinics.GetSize()) m_pExam->m_Header.m_ClinicID = GUID_NULL;
	else m_pExam->m_Header.m_ClinicID = ::DB.m_Clinics[i - 1].m_ClinicID;

	i = m_PhysiciansComboBox.GetCurSel();
	if (i < 1 || i > ::DB.m_Physicians.GetSize()) m_pExam->m_Header.m_PhysicianID = GUID_NULL;
	else m_pExam->m_Header.m_PhysicianID = ::DB.m_Physicians[i - 1].m_PhysicianID;

	i = m_OperatorsComboBox.GetCurSel();
	if (i < 1 || i > ::DB.m_Operators.GetSize()) m_pExam->m_Header.m_OperatorID = GUID_NULL;
	else m_pExam->m_Header.m_OperatorID = ::DB.m_Operators[i - 1].m_OperatorID;

	i = m_SeriesComboBox.GetCurSel();
	m_pExam->m_Header.m_SeriesNumber = i != -1 ? i : 0;

	CString Note;
	m_NoteEdit.GetWindowText(Note);
	Note.Remove((char)10);

	for (int i = 0; i < Note.GetLength(); i++)
	{
		if (Note.GetAt(i) == (char)13)
		{
			Note.SetAt(i, ' ');
		}
	}

	m_pExam->m_Header.m_Note = G_ConvertChineseStr(Note);//6.2.0

	if (::Settings.m_ShowWavetouch && m_pExam->m_Header.m_Type == EXAM_TYPE_WF)
	{

		CWFExam* pWFExam = (CWFExam*)m_pExam;
		CString s;

		m_WavetouchLensPowerEdit.GetWindowText(s);
		G_As = s;
		pWFExam->m_WavetouchLensPower = s.IsEmpty() ? INVALID_VALUE : atof(G_As);

		if (m_WavetouchLensFlatCurveRadio.GetCheck())
		{
			pWFExam->m_WavetouchLensBaseCurve = WAVETOUCH_LENS_BASECURVE_MEDIAN;
		}
		else if (m_WavetouchLensSteepCurveRadio.GetCheck())
		{
			pWFExam->m_WavetouchLensBaseCurve = WAVETOUCH_LENS_BASECURVE_STEEP;
		}
		else {
			pWFExam->m_WavetouchLensBaseCurve = WAVETOUCH_LENS_BASECURVE_UNKNOWN;
		}

		m_WavetouchOtherEdit.GetWindowText(pWFExam->m_WavetouchOther);

		m_WavetouchNoteEdit.GetWindowText(pWFExam->m_WavetouchNote);

		m_WavetouchSphEdit.GetWindowText(s);
		G_As = s;
		pWFExam->m_WavetouchSph = s.IsEmpty() ? INVALID_VALUE : atof(G_As);

		m_WavetouchCylEdit.GetWindowText(s);
		G_As = s;
		pWFExam->m_WavetouchCyl = s.IsEmpty() ? INVALID_VALUE : atof(G_As);

		m_WavetouchAxisEdit.GetWindowText(s);
		G_As = s;
		pWFExam->m_WavetouchAxis = s.IsEmpty() ? INVALID_VALUE : atoi(G_As);
	}

	::Settings.m_ClinicID = m_pExam->m_Header.m_ClinicID;
	::Settings.m_PhysicianID = m_pExam->m_Header.m_PhysicianID;
	::Settings.m_OperatorID = m_pExam->m_Header.m_OperatorID;

	CDialog::OnOK();
}

//***************************************************************************************

void CExamDlg::CreateClinicsList(const GUID& ClinicID)
{
	::DB.LoadClinics();
	m_ClinicsComboBox.ResetContent();
	m_ClinicsComboBox.AddString(_T(""));
	int k = 0;
	int pos = 0;

	for (int i = 0; i < ::DB.m_Clinics.GetSize(); i++)
	{
		m_ClinicsComboBox.AddString(G_ReadChinese(::DB.m_Clinics[i].m_Name));//6.2.0
		if (::DB.m_Clinics[i].m_ClinicID == ClinicID) k = i + 1;
		if (::DB.m_Clinics[i].m_ClinicID == ::Settings.m_ClinicID) pos = i + 1;
	}

	//if no ClinicID match to it, set the default clinic as the last clinic
	if (k == 0) k = pos;
	m_ClinicsComboBox.SetCurSel(k);
}

//***************************************************************************************

void CExamDlg::CreatePhysiciansList(const GUID& PhysicianID)
{
	::DB.LoadPhysicians();
	m_PhysiciansComboBox.ResetContent();
	m_PhysiciansComboBox.AddString(_T(""));

	int k = 0;
	int pos = 0;

	for (int i = 0; i < ::DB.m_Physicians.GetSize(); i++)
	{
		m_PhysiciansComboBox.AddString(G_ReadChinese(::DB.m_Physicians[i].FullName()));
		if (::DB.m_Physicians[i].m_PhysicianID == PhysicianID) k = i + 1;
		if (::DB.m_Physicians[i].m_PhysicianID == ::Settings.m_PhysicianID) pos = i + 1;
	}

	//if no PhysicianID match to it, set the default Physician as the last Physician
	if (k == 0)  k = pos;

	m_PhysiciansComboBox.SetCurSel(k);
}

//***************************************************************************************

void CExamDlg::CreateOperatorsList(const GUID& OperatorID)
{
	::DB.LoadOperators();
	m_OperatorsComboBox.ResetContent();
	m_OperatorsComboBox.AddString(_T(""));
	int k = 0;
	int pos = 0;

	for (int i = 0; i < ::DB.m_Operators.GetSize(); i++) 
	{
		m_OperatorsComboBox.AddString(G_ReadChinese(::DB.m_Operators[i].FullName()));
		if (::DB.m_Operators[i].m_OperatorID == OperatorID) k = i + 1;
		if (::DB.m_Operators[i].m_OperatorID == ::Settings.m_OperatorID) pos = i + 1;
	}

	//if no OperatorID match to it, set the default Operator as the last Operator
	if (k == 0)  k = pos;

	m_OperatorsComboBox.SetCurSel(k);
}

//***************************************************************************************

void CExamDlg::OnWavetouchLensFlatCurveRadioClicked()
{
	m_WavetouchOtherEdit.EnableWindow(FALSE);
}

//***************************************************************************************

void CExamDlg::OnWavetouchLensSteepCurveRadioClicked()
{
	m_WavetouchOtherEdit.EnableWindow(FALSE);
}

//***************************************************************************************

void CExamDlg::OnWavetouchLensOtherCurveRadioClicked()
{
	m_WavetouchOtherEdit.EnableWindow(TRUE);
}

//***************************************************************************************
