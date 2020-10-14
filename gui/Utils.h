//***************************************************************************************

#pragma once

//***************************************************************************************

template<typename T>
void Swap(T& x, T& y) { T t = x; x = y, y = t; }

template<typename T>
T sqr(const T& x) { return x * x; }

template<typename T>
T cube(const T& x) { return x * x * x; }

int intRound(const real v);
real RealRound(const real v, const real d);
real round8(const real v);

real angle(const real y, const real x);

int CheckAngle(int a);

real hyp(const real x, const real y);

int log2(const uint n);

real factorial(const int n);

void rev_byte_memcpy(void* dst_bytes, const void* src_bytes, uint num_bytes);
void rev_word_memcpy(void* dst_words, const void* src_words, uint num_words);

void OpenTxtFile(const char* FileName);

void GUIDToStr(const GUID& guid, CString& s);
void StrToGUID(const char* s, GUID& guid);

//***************************************************************************************
