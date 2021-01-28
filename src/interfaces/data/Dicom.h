#pragma once

#include <vector>
#include <array>
#include <atlstr.h>
#include <ATLComTime.h>

namespace dicom {
  struct Date
  {
    int year, month, day;
  };

  struct DateTime
  {
    int year, month, day;
    int hour, minute, second;
  };

  struct Name
  {
    CString last;
    CString first;
    CString middle;
    CString title;
  };

  enum class Sex { Other, Male, Female };
  inline auto Sex2Str(Sex s)
  {
    static std::array ss = {L"O", L"M", L"F"};
    return ss[static_cast<unsigned>(s) < ss.size() ? static_cast<unsigned>(s) : 0];
  }

  struct Patient
  {
    Name    name;
    CString patientID;
    GUID    uniquePatientID{};
    Date    birthDate;
    Sex     sex;
  };

  struct Work
  {
    CString  assentionNumber;
    CString  requestingPhysician;
    CString  referringPhysician;
    CString  accession;
    CString  modality;
    CString  imageLaterality;
    CString  procedureID;
    CString  procedureDescription;
    CString  studyInstanceUID;
    CString  studyID;
    DateTime studyTime;
    CString  seriesInstanceUID;
    CString  seriesNumber;
    CString  seriesDescription;
    DateTime seriesTime;
    CString  softwareVersion;
  };

  struct WorkItems
  {
    Patient           patient;
    std::vector<Work> items;
  };

  using WorkList = std::vector<WorkItems>;

  enum class LogLevel : int { None, Info, Warn, Error };

  struct Config
  {

    struct Server
    {
      CString       host;
      unsigned      port{};
      CString       remoteAETitle;
      CString       localAETitle;
    };

    LogLevel      loggingLevel{LogLevel::Error};
    Server        mwl;
    Server        store;
    std::uint32_t selectionFlags;
  };

  using Server = Config::Server;

} // namespace dicom
