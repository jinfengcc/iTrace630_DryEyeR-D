#pragma once

#include <comutil.h>
#include <string>

#include "libs/CommonLib/Guid.h"

#import "rzdcx.tlb" rename_namespace("rzdcxLib") exclude("IStream", "ISequentialStream", "_LARGE_INTEGER", "_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME")

namespace rzdcxLib {

  inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag)
  {
    IDCXELMPtr el(obj->createElement(tag));
    obj->insertElement(el);
  }

  template <class Value>
  inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, const Value &v)
  {
    auto el = obj->createElement(tag);

    el->Value = v;
    obj->insertElement(el);
  }

  template <>
  inline void InsertElement<CString>(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, const CString &v)
  {
    auto el = obj->createElement(tag);

    // auto a    = CStringA(v);
    el->Value = v.GetString();
    obj->insertElement(el);
  }

  template <>
  inline void InsertElement<GUID>(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, const GUID &v)
  {
    auto el = obj->createElement(tag);

    auto a    = Guid(v).wstring();
    el->Value = a.c_str();
    obj->insertElement(el);
  }

  template <typename T>
  inline void InsertElement(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, T *valuePtr, int vm)
  {
    auto el    = obj->createElement(tag);

    el->Length = vm;
    el->Value  = (unsigned long)valuePtr;
    obj->insertElement(el);
  }

  //inline void InsertIterator(IDCXOBJ *obj, DICOM_TAGS_ENUM tag, IDCXOBJIterator *value)
  //{
  //  auto el = obj->createElement(tag);

  //  auto x = _variant_t((IUnknown *)value, false);
  //  el->PutValue(&x);
  //  obj->insertElement(el);
  //}

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

} // namespace

