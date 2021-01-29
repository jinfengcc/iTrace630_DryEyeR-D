#pragma once

#include <interfaces/IDatabaseServices.h>
#include <libs/CommonLib/Implements.h>
#include <libs/CommonLib/Guid.h>
#include "Legacy/DB.h"
#include "dbconfig.h"

class DatabaseServices : public Implements<IDatabaseServices>
{
public:
  DatabaseServices()
    : Implements<IDatabaseServices>("IDatabaseServices")
  {
  }

  path GetExamFile(const path &db_or_dbFolder, const GUID examId) const       override
  {
    auto path = DbFolder(db_or_dbFolder) / "Exams" / Guid(examId).string();
    path.replace_extension(".exam");
    return path;
  }

  path GetCalibrationFile(const path &db_or_dbFolder, const GUID calId) const override
  {
    auto path = DbFolder(db_or_dbFolder) / "Cals" / Guid(calId).string();
    path.replace_extension(".cal");
    return path;
  }

private:
  static path DbFolder( const path &db_or_dbFolder)
  {
    if (fs::is_directory(db_or_dbFolder))
      return db_or_dbFolder;
    else if (fs::is_regular_file(db_or_dbFolder))
      return db_or_dbFolder.parent_path();

    TRACEY_THROW("Passed path {} is not a folder or a directory", db_or_dbFolder.string());
  }
};
