//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Hardware.h"
#include "GridWnd.h"
#include "LADWnd.h"

//***************************************************************************************

class CAODToolDlg : public CDialog
{
public:

	CAODToolDlg(CHW* pHW, CWnd* pParentWnd);

	int m_BackupAODXFreq0;
	int m_BackupAODYFreq0;
	int m_BackupAODXHzUm;
	int m_BackupAODYHzUm;
	int m_BackupAODXRms;//
	int m_BackupAODYRms;//

	int m_LADZoom;

	CHW* m_pHW;

	CGridWnd      m_GridWnd;
	CLADWnd       m_LADXWnd;
	CLADWnd       m_LADYWnd;

	CEdit         m_RadiusEdit;
	CButton       m_VLineRadio;
	CButton       m_HLineRadio;
	CButton       m_LTRBLineRadio;
	CButton       m_LBRTLineRadio;
	CButton       m_CircleRadio;
	CButton       m_CrossRadio;
	CButton       m_LetterRadio;
	CButton       m_SquareRadio;
	CButton       m_ProbeRadio;
	CButton       m_ScanRadio;

	CButton       m_FirstButton;
	CButton       m_NextButton;
	CButton       m_PrevButton;
	CButton       m_LoopButton;
	CButton       m_StopButton;

	CEdit         m_CentFreqXEdit;
	CEdit         m_HzUmXEdit;
	CEdit         m_CentFreqYEdit;
	CEdit         m_HzUmYEdit;
	CButton       m_MoveRtButton;
	CButton       m_MoveLtButton;
	CButton       m_MoveUpButton;
	CButton       m_MoveDnButton;
	CButton       m_IncXButton;
	CButton       m_DecXButton;
	CButton       m_IncYButton;
	CButton       m_DecYButton;

	CButton       m_LaserOnCheck;
	CButton       m_AODsOnCheck;
	CButton       m_LADsOnCheck;

	CButton       m_DecLADZoomButton;
	CButton       m_IncLADZoomButton;

	CEdit         m_AODXRMSEdit;//
	CEdit         m_AODYRMSEdit;//

	CButton       m_CalSaveButton;//

	BOOL m_ThreadAborted;
	void* m_Thread;
	void Main();
	void StartThread();
	void AbortThread();

	void EnableRadios(const BOOL enable);
	void EnableFreqButtons(const BOOL enable);

	afx_msg void OnRadioClicked();

	afx_msg void OnFirstButtonClicked();
	afx_msg void OnNextButtonClicked();
	afx_msg void OnPrevButtonClicked();
	afx_msg void OnLoopButtonClicked();
	afx_msg void OnStopButtonClicked();

	afx_msg void OnMoveRtButtonClicked();
	afx_msg void OnMoveLtButtonClicked();
	afx_msg void OnMoveUpButtonClicked();
	afx_msg void OnMoveDnButtonClicked();
	afx_msg void OnIncXButtonClicked();
	afx_msg void OnDecXButtonClicked();
	afx_msg void OnIncYButtonClicked();
	afx_msg void OnDecYButtonClicked();

	afx_msg void OnIncLADZoomButtonClicked();
	afx_msg void OnDecLADZoomButtonClicked();

	afx_msg LRESULT OnThreadUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadFinish(WPARAM wParam, LPARAM lParam);

	afx_msg void OnCalSaveBtnClicked();//

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()

	//[cjf***40092012]
private:
	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
};

//***************************************************************************************
