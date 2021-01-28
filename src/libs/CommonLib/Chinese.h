#pragma once

namespace chinese {
  BOOL           IsChinese(const wchar_t *str);
  const wchar_t *GetChinese(int ID);
  int            GetChineseId(const wchar_t *str);
  std::wstring   TransformEurope(const wchar_t *str);
} // namespace chinese
