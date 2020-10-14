//***************************************************************************************

#pragma once

//***************************************************************************************
#include "Settings.h"

class CSettingsDlg : public CDialog
{
public:

	CSettingsDlg(CWnd* pParentWnd);

private:

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;//530

	BOOL      m_BackSomeDefaults;//520

								 
	BOOL  m_CtlDef;
	BOOL  m_GUIDef;
	BOOL  m_ExcDef;
	

	BOOL          m_OriAliMethod;//531

	BOOL          m_TaskChanged[5];//530
	CTaskSetting  m_DlgTask[5];//530

							   //530
	real  m_IOLSel_Alpha[2];//530 IOL SELECTION threshold
	real  m_IOLSel_CHOA;//530 IOL SELECTION Corneal HOAs threshold
	real  m_IOLSel_IHOA;//530 IOL SELECTION Corneal HOAs threshold
	real  m_IOLSel_Astig;//530 IOL SELECTION threshold
	real  m_IOLSel_Sph_Abbe[4];//530 IOL SELECTION threshold
							   //530

	int   m_SelTaskRadio;//531

	CButton   m_PosCylCheck;
	CComboBox m_VertexComboBox;
	CComboBox m_ZonesComboBox[3];
	CButton   m_ProbingEnabledCheck;
	CButton   m_FoggingEnabledCheck;
	CEdit     m_ManualScanDiameterEdit;
	CEdit     m_AutoScanDiameterEdit;
	CButton   m_AnimateAfterNewExamCheck;
	CButton   m_MMDDYYYYRadio;
	CButton   m_DDMMYYYYRadio;
	CButton   m_YYYYMMDDRadio;
	CEdit     m_Button1Edit;
	CEdit     m_Button2Edit;
	CEdit     m_Button3Edit;
	CEdit     m_Button4Edit;
	CEdit     m_Button5Edit;
	CEdit     m_Button6Edit;
	CEdit     m_Button7Edit;
	CEdit     m_Button8Edit;
	CEdit     m_Button9Edit;
	CEdit     m_Button10Edit;
	CEdit     m_Button11Edit;
	CEdit     m_Button12Edit;
	CEdit     m_Button13Edit;//531
	CButton   m_WavetouchCheck;
	CButton   m_AccommTargetLightOnCheck;
	CButton   m_ShowSendARKRCheck;
	CButton   m_UseJPGFormatRadio;
	CButton   m_UsePNGFormatRadio;
	CButton   m_IncludeLADsCheck;
	CEdit     m_FolderNameEdit;
	CButton   m_FolderBrowseButton;
	CButton   m_WFVrfRadio;
	CButton   m_WFVfaRadio;
	CButton   m_WFCmpRadio;
	CButton   m_WFSum1Radio;
	CButton   m_WFSum2Radio;
	CButton   m_WFSum3Radio;
	CButton   m_WFSum4Radio;
	CButton   m_CTVrfRadio;
	CButton   m_CTDblRadio;
	CButton   m_CTCmpRadio;
	CButton   m_CTSum1Radio;
	CButton   m_CTSum2Radio;
	CButton   m_CTSum3Radio;
	CButton   m_CTSum4Radio;
	CButton   m_INSum1Radio;
	CButton   m_INSum2Radio;
	CButton   m_INSum3Radio;
	CButton   m_INSum4Radio;
	CButton   m_INSum5Radio;//530
	CButton   m_INSum6Radio;//530
	CButton   m_INSum7Radio;//531 OU Overview
	CButton   m_AlignmentPupilRadio;
	CButton   m_Alignment4IrdotsRadio;
	CButton   m_Round8Radio;
	CButton   m_Round100Radio;
	CButton   m_CancelButton;
	CButton   m_OKButton;
	CButton   m_DefaultButton;
	CButton   m_SaveButton;
	CButton   m_LoadButton;
	CButton   m_CapColorImgCheck;//[520]
	CButton   m_CapColorImgAutoRadio;//520
	CButton   m_CapColorImgManuRadio;//520

	CButton   m_ExcelSettingButton;//520 121126
	CButton   m_ExcelExportCheck;//520 121126
	CEdit     m_FolderNameEdit2;//520  IDC_FOLDERNAME_EDIT2
	CButton   m_FolderBrowseButton2;//520 IDC_FOLDER_BROWSE2 

	CButton   m_IolSelButton;//530 iol selection assistant button

	CButton   m_Task_ShortCutCheck;//530 

	CStatic   m_Static[5];//531

	CButton   m_SubTask_Check[5];//531

	CButton   m_WFSum_Check[7];//531 
	CButton   m_CTSum_Check[7];//531 
	CButton   m_INSum_Check[8];//531 

	CEdit     m_TargetTimeEdit;//530
	CStatic   m_TargetTimeLabel;//530

	CButton   m_SimkRadio;//530
	CButton   m_RefkRadio;//530

	CButton   m_TaskRadio[5];//530

	CEdit     m_TaskEdit[5];//530

	CButton   m_TaskAdBtn;//530

	CButton m_Super_ExamCheck;//531

	void      HideTaskSettings();//530
	void      ShowTaskSettings();//530
	CTaskSetting  SetDefTask0();//530
	CTaskSetting  SetDefTask1();//530
	CTaskSetting  SetDefTask2();//530
	CTaskSetting  SetDefTask3();//530
	CTaskSetting  SetDefTask4();//530

	void  SetDefTask_WCVA(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_CTSU(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_CTKE(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_CHAN(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_WFAN(CWndSettings InWndSetting[4]);//530 

	void  SetDefTask_WFVA(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_WFRM(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_CT3D(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_WCSU(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_WCAT(CWndSettings InWndSetting[4]);//531 Astigmatism Source

	afx_msg void OnProbingButtonClicked();
	afx_msg void OnFolderBrowseButtonClicked();
	afx_msg void OnDefaultButtonClicked();
	afx_msg void OnLoadButtonClicked();
	afx_msg void OnSaveButtonClicked();

	afx_msg void OnColImgButtonClicked();//520
	afx_msg void OnColImgAutoManRadio1Clicked();//520
	afx_msg void OnColImgAutoManRadio2Clicked();//520

	afx_msg void OnExcelSettingBtnClicked();//520 for excel export setting
	afx_msg void OnExcelCheckClicked();//520 for excel export setting
	afx_msg void OnFolderBrowse2ButtonClicked();//520 for excel export setting

	afx_msg void OnIOLSelectionBtnClicked();//530  

	afx_msg void OnInRadio1Clicked();//530  
	afx_msg void OnInRadio2Clicked();//530  
	afx_msg void OnInRadio3Clicked();//530  
	afx_msg void OnInRadio4Clicked();//530  
	afx_msg void OnInRadio5Clicked();//530  
	afx_msg void OnInRadio6Clicked();//530
	afx_msg void OnInRadio7Clicked();//531


	afx_msg void OnTaskAdBtnClicked();//530  
	afx_msg void OnTaskShowBtnClicked();//530 

	afx_msg void OnTargetLightBtnClicked();//530  

	afx_msg void OnSuperExamBtnClicked();

	void         OnSubTasckChkClicked(int i);//531
	afx_msg void OnSubTasck1ChkClicked() { OnSubTasckChkClicked(0); };//531 
	afx_msg void OnSubTasck2ChkClicked() { OnSubTasckChkClicked(1); };//531 
	afx_msg void OnSubTasck3ChkClicked() { OnSubTasckChkClicked(2); };//531 
	afx_msg void OnSubTasck4ChkClicked() { OnSubTasckChkClicked(3); };//531 
	afx_msg void OnSubTasck5ChkClicked() { OnSubTasckChkClicked(4); };//531 

	void         OnSubTasckRioClicked(int i);//531
	afx_msg void OnSubTasck1RioClicked() { OnSubTasckRioClicked(0); };//531 
	afx_msg void OnSubTasck2RioClicked() { OnSubTasckRioClicked(1); };//531 
	afx_msg void OnSubTasck3RioClicked() { OnSubTasckRioClicked(2); };//531 
	afx_msg void OnSubTasck4RioClicked() { OnSubTasckRioClicked(3); };//531 
	afx_msg void OnSubTasck5RioClicked() { OnSubTasckRioClicked(4); };//531 


	void         OnWFChkClicked(int i);//531
	afx_msg void OnWFChk2Clicked() { OnWFChkClicked(1); };//531 
	afx_msg void OnWFChk3Clicked() { OnWFChkClicked(2); };//531 
	afx_msg void OnWFChk4Clicked() { OnWFChkClicked(3); };//531 
	afx_msg void OnWFChk5Clicked() { OnWFChkClicked(4); };//531 
	afx_msg void OnWFChk6Clicked() { OnWFChkClicked(5); };//531 
	afx_msg void OnWFChk7Clicked() { OnWFChkClicked(6); };//531 

	void         OnCTChkClicked(int i);//531
	afx_msg void OnCTChk1Clicked() { OnCTChkClicked(1); };//531 
	afx_msg void OnCTChk2Clicked() { OnCTChkClicked(2); };//531 
	afx_msg void OnCTChk3Clicked() { OnCTChkClicked(3); };//531 
	afx_msg void OnCTChk4Clicked() { OnCTChkClicked(4); };//531 
	afx_msg void OnCTChk5Clicked() { OnCTChkClicked(5); };//531 
	afx_msg void OnCTChk6Clicked() { OnCTChkClicked(6); };//531 

	void         OnINChkClicked(int i);//531
	afx_msg void OnINChk0Clicked() { OnINChkClicked(0); };//531 
	afx_msg void OnINChk1Clicked() { OnINChkClicked(1); };//531 
	afx_msg void OnINChk2Clicked() { OnINChkClicked(2); };//531 
	afx_msg void OnINChk3Clicked() { OnINChkClicked(3); };//531 
	afx_msg void OnINChk4Clicked() { OnINChkClicked(4); };//531 
	afx_msg void OnINChk5Clicked() { OnINChkClicked(5); };//531 
	afx_msg void OnINChk6Clicked() { OnINChkClicked(6); };//531 



	void  SetDef_ExList_Width();//520
	BOOL  CheckSetting();//530
	virtual void SetSetting();//while save the setting file, read the setting values
	virtual void SetSelection();//wile load the seeting file, let the checked GUI to unchecked
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
