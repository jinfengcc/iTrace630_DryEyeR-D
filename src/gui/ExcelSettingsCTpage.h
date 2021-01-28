//***************************************************************************************

#pragma once

//***************************************************************************************

class CExcelSettingsCTpage : public CDialog
{
public:

	CExcelSettingsCTpage();

	CStatic m_ImgRequired1;
	CStatic m_ImgRequired2;
	CStatic m_ImgRequired3;

	CButton m_CheckAll;

	CButton m_CheckUserID;  //1
	CButton m_CheckFName;   //2
	CButton m_CheckMName;   //3
	CButton m_CheckLName;   //4
	CButton m_CheckDOB;     //5
	CButton m_CheckGender;  //6
	CButton m_CheckAddLine1;//7
	CButton m_CheckAddLine2;//8
	CButton m_CheckCity;    //9
	CButton m_CheckState;   //10

	CButton m_CheckZip;      //11
	CButton m_CheckCountry;  //12
	CButton m_CheckWPhone;   //13
	CButton m_CheckHPhone;   //14
	CButton m_CheckEmail;    //15
	CButton m_CheckNote;     //16
	CButton m_CheckGroupGUID;//17
	CButton m_CheckGroupName;//18
	CButton m_CheckCalID;    //19
	CButton m_CheckMode;     //20

	CButton m_CheckDate;       //21
	CButton m_CheckTime;       //22
	CButton m_CheckEye;        //23
	CButton m_CheckPrePost;    //24
	CButton m_CheckSW;         //25
	CButton m_CheckPhysicianID;//26
	CButton m_CheckPhysFName;  //27
	CButton m_CheckPhysLName;  //28
	CButton m_CheckOperatorID; //29
	CButton m_CheckOperFName;  //30

	CButton m_CheckOperLName;  //31
	CButton m_CheckExamNote;   //32
	CButton m_CheckLimbalX;    //33
	CButton m_CheckLimbalY;    //34
	CButton m_CheckLimbalD;    //35
	CButton m_CheckCentralPwr; //36
	CButton m_CheckBestFitSph; //37
	CButton m_CheckBestConCen; //38
	CButton m_CheckBestConEcc; //39
	CButton m_CheckBestConAsp; //40

	CButton m_CheckRefPwrStp;  //41
	CButton m_CheckRefPwrFlt;  //42
	CButton m_CheckEffRef;     //43
	CButton m_CheckRefPwrAst;  //44
	CButton m_CheckSimKStp;    //45
	CButton m_CheckSimKFlt;    //46
	CButton m_CheckSimKAvg;    //47
	CButton m_CheckSimKDel;    //48
	CButton m_CheckIS;         //49
	CButton m_CheckAvePwr;     //50

	CButton m_CheckStpSemi1; //51
	CButton m_CheckStpSemi2; //52
	CButton m_CheckFltSemi1; //53
	CButton m_CheckFltSemi2; //54
	CButton m_CheckStpSemi12; //55
	CButton m_CheckStpSemi22; //56
	CButton m_CheckFltSemi12; //57
	CButton m_CheckFltSemi22; //58
	CButton m_CheckStpSemi13; //59
	CButton m_CheckStpSemi23; //60

	CButton m_CheckFltSemi13; //61
	CButton m_CheckFltSemi23; //62
	CButton m_CheckCornealSph; //63
	CButton m_CheckCornealZer1; //64
	CButton m_CheckCornealZer2; //65
	CButton m_CheckCornealZer3; //66
	CButton m_CheckCornealZerF; //67

	CButton m_CheckClinic; //68

						   //
	CStatic m_CheckAll_Text;

	CStatic m_CheckUserID_Text;  //1
	CStatic m_CheckFName_Text;   //2
	CStatic m_CheckMName_Text;   //3
	CStatic m_CheckLName_Text;   //4
	CStatic m_CheckDOB_Text;     //5
	CStatic m_CheckGender_Text;  //6
	CStatic m_CheckAddLine1_Text;//7
	CStatic m_CheckAddLine2_Text;//8
	CStatic m_CheckCity_Text;    //9
	CStatic m_CheckState_Text;   //10

	CStatic m_CheckZip_Text;      //11
	CStatic m_CheckCountry_Text;  //12
	CStatic m_CheckWPhone_Text;   //13
	CStatic m_CheckHPhone_Text;   //14
	CStatic m_CheckEmail_Text;    //15
	CStatic m_CheckNote_Text;     //16
	CStatic m_CheckGroupGUID_Text;//17
	CStatic m_CheckGroupName_Text;//18
	CStatic m_CheckCalID_Text;    //19
	CStatic m_CheckMode_Text;     //20

	CStatic m_CheckDate_Text;       //21
	CStatic m_CheckTime_Text;       //22
	CStatic m_CheckEye_Text;        //23
	CStatic m_CheckPrePost_Text;    //24
	CStatic m_CheckSW_Text;         //25
	CStatic m_CheckPhysicianID_Text;//26
	CStatic m_CheckPhysFName_Text;  //27
	CStatic m_CheckPhysLName_Text;  //28
	CStatic m_CheckOperatorID_Text; //29
	CStatic m_CheckOperFName_Text;  //30

	CStatic m_CheckOperLName_Text;  //31
	CStatic m_CheckExamNote_Text;   //32
	CStatic m_CheckLimbalX_Text;    //33
	CStatic m_CheckLimbalY_Text;    //34
	CStatic m_CheckLimbalD_Text;    //35
	CStatic m_CheckCentralPwr_Text; //36
	CStatic m_CheckBestFitSph_Text; //37
	CStatic m_CheckBestConCen_Text; //38
	CStatic m_CheckBestConEcc_Text; //39
	CStatic m_CheckBestConAsp_Text; //40

	CStatic m_CheckRefPwrStp_Text;  //41
	CStatic m_CheckRefPwrFlt_Text;  //42
	CStatic m_CheckEffRef_Text;     //43
	CStatic m_CheckRefPwrAst_Text;  //44
	CStatic m_CheckSimKStp_Text;    //45
	CStatic m_CheckSimKFlt_Text;    //46
	CStatic m_CheckSimKAvg_Text;    //47
	CStatic m_CheckSimKDel_Text;    //48
	CStatic m_CheckIS_Text;         //49
	CStatic m_CheckAvePwr_Text;     //50

	CStatic m_CheckStpSemi1_Text; //51
	CStatic m_CheckStpSemi2_Text; //52
	CStatic m_CheckFltSemi1_Text; //53
	CStatic m_CheckFltSemi2_Text; //54
	CStatic m_CheckStpSemi12_Text; //55
	CStatic m_CheckStpSemi22_Text; //56
	CStatic m_CheckFltSemi12_Text; //57
	CStatic m_CheckFltSemi22_Text; //58
	CStatic m_CheckStpSemi13_Text; //59
	CStatic m_CheckStpSemi23_Text; //60

	CStatic m_CheckFltSemi13_Text; //61
	CStatic m_CheckFltSemi23_Text; //62
	CStatic m_CheckCornealSph_Text; //63
	CStatic m_CheckCornealZer1_Text; //64
	CStatic m_CheckCornealZer2_Text; //65
	CStatic m_CheckCornealZer3_Text; //66
	CStatic m_CheckCornealZerF_Text; //67

	CStatic m_CheckClinic_Text; //68
								//

private:

	virtual void DoDataExchange(CDataExchange* pDX) override;

	afx_msg void OnCheckAllClick();
	afx_msg void OnOneCheckClick();

	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);//

	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);//

	DECLARE_MESSAGE_MAP()
};
