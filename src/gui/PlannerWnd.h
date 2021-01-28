//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "PlanWnd.h"

//***************************************************************************************

class CPlannerWnd : public CSumWnd
{
public:

	CPlannerWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings, int OD);
	
private:

	BOOL     m_ShowGrayEye;

	BOOL     m_ShowColorEye;

	CString  m_FileNamesInFolder[3];

	CCTExam* m_pCTExam;

	CIOLData m_PatientIOL;

	CBtn     m_DataButton;
	CBtn     m_ExportSurDateButton;// "Export Toric"

	int      m_OD;
	BOOL     m_ESDBtnCreated;// whether the  m_Export SurDate Button created?

	virtual CString GetName() override { return "Toric Planner"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings; }

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	virtual void CreateMenu() override;

	void         PreopData(RECT Rect, int NumRows, BOOL* Rows);

	void         GetFileNamesFromFolder(CString FolderName);

	BOOL         DeleteDirectory(char* sDirName);

	afx_msg void OnDataButtonClicked();

	afx_msg LRESULT OnUpdateInfoMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBigWndShown(WPARAM wParam, LPARAM lParam);
	afx_msg void OnESDButtonClicked();  

	afx_msg void OnShowColorEyeClicked();
	afx_msg void OnShowGrayEyeClicked();


	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
