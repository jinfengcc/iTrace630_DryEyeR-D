//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"

//***************************************************************************************

class CCTSingleSumWnd : public CSumWnd
{
public:
  CCTSingleSumWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CCTExam *pCTExam, CWndSettings *pWndSettings, int i, int show = SW_SHOW);

private:
  BOOL SameCTSum();
  BOOL SameCTKera();
  BOOL SameCT3DZ();

  CCTExam *m_pCTExam;

  CString       GetNameImpl() const override;
  CWndSettings *GetWndSettings() override
  {
    return m_pWndSettings + m_d;
  }

  void OnSizeLargeItemClicked() override;
  void CreateMenu() override;
  void RepaintMemDC() override;
  void CreateChildWnd() override;

  afx_msg void    OnShowDefaultItemClicked();
  afx_msg LRESULT OnOkulix(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnChangeRmsGUI(WPARAM wParam, LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
