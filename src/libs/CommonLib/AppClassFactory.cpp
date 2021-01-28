#include "pch.h"
#include <fstream>
#include <libs/CommonLib/IClassFactoryInfo.h>
#include <libs/CommonLib/JsonSettings.h>
#include "AppClassFactory.h"
#include "AppFiles.h"

HRESULT AppClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
  // Look in the previously found guids first
  for (const auto &d : Globals::Instance()->_list) {
    for (const auto &g : d.guids) {
      if (g == riid)
        return d.factory->CreateInstance(pUnkOuter, riid, ppvObject);
    }
  }

  // First time for this guid. Create object and add riid in the guid-list
  for (auto &d : Globals::Instance()->_list) {
    if (d.factory) {
      if (auto hr = d.factory->CreateInstance(pUnkOuter, riid, ppvObject); hr != E_NOINTERFACE) {
        std::scoped_lock lock(Globals::Instance()->_mutex);
        if (std::find(d.guids.begin(), d.guids.end(), riid) == d.guids.end())
          d.guids.push_back(riid);
        return hr;
      }
    }
  }

  return E_NOINTERFACE;
}

static const char *dllList[] = {"TraceyAlgorithms.dll", "TraceyPresentation.dll"};

bool MatchesList(const fs::path &filename)
{
  auto fs = filename.filename().string();
  for (auto s : dllList) {
    if (_stricmp(s, fs.c_str()) == 0)
      return true;
  }
  return false;
}

void AppClassFactory::CreateDataList()
{
  std::scoped_lock lock(Globals::Instance()->_mutex);
  if (!Globals::Instance()->_list.empty())
    return;

  Globals::Instance()->_list = GetDllList();
  VerifyUnique(Globals::Instance()->_list);
}

auto AppClassFactory::CreateData(fs::path &&p) -> Data
{
  if (auto module = LoadLibraryA(p.string().c_str())) {
    using FUNC = IClassFactory *(*)();
    if (auto f = reinterpret_cast<FUNC>(GetProcAddress(module, "CreateClassFactory")))
      return {std::move(p), f()};
    FreeLibrary(module);
  }
  return {};
}

void AppClassFactory::VerifyUnique(const DataList &dl)
{
  std::vector<GUID> guids;
  for (auto &x : dl) {
    if (auto e = wil::com_query<IClassFactoryInfo>(x.factory)) {
      auto g = e->Implements();
      guids.insert(guids.end(), g.begin(), g.end());
    }
  }

  std::sort(guids.begin(), guids.end(), [](const GUID &a, const GUID &b) { return memcmp(&a, &b, sizeof(GUID)) < 0; });
  auto dup = std::adjacent_find(guids.begin(), guids.end());
  if (dup == guids.end())
    return;

  ATLASSERT(false);
  // TODO: Log this and, maybe, throw an exception
}

auto AppClassFactory::GetDllList() -> DataList
{
  DataList pv;

  auto append = [&](fs::path &&p) {
    if (auto d = CreateData(std::move(p)); d.factory != nullptr)
      pv.push_back(std::move(d));
  };

  auto folder = app::GetAppPath(app::Type::exeFolder);

  if (auto js = JsonSettings().Get("dll"); js.is_null()) {
    for (fs::directory_iterator b(folder), e; b != e; ++b) {
      fs::path p = *b;
      if (_stricmp(p.extension().string().c_str(), ".dll") == 0) {
        append(std::move(p));
      }
    }
  }
  else {
    auto files = js.get<std::vector<std::string>>();
    for (auto &f : files) {
      append(folder / f);
    }
  }

  return pv;
}
