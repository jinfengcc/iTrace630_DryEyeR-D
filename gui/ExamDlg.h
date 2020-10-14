//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CExamDlg : public CDialog
{
public:

	CExamDlg(CWnd* pParentWnd, CExam* pExam);
	~CExamDlg();

private:

	HACCEL m_hAccel;

	CExam* m_pExam;

	CButton m_PreOpRadio;
	CButton m_PostOpRadio;
	CButton m_UnspecifiedRadio;
	CComboBox m_ClinicsComboBox;
	CComboBox m_PhysiciansComboBox;
	CComboBox m_OperatorsComboBox;
	CComboBox m_SeriesComboBox;
	CEdit m_NoteEdit;

	CEdit m_WavetouchLensPowerEdit;
	CButton m_WavetouchLensFlatCurveRadio;
	CButton m_WavetouchLensSteepCurveRadio;
	CButton m_WavetouchLensOtherCurveRadio;
	CEdit m_WavetouchOtherEdit;
	CEdit m_WavetouchNoteEdit;
	CEdit m_WavetouchSphEdit;
	CEdit m_WavetouchCylEdit;
	CEdit m_WavetouchAxisEdit;

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

	afx_msg void OnNewClinicButtonClicked();
	afx_msg void OnEditClinicButtonClicked();
	afx_msg void OnNewPhysicianButtonClicked();
	afx_msg void OnEditPhysicianButtonClicked();
	afx_msg void OnNewOperatorButtonClicked();
	afx_msg void OnEditOperatorButtonClicked();
	afx_msg void OnWavetouchLensFlatCurveRadioClicked();
	afx_msg void OnWavetouchLensSteepCurveRadioClicked();
	afx_msg void OnWavetouchLensOtherCurveRadioClicked();

	afx_msg void OnCtrlE();

	void CreateClinicsList(const GUID& ClinicID);
	void CreatePhysiciansList(const GUID& PhysicianID);
	void CreateOperatorsList(const GUID& OperatorID);

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
