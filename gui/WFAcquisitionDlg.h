//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "WFAcquisition.h"
#include "AcquisitionDlg.h"

//***************************************************************************************

class CWFAcquisitionDlg : public CAcquisitionDlg
{
public:

	CWFAcquisitionDlg(CWnd* pParentWnd, CWFAcquisition* pWFAcquisition, BOOL DefaultScanModel);// add DefaultScanModel

	BOOL m_ScanRing;// Set the default scan ring type

private:

	CWFAcquisition* m_pWFAcquisition;

	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg void OnHome();
	afx_msg void OnF1();
	afx_msg void OnF10();

	//530
	void OnNum(int num);
	afx_msg void On1() { OnNum(1); }
	afx_msg void On2() { OnNum(2); }
	afx_msg void On3() { OnNum(3); }
	afx_msg void On4() { OnNum(4); }
	afx_msg void On5() { OnNum(5); }
	afx_msg void On6() { OnNum(6); }
	afx_msg void On7() { OnNum(7); }
	afx_msg void On8() { OnNum(8); }
	afx_msg void On9() { OnNum(9); }

	afx_msg void OnDot();

	afx_msg void OnN1() { OnNum(1); }
	afx_msg void OnN2() { OnNum(2); }
	afx_msg void OnN3() { OnNum(3); }
	afx_msg void OnN4() { OnNum(4); }
	afx_msg void OnN5() { OnNum(5); }
	afx_msg void OnN6() { OnNum(6); }
	afx_msg void OnN7() { OnNum(7); }
	afx_msg void OnN8() { OnNum(8); }
	afx_msg void OnN9() { OnNum(9); }
	//530

	virtual afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam) override;

	virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
