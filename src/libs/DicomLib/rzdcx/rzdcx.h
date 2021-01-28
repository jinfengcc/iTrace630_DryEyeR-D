#pragma once

#include "Libs/CommonLib/ComLibrary.h"
#include "Libs/CommonLib/Guid.h"
#include "Libs/CommonLib/Singleton.h"
#include <ATLComTime.h>

#import "rzdcx.dll" rename_namespace("rzdcxLib") exclude("IStream", "ISequentialStream", "_LARGE_INTEGER", "_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME")

using rzdcxLib::DICOM_TAGS_ENUM;
using rzdcxLib::IDCXOBJ;
using rzdcxLib::IDCXOBJIterator;

class RZDCX : public Singleton<RZDCX>
{
public:
  enum LogLevel {
    None    = static_cast<int>(rzdcxLib::LOG_LEVEL_NONE),
    Fatal   = static_cast<int>(rzdcxLib::LOG_LEVEL_FATAL),
    Error   = static_cast<int>(rzdcxLib::LOG_LEVEL_ERROR),
    Warning = static_cast<int>(rzdcxLib::LOG_LEVEL_WARNING),
    Info    = static_cast<int>(rzdcxLib::LOG_LEVEL_INFO),
    Debug   = static_cast<int>(rzdcxLib::LOG_LEVEL_DEBUG)
  };

  RZDCX();

  void SetLogging(LogLevel logLevel, const fs::path &logFile);
  bool TestActivation();

  template <class Obj>
  wil::com_ptr<Obj> CreateInstance() const
  {
    if constexpr (std::is_same_v<Obj, rzdcxLib::IDCXAPP>)
      return m_rzdcxLib.CreateInstance<Obj>(__uuidof(rzdcxLib::DCXAPP));

    if constexpr (std::is_same_v<Obj, rzdcxLib::IDCXOBJ>)
      return m_rzdcxLib.CreateInstance<Obj>(__uuidof(rzdcxLib::DCXOBJ));

    if constexpr (std::is_same_v<Obj, rzdcxLib::IDCXREQ>)
      return m_rzdcxLib.CreateInstance<Obj>(__uuidof(rzdcxLib::DCXREQ));

    if constexpr (std::is_same_v<Obj, rzdcxLib::IDCXOBJIterator>)
      return m_rzdcxLib.CreateInstance<Obj>(__uuidof(rzdcxLib::DCXOBJIterator));

    if constexpr (std::is_same_v<Obj, rzdcxLib::IDCXELM>)
      return m_rzdcxLib.CreateInstance<Obj>(__uuidof(rzdcxLib::DCXELM));

    if constexpr (std::is_same_v<Obj, rzdcxLib::IDCXUID>)
      return m_rzdcxLib.CreateInstance<Obj>(__uuidof(rzdcxLib::DCXUID));
  }

private:
  using IDCXAPP = rzdcxLib::IDCXAPP;

  ComLibrary            m_rzdcxLib;
  wil::com_ptr<IDCXAPP> m_app;
  LogLevel              m_logLevel{LogLevel::None};
  fs::path              m_logFile;
};

inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag)
{
  obj->insertElement(obj->createElement(tag));
}

template <class Value>
inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, const Value &v)
{
  auto el = obj->createElement(tag);

  el->Value = v;
  obj->insertElement(el);
}

template <>
inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, const CString &v)
{
  auto el = obj->createElement(tag);

  // auto a    = CStringA(v);
  el->Value = v.GetString();
  obj->insertElement(el);
}

template <>
inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, const GUID &v)
{
  auto el = obj->createElement(tag);

  auto a    = Guid(v).wstring();
  el->Value = a.c_str();
  obj->insertElement(el);
}

template <>
inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, const dicom::Sex &v)
{
  auto el = obj->createElement(tag);

  switch (v){
  case dicom::Sex::Male:
    el->Value = "M";
    break;

  case dicom::Sex::Female:
    el->Value = "F";
    break;

  default:
    el->Value = "O";
    break;
  }

  obj->insertElement(el);
}

template <typename T>
inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, T *valuePtr, int vm)
{
  auto el = obj->createElement(tag);

  el->Length = vm;
  el->Value  = (unsigned long)valuePtr;
  obj->insertElement(el);
}

inline void InsertUnknown(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, IUnknown *value)
{
  auto el = obj->createElement(tag);

  auto x = _variant_t(value, false);
  el->PutValue(&x);
  obj->insertElement(el);
}

template <class T>
inline bool GetElementValue(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, T &value)
{
  try {
    if (auto el = obj->getElementByTag(tag)) {
      _variant_t v(el->Value);
      v.ChangeType(VT_BSTR);
      value = v.bstrVal;
      return true;
    }
  }
  catch (...) {
  }

  return false;
}

template <>
inline bool GetElementValue<GUID>(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, GUID &value)
{
  std::wstring str;
  if (!GetElementValue(obj, tag, str))
    return false;

  value = Guid(str, false).get();
  return true;
}

template <>
inline bool GetElementValue<COleDateTime>(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, COleDateTime &value)
{
  std::wstring str;
  if (!GetElementValue(obj, tag, str))
    return false;

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

  value = COleDateTime(y, m, d, 0, 0, 0);
  return true;
}
