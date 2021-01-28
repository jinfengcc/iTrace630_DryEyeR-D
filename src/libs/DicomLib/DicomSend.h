#pragma once

#include <opencv2/opencv.hpp>
#include "DicomBase.h"

class DicomSend : DicomBase
{
public:
  DicomSend(const dicom::Config &cfg);

  void Insert(const dicom::Patient &p);
  void Insert(const dicom::Work &p);
  void Insert(const cv::Mat &image);

  bool SendToServer();
  bool SendToServer(const std::filesystem::path &dicomFile);

protected:
  wil::com_ptr<IDCXOBJ> m_obj;

  DicomSend() = default;
  bool SendToFile(const std::filesystem::path &dicomFile);

private:
  using IDCXOBJ = rzdcxLib::IDCXOBJ;
  using IDCXREQ = rzdcxLib::IDCXREQ;

  dicom::Server         m_cfg;
  LogLevel              m_logLevel;
};

class DicomSendFile : public DicomSend
{
public:
  DicomSendFile() = default;

  using DicomSend::SendToFile;
};
