/****************************************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          05-09-2016
Description:   Create a Tracey Dicom class by using Merge Dicom toolkit
*****************************************************************************************/

#pragma once

#include <vector>
#include <ATLComTime.h>
#include "DicomInfo.h"
#include "Libs/CommonLib/ComLibrary.h"

//***************************************************************************************

namespace rzdcxLib {
  struct IDCXOBJ;
}

class CTraceyDicomObsolete
{
public:
  CTraceyDicomObsolete();
  ~CTraceyDicomObsolete();

  static bool TestActivation();

  void OutputDicomImage(const wchar_t *filename, const DicomInfo &dicomInfo, const void *pData, int width, int height);
  void InputChinseeDicom(const wchar_t *filename, DicomInfo &dicomInfo);
  void InputDicom(const wchar_t *filename, DicomInfo &dicomInfo);
  void InputDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo &dicomInfo) const;
  void ReadDicomImage(char *filename, DicomInfo &dicomInfo);

  struct QParam
  {
    const char *host;
    unsigned    port{};
    const char *callAETitle;
    const char *calledTitle;
  };

  struct QFilter
  {
    const char  *modalityStr;
    COleDateTime dateFrom; // Optional filter
    COleDateTime dateTo;
    CString      patientID;
    CString      firstName;
    CString      lastName;
  };

  auto Query(const QParam &qp) const -> std::vector<DicomInfo>;
  auto Query(const QParam &qp, const QFilter &qf) const -> std::vector<DicomInfo>;

private:
  ComLibrary m_rzdcxLib;

  auto QueryImpl(const QParam &qp, const QFilter *qf) const -> std::vector<DicomInfo>;

  void GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Physician &p) const;
  void SetDicom(rzdcxLib::IDCXOBJ *obj, const DicomInfo::Patient &p);
  void GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Patient &p) const;
  void SetDicom(rzdcxLib::IDCXOBJ *obj, const DicomInfo::Study &p);
  void GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Study &p) const;
  void SetDicom(rzdcxLib::IDCXOBJ *obj, const DicomInfo::Series &p);
  void GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Series &p) const;
  void SetDicom(rzdcxLib::IDCXOBJ *obj, const DicomInfo::Manufacturer &p);
  void GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Manufacturer &p) const;

  void InsertImage(rzdcxLib::IDCXOBJ *obj, const void *pData, int width, int height);

  template <class Obj>
  wil::com_ptr<Obj> CreateInstance(const GUID &clsID) const
  {
    return m_rzdcxLib.CreateInstance<Obj>(clsID);
  }
};
