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
  CCTAcquisitionDlg(CWnd *pParentWnd, CCTAcquisition *pCTAcquisition);

private:
  CCTAcquisition *m_pCTAcquisition;

  LRESULT OnUpdate(WPARAM wParam, LPARAM lParam) override;
  BOOL    OnInitDialog() override;
};

//***************************************************************************************
