/****************************************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          05-09-2016
Description:   Create a Tracey Dicom class by using Merge Dicom toolkit
*****************************************************************************************/

//***************************************************************************************

#include "pch.h"

#include "TraceyDicomObsolete.h"
#include "Libs/CommonLib/TempFile.h"
#include "Libs/CommonLib/Chinese.h"
#include "Libs/CommonLib/StringSplit.h"
#include "rzdcx/rzdcxLib.h"

// using rzdcxLib::IDCXELMPtr;
using rzdcxLib::IDCXOBJ;
using rzdcxLib::IDCXOBJIterator;
// using rzdcxLib::IDCXOBJPtr;
using rzdcxLib::IDCXREQ;
using rzdcxLib::IDCXUID;

//***************************************************************************************

CTraceyDicomObsolete::CTraceyDicomObsolete()
{
  char sz[_MAX_PATH];
  GetModuleFileNameA(nullptr, sz, _MAX_PATH);
  auto rzdcx = std::filesystem::path(sz).parent_path() / "rzdcx.dll";

  m_rzdcxLib.Load(rzdcx.wstring().c_str());

  if (FAILED(CoInitialize(nullptr))) {
    LOG_Err("CoInitialize failed");
  }
}

CTraceyDicomObsolete::~CTraceyDicomObsolete()
{
  CoUninitialize();
}

bool CTraceyDicomObsolete::TestActivation()
{
  // The free license allows 100 calls
  const int N = 100 + 10;

  CTraceyDicomObsolete tdc;
  for (int i = 0; i < N; i++) {
    try {
      auto obj = tdc.CreateInstance<IDCXOBJ>(__uuidof(rzdcxLib::DCXOBJ));
    }
    catch (const _com_error) {
      return false;
    }
  }

  return true;
}

//***************************************************************************************

void CTraceyDicomObsolete::OutputDicomImage(const wchar_t *filename, const DicomInfo &dicomInfo, const void *pData, int width, int height)
{
  auto obj = CreateInstance<IDCXOBJ>(__uuidof(rzdcxLib::DCXOBJ));
  auto id  = CreateInstance<IDCXUID>(__uuidof(rzdcxLib::DCXUID));

  SetDicom(obj.get(), dicomInfo.patient);
  SetDicom(obj.get(), dicomInfo.study);
  SetDicom(obj.get(), dicomInfo.series);
  SetDicom(obj.get(), dicomInfo.manufacturer);

  // clang-format off
  InsertElement(obj.get(), rzdcxLib::sopInstanceUID          , rzdcxLib::UID_TYPE_INSTANCE       );
  InsertElement(obj.get(), rzdcxLib::sopClassUid             , SOP_CLASS_UID                     );
  InsertElement(obj.get(), rzdcxLib::Modality                , dicomInfo.modality                );
  //InsertElement(obj.get(), rzdcxLib::PerformingPhysicianName , dicomInfo.physicianName           );
  InsertElement(obj.get(), rzdcxLib::ConversionType          , dicomInfo.conversionType          );
  //InsertElement(obj, rzdcxLib::AccessionNumber         , dicomInfo.m_Acess_Num             );
  //InsertElement(obj, rzdcxLib::Laterality              , dicomInfo.m_Laterality            );
  //InsertElement(obj, rzdcxLib::InstanceNumber          , dicomInfo.m_Instance_Num          );
  //InsertElement(obj, rzdcxLib::PatientOrientation      , dicomInfo.m_Patient_Orientation   );
  // clang-format on

  InsertImage(obj.get(), pData, width, height);

  // Save the dcm file
  obj->saveFile(_bstr_t(filename));
}

//***************************************************************************************

void CTraceyDicomObsolete::InputDicom(const wchar_t *filename, DicomInfo &dicomInfo)
{
  auto obj = CreateInstance<IDCXOBJ>(__uuidof(rzdcxLib::DCXOBJ));

  // IDCXOBJPtr obj(__uuidof(rzdcxLib::DCXOBJ));
  obj->openFile(filename);

  InputDicom(obj.get(), dicomInfo);
}

void CTraceyDicomObsolete::InputDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo &dicomInfo) const
{
  try {
    GetDicom(obj, dicomInfo.physician);
    GetDicom(obj, dicomInfo.patient);
    GetDicom(obj, dicomInfo.study);
    GetDicom(obj, dicomInfo.series);
    GetDicom(obj, dicomInfo.manufacturer);

    // clang-format off
    GetElementValue(obj, rzdcxLib::Modality       , dicomInfo.modality       );
    GetElementValue(obj, rzdcxLib::ConversionType , dicomInfo.conversionType );
    GetElementValue(obj, rzdcxLib::AccessionNumber, dicomInfo.accessionNumber);
    // clang-format on
  }
  catch (const _com_error &e) {
    auto err = static_cast<const char *>(e.Description());
    LOG_Err("InputDicom Error: {}", err);
    throw std::runtime_error(fmt::format("Error reading dicom data ({})", err));
  }
  catch (const std::exception &e) {
    LOG_Err("InputDicom Error: {}", e.what());
    throw;
  }
}

//***************************************************************************************

void CTraceyDicomObsolete::InputChinseeDicom(const wchar_t *filename, DicomInfo &dicomInfo)
{
  // IDCXOBJPtr obj(__uuidof(rzdcxLib::DCXOBJ));
  // obj->openFile(filename);

  // CString patName;

  //// clang-format off
  // GetElementValue(obj, patientName          , patName                           );
  // GetElementValue(obj, patientID            , dicomInfo.m_Patient_ID            );
  // GetElementValue(obj, PatientBirthDate     , dicomInfo.m_Patient_BirthDate     );
  // GetElementValue(obj, PatientSex           , dicomInfo.m_Patient_Sex           );
  // GetElementValue(obj, Manufacturer         , dicomInfo.m_Equi_Manufacturer     );
  // GetElementValue(obj, ManufacturerModelName, dicomInfo.m_Equi_Manufacture_Model);
  // GetElementValue(obj, StudyDescription     , dicomInfo.m_StudyDes              );
  //// clang-format on

  // GetFiLaMiName(patName, dicomInfo.m_FileName, dicomInfo.m_Patient_LastName, dicomInfo.m_Patient_MiddleName);
}

auto CTraceyDicomObsolete::Query(const QParam &qp) const -> std::vector<DicomInfo>
{
  QFilter qf{};
  return QueryImpl(qp, &qf);
}

auto CTraceyDicomObsolete::Query(const QParam &qp, const QFilter &qf) const -> std::vector<DicomInfo>
{
  try {
    auto matches = [](CString value, CString f) -> bool {
      value.MakeLower();
      f.MakeLower();
      return value.Find(f.GetString()) >= 0;
    };

    auto res = QueryImpl(qp, &qf);
    auto end = res.end();

    if (!qf.lastName.IsEmpty())
      end = std::remove_if(res.begin(), end, [&](const DicomInfo &di) { return matches(di.patient.lastName, qf.lastName); });

    if (!qf.firstName.IsEmpty())
      end = std::remove_if(res.begin(), end, [&](const DicomInfo &di) { return matches(di.patient.firstName, qf.firstName); });

    if (!qf.patientID.IsEmpty())
      end = std::remove_if(res.begin(), end, [&](const DicomInfo &di) { return matches(di.patient.patientID, qf.patientID); });

    res.erase(end, res.end());
    return res;
  }
  catch (const _com_error &e) {
    LOG_Err("DicomQuery Error: {}", static_cast<const char *>(e.Description()));
    throw std::runtime_error("Query failed: No DICOM worklist server found.");
  }
  catch (const std::exception &e) {
    LOG_Err("DicomQuery Error: {}", e.what());
    throw;
  }
}

//***************************************************************************************
void CTraceyDicomObsolete::ReadDicomImage(char *filename, DicomInfo &dicomInfo)
{
  //// Create a DCXOBJ
  // IDCXOBJPtr obj(__uuidof(rzdcxLib::DCXOBJ));

  // obj->openFile(filename);

  //// You need to hold a IDCXELMPtr in order to get the elements
  // IDCXELMPtr elem;

  ////Patient name
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(patientName);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Patient_Name = elem->Value;
  //}

  ////Patient ID
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(patientID);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Patient_ID = elem->Value;
  //}

  ////Patient BirthDay
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(PatientBirthDate);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Patient_BirthDate = elem->Value;
  //}

  ////Patient Sex
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(PatientSex);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Patient_Sex = elem->Value;
  //}
  //
  ////Study Date
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(StudyDate);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Study_Date = elem->Value;
  //}

  ////Study Time
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(StudyTime);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Study_Time = elem->Value;
  //}
  //
  ////Study ID
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(studyInstanceUID);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Study_ID = elem->Value;
  //}

  ////Study description
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(StudyDescription);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_StudyDes = elem->Value;
  //}
  //

  ////Sop SopInstanceUID
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(sopInstanceUID);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_SopInstanceUID = elem->Value;
  //}
  //
  ////Sop SopClassUid UID
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(sopClassUid);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_SopClassUid = elem->Value;
  //}

  ////Modality
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(Modality);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Modality = elem->Value;
  //}

  ////Manufacturer
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(Manufacturer);
  //}
  // catch (const _com_error)
  //{
  //}
  //
  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Equi_Manufacturer = elem->Value;
  //}
  //

  ////Manufacturer Model Name
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(ManufacturerModelName);
  //}
  // catch (const _com_error)
  //{
  //}

  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Equi_Manufacture_Model = elem->Value;
  //}

  //
  ////Equi Institution Model Name
  // try
  //{
  //	elem = NULL;
  //	elem = obj->getElementByTag(InstitutionName);
  //}
  // catch (const _com_error)
  //{
  //}

  // if (elem != NULL)
  //{
  //	OutDicomInfo.m_Equi_Institution_Name = elem->Value;
  //}
  //
  //   //Images
  ////Done
}
//***************************************************************************************

auto CTraceyDicomObsolete::QueryImpl(const QParam &qp, const QFilter *qf) const -> std::vector<DicomInfo>
{
  _bstr_t hCallAETitle(qp.callAETitle);

  // Build query
  // Fill the query object
  //auto el    = CreateInstance<IDCXELM>(__uuidof(rzdcxLib::DCXELM));
  auto spsIt = CreateInstance<IDCXOBJIterator>(__uuidof(rzdcxLib::DCXOBJIterator));
  auto sps   = CreateInstance<IDCXOBJ>(__uuidof(rzdcxLib::DCXOBJ));

  {
    // Build the Scheduled procedure Step (SPS) item
    // InsertElement(sps, rzdcxLib::ScheduledStationAETitle, hCallAETitle);

    auto validDate = [](COleDateTime d) { return d.GetStatus() == COleDateTime::valid && d.GetYear() >= 2020; };

    if (qf && validDate(qf->dateFrom)) {
      fmt::memory_buffer buf;
      fmt::format_to(buf, "{:04d}{:02d}{:02d}", qf->dateFrom.GetYear(), qf->dateFrom.GetMonth(), qf->dateFrom.GetDay());
      if (validDate(qf->dateTo))
        fmt::format_to(buf, " - {:04d}{:02d}{:02d}", qf->dateTo.GetYear(), qf->dateTo.GetMonth(), qf->dateTo.GetDay());

      auto str = fmt::to_string(buf);
      auto el  = sps->createElement(rzdcxLib::ScheduledProcedureStepStartDate);

      el->Value = str.c_str();
      sps->insertElement(el);
    }
    else {
      auto el = sps->createElement(rzdcxLib::ScheduledProcedureStepStartDate);
      sps->insertElement(el);
    }

    // InsertElement(sps, rzdcxLib::Modality, qp.ModalityStr);
    InsertElement(sps.get(), rzdcxLib::ScheduledProcedureStepDescription); // Ask for SPS description
    InsertElement(sps.get(), rzdcxLib::ScheduledProcedureStepID);          // Ask for SPS ID

    // Now we put it as an item to sequence
    spsIt->Insert(sps.get());
  }

  auto rp = CreateInstance<IDCXOBJ>(__uuidof(rzdcxLib::DCXOBJ));

  InsertElement(rp.get(), rzdcxLib::PatientsName, "*");
  InsertElement(rp.get(), rzdcxLib::patientID);
  InsertElement(rp.get(), rzdcxLib::OtherPatientIDs);
  InsertElement(rp.get(), rzdcxLib::PatientsSex);
  InsertElement(rp.get(), rzdcxLib::StudyDescription);
  InsertElement(rp.get(), rzdcxLib::PatientBirthDate);
  InsertElement(rp.get(), rzdcxLib::RequestedProcedureID);
  InsertElement(rp.get(), rzdcxLib::RequestedProcedureDescription);
  InsertElement(rp.get(), rzdcxLib::ReferringPhysicianName);
  InsertElement(rp.get(), rzdcxLib::RequestingPhysician);
  InsertElement(rp.get(), rzdcxLib::PerformingPhysicianName);
  InsertElement(rp.get(), rzdcxLib::studyInstanceUID);
  InsertElement(rp.get(), rzdcxLib::AccessionNumber);
  InsertUnknown(rp.get(), rzdcxLib::ScheduledProcedureStepSequence, spsIt.get());

  // Query

  auto req = CreateInstance<IDCXREQ>(__uuidof(rzdcxLib::DCXREQ));
  auto it  = CreateInstance<IDCXOBJIterator>(__uuidof(rzdcxLib::DCXOBJIterator));

  // req->OnQueryResponseRecieved += new IDCXREQEvents_OnQueryResponseRecievedEventHandler(OnQueryResponseRecievedAction);
  // HRESULT hr = req->Echo(hCallAETitle, hCalledTitle, hhost, port);

  _bstr_t hhost(qp.host);
  _bstr_t hCalledTitle(qp.calledTitle);

  it = req->Query(hCallAETitle, hCalledTitle, hhost, qp.port, MWL_SOP_CLASS_UID, rp.get());

  std::vector<DicomInfo> ret;
  for (; !it->AtEnd(); it->Next()) {
    auto r = it->Get();

    if (qf && qf->modalityStr) {
      CString retModality;
      if (!GetElementValue(r, rzdcxLib::Modality, retModality) || retModality != qf->modalityStr)
        continue;
    }

    CString patName;
    if (!GetElementValue(r, rzdcxLib::PatientsName, patName))
      continue;

    DicomInfo di;
    InputDicom(r, di);

    ret.push_back(std::move(di));
  }

  return ret;
}

//***************************************************************************************

void CTraceyDicomObsolete::GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Physician &p) const
{
  CString name;

  GetElementValue(obj, rzdcxLib::RequestingPhysician, name);

  auto pl = StringSplit(name.GetString(), L"^");

  pl.resize(3);
  p.lastName   = pl[0].c_str();
  p.firstName  = pl[1].c_str();
  p.middleName = pl[2].c_str();
}

void CTraceyDicomObsolete::SetDicom(IDCXOBJ *obj, const DicomInfo::Patient &p)
{
  auto patName  = fmt::format(L"{}^{}^{}^", p.lastName.GetString(), p.firstName.GetString(), p.middleName.GetString());
  auto birthday = fmt::format(L"{:04d}{:02d}{:02d}", p.birthDate.GetYear(), p.birthDate.GetMonth(), p.birthDate.GetDay());
  auto uniqueId = Guid(p.uniquePatientID).wstring();

  // clang-format off
  InsertElement(obj, rzdcxLib::patientName      , patName.c_str() );
  InsertElement(obj, rzdcxLib::patientID        , p.patientID     );
  InsertElement(obj, rzdcxLib::OtherPatientIDs  , uniqueId.c_str());
  InsertElement(obj, rzdcxLib::PatientBirthDate , birthday.c_str());
  InsertElement(obj, rzdcxLib::PatientSex       , p.sex           );
  // clang-format on
}

void CTraceyDicomObsolete::GetDicom(IDCXOBJ *obj, DicomInfo::Patient &p) const
{
  CString name;
  CString birthday;
  CString uniqueId;

  // clang-format off
  GetElementValue(obj, rzdcxLib::patientName      , name       );
  GetElementValue(obj, rzdcxLib::patientID        , p.patientID);
  GetElementValue(obj, rzdcxLib::OtherPatientIDs  , uniqueId   );
  GetElementValue(obj, rzdcxLib::PatientBirthDate , birthday   );
  GetElementValue(obj, rzdcxLib::PatientSex       , p.sex      );
  // clang-format on

  // The full name contains:
  // family_name^given_names^middle_name^prefix^suffix1^suffix2
  // We only use the first 3

  auto pl = StringSplit(name.GetString(), L"^");

  pl.resize(3);
  p.lastName   = pl[0].c_str();
  p.firstName  = pl[1].c_str();
  p.middleName = pl[2].c_str();

  p.uniquePatientID = Guid(uniqueId.GetString(), false).get();
}

void CTraceyDicomObsolete::SetDicom(IDCXOBJ *obj, const DicomInfo::Study &p)
{
  auto id = CreateInstance<IDCXUID>(__uuidof(rzdcxLib::DCXUID));

  // clang-format off
  InsertElement(obj, rzdcxLib::studyInstanceUID, id->CreateUID(rzdcxLib::UID_TYPE_STUDY));
  InsertElement(obj, rzdcxLib::StudyID         , p.studyID);
  InsertElement(obj, rzdcxLib::StudyDescription, p.description);
  //InsertElement(obj, rzdcxLib::StudyDate       , p._Date);
  //InsertElement(obj, rzdcxLib::StudyTime       , p._Time);
  // clang-format on
}

void CTraceyDicomObsolete::GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Study &p) const
{
  GetElementValue(obj, rzdcxLib::studyInstanceUID, p.instanceID);
}

void CTraceyDicomObsolete::SetDicom(IDCXOBJ *obj, const DicomInfo::Series &p)
{
  auto id = CreateInstance<IDCXUID>(__uuidof(rzdcxLib::DCXUID));

  // clang-format off
  InsertElement(obj, rzdcxLib::seriesInstanceUID  , p.instanceUID   );
  InsertElement(obj, rzdcxLib::SeriesNumber       , p.number        );
    //InsertElement(obj, rzdcxLib::StudyDate       , p._Date);
    //InsertElement(obj, rzdcxLib::StudyTime       , p._Time);
  // clang-format on
}

void CTraceyDicomObsolete::GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Series &p) const
{
  // clang-format off
  GetElementValue(obj, rzdcxLib::studyInstanceUID, p.instanceUID);
  GetElementValue(obj, rzdcxLib::SeriesNumber    , p.number);
  // clang-format on
}

void CTraceyDicomObsolete::SetDicom(IDCXOBJ *obj, const DicomInfo::Manufacturer &p)
{
  auto id = CreateInstance<IDCXUID>(__uuidof(rzdcxLib::DCXUID));

  // clang-format off
   InsertElement(obj, rzdcxLib::Manufacturer           , p.name     );
   InsertElement(obj, rzdcxLib::ManufacturerModelName  , p.modelName);
  // clang-format on
}

void CTraceyDicomObsolete::GetDicom(rzdcxLib::IDCXOBJ *obj, DicomInfo::Manufacturer &p) const
{
  // clang-format off
  GetElementValue(obj, rzdcxLib::Manufacturer           , p.name     );
  GetElementValue(obj, rzdcxLib::ManufacturerModelName  , p.modelName);
  // clang-format on
}

void CTraceyDicomObsolete::InsertImage(IDCXOBJ *obj, const void *pData, int width, int height)
{
  const int   SAMPLES_PER_PIXEL          = 3;
  const int   BITS_ALLOCATED             = 8;
  const int   BITS_STORED                = 8;
  const int   NUMBER_OF_FRAMES           = 1;
  const int   RESCALE_INTERCEPT          = 0;
  const char *PHOTOMETRIC_INTERPRETATION = "RGB";

  // clang-format off
  InsertElement(obj, rzdcxLib::Rows                     , width                     );
  InsertElement(obj, rzdcxLib::Columns                  , height                    );
  InsertElement(obj, rzdcxLib::SamplesPerPixel          , SAMPLES_PER_PIXEL         );
  InsertElement(obj, rzdcxLib::PhotometricInterpretation, PHOTOMETRIC_INTERPRETATION);
  InsertElement(obj, rzdcxLib::PlanarConfiguration      , 0                         );
  InsertElement(obj, rzdcxLib::BitsAllocated            , BITS_ALLOCATED            );
  InsertElement(obj, rzdcxLib::BitsStored               , BITS_STORED               );
  InsertElement(obj, rzdcxLib::HighBit                  , BITS_STORED - 1           );
  InsertElement(obj, rzdcxLib::PixelRepresentation      , 0                         );
  // clang-format on

  const int frameSize    = height * width * SAMPLES_PER_PIXEL;
  const int pixelsLength = frameSize * NUMBER_OF_FRAMES;
  auto      temp_file    = util::GetTempFile();

  std::ofstream sf(temp_file, std::ios_base::binary);
  sf.write(static_cast<const char *>(pData), pixelsLength);
  sf.close();

  auto el = obj->getElementByTag(rzdcxLib::PixelData);
  el->ValueRepresentation = rzdcxLib::VR_CODE_OB;
  el->SetValueFromFile(temp_file.string().c_str(), 0, pixelsLength);
  obj->insertElement(el);
}
