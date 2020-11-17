#pragma once

#include <fstream>
#include <wil/filesystem.h>
#include <interfaces/ISettingsStorage.h>
#include "Singleton.h"
#include "SignalUI.h"

class AppSettingsImpl : Singleton<AppSettingsImpl>
{
  friend class AppSettings;
  friend class Singleton<AppSettingsImpl>;

  sig::SignalUI<const json *>              m_notify;
  fs::path                                 m_file;
  json                                     m_jsonDONOTUSEDIRECTLY;
  mutable std::mutex                       m_mutex;
  wil::unique_folder_change_reader_nothrow m_dirWatcher;

  AppSettingsImpl();

  json GetJson() const;
  void SetJson(json j);

  static json LoadJson(std::istream &&ss, int retries = 1);
};
