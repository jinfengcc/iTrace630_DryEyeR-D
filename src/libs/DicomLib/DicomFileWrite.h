#pragma once

class DicomFileWriter
{
public:
  DicomFileWriter();
  ~DicomFileWriter();

  void Insert(const dicom::Patient &pt);
  void Insert(const dicom::Work &work);
  void Insert(const cv::Mat &image);

  void Save(const std::filesystem::path &path);

private:
  struct Impl;
  std::unique_ptr<Impl> m_pimpl;
};

