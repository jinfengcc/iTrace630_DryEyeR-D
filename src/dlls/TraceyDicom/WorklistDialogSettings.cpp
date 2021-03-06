#include "pch.h"
#include "libs/CommonLib/StringSplit.h"
#include "WorklistDialogSettings.h"

WorklistDialogSettings::WorklistDialogSettings()
  : TraceyRegistryImpl<WorklistDialogSettings>(L"Dicom\\Worklist")
{
}

bool WorklistDialogSettings::Load()
{
  TraceyRegistryImpl<WorklistDialogSettings>::Load();
  return static_cast<bool>(*this);
}

void WorklistDialogSettings::Save()
{
  m_version = VERSION;
  TraceyRegistryImpl<WorklistDialogSettings>::Save();
}

std::string WorklistDialogSettings::GetListWidths(const CListViewCtrl &lc)
{
  std::vector<int> widths;

  auto header = lc.GetHeader();
  for (int i=0; i < header.GetItemCount(); ++i)
    widths.push_back(lc.GetColumnWidth(i));

  return Vec2Str(widths);
}

void WorklistDialogSettings::SetListWidths(CListViewCtrl &lc, const std::string &widths)
{
  auto w = Str2Vec(widths);

  auto header = lc.GetHeader();
  for (int i = 0; i < header.GetItemCount() && std::cmp_less(i, w.size()); ++i) {
    if (w[i] > 10)
      lc.SetColumnWidth(i, w[i]);
  }
}

std::string WorklistDialogSettings::Vec2Str(const std::vector<int> &v)
{
  return fmt::format("{}", fmt::join(v, ","));
}

std::vector<int> WorklistDialogSettings::Str2Vec(const std::string &s)
{
  std::vector<int> x;
  for (auto v = StringSplit(s.c_str(), ","); auto &i : v)
    x.push_back(std::stoi(i));
  return x;
}
