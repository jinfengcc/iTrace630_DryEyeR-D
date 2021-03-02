//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "PlanWnd.h"

#include "PreBtn.h"

//***************************************************************************************

class CPlannerWnd : public CSumWnd
{
public:

	CPlannerWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings, int OD);

private:

	BOOL     m_ShowGrayEye;

	BOOL     m_ShowColorEye;

	CString  m_FileNamesInFolder[3];

	CPatient* m_Patient;//7.0.0


	CCTExam* m_pCTExam;

	CIOLData m_PatientIOL;

	CPreBtn     m_DataButton;
	CPreBtn     m_ExportSurDateButton;// "Export Toric"

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

	CString		 GetPatientAge(CPatient* m_Patient);//7.0.0

	afx_msg void OnDataButtonClicked();

	afx_msg LRESULT OnUpdateInfoMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBigWndShown(WPARAM wParam, LPARAM lParam);
	afx_msg void OnESDButtonClicked();

	afx_msg void OnShowColorEyeClicked();
	afx_msg void OnShowGrayEyeClicked();

	afx_msg void OnMouseMove(uint nFlags, CPoint Point);//7.0.0

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
