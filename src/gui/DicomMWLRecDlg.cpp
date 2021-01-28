/***************************************************************************************

Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          
Description:   Create a dialog for the source of import patient selection.
Since customer has their MWL system for DICOM import
read '//dicomiseasy.blogspot.com/2012/04/dicom-modality-worklist.html'
and '//dicomiseasy.blogspot.com/2011/12/introduction-to-dicom-chapter-5-solving.html'
verison:       6.2.0

***************************************************************************************/

//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "BusyCursor.h"
#include "TraceyDicom.h"
#include "INIParser.h"     //6.2.0 Ini registration

#include "DicomMWLRecDlg.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CDicomMWLRecDlg, CDialog)

	ON_BN_CLICKED(IDQUERY, OnQueryClicked)

END_MESSAGE_MAP()

//***************************************************************************************

void CDicomMWLRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_IP_EDIT, m_IPEdit);
	DDX_Control(pDX, IDC_PORT_EDIT, m_PortEdit);
	DDX_Control(pDX, IDC_TARAE_EDIT, m_TarGetAEEdit);
	DDX_Control(pDX, IDC_LOCALAE_EDIT, m_LocalAEEdit);
	DDX_Control(pDX, IDC_MODALITY_COMBO, m_ModalityCombo);

	DDX_Control(pDX, IDC_FNAME_EDIT, m_FNameEdit);
	DDX_Control(pDX, IDC_LNAME_EDIT, m_LNameEdit);
	DDX_Control(pDX, IDC_PAID_EDIT, m_PaIDEdit);//Patient ID

	DDX_Control(pDX, IDC_DATETIMEFROM, m_FromEdit);
	DDX_Control(pDX, IDC_DATETIMETO, m_ToEdit);	

	DDX_Control(pDX, IDQUERY, m_QueryBtn);	
}

//***************************************************************************************

CDicomMWLRecDlg::CDicomMWLRecDlg(CWnd* pParentWnd) : CDialog(IDD_DICOMMWLRECDLG, pParentWnd)
{

}

//***************************************************************************************

BOOL CDicomMWLRecDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();

	CINIParser IniPar;

	CString LoacalAe = IniPar.GetStrKey(IniSection, "Dicom MWL Local AE", IniPath);	//6.2.0 ini registration
	CString TragetAe = IniPar.GetStrKey(IniSection, "Dicom MWL Target AE", IniPath);	//6.2.0 ini registration
	CString Ip = IniPar.GetStrKey(IniSection, "Dicom MWL IP Address", IniPath);	//6.2.0 ini registration
	CString Port = IniPar.GetStrKey(IniSection, "Dicom MWL IP Port", IniPath);	//6.2.0 ini registration
	CString Modality = IniPar.GetStrKey(IniSection, "Dicom Modality", IniPath);	//6.2.0 ini registration
			
	m_IPEdit.SetWindowText(Ip);
	m_PortEdit.SetWindowText(Port);
	m_TarGetAEEdit.SetWindowText(TragetAe);
	m_LocalAEEdit.SetWindowText(LoacalAe);

	m_ModalityCombo.AddString(_T(""));
	m_ModalityCombo.AddString(_T("OPM – Ophthalmic Mapping"));
	m_ModalityCombo.AddString(_T("AR – Autorefraction"));
	m_ModalityCombo.AddString(_T("IOL – Intraocular lens data"));
	m_ModalityCombo.AddString(_T("KER – Keratometry"));

	m_ModalityCombo.SetWindowText(Modality);

	m_FromEdit.SetWindowText(_T("1/1/1990"));

	return FALSE;
}

//***************************************************************************************

BOOL CDicomMWLRecDlg::PreTranslateMessage(MSG* pMsg)
{
	//Number check
	if (pMsg->message == WM_KEYUP)
	{
		int Edit = 0;

		if (GetFocus() == &m_IPEdit)   Edit = 0;
		else if (GetFocus() == &m_PortEdit)   Edit = 2;


		if (Edit != 0)
		{
			CString DOB;
			GetFocus()->GetWindowText(DOB);

			BOOL IsNum;

	/*		if (Edit == 1) IsNum = ::InputIsIpADD(DOB);
			if (Edit == 2) */
			IsNum = ::InputIsIntNum(DOB);

			if (!IsNum)
			{
				int len = DOB.GetLength();
				DOB = DOB.Left(len - 1);
				GetFocus()->SetWindowText(DOB);

				if (Edit == 1)  m_IPEdit.SetSel(-1);
				if (Edit == 2)  m_IPEdit.SetSel(-1);
			}
		}
	}
	//Done

	return CDialog::PreTranslateMessage(pMsg);
}

//***************************************************************************************

void CDicomMWLRecDlg::OnQueryClicked()
{
	CBusyCursor Cursor;

	CTraceyDicom TraceyDicom;

	int Port;
	CString Modality, IpStr, PortStr, LocalAE, TargetAE, PatientID, FirstName, LastName, DateFrom, DateTo;

	m_ModalityCombo.GetWindowText(Modality);
	m_IPEdit.GetWindowText(IpStr);
	m_PortEdit.GetWindowText(PortStr);

	m_LocalAEEdit.GetWindowText(LocalAE);
	m_TarGetAEEdit.GetWindowText(TargetAE);

	m_FNameEdit.GetWindowText(FirstName);
	m_LNameEdit.GetWindowText(LastName);

	m_FromEdit.GetWindowText(DateFrom);
	m_ToEdit.GetWindowText(DateTo);

	m_PaIDEdit.GetWindowText(PatientID);

	IpStr.Trim();
	PortStr.Trim();
	LocalAE.Trim();
	TargetAE.Trim();

	FirstName.Trim();
	LastName.Trim();

	PatientID.Trim();

	DateFrom.Trim();
	DateTo.Trim();

	if (IpStr == "")
	{
		::Info("Please input Ip Address.");
		m_IPEdit.SetFocus();
		return;
	}
	if (PortStr == "")
	{
		::Info("Please input Port.");
		m_PortEdit.SetFocus();
		return;
	}
	if (LocalAE == "")
	{
		::Info("Please input Local AE.");
		m_LocalAEEdit.SetFocus();
		return;
	}
	if (TargetAE == "")
	{
		::Info("Please input Target AE.");
		m_TarGetAEEdit.SetFocus();
		return;
	}

	G_As = PortStr;
	Port = atoi(G_As);
	unsigned short PortShort = Port;

	m_RecCount = 0;

	CString Mstr = "";

	     if (Modality ==  "OPM – Ophthalmic Mapping") Mstr = "OPM";
	else if (Modality == "AR – Autorefraction")  Mstr = "AR";
	else if (Modality == "IOL – Intraocular lens data")  Mstr = "IOL";
	else if (Modality == "KER – Keratometry")  Mstr = "KER";


	if (TraceyDicom.Query(Mstr, IpStr, PortShort, LocalAE, TargetAE, PatientID, FirstName, LastName, DateFrom, DateTo, m_RecCount))
	{
		CString nStr;
		nStr.Format(_T("%i"), m_RecCount);
		::Info("Received " + nStr + " record(s).");

		for (int i = 0; i < m_RecCount; i++)
		{
			m_DicomInfo[i] = TraceyDicom.m_DicomInfo[i];
		}

		//6.2.0 ini registration
		CINIParser IniPar;

		IniPar.SetStrKey(IniSection, "Dicom MWL Local AE", LocalAE, IniPath);
		IniPar.SetStrKey(IniSection, "Dicom MWL Target AE", TargetAE, IniPath);
		IniPar.SetStrKey(IniSection, "Dicom MWL IP Address", IpStr, IniPath);
		IniPar.SetStrKey(IniSection, "Dicom MWL IP Port", PortStr, IniPath);
		IniPar.SetStrKey(IniSection, "Dicom Modality", Modality, IniPath);

		CDialog::OnOK();
	}	
}

//***************************************************************************************
