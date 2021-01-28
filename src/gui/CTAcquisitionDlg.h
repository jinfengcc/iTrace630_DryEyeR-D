//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MemWnd.h"
#include "CTAcquisition.h"
#include "AcquisitionDlg.h"

//***************************************************************************************

class CCTAcquisitionDlg : public CAcquisitionDlg
{
public:

	CCTAcquisitionDlg(CWnd* pParentWnd, CCTAcquisition* pCTAcquisition);

private:

	CCTAcquisition* m_pCTAcquisition;

	virtual afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam) override;

	virtual BOOL OnInitDialog() override;
};

//***************************************************************************************
