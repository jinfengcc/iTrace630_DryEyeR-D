#include "pch.h"
#include <libs/CommonLib/AppSettings.h>

static const char test_settings[] = R"({
  "DatabaseJet": {
    "database": "C:\\tracey\\data\\tdb.mdb",
    "examFolder": "C:\\tracey\\data\\data\\Exams",
    "calibFolder": "C:\\tracey\\data\\data\\Calib",
    "readOnly": true
  }
})";

#if 0
using json = nlohmann::json;

GTEST_TEST(Settings, Load)
{
  AppSettings aps(test_settings);

  EXPECT_EQ(json::parse(test_settings), json::parse(aps.ToString()));
}

GTEST_TEST(Settings, LoadFile)
{
  // TODO:
}

GTEST_TEST(Settings, LoadInvalid)
{
  auto func = []() {
    AppSettings aps(R"({"x": Not a valid json)");
    aps.Get("xxx");
  };

  EXPECT_THROW(func(), std::exception);
}

GTEST_TEST(Settings, Get)
{
  AppSettings aps(test_settings);

  EXPECT_EQ(json::parse(test_settings)["DatabaseJet"], aps.Get("DatabaseJet"));
  EXPECT_THROW(([&aps]() { aps.Get("Something"); })(), std::exception);
}

#endif
