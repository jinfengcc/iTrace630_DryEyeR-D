#pragma once

#include <string>
#include <string_view>
#include <vector>

template <class T>
std::vector<std::basic_string<T>> StringSplit(const T *str_, const T *delim_, bool trim = true)
{
  using Str = std::basic_string<T>;

  auto res   = std::vector<Str>();
  auto str   = std::basic_string_view<T>(str_);
  auto delim = std::basic_string_view<T>(delim_);

  for (int n = 0; n != std::string::npos;) {
    auto m = str.find(delim, n);
    auto s = Str(str.substr(n, m - n));

    if (trim) {
      if (auto n = s.find_first_not_of(' '); n > 0)
        s.erase(0, n);

      while (!s.empty() && s[s.size() - 1] == ' ')
        s.resize(s.size() - 1);
    }

    res.push_back(std::move(s));

    n = (m == std::string::npos) ? m : (m + delim.size());
  }

  if (res.size() > 0 && res.back().empty())
    res.resize(res.size() - 1);

  return res;
}
