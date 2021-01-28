
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ExcelSettingsCTpage.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CExcelSettingsCTpage, CDialog)

	ON_WM_CTLCOLOR()//

	ON_WM_LBUTTONDOWN()//

	ON_BN_CLICKED(IDC_CHECKb0, OnCheckAllClick)

	ON_BN_CLICKED(IDC_CHECKb1, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb2, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb3, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb4, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb5, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb6, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb7, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb8, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb9, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb10, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECKb11, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb12, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb13, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb14, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb15, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb16, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb17, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb18, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb19, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb20, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECKb21, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb22, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb23, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb24, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb25, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb26, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb27, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb28, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb29, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb30, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECKb31, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb32, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb33, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb34, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb35, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb36, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb37, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb38, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb39, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb40, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECKb41, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb42, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb43, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb44, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb45, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb46, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb47, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb48, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb49, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb50, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECKb51, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb52, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb53, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb54, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb55, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb56, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb57, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb58, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb59, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb60, OnOneCheckClick)

	ON_BN_CLICKED(IDC_CHECKb61, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb62, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb63, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb64, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb65, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb66, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb67, OnOneCheckClick)
	ON_BN_CLICKED(IDC_CHECKb68, OnOneCheckClick)

END_MESSAGE_MAP()

//***************************************************************************************

CExcelSettingsCTpage::CExcelSettingsCTpage() : CDialog(IDD_EXCEL_CT_PAGE)
{
}

//***************************************************************************************

void CExcelSettingsCTpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REQ_IMG, m_ImgRequired1);
	DDX_Control(pDX, IDC_REQ_IMG2, m_ImgRequired2);
	DDX_Control(pDX, IDC_REQ_IMG3, m_ImgRequired3);

	DDX_Control(pDX, IDC_CHECKb0, m_CheckAll);
	DDX_Control(pDX, IDC_CHECKb1, m_CheckUserID);
	DDX_Control(pDX, IDC_CHECKb2, m_CheckFName);
	DDX_Control(pDX, IDC_CHECKb3, m_CheckMName);
	DDX_Control(pDX, IDC_CHECKb4, m_CheckLName);
	DDX_Control(pDX, IDC_CHECKb5, m_CheckDOB);
	DDX_Control(pDX, IDC_CHECKb6, m_CheckGender);
	DDX_Control(pDX, IDC_CHECKb7, m_CheckAddLine1);
	DDX_Control(pDX, IDC_CHECKb8, m_CheckAddLine2);
	DDX_Control(pDX, IDC_CHECKb9, m_CheckCity);
	DDX_Control(pDX, IDC_CHECKb10, m_CheckState);

	DDX_Control(pDX, IDC_CHECKb11, m_CheckZip);
	DDX_Control(pDX, IDC_CHECKb12, m_CheckCountry);
	DDX_Control(pDX, IDC_CHECKb13, m_CheckWPhone);
	DDX_Control(pDX, IDC_CHECKb14, m_CheckHPhone);
	DDX_Control(pDX, IDC_CHECKb15, m_CheckEmail);
	DDX_Control(pDX, IDC_CHECKb16, m_CheckNote);
	DDX_Control(pDX, IDC_CHECKb17, m_CheckGroupGUID);
	DDX_Control(pDX, IDC_CHECKb18, m_CheckGroupName);
	DDX_Control(pDX, IDC_CHECKb19, m_CheckCalID);
	DDX_Control(pDX, IDC_CHECKb20, m_CheckMode);

	DDX_Control(pDX, IDC_CHECKb21, m_CheckDate);
	DDX_Control(pDX, IDC_CHECKb22, m_CheckTime);
	DDX_Control(pDX, IDC_CHECKb23, m_CheckEye);
	DDX_Control(pDX, IDC_CHECKb24, m_CheckPrePost);
	DDX_Control(pDX, IDC_CHECKb25, m_CheckSW);
	DDX_Control(pDX, IDC_CHECKb26, m_CheckPhysicianID);
	DDX_Control(pDX, IDC_CHECKb27, m_CheckPhysFName);
	DDX_Control(pDX, IDC_CHECKb28, m_CheckPhysLName);
	DDX_Control(pDX, IDC_CHECKb29, m_CheckOperatorID);
	DDX_Control(pDX, IDC_CHECKb30, m_CheckOperFName);

	DDX_Control(pDX, IDC_CHECKb31, m_CheckOperLName);
	DDX_Control(pDX, IDC_CHECKb32, m_CheckExamNote);
	DDX_Control(pDX, IDC_CHECKb33, m_CheckLimbalX);
	DDX_Control(pDX, IDC_CHECKb34, m_CheckLimbalY);
	DDX_Control(pDX, IDC_CHECKb35, m_CheckLimbalD);
	DDX_Control(pDX, IDC_CHECKb36, m_CheckCentralPwr);
	DDX_Control(pDX, IDC_CHECKb37, m_CheckBestFitSph);
	DDX_Control(pDX, IDC_CHECKb38, m_CheckBestConCen);
	DDX_Control(pDX, IDC_CHECKb39, m_CheckBestConEcc);
	DDX_Control(pDX, IDC_CHECKb40, m_CheckBestConAsp);

	DDX_Control(pDX, IDC_CHECKb41, m_CheckRefPwrStp);
	DDX_Control(pDX, IDC_CHECKb42, m_CheckRefPwrFlt);
	DDX_Control(pDX, IDC_CHECKb43, m_CheckEffRef);
	DDX_Control(pDX, IDC_CHECKb44, m_CheckRefPwrAst);
	DDX_Control(pDX, IDC_CHECKb45, m_CheckSimKStp);
	DDX_Control(pDX, IDC_CHECKb46, m_CheckSimKFlt);
	DDX_Control(pDX, IDC_CHECKb47, m_CheckSimKAvg);
	DDX_Control(pDX, IDC_CHECKb48, m_CheckSimKDel);
	DDX_Control(pDX, IDC_CHECKb49, m_CheckIS);
	DDX_Control(pDX, IDC_CHECKb50, m_CheckAvePwr);

	DDX_Control(pDX, IDC_CHECKb51, m_CheckStpSemi1);
	DDX_Control(pDX, IDC_CHECKb52, m_CheckStpSemi2);
	DDX_Control(pDX, IDC_CHECKb53, m_CheckFltSemi1);
	DDX_Control(pDX, IDC_CHECKb54, m_CheckFltSemi2);
	DDX_Control(pDX, IDC_CHECKb55, m_CheckStpSemi12);
	DDX_Control(pDX, IDC_CHECKb56, m_CheckStpSemi22);
	DDX_Control(pDX, IDC_CHECKb57, m_CheckFltSemi12);
	DDX_Control(pDX, IDC_CHECKb58, m_CheckFltSemi22);
	DDX_Control(pDX, IDC_CHECKb59, m_CheckStpSemi13);
	DDX_Control(pDX, IDC_CHECKb60, m_CheckStpSemi23);

	DDX_Control(pDX, IDC_CHECKb61, m_CheckFltSemi13);
	DDX_Control(pDX, IDC_CHECKb62, m_CheckFltSemi23);
	DDX_Control(pDX, IDC_CHECKb63, m_CheckCornealSph);
	DDX_Control(pDX, IDC_CHECKb64, m_CheckCornealZer1);
	DDX_Control(pDX, IDC_CHECKb65, m_CheckCornealZer2);
	DDX_Control(pDX, IDC_CHECKb66, m_CheckCornealZer3);
	DDX_Control(pDX, IDC_CHECKb67, m_CheckCornealZerF);

	DDX_Control(pDX, IDC_CHECKb68, m_CheckClinic);//New added item

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
	DDX_Control(pDX, IDC_STATIC70, m_CheckState_Text);

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
	DDX_Control(pDX, IDC_STATIC72, m_CheckLimbalX_Text);
	DDX_Control(pDX, IDC_STATIC34, m_CheckLimbalY_Text);
	DDX_Control(pDX, IDC_STATIC35, m_CheckLimbalD_Text);
	DDX_Control(pDX, IDC_STATIC36, m_CheckCentralPwr_Text);
	DDX_Control(pDX, IDC_STATIC37, m_CheckBestFitSph_Text);
	DDX_Control(pDX, IDC_STATIC38, m_CheckBestConCen_Text);
	DDX_Control(pDX, IDC_STATIC39, m_CheckBestConEcc_Text);
	DDX_Control(pDX, IDC_STATIC40, m_CheckBestConAsp_Text);

	DDX_Control(pDX, IDC_STATIC41, m_CheckRefPwrStp_Text);
	DDX_Control(pDX, IDC_STATIC42, m_CheckRefPwrFlt_Text);
	DDX_Control(pDX, IDC_STATIC43, m_CheckEffRef_Text);
	DDX_Control(pDX, IDC_STATIC45, m_CheckRefPwrAst_Text);
	DDX_Control(pDX, IDC_STATIC46, m_CheckSimKStp_Text);
	DDX_Control(pDX, IDC_STATIC73, m_CheckSimKFlt_Text);
	DDX_Control(pDX, IDC_STATIC47, m_CheckSimKAvg_Text);
	DDX_Control(pDX, IDC_STATIC48, m_CheckSimKDel_Text);
	DDX_Control(pDX, IDC_STATIC49, m_CheckIS_Text);
	DDX_Control(pDX, IDC_STATIC50, m_CheckAvePwr_Text);

	DDX_Control(pDX, IDC_STATIC51, m_CheckStpSemi1_Text);
	DDX_Control(pDX, IDC_STATIC71, m_CheckStpSemi2_Text);
	DDX_Control(pDX, IDC_STATIC53, m_CheckFltSemi1_Text);
	DDX_Control(pDX, IDC_STATIC54, m_CheckFltSemi2_Text);
	DDX_Control(pDX, IDC_STATIC69, m_CheckStpSemi12_Text);
	DDX_Control(pDX, IDC_STATIC56, m_CheckStpSemi22_Text);
	DDX_Control(pDX, IDC_STATIC57, m_CheckFltSemi12_Text);
	DDX_Control(pDX, IDC_STATIC58, m_CheckFltSemi22_Text);
	DDX_Control(pDX, IDC_STATIC59, m_CheckStpSemi13_Text);
	DDX_Control(pDX, IDC_STATIC60, m_CheckStpSemi23_Text);

	DDX_Control(pDX, IDC_STATIC61, m_CheckFltSemi13_Text);
	DDX_Control(pDX, IDC_STATIC62, m_CheckFltSemi23_Text);
	DDX_Control(pDX, IDC_STATIC63, m_CheckCornealSph_Text);
	DDX_Control(pDX, IDC_STATIC64, m_CheckCornealZer1_Text);
	DDX_Control(pDX, IDC_STATIC65, m_CheckCornealZer2_Text);
	DDX_Control(pDX, IDC_STATIC66, m_CheckCornealZer3_Text);
	DDX_Control(pDX, IDC_STATIC67, m_CheckCornealZerF_Text);

	DDX_Control(pDX, IDC_STATIC68, m_CheckClinic_Text);//New added item
													   //
}

//***************************************************************************************

void CExcelSettingsCTpage::OnOneCheckClick()
{
	BOOL AllCheck = TRUE;

	if (!m_CheckUserID.GetCheck()) { AllCheck = FALSE;	goto End; }  //1
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
	if (!m_CheckLimbalX.GetCheck()) { AllCheck = FALSE;	goto End; }    //33
	if (!m_CheckLimbalY.GetCheck()) { AllCheck = FALSE;	goto End; }    //34
	if (!m_CheckLimbalD.GetCheck()) { AllCheck = FALSE;	goto End; }    //35
	if (!m_CheckCentralPwr.GetCheck()) { AllCheck = FALSE;	goto End; } //36
	if (!m_CheckBestFitSph.GetCheck()) { AllCheck = FALSE;	goto End; } //37
	if (!m_CheckBestConCen.GetCheck()) { AllCheck = FALSE;	goto End; } //38
	if (!m_CheckBestConEcc.GetCheck()) { AllCheck = FALSE;	goto End; } //39
	if (!m_CheckBestConAsp.GetCheck()) { AllCheck = FALSE;	goto End; } //40

	if (!m_CheckRefPwrStp.GetCheck()) { AllCheck = FALSE;	goto End; }  //41
	if (!m_CheckRefPwrFlt.GetCheck()) { AllCheck = FALSE;	goto End; }  //42
	if (!m_CheckEffRef.GetCheck()) { AllCheck = FALSE;	goto End; }     //43
	if (!m_CheckRefPwrAst.GetCheck()) { AllCheck = FALSE;	goto End; }  //44
	if (!m_CheckSimKStp.GetCheck()) { AllCheck = FALSE;	goto End; }    //45
	if (!m_CheckSimKFlt.GetCheck()) { AllCheck = FALSE;	goto End; }    //46
	if (!m_CheckSimKAvg.GetCheck()) { AllCheck = FALSE;	goto End; }    //47
	if (!m_CheckSimKDel.GetCheck()) { AllCheck = FALSE;	goto End; }    //48
	if (!m_CheckIS.GetCheck()) { AllCheck = FALSE;	goto End; }         //49
	if (!m_CheckAvePwr.GetCheck()) { AllCheck = FALSE;	goto End; }     //50

	if (!m_CheckStpSemi1.GetCheck()) { AllCheck = FALSE;	goto End; } //51
	if (!m_CheckStpSemi2.GetCheck()) { AllCheck = FALSE;	goto End; } //52
	if (!m_CheckFltSemi1.GetCheck()) { AllCheck = FALSE;	goto End; } //53
	if (!m_CheckFltSemi2.GetCheck()) { AllCheck = FALSE;	goto End; } //54
	if (!m_CheckStpSemi12.GetCheck()) { AllCheck = FALSE;	goto End; } //55
	if (!m_CheckStpSemi22.GetCheck()) { AllCheck = FALSE;	goto End; } //56
	if (!m_CheckFltSemi12.GetCheck()) { AllCheck = FALSE;	goto End; } //57
	if (!m_CheckFltSemi22.GetCheck()) { AllCheck = FALSE;	goto End; } //58
	if (!m_CheckStpSemi13.GetCheck()) { AllCheck = FALSE;	goto End; } //59
	if (!m_CheckStpSemi23.GetCheck()) { AllCheck = FALSE;	goto End; } //60

	if (!m_CheckFltSemi13.GetCheck()) { AllCheck = FALSE;	goto End; } //61
	if (!m_CheckFltSemi23.GetCheck()) { AllCheck = FALSE;	goto End; } //62
	if (!m_CheckCornealSph.GetCheck()) { AllCheck = FALSE;	goto End; } //63
	if (!m_CheckCornealZer1.GetCheck()) { AllCheck = FALSE;	goto End; } //64
	if (!m_CheckCornealZer2.GetCheck()) { AllCheck = FALSE;	goto End; } //65
	if (!m_CheckCornealZer3.GetCheck()) { AllCheck = FALSE;	goto End; } //66
	if (!m_CheckCornealZerF.GetCheck()) { AllCheck = FALSE;	goto End; } //67

	if (!m_CheckClinic.GetCheck()) { AllCheck = FALSE;	goto End; } //68

End:
	m_CheckAll.SetCheck(AllCheck);

	Invalidate(FALSE);//
}

//***************************************************************************************
void CExcelSettingsCTpage::OnCheckAllClick()
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
	m_CheckLimbalX.SetCheck(Check);    //33
	m_CheckLimbalY.SetCheck(Check);    //34
	m_CheckLimbalD.SetCheck(Check);    //35
	m_CheckCentralPwr.SetCheck(Check); //36
	m_CheckBestFitSph.SetCheck(Check); //37
	m_CheckBestConCen.SetCheck(Check); //38
	m_CheckBestConEcc.SetCheck(Check); //39
	m_CheckBestConAsp.SetCheck(Check); //40

	m_CheckRefPwrStp.SetCheck(Check);  //41
	m_CheckRefPwrFlt.SetCheck(Check);  //42
	m_CheckEffRef.SetCheck(Check);     //43
	m_CheckRefPwrAst.SetCheck(Check);  //44
	m_CheckSimKStp.SetCheck(Check);    //45
	m_CheckSimKFlt.SetCheck(Check);    //46
	m_CheckSimKAvg.SetCheck(Check);    //47
	m_CheckSimKDel.SetCheck(Check);    //48
	m_CheckIS.SetCheck(Check);         //49
	m_CheckAvePwr.SetCheck(Check);     //50

	m_CheckStpSemi1.SetCheck(Check); //51
	m_CheckStpSemi2.SetCheck(Check); //52
	m_CheckFltSemi1.SetCheck(Check); //53
	m_CheckFltSemi2.SetCheck(Check); //54
	m_CheckStpSemi12.SetCheck(Check); //55
	m_CheckStpSemi22.SetCheck(Check); //56
	m_CheckFltSemi12.SetCheck(Check); //57
	m_CheckFltSemi22.SetCheck(Check); //58
	m_CheckStpSemi13.SetCheck(Check); //59
	m_CheckStpSemi23.SetCheck(Check); //60

	m_CheckFltSemi13.SetCheck(Check); //61
	m_CheckFltSemi23.SetCheck(Check); //62
	m_CheckCornealSph.SetCheck(Check); //63
	m_CheckCornealZer1.SetCheck(Check); //64
	m_CheckCornealZer2.SetCheck(Check); //65
	m_CheckCornealZer3.SetCheck(Check); //66
	m_CheckCornealZerF.SetCheck(Check); //67 

	m_CheckClinic.SetCheck(Check); //68 

	Invalidate(FALSE);//
}

//***************************************************************************************
//
HBRUSH CExcelSettingsCTpage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		if (m_CheckDOB.GetCheck())	pDC->SetTextColor(BLUE);
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
		if (m_CheckCity.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC10)
	{
		if (m_CheckState.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC11)
	{
		if (m_CheckZip.GetCheck())	pDC->SetTextColor(BLUE);
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

	if (iID == IDC_STATIC72)
	{
		if (m_CheckLimbalX.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC34)
	{
		if (m_CheckLimbalY.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC35)
	{
		if (m_CheckLimbalD.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC36)
	{
		if (m_CheckCentralPwr.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC37)
	{
		if (m_CheckBestFitSph.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC38)
	{
		if (m_CheckBestConCen.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC39)
	{
		if (m_CheckBestConEcc.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC40)
	{
		if (m_CheckBestConAsp.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC41)
	{
		if (m_CheckRefPwrStp.GetCheck())
			pDC->SetTextColor(BLUE);
		else
			pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC42)
	{
		if (m_CheckRefPwrFlt.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC43)
	{
		if (m_CheckEffRef.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC45)
	{
		if (m_CheckRefPwrAst.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC46)
	{
		if (m_CheckSimKStp.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC73)
	{
		if (m_CheckSimKFlt.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC47)
	{
		if (m_CheckSimKAvg.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC48)
	{
		if (m_CheckSimKDel.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC49)
	{
		if (m_CheckIS.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC50)
	{
		if (m_CheckAvePwr.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC51)
	{
		if (m_CheckStpSemi1.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC71)
	{
		if (m_CheckStpSemi2.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC53)
	{
		if (m_CheckFltSemi1.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC54)
	{
		if (m_CheckFltSemi2.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC69)
	{
		if (m_CheckStpSemi12.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC56)
	{
		if (m_CheckStpSemi22.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC57)
	{
		if (m_CheckFltSemi12.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC58)
	{
		if (m_CheckFltSemi22.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC59)
	{
		if (m_CheckStpSemi13.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC60)
	{
		if (m_CheckStpSemi23.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC61)
	{
		if (m_CheckFltSemi13.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC62)
	{
		if (m_CheckFltSemi23.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC63)
	{
		if (m_CheckCornealSph.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC64)
	{
		if (m_CheckCornealZer1.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC65)
	{
		if (m_CheckCornealZer2.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC66)
	{
		if (m_CheckCornealZer3.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC67)
	{
		if (m_CheckCornealZerF.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	if (iID == IDC_STATIC68)
	{
		if (m_CheckClinic.GetCheck())	pDC->SetTextColor(BLUE);
		else			                pDC->SetTextColor(DARK_GRAY);
	}

	return hbr;
}

//***************************************************************************************

//
void CExcelSettingsCTpage::OnLButtonDown(uint nFlags, CPoint Point)
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

	//***
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

	//***
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

	//***
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
	m_CheckLimbalX_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckLimbalX_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckLimbalX.SetCheck(!m_CheckLimbalX.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckLimbalY_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckLimbalY_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckLimbalY.SetCheck(!m_CheckLimbalY.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckLimbalD_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckLimbalD_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckLimbalD.SetCheck(!m_CheckLimbalD.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCentralPwr_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCentralPwr_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCentralPwr.SetCheck(!m_CheckCentralPwr.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckBestFitSph_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckBestFitSph_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckBestFitSph.SetCheck(!m_CheckBestFitSph.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckBestConCen_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckBestConCen_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckBestConCen.SetCheck(!m_CheckBestConCen.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckBestConEcc_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckBestConEcc_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckBestConEcc.SetCheck(!m_CheckBestConEcc.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckBestConAsp_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckBestConAsp_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckBestConAsp.SetCheck(!m_CheckBestConAsp.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckRefPwrStp_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckRefPwrStp_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckRefPwrStp.SetCheck(!m_CheckRefPwrStp.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckRefPwrFlt_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckRefPwrFlt_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckRefPwrFlt.SetCheck(!m_CheckRefPwrFlt.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckEffRef_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckEffRef_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckEffRef.SetCheck(!m_CheckEffRef.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckRefPwrAst_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckRefPwrAst_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckRefPwrAst.SetCheck(!m_CheckRefPwrAst.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckSimKStp_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckSimKStp_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckSimKStp.SetCheck(!m_CheckSimKStp.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckSimKFlt_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckSimKFlt_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckSimKFlt.SetCheck(!m_CheckSimKFlt.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckSimKAvg_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckSimKAvg_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckSimKAvg.SetCheck(!m_CheckSimKAvg.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckSimKDel_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckSimKDel_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckSimKDel.SetCheck(!m_CheckSimKDel.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckIS_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckIS_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckIS.SetCheck(!m_CheckIS.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckAvePwr_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckAvePwr_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckAvePwr.SetCheck(!m_CheckAvePwr.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckStpSemi1_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckStpSemi1_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckStpSemi1.SetCheck(!m_CheckStpSemi1.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckStpSemi2_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckStpSemi2_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckStpSemi2.SetCheck(!m_CheckStpSemi2.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckFltSemi1_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckFltSemi1_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckFltSemi1.SetCheck(!m_CheckFltSemi1.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckFltSemi2_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckFltSemi2_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckFltSemi2.SetCheck(!m_CheckFltSemi2.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckStpSemi12_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckStpSemi12_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckStpSemi12.SetCheck(!m_CheckStpSemi12.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckStpSemi22_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckStpSemi22_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckStpSemi22.SetCheck(!m_CheckStpSemi22.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckFltSemi12_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckFltSemi12_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckFltSemi12.SetCheck(!m_CheckFltSemi12.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckFltSemi22_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckFltSemi22_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckFltSemi22.SetCheck(!m_CheckFltSemi22.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckStpSemi13_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckStpSemi13_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckStpSemi13.SetCheck(!m_CheckStpSemi13.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckStpSemi23_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckStpSemi23_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckStpSemi23.SetCheck(!m_CheckStpSemi23.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckFltSemi13_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckFltSemi13_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckFltSemi13.SetCheck(!m_CheckFltSemi13.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckFltSemi23_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckFltSemi23_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckFltSemi23.SetCheck(!m_CheckFltSemi23.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCornealSph_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCornealSph_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCornealSph.SetCheck(!m_CheckCornealSph.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCornealZer1_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCornealZer1_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCornealZer1.SetCheck(!m_CheckCornealZer1.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCornealZer2_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCornealZer2_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCornealZer2.SetCheck(!m_CheckCornealZer2.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCornealZer3_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCornealZer3_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCornealZer3.SetCheck(!m_CheckCornealZer3.GetCheck());
		OnOneCheckClick();
		return;
	}

	//***
	OriPoint = point;
	m_CheckCornealZerF_Text.ScreenToClient(&OriPoint);
	::GetClientRect(m_CheckCornealZerF_Text.GetSafeHwnd(), &rect);
	rect.left -= 5;
	rect.right += 3;

	if (rect.PtInRect(OriPoint))
	{
		m_CheckCornealZerF.SetCheck(!m_CheckCornealZerF.GetCheck());
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

}
//

//***************************************************************************************
