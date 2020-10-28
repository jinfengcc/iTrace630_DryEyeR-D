#include "pch.h"

#if 0
#include <libs/CommonLib/Utils.h>

GTEST_TEST(Utils, GUIDToStr)
{
  // {604E88CE-8FAA-4B6B-B547-A8613F347E17}
  static const GUID test = {0x604e88ce, 0x8faa, 0x4b6b, {0xb5, 0x47, 0xa8, 0x61, 0x3f, 0x34, 0x7e, 0x17}};

  EXPECT_EQ(test, test);

  CString s;
  GUIDToStr(test, s);
  EXPECT_EQ(s, _T("{604E88CE-8FAA-4B6B-B547-A8613F347E17}"));
}

GTEST_TEST(Utils, StrToGUID)
{
  // {604E88CE-8FAA-4B6B-B547-A8613F347E17}
  static const GUID test = {0x604e88ce, 0x8faa, 0x4b6b, {0xb5, 0x47, 0xa8, 0x61, 0x3f, 0x34, 0x7e, 0x17}};

  GUID out;

  StrToGUID("{604E88CE-8FAA-4B6B-B547-A8613F347E17}", out);
  EXPECT_EQ(test, out);

  StrToGUID("604E88CE-8FAA-4B6B-B547-A8613F347E17", out);
  EXPECT_EQ(test, out);

  EXPECT_THROW({ StrToGUID("{604E88CE-8FAA-4B6B-B547-}", out); }, std::runtime_error);

  StrToGUID(L"{604E88CE-8FAA-4B6B-B547-A8613F347E17}", out);
  EXPECT_EQ(test, out);

  StrToGUID(L"604E88CE-8FAA-4B6B-B547-A8613F347E17", out);
  EXPECT_EQ(test, out);

  EXPECT_THROW({ StrToGUID(L"{604E88CE-8FAA-4B6B-B547-}", out); }, std::runtime_error);
}

GTEST_TEST(Utils, Round)
{
  EXPECT_EQ(2, intRound(1.5));
  EXPECT_EQ(1, intRound(1.4));

  EXPECT_EQ(-2, intRound(-1.5));
  EXPECT_EQ(-1, intRound(-1.4));

  EXPECT_DOUBLE_EQ(100.0, RealRound(123.0, 100.0));
  EXPECT_DOUBLE_EQ(200.0, RealRound(173.0, 100.0));

  EXPECT_DOUBLE_EQ(1.13, round8(1.125));
  EXPECT_DOUBLE_EQ(1.13, round8(1.135));
  EXPECT_DOUBLE_EQ(1.13, round8(1.120));
}
#endif
