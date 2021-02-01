#pragma once

#include <fstream>
#include <filesystem>
#include <wil/filesystem.h>
#include <interfaces/ISettingsStorage.h>
#include <nlohmann/json.hpp>
#include "Singleton.h"
#include "SignalUI.h"

namespace fs = std::filesystem;

class AppSettingsImpl : Singleton<AppSettingsImpl>
{
  friend class AppSettings;
  friend class Singleton<AppSettingsImpl>;

  using json = nlohmann::json;

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
