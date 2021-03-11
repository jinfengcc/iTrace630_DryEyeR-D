#pragma once

#include "Data.h"
#include "MemWnd.h"
#include "Settings.h"
#include "libs/CommonLib/ViewTypes.h"

class ViewServices
{
public:
  ViewServices(CWFExam *wfExam, CCTExam *ctExam, const CPatient *patient = nullptr)
    : ViewServices(wfExam, nullptr, ctExam, nullptr, patient)
  {
  }
  ViewServices(CWFExam *wfExam, CWFExam *wfExam2, CCTExam *ctExam, CCTExam *ctExam2, const CPatient *patient = nullptr);

  using ViewPtr = std::unique_ptr<CMemWnd>;

  cv::Mat  Save(ViewType type) const;
  ViewPtr  CreateView(ViewType type, const RECT *rc = nullptr, int show = SW_HIDE) const;

private:
  CPatient *m_patient{};
  CWFExam * m_wfExam{};
  CCTExam * m_ctExam{};
  CWFExam * m_wfExam2{};
  CCTExam * m_ctExam2{};

  mutable std::unique_ptr<CWFExam> m_duplicateWfExam;

  struct Helper;

  static cv::Mat SaveImpl(CMemWnd *w);

  template <class T, int type = -1>
  ViewPtr CreateImpl(RECT rc, CWndSettings *ws) const;

  template <class T, int type = -1>
  ViewPtr CreateImpl(RECT rc, const CWndSettings &ws) const
  {
    return CreateImpl<T, type>(rc, const_cast<CWndSettings *>(&ws));
  }

  auto CreateWFCTToricCheck(RECT rc) const -> ViewPtr;
};
