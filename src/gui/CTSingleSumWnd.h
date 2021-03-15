//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"

//***************************************************************************************

class CCTSingleSumWnd : public CSumWnd
{
public:

	CCTSingleSumWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings, int i, int show = SW_SHOW);

private:

	//521
	BOOL     SameCTSum();
	BOOL     SameCTKera();
	BOOL     SameCT3DZ();
	//521

	CCTExam* m_pCTExam;

	//virtual CString GetNameImpl() const override { return "CT Summary Display"; }
	//6.3.0	 the clear display/dicom export titile
	//virtual CString GetName() override { return "CT Summary Display"; }
	// : CT_SUMMARY_GUI; : WF/RMS ; :Angle K/A ; : Custom; : Depth of focus;
	virtual CString GetNameImpl() const override
	{
		if (m_i == 0)
		{
			return "CT Summary Display";
		}
		else if (m_i == 1)//Angle K/A
		{
			return "CT Keratomery Display";
		}
		else if (m_i == 2)//Angle K/A
		{
			return "CT 3D-Elevation Display";
		}
		else if (m_i == 3)
		{
			return "CT Custom Display";
		}
	}
	//6.3.0

	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + m_d; }

	virtual void OnSizeLargeItemClicked() override;

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	afx_msg void OnShowDefaultItemClicked();//521
	afx_msg LRESULT OnOkulix(WPARAM wParam, LPARAM lParam);//521

														   /* afx_msg LRESULT OnShowSurgPlanMsg(WPARAM wParam, LPARAM lParam);
														   afx_msg LRESULT OnShowAxialMapMsg(WPARAM wParam, LPARAM lParam);*/

	afx_msg LRESULT OnChangeRmsGUI(WPARAM wParam, LPARAM lParam);//530

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
