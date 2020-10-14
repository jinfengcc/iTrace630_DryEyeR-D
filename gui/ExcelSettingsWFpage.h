//***************************************************************************************

#pragma once

//***************************************************************************************

class CExcelSettingsWFpage : public CDialog
{
public:

	CExcelSettingsWFpage();

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
	CButton m_CheckAngelKDist; //33
	CButton m_CheckAngelKDeg;  //34
							   //CButton m_CheckAngelADist; //35
							   //CButton m_CheckAngelADeg;  //36
							   //CButton m_CheckLimbalX;    //37
							   //CButton m_CheckLimbalY;    //38
							   //CButton m_CheckLimbalD;    //39
	CButton m_CheckPupilX;     //40

	CButton m_CheckPupilY;     //41
	CButton m_CheckPupilD;     //42
	CButton m_CheckScanD;      //43
	CButton m_CheckTraceRef;   //44
	CButton m_CheckRef1Zone;   //45
	CButton m_CheckRef2Zone;   //46
	CButton m_CheckRef3Zone;   //47
	CButton m_CheckRef4Zone;   //48
	CButton m_CheckEntireEyeZC1; //49
	CButton m_CheckEntireEyeZC2; //50
	CButton m_CheckEntireEyeZC3; //51
	CButton m_CheckEntireEyeZCF; //52

	CButton m_CheckClinic; //53

	CButton m_CheckOpacityGrade; //54   


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
	CStatic m_CheckAngelKDist_Text; //33
	CStatic m_CheckAngelKDeg_Text;  //34
									//CStatic m_CheckAngelADist_Text; //35
									//CStatic m_CheckAngelADeg_Text;  //36
									//CStatic m_CheckLimbalX_Text;    //37
									//CStatic m_CheckLimbalY_Text;    //38
									//CStatic m_CheckLimbalD_Text;    //39
	CStatic m_CheckPupilX_Text;     //40

	CStatic m_CheckPupilY_Text;     //41
	CStatic m_CheckPupilD_Text;     //42
	CStatic m_CheckScanD_Text;      //43
	CStatic m_CheckTraceRef_Text;   //44
	CStatic m_CheckRef1Zone_Text;   //45
	CStatic m_CheckRef2Zone_Text;   //46
	CStatic m_CheckRef3Zone_Text;   //47
	CStatic m_CheckRef4Zone_Text;   //48
	CStatic m_CheckEntireEyeZC1_Text; //49
	CStatic m_CheckEntireEyeZC2_Text; //50
	CStatic m_CheckEntireEyeZC3_Text; //51
	CStatic m_CheckEntireEyeZCF_Text; //52

	CStatic m_CheckClinic_Text; //53

	CStatic m_CheckOpacityGrade_Text; //54   

private:

	virtual void DoDataExchange(CDataExchange* pDX) override;

	afx_msg void OnCheckAllClick();

	afx_msg void OnOneCheckClick();

	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);//

	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);//

	DECLARE_MESSAGE_MAP()
};
