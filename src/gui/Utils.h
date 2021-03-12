//***************************************************************************************

#pragma once

//***************************************************************************************

template<typename T>
void Swap(T& x, T& y) { T t = x; x = y, y = t; }

template<typename T>
constexpr T sqr(const T& x) { return x * x; }

template<typename T>
constexpr T cube(const T& x) { return x * x * x; }

int intRound(const real_t v);
real_t RealRound(const real_t v, const real_t d);
real_t round8(const real_t v);

real_t angle(const real_t y, const real_t x);

int CheckAngle(int a);

real_t hyp(const real_t x, const real_t y);

int log2(const uint n);

real_t factorial(const int n);

void rev_byte_memcpy(void* dst_bytes, const void* src_bytes, uint num_bytes);
void rev_word_memcpy(void* dst_words, const void* src_words, uint num_words);

void OpenTxtFile(const char* FileName);

void GUIDToStr(const GUID& guid, CString& s);
void StrToGUID(const char* s, GUID& guid);

//***************************************************************************************
