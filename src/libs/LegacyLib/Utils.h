//***************************************************************************************

#pragma once

#include <atlstr.h>
#include <string>
#include <string_view>
#include <stdexcept>
#include <cmath>
#include <libs/CommonLib/types.h>

//***************************************************************************************

template <typename T>
inline void Swap(T &x, T &y)
{
  std::swap(x, y);
}

template <typename T>
inline T sqr(const T &x)
{
  return x * x;
}

template <typename T>
inline T cube(const T &x)
{
  return x * x * x;
}

inline int intRound(const real_t v)
{
  // return (int)floor(v + 0.5);
  return static_cast<int>(std::lround(v));
}
inline real_t RealRound(const real_t v, const real_t d)
{
  return std::round(v / d) * d;
}
inline real_t round8(const real_t v)
{
  return std::round(100.0 * RealRound(v, 0.125)) * 0.01;
}

real_t angle(const real_t y, const real_t x);

inline int CheckAngle(int a)
{
  return (a + 360) % 360;
}

real_t hyp(const real_t x, const real_t y);

int log2(const uint n);

real_t factorial(const int n);

void rev_byte_memcpy(void *dst_bytes, const void *src_bytes, uint num_bytes);
void rev_word_memcpy(void *dst_words, const void *src_words, uint num_words);

void OpenTxtFile(const char *FileName);

void GUIDToStr(const GUID &guid, std::string &s);
void GUIDToStr(const GUID &guid, std::wstring &s);
void StrToGUID(const char *s, GUID &guid);
void StrToGUID(const wchar_t *s, GUID &guid);

inline GUID StrToGUID(const char *s)
{
  GUID g;
  StrToGUID(s, g);
  return g;
}
inline GUID StrToGUID(const wchar_t *s)
{
  GUID g;
  StrToGUID(s, g);
  return g;
}
inline GUID GUIDNew()
{
  GUID g;
  if (UuidCreate(&g) != RPC_S_OK)
    throw std::runtime_error("Unable to Create GUID.");
  return g;
}

inline void GUIDToStr(const GUID &guid, CString &str)
{
  std::wstring s;
  GUIDToStr(guid, s);
  str = s.c_str();
}

inline std::wstring GUIDToStr(const GUID &guid)
{
  std::wstring s;
  GUIDToStr(guid, s);
  return s;
}

//***************************************************************************************

