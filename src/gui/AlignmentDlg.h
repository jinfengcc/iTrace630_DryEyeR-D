/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          09-29-2015
Description:   Create a dialog for technicians for tracey machine alignment
Function SetDlgData() load
*****************************************************************************************/


//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Hardware.h"
#include "MGDI.h"

//***************************************************************************************

class CAlignmentDlg : public CDialog
{
public:

	CAlignmentDlg(CHW* pHW, CALIBRATION* pCalibration, CWnd* pParentWnd);

	CHW*         m_pHW;
	CALIBRATION* m_pCalibration;

	CWFExam    *m_pWFExam;

	CButton      m_LoadButton;
	CButton      m_SaveButton;
	CButton      m_RestoreButton;
	CButton      m_BackupButton;
	CButton      m_OKButton;
	CButton      m_SaveSettingsButton;//Add 1204

	CEdit        m_GUIDEdit;
	CEdit        m_SerialEdit;
	CEdit        m_DateEdit;
	CEdit        m_TimeEdit;
	CButton      m_FirmwareToolButton;

	CButton      m_AODToolButton;
	CButton      m_WFCalButton;
	CButton      m_RingsToolButton;
	CButton      m_MonoButton;
	CButton      m_ColorVideoButton;
	CButton      m_CameraAliButton;

	CButton      m_FlipAODXCheck;
	CEdit        m_AODXFreq0Edit;
	CEdit        m_AODXHzUmEdit;
	CEdit        m_AODXRMSEdit;
	CButton      m_FlipAODYCheck;
	CEdit        m_AODYFreq0Edit;
	CEdit        m_AODYHzUmEdit;
	CEdit        m_AODYRMSEdit;

	CEdit        m_LADExpTimeEdit;
	CButton      m_SwapLADsCheck;
	CButton      m_FlipLADXCheck;
	CEdit        m_LADXFocalDistanceEdit;
	CButton      m_FlipLADYCheck;
	CEdit        m_LADYFocalDistanceEdit;
	CButton      m_OptometerCheck;
	CStatic      m_Opt_Enable_Text;

	CEdit        m_C3Edit;
	CEdit        m_C4Edit;
	CEdit        m_C5Edit;

	CButton      m_ProbingEnabledCheck;
	CButton      m_FoggingEnabledCheck;
	CButton      m_AccommTargetLightOnCheck;
	CEdit        m_TargetTimeEdit;
	CStatic		 m_TargetTimeLabel;
	CEdit        m_ManualScanDiameterEdit;
	CEdit        m_AutoScanDiameterEdit;

	void SetDlgData();
	void GetDlgData();

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	afx_msg void OnFirmwareToolButtonClicked();
	afx_msg void OnLoadButtonClicked();
	afx_msg void OnSaveButtonClicked();
	afx_msg void OnRestoreButtonClicked();
	afx_msg void OnBackupButtonClicked();

	afx_msg void OnProbingButtonClicked();
	afx_msg void OnTargetLightBtnClicked();

	afx_msg void OnAODToolButtonClicked();
	afx_msg void OnVideoQkWFExamBtnClicked();
	afx_msg void OnRingsToolButtonClicked();
	afx_msg void OnMonoToolButtonClicked();
	afx_msg void OnColorButtonClicked();

	afx_msg void OnOptometerCheckClicked();

	afx_msg void OnCameraAliBtnClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
