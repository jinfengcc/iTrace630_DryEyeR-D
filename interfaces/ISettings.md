# Introduction

Application settings object.

# Definition

The Settings interface is as follows:

```
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
struct __declspec(uuid("63E5EFF7-DD5D-4D6F-AF70-4855E135638C")) ISettings : public IUnknown
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
```

It is defined in [ISettings.h](../../libs/CommonLib/ISettings.h)
and implemented in [AppSettings.cpp](../../libs/CommonLib/AppSettings.cpp)

# Unit tests

Test | Description
-----|------------
Settings.Load | Load a valid json string and verify the string is parsed properly
Settings.LoadFile | Load setting from file. __Not implemented yet__
Settings.LoadInvalid | Load an invalid string and confrm an exception is thrown
Settings.Get | Veifies that getting a section works properly
Settings.Set | Veifies that setting a section works properly. __Not implemented yet__

### Test Results

```
[==========] Running 4 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 4 tests from Settings
[ RUN      ] Settings.Load
[       OK ] Settings.Load (0 ms)
[ RUN      ] Settings.LoadFile
[       OK ] Settings.LoadFile (0 ms)
[ RUN      ] Settings.LoadInvalid
[       OK ] Settings.LoadInvalid (0 ms)
[ RUN      ] Settings.Get
[       OK ] Settings.Get (0 ms)
[----------] 4 tests from Settings (2 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test case ran. (3 ms total)
[  PASSED  ] 4 tests.
```