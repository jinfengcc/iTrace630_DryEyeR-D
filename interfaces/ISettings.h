#pragma once

#include <string_view>
#include <nlohmann/json.hpp>
#include "TraceyObj.h"

//! This class encapsulates the application settings
//! The settings are stored in a json format - here is an example
//!
//!      {
//!        "DatabaseJet":
//!        {
//!          "database"   : "C:\\tracey\\data\\tdb.mdb",
//!          "examFolder" : "C:\\tracey\\data\\data\\Exams",
//!          "calibFolder": "C:\\tracey\\data\\data\\Calib",
//!          "readOnly"   : true
//!        }
//!      }
//!
TRACEYOBJ("63E5EFF7-DD5D-4D6F-AF70-4855E135638C") ISettings : public IUnknown
{
  using json = nlohmann::json;

  //! Gets a section from the json settings
  //! For example calling Get("DatabaseJet") will return:
  //!        {
  //!          "database"   : "C:\\tracey\\data\\tdb.mdb",
  //!          "examFolder" : "C:\\tracey\\data\\data\\Exams",
  //!          "calibFolder": "C:\\tracey\\data\\data\\Calib",
  //!          "readOnly"   : true
  //!        }
  //!
  virtual json Get(std::string_view section) const = 0;

  //! Sets the section json
  virtual void Set(std::string_view section, const json &js) = 0;

  //! Returns the json as string
  virtual std::string ToString(bool formatted = true) const = 0;
};
