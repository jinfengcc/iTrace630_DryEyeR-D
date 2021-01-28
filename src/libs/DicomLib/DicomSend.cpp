#include "pch.h"
#include "DicomSend.h"
#include "rzdcx/rzdcx.h"
#include "Libs/CommonLib/TempFile.h"

namespace {
  auto FullName(const dicom::Name &n)
  {
    auto fullName = fmt::format(L"{}^{}^{}", n.last.GetString(), n.first.GetString(), n.middle.GetString());
    while (!fullName.empty() && fullName.back() == L'^')
      fullName.pop_back();
    return fullName;
  }
} // namespace

DicomSend::DicomSend(const dicom::Config &cfg)
  : DicomBase(LogLevel::Warn)
  , m_cfg(cfg.store)
{
  m_obj = RZDCX::Instance()->CreateInstance<IDCXOBJ>();
}

void DicomSend::Insert(const dicom::Patient &p)
{
  auto patName  = FullName(p.name);
  auto birthday = fmt::format(L"{:04d}{:02d}{:02d}", p.birthDate.year, p.birthDate.month, p.birthDate.day);
  auto uniqueId = Guid(p.uniquePatientID).wstring();

  DCMLOG_Info("DicomSend: Updating patient");

  // clang-format off
  InsertElement(m_obj.get(), rzdcxLib::patientName      , patName.c_str() );
  InsertElement(m_obj.get(), rzdcxLib::patientID        , p.patientID     );
  InsertElement(m_obj.get(), rzdcxLib::PatientBirthDate , birthday.c_str());
  InsertElement(m_obj.get(), rzdcxLib::PatientSex       , p.sex           );

  /* Not needed (for now)
  InsertElement(m_obj.get(), rzdcxLib::OtherPatientIDs  , uniqueId.c_str());
  InsertElement(m_obj.get(), rzdcxLib::AccessionNumber  , ""              );
  ********************/

  // clang-format on
}

void DicomSend::Insert(const dicom::Work &p)
{
  auto id = RZDCX::Instance()->CreateInstance<rzdcxLib::IDCXUID>();

  auto D = [](const dicom::DateTime &d) { return fmt::format("{:04d}{:02d}{:02d}", d.year, d.month, d.day); };
  auto T = [](const dicom::DateTime &d) { return fmt::format("{:02d}{:02d}{:02d}", d.hour, d.minute, d.second); };

  DCMLOG_Info("DicomSend: Updating Work Item");

  // If we have a study-instance-UID use it, else create a new one
  if (p.studyInstanceUID.IsEmpty()) {
    InsertElement(m_obj.get(), rzdcxLib::studyInstanceUID, id->CreateUID(rzdcxLib::UID_TYPE_STUDY));
  }
  else {
    InsertElement(m_obj.get(), rzdcxLib::studyInstanceUID, p.studyInstanceUID);
    InsertElement(m_obj.get(), rzdcxLib::StudyDate, D(p.studyTime).c_str());
    InsertElement(m_obj.get(), rzdcxLib::StudyTime, T(p.studyTime).c_str());
  }

  // clang-format off
  InsertElement(m_obj.get(), rzdcxLib::seriesInstanceUID     , id->CreateUID(rzdcxLib::UID_TYPE_INSTANCE));
  InsertElement(m_obj.get(), rzdcxLib::sopInstanceUID        , id->CreateUID(rzdcxLib::UID_TYPE_INSTANCE));
  InsertElement(m_obj.get(), rzdcxLib::sopClassUid           , SOP_CLASS_UID);

  InsertElement(m_obj.get(), rzdcxLib::AccessionNumber       , p.accession            );
  InsertElement(m_obj.get(), rzdcxLib::ConversionType        , "SYN"                  );
  InsertElement(m_obj.get(), rzdcxLib::ImageLaterality       , p.imageLaterality      );
  InsertElement(m_obj.get(), rzdcxLib::Manufacturer          , "Tracey Technologies"  );
  InsertElement(m_obj.get(), rzdcxLib::ManufacturerModelName , "iTrace VFA"           );
  InsertElement(m_obj.get(), rzdcxLib::SeriesNumber          , p.seriesNumber         );
  InsertElement(m_obj.get(), rzdcxLib::SeriesDate            , D(p.seriesTime).c_str());
  InsertElement(m_obj.get(), rzdcxLib::SeriesTime            , T(p.seriesTime).c_str());
  InsertElement(m_obj.get(), rzdcxLib::Modality              , p.modality             );
  InsertElement(m_obj.get(), rzdcxLib::RequestingPhysician   , p.requestingPhysician  );
  InsertElement(m_obj.get(), rzdcxLib::ReferringPhysicianName, p.referringPhysician   );
  InsertElement(m_obj.get(), rzdcxLib::SeriesDescription     , p.seriesDescription    );
  InsertElement(m_obj.get(), rzdcxLib::StudyID               , p.studyID              );
  InsertElement(m_obj.get(), rzdcxLib::SoftwareVersions      , p.softwareVersion      );


  ////auto guid = Guid::NewGuid();
  ////InsertElement(m_obj.get(), rzdcxLib::seriesInstanceUID, guid.string().c_str());  // Required and non-empty

  /* Not needed (for now)
  InsertElement(obj, rzdcxLib::PerformingPhysicianName, "XXXXX");
  *************************/

  // clang-format on
}

void DicomSend::Insert(const cv::Mat &image)
{
  const int   SAMPLES_PER_PIXEL          = 3;
  const int   BITS_ALLOCATED             = 8;
  const int   BITS_STORED                = 8;
  const int   NUMBER_OF_FRAMES           = 1;
  const int   RESCALE_INTERCEPT          = 0;
  const char *PHOTOMETRIC_INTERPRETATION = "RGB";
  const int   width                      = image.cols;
  const int   height                     = image.rows;
  const void *pData                      = image.ptr<BYTE *>(0);

  DCMLOG_Info("DicomSend: Inserting image ({}x{})", image.cols, image.rows);

  // clang-format off
  InsertElement(m_obj.get(), rzdcxLib::Rows                     , height                    );
  InsertElement(m_obj.get(), rzdcxLib::Columns                  , width                     );
  InsertElement(m_obj.get(), rzdcxLib::SamplesPerPixel          , SAMPLES_PER_PIXEL         );
  InsertElement(m_obj.get(), rzdcxLib::PhotometricInterpretation, PHOTOMETRIC_INTERPRETATION);
  InsertElement(m_obj.get(), rzdcxLib::PlanarConfiguration      , 0                         );
  InsertElement(m_obj.get(), rzdcxLib::BitsAllocated            , BITS_ALLOCATED            );
  InsertElement(m_obj.get(), rzdcxLib::BitsStored               , BITS_STORED               );
  InsertElement(m_obj.get(), rzdcxLib::HighBit                  , BITS_STORED - 1           );
  InsertElement(m_obj.get(), rzdcxLib::PixelRepresentation      , 0                         );
  // clang-format on

  const int frameSize    = height * width * SAMPLES_PER_PIXEL;
  const int pixelsLength = frameSize * NUMBER_OF_FRAMES;
  auto      temp_file    = util::GetTempFile();

  std::ofstream sf(temp_file, std::ios_base::binary);
  sf.write(static_cast<const char *>(pData), pixelsLength);
  sf.close();

  auto el = m_obj->createElement(rzdcxLib::PixelData);

  el->ValueRepresentation = rzdcxLib::VR_CODE_OB;
  el->SetValueFromFile(temp_file.string().c_str(), 0, pixelsLength);
  m_obj->insertElement(el);
}

bool DicomSend::SendToServer()
{
  auto temp_file = util::GetTempFile();
  return SendToFile(temp_file) ? SendToServer(temp_file) : false;
}

bool DicomSend::SendToServer(const std::filesystem::path &dicomFile)
{
#if 0
  dicomFile = R"(D:\_Proj\_Active\Tracey\data\TestData\dicom\Export1.dcm)";
#endif

  auto rzdcx = RZDCX::Instance();
  auto req   = rzdcx->CreateInstance<IDCXREQ>();

  {
    auto h = CStringA(m_cfg.host);
    auto a = CStringA(m_cfg.remoteAETitle);
    auto c = CStringA(m_cfg.localAETitle);

    DCMLOG_Info("Send: host = {}, port = {}, AE-Title = {}, local-AE-Title = {}", h.GetString(), m_cfg.port, a.GetString(), c.GetString());
  }

  BSTR succeeded{};
  BSTR failed{};

  auto hr = req->Send(m_cfg.localAETitle.GetString(), m_cfg.remoteAETitle.GetString(), m_cfg.host.GetString(), m_cfg.port, dicomFile.string().c_str(),
                      &succeeded, &failed);

  if (hr != S_OK) {
    DCMLOG_Error("DicomSend: Send returned and error (0x{:0X})", hr);
  }

  if (*failed) {
    DCMLOG_Error("DicomSend: Failed to store file");
    hr = E_FAIL;
  }

  SysFreeString(succeeded);
  SysFreeString(failed);

  return hr == S_OK;
}

bool DicomSend::SendToFile(const std::filesystem::path &path)
{
  return m_obj->saveFile(path.string().c_str()) == S_OK;
}
