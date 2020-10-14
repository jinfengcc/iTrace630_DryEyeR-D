//***************************************************************************************

#pragma once

//***************************************************************************************

#include "XPListCtrl.h"
#include "ImgInfoWnd.h"

//***************************************************************************************

#define NUM_EXAMS_MAX 1024
class CSelectExamDlg : public CDialog
{
public:

	CSelectExamDlg(CWnd* pParentWnd, GUID& PatientID, int ExamOrder, int Type, int Eye, CMFont* pFont, int NumImages[NUM_EXAMS_MAX]);//520 add int NumImages[NUM_EXAMS_MAX]

	GUID m_ExamID;

	int  m_SelNum; 
	GUID m_ExamIDs[NUM_EXAMS_MAX];

private:

	CButton m_OKButton;
	CButton m_CancelButton;

	CXPListCtrl m_ExamHeadersList;

	CMFont* m_pFont;

	int          m_LastRow;
	CImgInfoWnd  m_ExamImgInfo;//520

	int m_ExamOrder;

	int m_Type;
	int m_Eye;

	int  m_NumImages[NUM_EXAMS_MAX];//520

	int m_EXAM_List_Left;//520
	int m_EXAM_List_Top;//520
	int m_EXAM_List_Hight;//520
	int m_EXAM_List_ColW0;//520

	afx_msg void OnExamsListDblClicked(NMHDR* pNMHDR, LRESULT* pRes);

	afx_msg void OnExamsListHOVER(NMHDR* pNotifyStruct, LRESULT* pRes); //520
	afx_msg void OnMouseMove(uint nFlags, CPoint Point);//520

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
