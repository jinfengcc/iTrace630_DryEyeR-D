#pragma once

#include <string>
#include <string_view>

std::string utf8_convert(const std::wstring_view wstr);
std::wstring utf8_convert(const std::string_view str);
