#pragma once

#include "libs/CommonLib/TraceyRegistry.h"

class WorklistDialogSettings : public TraceyRegistryImpl<WorklistDialogSettings>
{
public:
  WorklistDialogSettings();

  explicit operator bool() const
  {
    return m_version == VERSION && m_winSize.cx > 100 && m_winSize.cy > 100;
  }

  bool Load();
  void Save();

  static std::string GetListWidths(const CListViewCtrl &lc);
  static void        SetListWidths(CListViewCtrl &lc, const std::string &widths);

  CSize       m_winSize;
  std::string m_patListWidths;
  std::string m_woritemkListWidths;

  BEGIN_REGPROP_MAP(WorklistDialogSettings)
    REG_PROPERTY(L"WinSizeVer", m_version)
    REG_PROPERTY(L"WinSizeCX", m_winSize.cx)
    REG_PROPERTY(L"WinSizeCY", m_winSize.cy)
    REG_PROPERTY(L"WinListPatientWidths", m_patListWidths)
    REG_PROPERTY(L"WinListWorkitemsWidths", m_woritemkListWidths)
  END_REGPROP_MAP()

private:
  static inline int VERSION = 1;

  int m_version{0};

  static std::string      Vec2Str(const std::vector<int> &v);
  static std::vector<int> Str2Vec(const std::string &s);
};
