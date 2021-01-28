#pragma once

class TraceyDicomSend : public Implements<ITraceyDicomSend>
{
public:
  TraceyDicomSend();

  bool SetPatient(const Patient &pt) override;
  bool SendToServer(const Work &work, const cv::Mat &image) override;
  bool SendToServer(ITraceyDicomConfig *cfg, const Work &work, const cv::Mat &image) override;
  bool SendToFile(const Path &path, const Patient &pt, const Work &work, const cv::Mat &image) override;

private:
  std::unique_ptr<Patient> m_patient;
  CStringA                 m_accessionNumber;
  CStringA                 m_modality;
  CStringA                 m_studyInstanceUID;
  CStringA                 m_requestingPhysician;
  CStringA                 m_referringPhysician;
  dicom::DateTime          m_studyTime;

  bool GetAccessionEtc();
  void UpdateAccessionEtc(dicom::Work &work);
};
