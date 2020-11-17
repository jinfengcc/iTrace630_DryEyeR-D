#include "pch.h"
#include "AppSettingsImpl.h"

AppSettingsImpl::AppSettingsImpl()
{
  auto ss                = CreateObj<ISettingsStorage>();
  m_file                 = ss->GetSysFile();
  m_jsonDONOTUSEDIRECTLY = LoadJson(std::ifstream(m_file));

  wchar_t sz[_MAX_PATH * 2];
  wcscpy_s(sz, m_file.parent_path().wstring().c_str());

  if (sz[wcslen(sz) - 1] != '\\')
    wcscat_s(sz, L"\\");

  m_dirWatcher.create(sz, false, wil::FolderChangeEvents::All, [this](wil::FolderChangeEvent event, PCWSTR fileName) {
    if (_wcsicmp(fileName, m_file.filename().wstring().c_str()) == 0) {
      SetJson(LoadJson(std::ifstream(m_file), 3));
      m_notify(&m_jsonDONOTUSEDIRECTLY);
    }
  });
}

json AppSettingsImpl::GetJson() const
{
  std::lock_guard lock(m_mutex);
  return m_jsonDONOTUSEDIRECTLY;
}

void AppSettingsImpl::SetJson(json j)
{
  std::lock_guard lock(m_mutex);
  m_jsonDONOTUSEDIRECTLY = j;
}

json AppSettingsImpl::LoadJson(std::istream &&ss, int retries)
{
  for (int i = 0; i++ < retries;) {
    try {
      return json::parse(ss, nullptr, true, true);
    }
    catch (std::exception &e) {
      DILASCIA_TRACE("Error: {}\n", e.what());
      Sleep(i * 100);
    }
  }

  return {};
}
