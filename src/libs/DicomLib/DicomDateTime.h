#pragma once

bool ParseDate(const std::wstring &str, dicom::DateTime &value);
bool ParseTime(const std::wstring &str, dicom::DateTime &value);

inline bool InRange( int val, int min, int max)
{
  return val >= min && val <= max;
}

inline bool Valid(const dicom::Date &d)
{
  return InRange(d.year, 1900, 2200) && InRange(d.month, 1, 12) && InRange(d.day, 1, 31);
}

inline bool Valid(const dicom::DateTime &d)
{
  if (!Valid(dicom::Date{d.year, d.month, d.day}))
    return false;

  return InRange(d.hour, 0, 23) && InRange(d.minute, 0, 59) && InRange(d.second, 0, 59);
}

inline dicom::Date NextDay(const dicom::Date &d)
{
  COleDateTime dt(d.year, d.month, d.day, 0, 0, 0);

  dt += COleDateTimeSpan(1, 0, 0, 0);

  return {dt.GetYear(), dt.GetMonth(), dt.GetDay()};
}
