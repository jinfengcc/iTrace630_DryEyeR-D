// for averaged wf exam save
//IDD_EXAM_AVE_DLG

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"
#include "Btn.h"
#include "ImgInfoWnd.h"

//***************************************************************************************
#define NUM_EXAMS_MAX 1024

class CExamAveDlg : public CDialog
{
public:

	CExamAveDlg(CWnd* pParentWnd, CWFExam* pExam[NUM_EXAMS_MAX], int SelNum);
	~CExamAveDlg();

private:

	HACCEL m_hAccel;

	CWFExam* m_pExam[NUM_EXAMS_MAX];

	int       m_SelNum;
	int       m_CurEye;

	CButton   m_PreOpRadio;
	CButton   m_PostOpRadio;
	CButton   m_UnspecifiedRadio;
	CComboBox m_ClinicsComboBox;
	CComboBox m_PhysiciansComboBox;
	CComboBox m_OperatorsComboBox;
	CEdit     m_NoteEdit;
	CBtn      m_LeftButton;
	CBtn      m_RightButton;
	CImgInfoWnd   m_EyeImageWnd;

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

	afx_msg void OnNewClinicButtonClicked();
	afx_msg void OnEditClinicButtonClicked();
	afx_msg void OnNewPhysicianButtonClicked();
	afx_msg void OnEditPhysicianButtonClicked();
	afx_msg void OnNewOperatorButtonClicked();
	afx_msg void OnEditOperatorButtonClicked();
	afx_msg void OnLeftButtonClicked();
	afx_msg void OnRightButtonClicked();

	void CreateClinicsList(const GUID& ClinicID);
	void CreatePhysiciansList(const GUID& PhysicianID);
	void CreateOperatorsList(const GUID& OperatorID);

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
