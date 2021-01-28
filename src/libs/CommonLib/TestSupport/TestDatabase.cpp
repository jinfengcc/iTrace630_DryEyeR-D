#include "pch.h"
#include "TestDatabase.h"
#include <libs/CommonLib/JsonSettings.h>
#include <libs/CommonLib/ClassFactoryImpl.h>
#include "TestDataFolder.h"

namespace {
  const char config[] = R"({{
    "DatabaseJet": {{
      "database": "{0}",
      "examFolder": "{1}",
      "calFolder": "{2}",
      "readOnly": {3}
    }}
  }})";

  std::string CopyOrigDb(bool readOnly)
  {
    auto orig = GetTestDataFolder() / "jet" / "tdb.mdb";
    if (readOnly)
      return orig.generic_string<char>();

    auto temp = fs::path(__argv[0]).parent_path() / "tdb.mdb";
    fs::copy(orig, temp, fs::copy_options::overwrite_existing);
    return temp.generic_string<char>();
  }
} // namespace

TestDatabase::TestDatabase(bool readOnly)
{
  if (m_db = CreateObj<IDatabaseJet>())
    m_db->Open(GetDbConfigJson(readOnly).c_str());
}

std::string TestDatabase::GetDbConfigJson(bool readOnly)
{
  auto exams = (GetTestDataFolder() / "jet" / "Exams").generic_string<char>();
  auto cals  = (GetTestDataFolder() / "jet" / "Cals" ).generic_string<char>();

  return fmt::format(config, CopyOrigDb(readOnly), exams, cals, readOnly ? "true" : "false");
}
