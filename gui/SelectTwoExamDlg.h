//***************************************************************************************

#pragma once

//***************************************************************************************

#include "XPListCtrl.h"
#include "ImgInfoWnd.h"

//***************************************************************************************
#define NUM_EXAMS_MAX 1024

class CSelectTwoExamDlg : public CDialog
{
public:

	CSelectTwoExamDlg(CWnd* pParentWnd, GUID& PatientID, int Eye, CMFont* pFont, int WFn, int CTn, real ScanSize[NUM_EXAMS_MAX], int NumImages[NUM_EXAMS_MAX], CString  FellowIDStr[NUM_EXAMS_MAX]);

	GUID m_WFExamID;
	GUID m_CTExamID;

private:

	CStatic m_textWF;
	CStatic m_textCT;
	CStatic m_textWFNum;
	CStatic m_textCTNum;

	CButton m_OKButton;
	CButton m_CancelButton;

	CXPListCtrl m_WFExamHeadersList;
	CXPListCtrl m_CTExamHeadersList;

	CMFont* m_pFont;

	int         m_LastExam;
	int         m_LastRow;
	CImgInfoWnd  m_ExamImgInfo;

	int m_WFn;
	int m_CTn;
	int m_Type;

	int m_EXAM_List_Left;
	int m_EXAM_CTListTop;
	int m_EXAM_List_ColW0;

	CString  m_FellowIDStr[NUM_EXAMS_MAX];
	real m_ScanSize[NUM_EXAMS_MAX];
	int  m_NumImages[NUM_EXAMS_MAX];

	int m_Eye;

	afx_msg void OnListClicked(NMHDR* pNotifyStruct, LRESULT* pRes);
	afx_msg void OnListRClicked(NMHDR* pNotifyStruct, LRESULT* pRes);
	afx_msg void OnListDblClicked(NMHDR* pNotifyStruct, LRESULT* pRes);

	afx_msg void OnExamsListHOVER(NMHDR* pNotifyStruct, LRESULT* pRes);
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
