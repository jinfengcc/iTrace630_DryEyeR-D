#pragma once

#include "DicomDateTime.h"

static bool Valid(const dicom::Patient &p)
{
  return !p.name.last.IsEmpty() && !p.name.first.IsEmpty() && !p.patientID.IsEmpty() && Valid(p.birthDate);
}

static bool Valid(const dicom::Work &p)
{
  return !p.modality.IsEmpty();
}

static bool operator==(const dicom::Name &a, const dicom::Name &b)
{
  return a.last == b.last && a.first == b.first && a.middle == b.middle && a.title == b.title;
}

static bool operator==(const dicom::Patient &a, const dicom::Patient &b)
{
  return a.name == b.name && a.sex == b.sex;
}

static bool operator<(const dicom::DateTime &a, const dicom::DateTime &b)
{
  auto t = [](const dicom::DateTime &a) { return std::make_tuple(a.year, a.month, a.day, a.hour, a.minute, a.second); };

  return t(a) < t(b);
}

template <>
static bool GetElementValue<dicom::Name>(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, dicom::Name &value)
{
  CString x;

  if (!GetElementValue(obj, tag, x) && x.IsEmpty())
    return false;

  auto pl = StringSplit(x.GetString(), L"^");

  pl.resize(4);
  value.last   = pl[0].c_str();
  value.first  = pl[1].c_str();
  value.middle = pl[2].c_str();
  value.title  = pl[3].c_str();

  return true;
}

template <>
inline bool GetElementValue<dicom::Date>(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, dicom::Date &value)
{
  COleDateTime dt;
  if (!GetElementValue(obj, tag, dt))
    return false;

  value.year  = dt.GetYear();
  value.month = dt.GetMonth();
  value.day   = dt.GetDay();

  return Valid(value);
}

template <>
static bool GetElementValue<dicom::Sex>(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, dicom::Sex &value)
{
  value = dicom::Sex::Other;

  CStringA x;
  if (GetElementValue(obj, tag, x) && !x.IsEmpty()) {
    if (auto ch = tolower(x[0]); ch == 'm')
      value = dicom::Sex::Male;
    else if (ch == 'f')
      value = dicom::Sex::Female;
  }

  return true;
}
