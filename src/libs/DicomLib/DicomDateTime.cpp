#include "pch.h"
#include "DicomDateTime.h"

bool ParseDate(const std::wstring &str, dicom::DateTime &value)
{
  value.year = value.month = value.day = 0;

  int m, d, y;
  if (str.find('/') != std::string::npos) {
    if (swscanf_s(str.c_str(), L"%d/%d/%d", &m, &d, &y) != 3)
      return false;
  }
  else if (str.find('-') != std::string::npos) {
    if (swscanf_s(str.c_str(), L"%d-%d-%d", &y, &m, &d) != 3)
      return false;
  }
  else {
    if (swscanf_s(str.c_str(), L"%4d%2d%2d", &y, &m, &d) != 3)
      return false;
  }

  value = dicom::DateTime{y, m, d};
  return Valid(value);
}

bool ParseTime(const std::wstring &str, dicom::DateTime &value)
{
  value.hour = value.minute = value.second = 0;

  int h, m, s;
  if (str.find(':') != std::string::npos) {
    if (swscanf_s(str.c_str(), L"%d:%d:%d", &h, &m, &s) != 3)
      return false;
  }
  else {
    if (swscanf_s(str.c_str(), L"%2d%2d%2d", &h, &m, &s) != 3)
      return false;
  }

  value.hour   = h;
  value.minute = m;
  value.second = s;

  return true;
}
