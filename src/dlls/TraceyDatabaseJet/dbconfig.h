#pragma once

#include <string>

struct DBCONFIG
{
  std::string database;
  std::string examFolder;
  std::string calsFolder;
  bool        readOnly;
};

namespace nlohmann {
  template <>
  struct adl_serializer<DBCONFIG>
  {
    // clang-format off
    static void to_json(json &j, const DBCONFIG &p)
    {
       j = json{
         {"database"  , p.database},
         {"examFolder", p.examFolder},
         {"calFolder" , p.calsFolder},
         {"readOnly"  , p.readOnly}
       };
    }

    static void from_json(const json &j, DBCONFIG &p)
    {
      p.database   = j.at("database"  ).get<std::string>();
      p.examFolder = j.at("examFolder").get<std::string>();
      p.calsFolder = j.at("calFolder" ).get<std::string>();
      p.readOnly   = j.at("readOnly"  ).get<bool>();
    }
    // clang-format on
  };
} // namespace nlohmann
