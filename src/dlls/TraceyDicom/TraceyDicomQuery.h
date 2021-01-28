#pragma once

class TraceyDicomQuery : public Implements<ITraceyDicomQuery>
{
public:
  TraceyDicomQuery();

  bool Query(HWND hWndParent) override;
  bool Query(ITraceyDicomConfig *cfg, HWND hWndParent) override;

  const Patient *GetPatient() const
  {
    return m_patient.get();
  }
  const Work *GetWork() const
  {
    return m_work.get();
  }

private:
  std::unique_ptr<dicom::Patient> m_patient;
  std::unique_ptr<dicom::Work>    m_work;
};
