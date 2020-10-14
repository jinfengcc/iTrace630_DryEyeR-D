//***************************************************************************************

#pragma once

//***************************************************************************************
#include "Settings.h"

class CSettingsComboDlg : public CDialog
{
public:

	CSettingsComboDlg(CWnd* pParentWnd);

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
	CStatic   m_PosCylText;//
	CComboBox m_VertexComboBox;
	CComboBox m_ZonesComboBox[3];
	CButton   m_ProbingEnabledCheck;
	CStatic   m_ProbingEnabledText;//
	CButton   m_FoggingEnabledCheck;
	CStatic   m_FoggingEnabledText;//
	CEdit     m_ManualScanDiameterEdit;
	CEdit     m_AutoScanDiameterEdit;
	CButton   m_AnimateAfterNewExamCheck;
	CStatic   m_AnimateAfterNewExamText;//
	CButton   m_MMDDYYYYRadio;
	CButton   m_DDMMYYYYRadio;
	CButton   m_YYYYMMDDRadio;
	CStatic   m_MMDDYYYYText;//
	CStatic   m_DDMMYYYYText;//
	CStatic   m_YYYYMMDDText;//
	CEdit     m_Button1Edit;
	CEdit     m_Button2Edit;
	CEdit     m_Button3Edit;
	CEdit     m_Button4Edit;
	CEdit     m_Button4Edit2;
	CEdit     m_Button5Edit;
	CEdit     m_Button6Edit;
	CEdit     m_Button7Edit;
	CEdit     m_Button8Edit;
	CEdit     m_Button8Edit2;//531
	CEdit     m_Button9Edit;
	CEdit     m_Button10Edit;
	CEdit     m_Button11Edit;
	CEdit     m_Button12Edit;
	CEdit     m_Button13Edit;//531
	CEdit     m_Button14Edit;//
	CEdit     m_Button15Edit;//  WFCT Angle K/A

	CButton   m_WavetouchCheck;
	CButton   m_WavetouchText;
	CButton   m_AccommTargetLightOnCheck;
	CButton   m_AccommTargetLightOnText;
	CButton   m_ShowSendARKRCheck;
	CButton   m_ShowSendARKRText;
	CButton   m_UseJPGFormatRadio;
	CButton   m_UseJPGFormatText;
	CButton   m_UsePNGFormatRadio;
	CButton   m_UsePNGFormatText;
	CButton   m_IncludeLADsCheck;
	CButton   m_IncludeLADsText;
	CEdit     m_FolderNameEdit;
	CButton   m_FolderBrowseButton;
	CButton   m_WFVrfRadio;
	CStatic   m_WFVrfText;
	CButton   m_WFVfaRadio;
	CStatic   m_WFVfaText;
	CButton   m_WFCmpRadio;
	CButton   m_WFNearVisionRadio;//6.2.0
	CStatic   m_WFCmpText;
	CStatic   m_WFNearVisonText;//6.2.0
	CButton   m_WFSum1Radio;
	CButton   m_WFSum2Radio;
	CButton   m_WFSum3Radio;
	CButton   m_WFSum4Radio;
	CButton   m_WFSum5Radio;
	CButton   m_WFSum6Radio;// 'depth of focus comparison' 
	CButton   m_CTVrfRadio;
	CStatic   m_CTVrfText;
	CButton   m_CTDblRadio;
	CStatic   m_CTDblText;
	CButton   m_CTCmpRadio;
	CStatic   m_CTCmpText;
	CButton   m_CTSum1Radio;
	CButton   m_CTSum2Radio;
	CButton   m_CTSum3Radio;
	CButton   m_CTSum4Radio;
	CButton   m_CTSum5Radio;
	CButton   m_INSum1Radio;
	CButton   m_INSum2Radio;
	CButton   m_INSum3Radio;
	CButton   m_INSum4Radio;
	CStatic   m_INSum4Text;
	CButton   m_INSum5Radio;//530
	CStatic   m_INSum5Text;//
	CButton   m_INSum6Radio;//531
	CStatic   m_INSum6Text;//
	CButton   m_INSum7Radio;//531
	CStatic   m_INSum7Text;//
	CButton   m_INSum8Radio;//531 
	CButton   m_INSum9Radio;
	CButton   m_INSum10Radio;// WFCT Angle K/A
	CButton   m_INSum11Radio;// Toric Alignment
	CStatic   m_INSum11Text;

	CStatic   m_SelNumText[3];

	CButton   m_AlignmentPupilRadio;
	CButton   m_AlignmentPupilText;
	CButton   m_Alignment4IrdotsRadio;
	CButton   m_Alignment4IrdotsText;
	CButton   m_Round8Radio;
	CStatic   m_Round8Text;
	CButton   m_Round100Radio;
	CStatic   m_Round100Text;
	CButton   m_CancelButton;
	CButton   m_OKButton;
	CButton   m_DefaultButton;
	CButton   m_SaveButton;
	CButton   m_LoadButton;
	CButton   m_CapColorImgCheck;
	CStatic   m_CapColorImgText;
	CButton   m_CapColorImgAutoRadio;
	CButton   m_CapColorImgManuRadio;
	CStatic   m_CapColorImgAutoText;
	CStatic   m_CapColorImgManuText;

	CButton   m_ExcelSettingButton;
	CButton   m_ExcelExportCheck;
	CStatic   m_ExcelExportText;
	CEdit     m_FolderNameEdit2;//IDC_FOLDERNAME_EDIT2
	CButton   m_FolderBrowseButton2;// IDC_FOLDER_BROWSE2 

	CButton   m_IolSelButton;//iol selection assistant button

	CButton   m_Task_ShortCutCheck;
	CStatic   m_Task_ShortCutText;

	CStatic   m_Static[4];

	CButton   m_SubTask_Check[5];

	CButton   m_WFSum_Check[10];// 6.2.0 from [9] to [10] add the 'WF Near Vision'
	CButton   m_CTSum_Check[8];
	CButton   m_INSum_Check[11];// Add WFCT Angle K/A (Transform from WF Screen) and Toric Alignment

	CEdit     m_TargetTimeEdit;
	CStatic   m_TargetTimeLabel;

	CButton   m_SimkRadio;
	CButton   m_RefkRadio;//530
	CStatic   m_SimkText;//
	CStatic   m_RefkText;// 

	CButton   m_TaskRadio[5];//530

	CEdit     m_TaskEdit[5];//530

	CButton   m_TaskAdBtn;//530

	CButton  m_Super_ExamCheck;//531
	CButton  m_Super_ExamText;//

	void      HideTaskSettings();//530
	void      ShowTaskSettings();//530
	CTaskSetting  SetDefTask0();//530
	CTaskSetting  SetDefTask1();//530
	CTaskSetting  SetDefTask2();//530
	CTaskSetting  SetDefTask3();//530
	CTaskSetting  SetDefTask4();//530

	void  SetDefTask_WCVA(CWndSettings InWndSetting[4]);
	void  SetDefTask_CTSU(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_CTKE(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_CHAN(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_WFAN(CWndSettings InWndSetting[4]);//530 

	void  SetDefTask_WFVA(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_WFRM(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_CT3D(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_WCSU(CWndSettings InWndSetting[4]);//530
	void  SetDefTask_WCAT(CWndSettings InWndSetting[4]);//531 Astigmatism Source
	void  SetDefTask_WCDP(CWndSettings InWndSetting[4]);//531 Depth of focus

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
	afx_msg void OnWFChk8Clicked() { OnWFChkClicked(7); };//531 
	afx_msg void OnWFChk9Clicked() { OnWFChkClicked(8); };//
	afx_msg void OnWFChk10Clicked() { OnWFChkClicked(9); };//6.2.0 'Wf near vision'



	void         OnCTChkClicked(int i);//531
	afx_msg void OnCTChk1Clicked() { OnCTChkClicked(1); };//531 
	afx_msg void OnCTChk2Clicked() { OnCTChkClicked(2); };//531 
	afx_msg void OnCTChk3Clicked() { OnCTChkClicked(3); };//531 
	afx_msg void OnCTChk4Clicked() { OnCTChkClicked(4); };//531 
	afx_msg void OnCTChk5Clicked() { OnCTChkClicked(5); };//531 
	afx_msg void OnCTChk6Clicked() { OnCTChkClicked(6); };//531 
	afx_msg void OnCTChk7Clicked() { OnCTChkClicked(7); };//531 

	void         OnINChkClicked(int i);//531
	afx_msg void OnINChk0Clicked() { OnINChkClicked(0); };//531 
	afx_msg void OnINChk1Clicked() { OnINChkClicked(1); };//531 
	afx_msg void OnINChk2Clicked() { OnINChkClicked(2); };//531 
	afx_msg void OnINChk3Clicked() { OnINChkClicked(3); };//531 
	afx_msg void OnINChk4Clicked() { OnINChkClicked(4); };//531 
	afx_msg void OnINChk5Clicked() { OnINChkClicked(5); };//531 
	afx_msg void OnINChk6Clicked() { OnINChkClicked(6); };//531 
	afx_msg void OnINChk7Clicked() { OnINChkClicked(7); };
	afx_msg void OnINChk8Clicked() { OnINChkClicked(8); };
	afx_msg void OnINChk9Clicked() { OnINChkClicked(9); };// WFCT Angle ka 
	afx_msg void OnINChk10Clicked() { OnINChkClicked(10); };// Toric Alignment

	afx_msg void OnInvalidClicked() { Invalidate(FALSE); };//  

	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);//

	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);//

	void  SetDef_ExList_Width();//520
	void  SetSettingsDefault();//
	BOOL  CheckSetting();//530
	virtual void SetSetting();//while save the setting file, read the setting values
	virtual void SetSelection();//wile load the seeting file, let the checked GUI to unchecked
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
