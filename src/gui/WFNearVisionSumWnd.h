/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Version:       6.2.0
Date:          04-24-2017
Description:   Generate a new WF Display which shows the Near and Far vision comparison
*****************************************************************************************/

#pragma once

//***************************************************************************************

#include "SumWnd.h"
#include "HorSliderCtrl.h"

//***************************************************************************************

class CWFNearVisionSumWnd : public CSumWnd
{
public:

  CWFNearVisionSumWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExamFar, CWFExam *pWFExamNear, int ExamAmount, int ExamOrder,
                      int show = SW_SHOW);

private:
  void PaintSlider(); // 6.2.1 20181218

  virtual CString GetName() override
  {
    return "Simulated Near Vision Display";
  }

  virtual CWndSettings *GetWndSettings() override
  {
    return &m_ThisWndSetting;
  }

  RECT                            m_OutlineRect[2];
  int                             m_ExamAmount;
  int                             m_ExamOrder;
  real_t                          m_OriMc4; // Original mc[4]
  real_t                          m_ShiftVal;
  CWFExam *                       m_pWFExamFar;
  CWFExam *                       m_pWFExamNear;
  CBtn                            m_CompareBtn;
  std::unique_ptr<CHorSliderCtrl> m_Slider;

  virtual void PaintControls() override; // 6.2.1

  virtual void RepaintMemDC() override;
  virtual void CreateChildWnd() override;

  afx_msg void OnComBtnClicked();

  afx_msg LRESULT OnSliderClick(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnSliderChangedPos(WPARAM wParam, LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
