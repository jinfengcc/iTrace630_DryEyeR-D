#include "pch.h"
#include "DicomFileWrite.h"
#include "libs\CommonLib\Guid.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "netapi32.lib")

// !!! Somehow DCMTK does not compile with UNICODE
// ??? Can we save UNICODE data?
#ifdef UNICODE
#  undef UNICODE
#endif

#include <dcmtk\dcmdata\dctk.h>
#include <dcmtk\dcmimgle\dcmimage.h>
#include "libs\CommonLib\TraceyException.h"

const char SOP_CLASS_UID[]     = UID_SecondaryCaptureImageStorage;
const char MWL_SOP_CLASS_UID[] = UID_FINDModalityWorklistInformationModel;

namespace {
  void CheckError(OFCondition c)
  {
    TRACEY_THROW_IF(c.bad(), "Dicom Error: {}", c.text());
  }

  template <class DCM, class Value>
  void PutAndInsert(DCM *dataset, const DcmTag &tag, Value v)
  {
    OFCondition c;

    if constexpr (std::is_unsigned_v<Value>) {
      c = dataset->putAndInsertUint16(tag, static_cast<std::uint16_t>(v));
    }
    if constexpr (std::is_signed_v<Value>) {
      c = dataset->putAndInsertUint16(tag, static_cast<std::uint16_t>(v));
      // c = dataset->putAndInsertSint16(tag, static_cast<std::int16_t>(v));
    }
    else if constexpr (std::is_same_v<Value, std::string>) {
      c = dataset->putAndInsertString(tag, v.c_str());
    }
    else if constexpr (std::is_same_v<Value, std::wstring>) {
      CStringA s(v.c_str());
      c = dataset->putAndInsertString(tag, s.GetString());
    }
    else if constexpr (std::is_same_v<Value, CString>) {
      CStringA s(v);
      c = dataset->putAndInsertString(tag, s.GetString());
    }
    else if constexpr (std::is_same_v<Value, const char *>) {
      c = dataset->putAndInsertString(tag, v);
    }
    else if constexpr (std::is_same_v<Value, const wchar_t *>) {
      CStringA s(v);
      c = dataset->putAndInsertString(tag, s.GetString());
    }
    else if constexpr (std::is_same_v<Value, dicom::Sex>) {
      const char *sex[] = {"", "M", "F"};
      c                 = dataset->putAndInsertString(tag, sex[static_cast<int>(v)]);
    }
    else {
      ATLASSERT(FALSE);
      c = {EC_NotYetImplemented};
    }

    CheckError(c);
  }
} // namespace

struct DicomFileWriter::Impl
{
  DcmFileFormat fileformat;

  auto Dataset()
  {
    return fileformat.getDataset();
  }
  auto MetaInfo()
  {
    return fileformat.getMetaInfo();
  }

  const char *GenerateUniqueIdentifier(const char *root)
  {
    return dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
  }

  auto Save(const fs::path &p)
  {
    return fileformat.saveFile(p.string().c_str(), EXS_LittleEndianImplicit, EET_UndefinedLength, EGL_withoutGL);
  }

private:
  char uid[100];
};

DicomFileWriter::DicomFileWriter()
  : m_pimpl(std::make_unique<Impl>())
{
  auto metaInfo = m_pimpl->MetaInfo();

  // clang-format off
  metaInfo->putAndInsertString( DCM_FileMetaInformationVersion, "Tracey Dicom file");
  PutAndInsert(metaInfo, DCM_MediaStorageSOPInstanceUID, m_pimpl->GenerateUniqueIdentifier(SITE_INSTANCE_UID_ROOT));
  PutAndInsert(metaInfo, DCM_TransferSyntaxUID         , UID_LittleEndianExplicitTransferSyntax);
  PutAndInsert(metaInfo, DCM_ImplementationClassUID    , "999.999");
  // clang-format on

  // PutAndInsert(metaInfo, DCM_MediaStorageSOPClassUID, UID_HardcopyColorImageStorage);

  auto dataset = m_pimpl->Dataset();

  PutAndInsert(dataset, DCM_SOPInstanceUID, m_pimpl->GenerateUniqueIdentifier(SITE_INSTANCE_UID_ROOT));
}

DicomFileWriter::~DicomFileWriter()
{
}

void DicomFileWriter::Insert(const dicom::Patient &p)
{
  DcmDataset *dataset = m_pimpl->Dataset();

  auto patName  = fmt::format(L"{}^{}^{}^", p.name.last.GetString(), p.name.first.GetString(), p.name.middle.GetString());
  auto birthday = fmt::format(L"{:04d}{:02d}{:02d}", p.birthDate.year, p.birthDate.month, p.birthDate.day);
  auto uniqueId = Guid(p.uniquePatientID).wstring();

  // clang-format off
  PutAndInsert(dataset, DCM_PatientName      , patName    );
  PutAndInsert(dataset, DCM_PatientID        , p.patientID);
  PutAndInsert(dataset, DCM_PatientBirthDate , birthday   );
  PutAndInsert(dataset, DCM_PatientSex       , p.sex      );
}

void DicomFileWriter::Insert(const dicom::Work &p)
{
  DcmDataset *dataset = m_pimpl->Dataset();

  auto date = fmt::format("{:04d}{:02d}{:02d}", p.seriesTime.year, p.seriesTime.month, p.seriesTime.day);
  auto time = fmt::format("{:02d}{:02d}{:02d}", p.seriesTime.hour, p.seriesTime.minute, p.seriesTime.second);

  // clang-format off
  PutAndInsert(dataset, DCM_StudyDescription     , "OD Wavefront and Corneal Topography");
  PutAndInsert(dataset, DCM_StudyDate            , date);
  PutAndInsert(dataset, DCM_StudyTime            , time);
  PutAndInsert(dataset, DCM_Modality             , p.modality);
  PutAndInsert(dataset, DCM_Manufacturer         , "Tracey Technologies");
  PutAndInsert(dataset, DCM_ManufacturerModelName, "iTrace VFA");
  PutAndInsert(dataset, DCM_StudyID              , p.studyID);
  PutAndInsert(dataset, DCM_SeriesNumber         , p.seriesNumber);
  PutAndInsert(dataset, DCM_ConversionType       , "SYN");

  PutAndInsert(dataset, DCM_StudyInstanceUID     , m_pimpl->GenerateUniqueIdentifier(SITE_STUDY_UID_ROOT));
  PutAndInsert(dataset, DCM_SeriesInstanceUID    , m_pimpl->GenerateUniqueIdentifier(SITE_SERIES_UID_ROOT));
  PutAndInsert(dataset, DCM_SOPInstanceUID       , m_pimpl->GenerateUniqueIdentifier(SITE_INSTANCE_UID_ROOT));
  PutAndInsert(dataset, DCM_SOPClassUID          , SOP_CLASS_UID);

  PutAndInsert(dataset, DCM_SeriesNumber         , p.seriesNumber       );
  PutAndInsert(dataset, DCM_SeriesDate           , date.c_str()         );
  PutAndInsert(dataset, DCM_SeriesTime           , time.c_str()         );
  PutAndInsert(dataset, DCM_SeriesDescription    , p.seriesDescription   );
  PutAndInsert(dataset, DCM_Modality             , p.modality           );
  PutAndInsert(dataset, DCM_Manufacturer         , "Tracey Technologies");
  PutAndInsert(dataset, DCM_ManufacturerModelName, "iTrace VFA"         );
  PutAndInsert(dataset, DCM_StudyID              , p.studyID            );
  PutAndInsert(dataset, DCM_ConversionType       , "SYN"                );

  /* Not needed (for now)
  PutAndInsert(dataset, DCM_studyInstanceUID       , id->CreateUID(rzdcxLib::UID_TYPE_STUDY));
  PutAndInsert(dataset, DCM_SOPClassUID            , SITE_SOP_CLASS_UID);
  PutAndInsert(dataset, DCM_PerformingPhysicianName, "XXXXX");
  *************************/

  // clang-format on
}

void DicomFileWriter::Insert(const cv::Mat &image)
{
  DcmDataset *dataset = m_pimpl->Dataset();

  // clang-format off
  PutAndInsert(dataset, DCM_Rows                     , image.rows);
  PutAndInsert(dataset, DCM_Columns                  , image.cols);
  PutAndInsert(dataset, DCM_SamplesPerPixel          , 3         );
  PutAndInsert(dataset, DCM_PhotometricInterpretation, "RGB"     );
  PutAndInsert(dataset, DCM_PlanarConfiguration      , 0         );
  PutAndInsert(dataset, DCM_BitsAllocated            , 8         );
  PutAndInsert(dataset, DCM_BitsStored               , 8         );
  PutAndInsert(dataset, DCM_HighBit                  , 7         );
  PutAndInsert(dataset, DCM_PixelRepresentation      , 0         );
  //dataset->putAndInsertString(DCM_PixelAspectRatio         , "4\\3"              ));
  // clang-format on

  CheckError(dataset->putAndInsertUint8Array(DCM_PixelData, image.ptr(0), image.rows * image.cols * 3));
}

void DicomFileWriter::Save(const std::filesystem::path &path)
{
  CheckError(m_pimpl->Save(path));
}
