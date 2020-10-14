
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ExcelSettingsWFpage.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CExcelSettingsWFpage, CDialog)

	ON_WM_CTLCOLOR()//

	ON_WM_LBUTTONDOWN()//

	ON_BN_CLICKED(IDC_CHECK0, OnCheckAllClick)

	ON_BN_CLICKED(IDC_CHECK1, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK2, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK3, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK4, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK5, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK6, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK7, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK8, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK9, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK10, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECK11, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK12, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK13, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK14, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK15, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK16, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK17, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK18, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK19, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK20, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECK21, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK22, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK23, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK24, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK25, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK26, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK27, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK28, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK29, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK30, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECK31, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK32, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK33, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK34, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK35, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK36, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK37, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK38, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK39, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK40, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECK41, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK42, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK43, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK44, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK45, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK46, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK47, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK48, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK49, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK50, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECK51, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK52, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECK53, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECK54, OnOneCheckClick)//

END_MESSAGE_MAP()

//***************************************************************************************
CExcelSettingsWFpage::CExcelSettingsWFpage() : CDialog(IDD_EXCEL_WF_PAGE)
{
}

//***************************************************************************************

void CExcelSettingsWFpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REQ_IMG, m_ImgRequired1);
	DDX_Control(pDX, IDC_REQ_IMG2, m_ImgRequired2);
	DDX_Control(pDX, IDC_REQ_IMG3, m_ImgRequired3);

	DDX_Control(pDX, IDC_CHECK0, m_CheckAll);
	DDX_Control(pDX, IDC_CHECK1, m_CheckUserID);
	DDX_Control(pDX, IDC_CHECK2, m_CheckFName);
	DDX_Control(pDX, IDC_CHECK3, m_CheckMName);
	DDX_Control(pDX, IDC_CHECK4, m_CheckLName);
	DDX_Control(pDX, IDC_CHECK5, m_CheckDOB);
	DDX_Control(pDX, IDC_CHECK6, m_CheckGender);
	DDX_Control(pDX, IDC_CHECK7, m_CheckAddLine1);
	DDX_Control(pDX, IDC_CHECK8, m_CheckAddLine2);
	DDX_Control(pDX, IDC_CHECK9, m_CheckCity);
	DDX_Control(pDX, IDC_CHECK10, m_CheckState);

	DDX_Control(pDX, IDC_CHECK11, m_CheckZip);
	DDX_Control(pDX, IDC_CHECK12, m_CheckCountry);
	DDX_Control(pDX, IDC_CHECK13, m_CheckWPhone);
	DDX_Control(pDX, IDC_CHECK14, m_CheckHPhone);
	DDX_Control(pDX, IDC_CHECK15, m_CheckEmail);
	DDX_Control(pDX, IDC_CHECK16, m_CheckNote);
	DDX_Control(pDX, IDC_CHECK17, m_CheckGroupGUID);
	DDX_Control(pDX, IDC_CHECK18, m_CheckGroupName);
	DDX_Control(pDX, IDC_CHECK19, m_CheckCalID);
	DDX_Control(pDX, IDC_CHECK20, m_CheckMode);

	DDX_Control(pDX, IDC_CHECK21, m_CheckDate);
	DDX_Control(pDX, IDC_CHECK22, m_CheckTime);
	DDX_Control(pDX, IDC_CHECK23, m_CheckEye);
	DDX_Control(pDX, IDC_CHECK24, m_CheckPrePost);
	DDX_Control(pDX, IDC_CHECK25, m_CheckSW);
	DDX_Control(pDX, IDC_CHECK26, m_CheckPhysicianID);
	DDX_Control(pDX, IDC_CHECK27, m_CheckPhysFName);
	DDX_Control(pDX, IDC_CHECK28, m_CheckPhysLName);
	DDX_Control(pDX, IDC_CHECK29, m_CheckOperatorID);
	DDX_Control(pDX, IDC_CHECK30, m_CheckOperFName);

	DDX_Control(pDX, IDC_CHECK31, m_CheckOperLName);
	DDX_Control(pDX, IDC_CHECK32, m_CheckExamNote);
	DDX_Control(pDX, IDC_CHECK33, m_CheckAngelKDist);
	DDX_Control(pDX, IDC_CHECK34, m_CheckAngelKDeg);
	/* DDX_Control(pDX, IDC_CHECK35 , m_CheckAngelADist );
	DDX_Control(pDX, IDC_CHECK36 , m_CheckAngelADeg );
	DDX_Control(pDX, IDC_CHECK37 , m_CheckLimbalX );
	DDX_Control(pDX, IDC_CHECK38 , m_CheckLimbalY );
	DDX_Control(pDX, IDC_CHECK39 , m_CheckLimbalD );*/
	DDX_Control(pDX, IDC_CHECK40, m_CheckPupilX);

	DDX_Control(pDX, IDC_CHECK41, m_CheckPupilY);
	DDX_Control(pDX, IDC_CHECK42, m_CheckPupilD);
	DDX_Control(pDX, IDC_CHECK43, m_CheckScanD);
	DDX_Control(pDX, IDC_CHECK44, m_CheckTraceRef);
	DDX_Control(pDX, IDC_CHECK45, m_CheckRef1Zone);
	DDX_Control(pDX, IDC_CHECK46, m_CheckRef2Zone);
	DDX_Control(pDX, IDC_CHECK47, m_CheckRef3Zone);
	DDX_Control(pDX, IDC_CHECK48, m_CheckRef4Zone);
	DDX_Control(pDX, IDC_CHECK49, m_CheckEntireEyeZC1);
	DDX_Control(pDX, IDC_CHECK50, m_CheckEntireEyeZC2);

	DDX_Control(pDX, IDC_CHECK51, m_CheckEntireEyeZC3);
	DDX_Control(pDX, IDC_CHECK52, m_CheckEntireEyeZCF);

	DDX_Control(pDX, IDC_CHECK53, m_CheckClinic);

	DDX_Control(pDX, IDC_CHECK54, m_CheckOpacityGrade);//


													   //
	DDX_Control(pDX, IDC_STATIC0, m_CheckAll_Text);
	DDX_Control(pDX, IDC_STATIC1, m_CheckUserID_Text);
	DDX_Control(pDX, IDC_STATIC2, m_CheckFName_Text);
	DDX_Control(pDX, IDC_STATIC3, m_CheckMName_Text);
	DDX_Control(pDX, IDC_STATIC4, m_CheckLName_Text);
	DDX_Control(pDX, IDC_STATIC5, m_CheckDOB_Text);
	DDX_Control(pDX, IDC_STATIC6, m_CheckGender_Text);
	DDX_Control(pDX, IDC_STATIC7, m_CheckAddLine1_Text);
	DDX_Control(pDX, IDC_STATIC8, m_CheckAddLine2_Text);
	DDX_Control(pDX, IDC_STATIC9, m_CheckCity_Text);
	DDX_Control(pDX, IDC_STATIC10, m_CheckState_Text);

	DDX_Control(pDX, IDC_STATIC11, m_CheckZip_Text);
	DDX_Control(pDX, IDC_STATIC12, m_CheckCountry_Text);
	DDX_Control(pDX, IDC_STATIC13, m_CheckWPhone_Text);
	DDX_Control(pDX, IDC_STATIC14, m_CheckHPhone_Text);
	DDX_Control(pDX, IDC_STATIC15, m_CheckEmail_Text);
	DDX_Control(pDX, IDC_STATIC16, m_CheckNote_Text);
	DDX_Control(pDX, IDC_STATIC17, m_CheckGroupGUID_Text);
	DDX_Control(pDX, IDC_STATIC18, m_CheckGroupName_Text);
	DDX_Control(pDX, IDC_STATIC19, m_CheckCalID_Text);
	DDX_Control(pDX, IDC_STATIC20, m_CheckMode_Text);

	DDX_Control(pDX, IDC_STATIC21, m_CheckDate_Text);
	DDX_Control(pDX, IDC_STATIC22, m_CheckTime_Text);
	DDX_Control(pDX, IDC_STATIC23, m_CheckEye_Text);
	DDX_Control(pDX, IDC_STATIC24, m_CheckPrePost_Text);
	DDX_Control(pDX, IDC_STATIC25, m_CheckSW_Text);
	DDX_Control(pDX, IDC_STATIC26, m_CheckPhysicianID_Text);
	DDX_Control(pDX, IDC_STATIC27, m_CheckPhysFName_Text);
	DDX_Control(pDX, IDC_STATIC28, m_CheckPhysLName_Text);
	DDX_Control(pDX, IDC_STATIC29, m_CheckOperatorID_Text);
	DDX_Control(pDX, IDC_STATIC30, m_CheckOperFName_Text);

	DDX_Control(pDX, IDC_STATIC31, m_CheckOperLName_Text);
	DDX_Control(pDX, IDC_STATIC32, m_CheckExamNote_Text);
	DDX_Control(pDX, IDC_STATIC57, m_CheckAngelKDist_Text);
	DDX_Control(pDX, IDC_STATIC34, m_CheckAngelKDeg_Text);
	/* DDX_Control(pDX, IDC_STATIC35, m_CheckAngelADist_Text );
	DDX_Control(pDX, IDC_STATIC36, m_CheckAngelADeg_Text );
	DDX_Control(pDX, IDC_STATIC37, m_CheckLimbalX_Text );
	DDX_Control(pDX, IDC_STATIC38, m_CheckLimbalY_Text );
	DDX_Control(pDX, IDC_STATIC39, m_CheckLimbalD_Text );*/
	DDX_Control(pDX, IDC_STATIC40, m_CheckPupilX_Text);

	DDX_Control(pDX, IDC_STATIC41, m_CheckPupilY_Text);
	DDX_Control(pDX, IDC_STATIC42, m_CheckPupilD_Text);
	DDX_Control(pDX, IDC_STATIC43, m_CheckScanD_Text);
	DDX_Control(pDX, IDC_STATIC56, m_CheckTraceRef_Text);
	DDX_Control(pDX, IDC_STATIC45, m_CheckRef1Zone_Text);
	DDX_Control(pDX, IDC_STATIC46, m_CheckRef2Zone_Text);
	DDX_Control(pDX, IDC_STATIC47, m_CheckRef3Zone_Text);
	DDX_Control(pDX, IDC_STATIC48, m_CheckRef4Zone_Text);
	DDX_Control(pDX, IDC_STATIC49, m_CheckEntireEyeZC1_Text);
	DDX_Control(pDX, IDC_STATIC50, m_CheckEntireEyeZC2_Text);

	DDX_Control(pDX, IDC_STATIC51, m_CheckEntireEyeZC3_Text);
	DDX_Control(pDX, IDC_STATIC52, m_CheckEntireEyeZCF_Text);

	DDX_Control(pDX, IDC_STATIC53, m_CheckClinic_Text);

	DDX_Control(pDX, IDC_STATIC54, m_CheckOpacityGrade_Text);
	//
}

//***************************************************************************************

void CExcelSettingsWFpage::OnOneCheckClick()
{
	BOOL AllCheck = TRUE;

	if (!m_CheckUserID.GetCheck()) { AllCheck = FALSE;	goto End; }  //1
	if (!m_CheckFName.GetCheck()) { AllCheck = FALSE;	goto End; }   //2
	if (!m_CheckMName.GetCheck()) { AllCheck = FALSE;	goto End; }   //3
	if (!m_CheckLName.GetCheck()) { AllCheck = FALSE;	goto End; }   //4
	if (!m_CheckDOB.GetCheck()) { AllCheck = FALSE;	goto End; }     //5
	if (!m_CheckGender.GetCheck()) { AllCheck = FALSE;	goto End; }  //6
	if (!m_CheckAddLine1.GetCheck()) { AllCheck = FALSE;	goto End; }//7
	if (!m_CheckAddLine2.GetCheck()) { AllCheck = FALSE;	goto End; }//8
	if (!m_CheckCity.GetCheck()) { AllCheck = FALSE;	goto End; }    //9
	if (!m_CheckState.GetCheck()) { AllCheck = FALSE;	goto End; }   //10

	if (!m_CheckZip.GetCheck()) { AllCheck = FALSE;	goto End; }      //11
	if (!m_CheckCountry.GetCheck()) { AllCheck = FALSE;	goto End; }  //12
	if (!m_CheckWPhone.GetCheck()) { AllCheck = FALSE;	goto End; }   //13
	if (!m_CheckHPhone.GetCheck()) { AllCheck = FALSE;	goto End; }   //14
	if (!m_CheckEmail.GetCheck()) { AllCheck = FALSE;	goto End; }    //15
	if (!m_CheckNote.GetCheck()) { AllCheck = FALSE;	goto End; }     //16
	if (!m_CheckGroupGUID.GetCheck()) { AllCheck = FALSE;	goto End; }//17
	if (!m_CheckGroupName.GetCheck()) { AllCheck = FALSE;	goto End; }//18
	if (!m_CheckCalID.GetCheck()) { AllCheck = FALSE;	goto End; }    //19
	if (!m_CheckMode.GetCheck()) { AllCheck = FALSE;	goto End; }     //20

	if (!m_CheckDate.GetCheck()) { AllCheck = FALSE;	goto End; }       //21
	if (!m_CheckTime.GetCheck()) { AllCheck = FALSE;	goto End; }       //22
	if (!m_CheckEye.GetCheck()) { AllCheck = FALSE;	goto End; }        //23
	if (!m_CheckPrePost.GetCheck()) { AllCheck = FALSE;	goto End; }    //24
	if (!m_CheckSW.GetCheck()) { AllCheck = FALSE;	goto End; }         //25
	if (!m_CheckPhysicianID.GetCheck()) { AllCheck = FALSE;	goto End; }//26
	if (!m_CheckPhysFName.GetCheck()) { AllCheck = FALSE;	goto End; }  //27
	if (!m_CheckPhysLName.GetCheck()) { AllCheck = FALSE;	goto End; }  //28
	if (!m_CheckOperatorID.GetCheck()) { AllCheck = FALSE;	goto End; } //29
	if (!m_CheckOperFName.GetCheck()) { AllCheck = FALSE;	goto End; }  //30

	if (!m_CheckOperLName.GetCheck()) { AllCheck = FALSE;	goto End; }  //31
	if (!m_CheckExamNote.GetCheck()) { AllCheck = FALSE;	goto End; }   //32
	if (!m_CheckAngelKDist.GetCheck()) { AllCheck = FALSE;	goto End; } //33
	if (!m_CheckAngelKDeg.GetCheck()) { AllCheck = FALSE;	goto End; }  //34
																		 //if(!m_CheckAngelADist.GetCheck()) 	{	AllCheck = FALSE;	goto End;	} //35
																		 //if(!m_CheckAngelADeg.GetCheck()) 	{	AllCheck = FALSE;	goto End;	}  //36
																		 //if(!m_CheckLimbalX.GetCheck()) 	{	AllCheck = FALSE;	goto End;	}    //37
																		 //if(!m_CheckLimbalY.GetCheck()) 	{	AllCheck = FALSE;	goto End;	}    //38
																		 //if(!m_CheckLimbalD.GetCheck()) 	{	AllCheck = FALSE;	goto End;	}    //39
	if (!m_CheckPupilX.GetCheck()) { AllCheck = FALSE;	goto End; }     //40

	if (!m_CheckPupilY.GetCheck()) { AllCheck = FALSE;	goto End; }     //41
	if (!m_CheckPupilD.GetCheck()) { AllCheck = FALSE;	goto End; }     //42
	if (!m_CheckScanD.GetCheck()) { AllCheck = FALSE;	goto End; }      //43
	if (!m_CheckTraceRef.GetCheck()) { AllCheck = FALSE;	goto End; }   //44
	if (!m_CheckRef1Zone.GetCheck()) { AllCheck = FALSE;	goto End; }   //45
	if (!m_CheckRef2Zone.GetCheck()) { AllCheck = FALSE;	goto End; }   //46
	if (!m_CheckRef3Zone.GetCheck()) { AllCheck = FALSE;	goto End; }   //47
	if (!m_CheckRef4Zone.GetCheck()) { AllCheck = FALSE;	goto End; }   //48
	if (!m_CheckEntireEyeZC1.GetCheck()) { AllCheck = FALSE;	goto End; } //49
	if (!m_CheckEntireEyeZC2.GetCheck()) { AllCheck = FALSE;	goto End; } //50
	if (!m_CheckEntireEyeZC3.GetCheck()) { AllCheck = FALSE;	goto End; } //51
	if (!m_CheckEntireEyeZCF.GetCheck()) { AllCheck = FALSE;	goto End; } //52

	if (!m_CheckOpacityGrade.GetCheck()) { AllCheck = FALSE;	goto End; } //53 

End:
	m_CheckAll.SetCheck(AllCheck);

	Invalidate(FALSE);//
}

//***************************************************************************************
void CExcelSettingsWFpage::OnCheckAllClick()
{
	BOOL Check = m_CheckAll.GetCheck();

	m_CheckUserID.SetCheck(Check);
	m_CheckFName.SetCheck(Check);
	m_CheckMName.SetCheck(Check);
	m_CheckLName.SetCheck(Check);
	m_CheckDOB.SetCheck(Check);
	m_CheckGender.SetCheck(Check);
	m_CheckAddLine1.SetCheck(Check);
	m_CheckAddLine2.SetCheck(Check);
	m_CheckCity.SetCheck(Check);
	m_CheckState.SetCheck(Check);
	m_CheckZip.SetCheck(Check);
	m_CheckCountry.SetCheck(Check);
	m_CheckWPhone.SetCheck(Check);
	m_CheckHPhone.SetCheck(Check);
	m_CheckEmail.SetCheck(Check);
	m_CheckNote.SetCheck(Check);
	m_CheckGroupGUID.SetCheck(Check);
	m_CheckGroupName.SetCheck(Check);
	m_CheckCalID.SetCheck(Check);
	m_CheckMode.SetCheck(Check);
	m_CheckDate.SetCheck(Check);
	m_CheckTime.SetCheck(Check);
	m_CheckEye.SetCheck(Check);
	m_CheckPrePost.SetCheck(Check);
	m_CheckSW.SetCheck(Check);
	m_CheckPhysicianID.SetCheck(Check);
	m_CheckPhysFName.SetCheck(Check);
	m_CheckPhysLName.SetCheck(Check);
	m_CheckOperatorID.SetCheck(Check);
	m_CheckOperFName.SetCheck(Check);
	m_CheckOperLName.SetCheck(Check);
	m_CheckExamNote.SetCheck(Check);
	m_CheckAngelKDist.SetCheck(Check);
	m_CheckAngelKDeg.SetCheck(Check);
	//m_CheckAngelADist.SetCheck(Check);
	//m_CheckAngelADeg.SetCheck(Check);
	//m_CheckLimbalX.SetCheck(Check);
	//m_CheckLimbalY.SetCheck(Check);
	//m_CheckLimbalD.SetCheck(Check);
	m_CheckPupilX.SetCheck(Check);
	m_CheckPupilY.SetCheck(Check);
	m_CheckPupilD.SetCheck(Check);
	m_CheckScanD.SetCheck(Check);
	m_CheckTraceRef.SetCheck(Check);
	m_CheckRef1Zone.SetCheck(Check);
	m_CheckRef2Zone.SetCheck(Check);
	m_CheckRef3Zone.SetCheck(Check);
	m_CheckRef4Zone.SetCheck(Check);
	m_CheckEntireEyeZC1.SetCheck(Check);
	m_CheckEntireEyeZC2.SetCheck(Check);
	m_CheckEntireEyeZC3.SetCheck(Check);
	m_CheckEntireEyeZCF.SetCheck(Check);

	m_CheckClinic.SetCheck(Check);

	m_CheckOpacityGrade.SetCheck(Check);//

	Invalidate(FALSE);//
}
//***************************************************************************************
//
HBRUSH CExcelSettingsWFpage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int iID = pWnd->GetDlgCtrlID();

	if (iID == IDC_STATIC0)
	{
		if (m_CheckAll.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}


	if (iID == IDC_STATIC_A || iID == IDC_STATIC_B || iID == IDC_STATIC_C)
	{
		pDC->SetTextColor(BLUE);
	}

	if (iID == IDC_STATIC1)
	{
		if (m_CheckUserID.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC2)
	{
		if (m_CheckFName.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC3)
	{
		if (m_CheckMName.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC4)
	{
		if (m_CheckLName.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC5)
	{
		if (m_CheckDOB.GetCheck())	    pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC6)
	{
		if (m_CheckGender.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC7)
	{
		if (m_CheckAddLine1.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC8)
	{
		if (m_CheckAddLine2.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC9)
	{
		if (m_CheckCity.GetCheck())	    pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC10)
	{
		if (m_CheckState.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC11)
	{
		if (m_CheckZip.GetCheck())	    pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC12)
	{
		if (m_CheckCountry.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC13)
	{
		if (m_CheckWPhone.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC14)
	{
		if (m_CheckHPhone.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC15)
	{
		if (m_CheckEmail.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC16)
	{
		if (m_CheckNote.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC17)
	{
		if (m_CheckGroupGUID.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC18)
	{
		if (m_CheckGroupName.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC19)
	{
		if (m_CheckCalID.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC20)
	{
		if (m_CheckMode.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC21)
	{
		if (m_CheckDate.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC22)
	{
		if (m_CheckTime.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC23)
	{
		if (m_CheckEye.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC24)
	{
		if (m_CheckPrePost.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC25)
	{
		if (m_CheckSW.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC26)
	{
		if (m_CheckPhysicianID.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC27)
	{
		if (m_CheckPhysFName.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC28)
	{
		if (m_CheckPhysLName.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC29)
	{
		if (m_CheckOperatorID.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC30)
	{
		if (m_CheckOperFName.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC31)
	{
		if (m_CheckOperLName.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC32)
	{
		if (m_CheckExamNote.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC57)
	{
		if (m_CheckAngelKDist.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC34)
	{
		if (m_CheckAngelKDeg.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	/*if (iID == IDC_STATIC35)
	{
	if (m_CheckAngelADist.GetCheck())	pDC->SetTextColor(BLUE);
	else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC36)
	{
	if (m_CheckAngelADeg.GetCheck())	pDC->SetTextColor(BLUE);
	else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC37)
	{
	if (m_CheckLimbalX.GetCheck())	pDC->SetTextColor(BLUE);
	else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC38)
	{
	if (m_CheckLimbalY.GetCheck())	pDC->SetTextColor(BLUE);
	else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC39)
	{
	if (m_CheckLimbalD.GetCheck())	pDC->SetTextColor(BLUE);
	else			                pDC->SetTextColor(DARK_GRAY);
	}*/
	if (iID == IDC_STATIC40)
	{
		if (m_CheckPupilX.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC41)
	{
		if (m_CheckPupilY.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC42)
	{
		if (m_CheckPupilD.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC43)
	{
		if (m_CheckScanD.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC56)
	{
		if (m_CheckTraceRef.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC45)
	{
		if (m_CheckRef1Zone.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC46)
	{
		if (m_CheckRef2Zone.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC47)
	{
		if (m_CheckRef3Zone.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC48)
	{
		if (m_CheckRef4Zone.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC49)
	{
		if (m_CheckEntireEyeZC1.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC50)
	{
		if (m_CheckEntireEyeZC2.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC51)
	{
		if (m_CheckEntireEyeZC3.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC52)
	{
		if (m_CheckEntireEyeZCF.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC53)
	{
		if (m_CheckClinic.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}
	if (iID == IDC_STATIC54)
	{
		if (m_CheckOpacityGrade.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	return hbr;
}
//

//***************************************************************************************

//
void CExcelSettingsWFpage::OnLButtonDown(uint nFlags, CPoint Point)
{
	int whichCheck = -1;
	CRect rect;
	CPoint point, OriPoint;

	GetCursorPos(&point);

	//Select All
	OriPoint = point;
	m_CheckAll_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckAll_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckAll.SetCheck(!m_CheckAll.GetCheck());
		OnCheckAllClick();
		return;
	}

	//ID assigned by user
	OriPoint = point;
	m_CheckUserID_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckUserID_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckUserID.SetCheck(!m_CheckUserID.GetCheck());
		OnOneCheckClick();
		return;
	}

	//First Name
	OriPoint = point;
	m_CheckFName_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckFName_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckFName.SetCheck(!m_CheckFName.GetCheck());
		OnOneCheckClick();
		return;
	}

	//Middle Name
	OriPoint = point;
	m_CheckMName_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckMName_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckMName.SetCheck(!m_CheckMName.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckLName_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckLName_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckLName.SetCheck(!m_CheckLName.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckDOB_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckDOB_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckDOB.SetCheck(!m_CheckDOB.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckGender_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckGender_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckGender.SetCheck(!m_CheckGender.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckAddLine1_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckAddLine1_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckAddLine1.SetCheck(!m_CheckAddLine1.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckAddLine2_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckAddLine2_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckAddLine2.SetCheck(!m_CheckAddLine2.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCity_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCity_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCity.SetCheck(!m_CheckCity.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckState_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckState_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckState.SetCheck(!m_CheckState.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckZip_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckZip_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckZip.SetCheck(!m_CheckZip.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCountry_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCountry_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCountry.SetCheck(!m_CheckCountry.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckWPhone_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckWPhone_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckWPhone.SetCheck(!m_CheckWPhone.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckHPhone_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckHPhone_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckHPhone.SetCheck(!m_CheckHPhone.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckEmail_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckEmail_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckEmail.SetCheck(!m_CheckEmail.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckNote_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckNote_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckNote.SetCheck(!m_CheckNote.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckGroupGUID_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckGroupGUID_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckGroupGUID.SetCheck(!m_CheckGroupGUID.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckGroupName_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckGroupName_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckGroupName.SetCheck(!m_CheckGroupName.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCalID_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCalID_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCalID.SetCheck(!m_CheckCalID.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckMode_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckMode_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckMode.SetCheck(!m_CheckMode.GetCheck());
		OnOneCheckClick();
		return;
	}


	//***
	OriPoint = point;
	m_CheckDate_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckDate_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckDate.SetCheck(!m_CheckDate.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckTime_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckTime_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckTime.SetCheck(!m_CheckTime.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckEye_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckEye_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckEye.SetCheck(!m_CheckEye.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckPrePost_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckPrePost_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckPrePost.SetCheck(!m_CheckPrePost.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckSW_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckSW_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckSW.SetCheck(!m_CheckSW.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckPhysicianID_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckPhysicianID_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckPhysicianID.SetCheck(!m_CheckPhysicianID.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckPhysFName_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckPhysFName_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckPhysFName.SetCheck(!m_CheckPhysFName.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckPhysLName_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckPhysLName_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckPhysLName.SetCheck(!m_CheckPhysLName.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckOperatorID_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckOperatorID_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckOperatorID.SetCheck(!m_CheckOperatorID.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckOperFName_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckOperFName_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckOperFName.SetCheck(!m_CheckOperFName.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckOperLName_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckOperLName_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckOperLName.SetCheck(!m_CheckOperLName.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckExamNote_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckExamNote_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckExamNote.SetCheck(!m_CheckExamNote.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckAngelKDist_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckAngelKDist_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckAngelKDist.SetCheck(!m_CheckAngelKDist.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckAngelKDeg_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckAngelKDeg_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckAngelKDeg.SetCheck(!m_CheckAngelKDeg.GetCheck());
		OnOneCheckClick();
		return;
	}

	////***
	//OriPoint = point;
	//m_CheckAngelADist_Text.ScreenToClient(&OriPoint);
	//::GetClientRect(m_CheckAngelADist_Text.GetSafeHwnd(), &rect);
	//rect.left -= 5;
	//rect.right += 3;

	//if (rect.PtInRect(OriPoint))
	//{
	//	m_CheckAngelADist.SetCheck(!m_CheckAngelADist.GetCheck());
	//	OnOneCheckClick();
	//	return;
	//}

	////***
	//OriPoint = point;
	//m_CheckAngelADeg_Text.ScreenToClient(&OriPoint);
	//::GetClientRect(m_CheckAngelADeg_Text.GetSafeHwnd(), &rect);
	//rect.left -= 5;
	//rect.right += 3;

	//if (rect.PtInRect(OriPoint))
	//{
	//	m_CheckAngelADeg.SetCheck(!m_CheckAngelADeg.GetCheck());
	//	OnOneCheckClick();
	//	return;
	//}

	////***
	//OriPoint = point;
	//m_CheckLimbalX_Text.ScreenToClient(&OriPoint);
	//::GetClientRect(m_CheckLimbalX_Text.GetSafeHwnd(), &rect);
	//rect.left -= 5;
	//rect.right += 3;

	//if (rect.PtInRect(OriPoint))
	//{
	//	m_CheckLimbalX.SetCheck(!m_CheckLimbalX.GetCheck());
	//	OnOneCheckClick();
	//	return;
	//}
	//
	////***
	//OriPoint = point;
	//m_CheckLimbalY_Text.ScreenToClient(&OriPoint);
	//::GetClientRect(m_CheckLimbalY_Text.GetSafeHwnd(), &rect);
	//rect.left -= 5;
	//rect.right += 3;

	//if (rect.PtInRect(OriPoint))
	//{
	//	m_CheckLimbalY.SetCheck(!m_CheckLimbalY.GetCheck());
	//	OnOneCheckClick();
	//	return;
	//}

	////***
	//OriPoint = point;
	//m_CheckLimbalD_Text.ScreenToClient(&OriPoint);
	//::GetClientRect(m_CheckLimbalD_Text.GetSafeHwnd(), &rect);
	//rect.left -= 5;
	//rect.right += 3;

	//if (rect.PtInRect(OriPoint))
	//{
	//	m_CheckLimbalD.SetCheck(!m_CheckLimbalD.GetCheck());
	//	OnOneCheckClick();
	//	return;
	//}

	//***
	OriPoint = point;
	m_CheckPupilX_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckPupilX_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckPupilX.SetCheck(!m_CheckPupilX.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckPupilY_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckPupilY_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckPupilY.SetCheck(!m_CheckPupilY.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckPupilD_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckPupilD_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckPupilD.SetCheck(!m_CheckPupilD.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckScanD_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckScanD_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckScanD.SetCheck(!m_CheckScanD.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckTraceRef_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckTraceRef_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckTraceRef.SetCheck(!m_CheckTraceRef.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckRef1Zone_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckRef1Zone_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckRef1Zone.SetCheck(!m_CheckRef1Zone.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckRef2Zone_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckRef2Zone_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckRef2Zone.SetCheck(!m_CheckRef2Zone.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckRef3Zone_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckRef3Zone_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckRef3Zone.SetCheck(!m_CheckRef3Zone.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckRef4Zone_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckRef4Zone_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckRef4Zone.SetCheck(!m_CheckRef4Zone.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckEntireEyeZC1_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckEntireEyeZC1_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckEntireEyeZC1.SetCheck(!m_CheckEntireEyeZC1.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckEntireEyeZC2_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckEntireEyeZC2_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckEntireEyeZC2.SetCheck(!m_CheckEntireEyeZC2.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckEntireEyeZC3_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckEntireEyeZC3_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckEntireEyeZC3.SetCheck(!m_CheckEntireEyeZC3.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckEntireEyeZCF_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckEntireEyeZCF_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckEntireEyeZCF.SetCheck(!m_CheckEntireEyeZCF.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckClinic_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckClinic_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckClinic.SetCheck(!m_CheckClinic.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckOpacityGrade_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckOpacityGrade_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckOpacityGrade.SetCheck(!m_CheckOpacityGrade.GetCheck());
		OnOneCheckClick();
		return;
	}

}

//***************************************************************************************

