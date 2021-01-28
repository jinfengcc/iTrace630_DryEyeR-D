#pragma once

#include <vector>
#include <optional>
#include "ViewServices.h"

class BatchSettings final
{
public:
  BatchSettings(std::uint32_t flags)
    : m_flags(flags)
  {
  }

  bool BatchEnabled(ViewType vt) const;
  auto BatchEnabled() const -> std::vector<ViewType>;

  static auto BatchLabel(ViewType vt, bool abbrvName = true) -> const wchar_t *;

private:
  std::optional<std::uint32_t> m_flags;
};
