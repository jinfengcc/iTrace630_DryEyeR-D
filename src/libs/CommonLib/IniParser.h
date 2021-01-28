#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace ini {
  enum class Type { 
     main_folder,
     db_ype,
     db_folder,
     install_date
  };

  std::optional<int> GetInt(Type type);
  int                GetIntOr(Type type, int defaultValue);
  void               SetInt(Type type, int value);

  std::string GetStr(Type type);
  void        SetStr(Type type, std::string_view value);
} // namespace ini