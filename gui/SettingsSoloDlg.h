//***************************************************************************************

#pragma once

//***************************************************************************************

class CSettingsSoloDlg : public CDialog
{
public:

	CSettingsSoloDlg(CWnd* pParentWnd);

private:

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

	BOOL      m_BackSomeDefaults;

	BOOL  m_CtlDef;
	BOOL  m_GUIDef;

	BOOL      m_OriAliMethod;

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
	CButton   m_WavetouchCheck;
	CButton   m_AccommTargetLightOnCheck;
	CButton   m_ShowSendARKRCheck;

	CEdit     m_TargetTimeEdit;
	CStatic   m_TargetTimeLabel;

	CButton   m_WFVrfRadio;
	CButton   m_WFRfaRadio;//Refraction analysis
	CButton   m_WFAbaRadio;//Aberration analysis
	CButton   m_WFVfaRadio;//Vfa

	CButton   m_AlignmentPupilRadio;
	CButton   m_Alignment4IrdotsRadio;
	CButton   m_Round8Radio;
	CButton   m_Round100Radio;
	CButton   m_CancelButton;
	CButton   m_OKButton;
	CButton   m_DefaultButton;
	CButton   m_SaveButton;
	CButton   m_LoadButton;

	CButton   m_Super_ExamCheck;

	afx_msg void OnProbingButtonClicked();

	afx_msg void OnDefaultButtonClicked();
	afx_msg void OnLoadButtonClicked();
	afx_msg void OnSaveButtonClicked();

	afx_msg void OnTargetLightBtnClicked();
	afx_msg void OnSuperExamBtnClicked();

	void  SetDef_ExList_Width();
	BOOL  CheckSetting();
	virtual void SetSetting();//while save the setting file, read the setting values
	virtual void SetSelection();//wile load the seeting file, let the checked GUI to unchecked
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
