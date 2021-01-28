#pragma once

#include <Unknwn.h>
#include <vector>
#include <string>

#include <libs/CommonLib/types.h>

using String = std::wstring;
inline bool operator<(const String &a, const String &b)
{
  return _wcsicmp(a.c_str(), b.c_str()) < 0;
}

//inline int Compare(const String &a, const String &b)
//{
//  return wcscmp(a.c_str(), b.c_str());
//}
//inline int CompareNoCase(const String &a, const String &b)
//{
//  return _wcsicmp(a.c_str(), b.c_str());
//}

struct DATETIME
{
  int Year;
  int Month;
  int Day;
  int Hour;
  int Minute;
  int Second;
};

struct PATGROUP
{
  GUID   GroupID;
  String Name;
};

struct ADDRESS
{
  String Line1;
  String Line2;
  String City;
  String State; // Region/province for non-US addresses is stored here
  String ZIP;
  String Country;
};

struct CLINIC
{
  GUID    ClinicID;
  String  Name;
  ADDRESS Address;
};

struct NAME
{
  String Last;
  String First;
  String Middle;
};

struct PHYSICIAN
{
  GUID PhysicianID;
  NAME Name;
};

struct OPERATOR
{
  GUID OperatorID;
  NAME Name;
};

struct PATIENT
{
  enum class SEX : int { Unknown, Male, Female};

  GUID     PatientID;
  GUID     GroupID;
  String   CustomID;
  NAME     Name;
  DATETIME Birthday;
  SEX      Sex;
  ADDRESS  Address;
  String   HomePhone;
  String   WorkPhone;
  String   EMail;
  String   Note;
  DATETIME Registration;
};

struct EXAMHEADER
{
  enum class TYPE : int { UNKNOWN, WF, CT};
  GUID     ExamID;
  GUID     PatientID;
  GUID     ClinicID;
  GUID     PhysicianID;
  GUID     OperatorID;
  GUID     CalibrationID;
  TYPE     Type;
  DATETIME Date;
  EYE      Eye;
  int      Preop;
  int      SeriesNumber;
  int      Reserved1;
  int      Reserved2;
  int      Reserved3;
  int      Reserved4;
  String   Note;
};

struct EYEDATA
{
  GUID   EyeID;
  GUID   PatientID;
  EYE    eye;         // Sergei's software names this 'type'
  double ManifestVertex;
  double ManifestSphere;
  double ManifestCylinder;
  int    ManifestAxis;
  double K1;
  double K2;
  int    K2Axis;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Comparisons

inline int Compare(const String &a, const String &b)
{
  return wcscmp(a.c_str(), b.c_str());
}
inline int CompareNoCase(const String &a, const String &b)
{
  return _wcsicmp(a.c_str(), b.c_str());
}

inline bool operator<(const PATGROUP &a, const PATGROUP &b)
{
  return CompareNoCase(a.Name, b.Name) < 0;
}

inline bool operator<(const NAME &a, const NAME &b)
{
  if (auto i = CompareNoCase(a.Last, b.Last))
    return i < 0;
  if (auto i = CompareNoCase(a.First, b.First))
    return i < 0;
  return CompareNoCase(a.Middle, b.Middle) < 0;
}

inline bool operator<(const CLINIC &a, const CLINIC &b)
{
  return a.Name < b.Name;
}

inline bool operator<(const PHYSICIAN &a, const PHYSICIAN &b)
{
  return a.Name < b.Name;
}

inline bool operator<(const OPERATOR &a, const OPERATOR &b)
{
  return a.Name < b.Name;
}

inline bool operator<(const PATIENT &a, const PATIENT &b)
{
  return a.Name < b.Name;
}

inline bool operator<(const DATETIME &a, const DATETIME &b)
{
  return std::tuple(a.Year, a.Month, a.Day, a.Hour, a.Minute, a.Second) < std::tuple(b.Year, b.Month, b.Day, b.Hour, b.Minute, b.Second);
}

inline bool operator>(const DATETIME &a, const DATETIME &b)
{
  return std::tuple(a.Year, a.Month, a.Day, a.Hour, a.Minute, a.Second) > std::tuple(b.Year, b.Month, b.Day, b.Hour, b.Minute, b.Second);
}

inline bool operator<(const EXAMHEADER &a, const EXAMHEADER &b)
{
  return a.Date < b.Date;
}
