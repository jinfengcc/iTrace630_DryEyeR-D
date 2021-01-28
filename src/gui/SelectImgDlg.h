// averaged wf exam save

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"
#include "Btn.h"
#include "ImgInfoWnd.h"

//***************************************************************************************
#define NUM_EXAMS_MAX 1024

class CSelectImgDlg : public CDialog
{
public:

	GUID  m_Sel_ID;

	CSelectImgDlg(CWnd* pParentWnd, int type);
	~CSelectImgDlg();

private:

	HACCEL m_hAccel;

	int   m_AverageExam;
	GUID  m_SelectExamsID[1024];

	CWFExam* m_pExam[NUM_EXAMS_MAX];

	int       m_CurEye;

	CBtn      m_LeftButton;
	CBtn      m_RightButton;
	CImgInfoWnd   m_EyeImageWnd;

	afx_msg void OnLeftButtonClicked();
	afx_msg void OnRightButtonClicked();

	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;
	virtual void OnCancel() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
