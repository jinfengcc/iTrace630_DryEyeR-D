#include "stdafx.h"
#include "Cmd.Target.h"
#include "Resource.h"

#include <nlohmann/json.hpp>
#include <interfaces/ITraceyDicom.h>
#include <libs/CommonLib/CreateObj.h>
#include "Data.h"
#include "iTraceApp.h"
#include "CustomMenu.h"

//***************************************************************************************

class CmdDicomImport : public CmdTargetBase
{
  DECLARE_MESSAGE_MAP()
  afx_msg void OnPatientImport();

  auto SaveDicomPatient(const dicom::Patient *dp) -> GUID;
  auto FindMatchingPatients(const dicom::Patient *dp) -> std::vector<CPatient *>;
  bool SamePatient(const dicom::Patient *dp, const CPatient &p);
  void Convert(const dicom::Patient *dp, CPatient &p);
};

static CmdDicomImport cmd;

BEGIN_MESSAGE_MAP(CmdDicomImport, CCmdTarget)
  ON_COMMAND(IDK_CTRL_Z, OnPatientImport)
  ON_COMMAND(IDC_IMPORT_PATIENT_BUTTON, OnPatientImport)
END_MESSAGE_MAP()

//***************************************************************************************

void CmdDicomImport::OnPatientImport()
{
  if (!m_isComboDicom) {
    SetMsgHandled(FALSE);
    return;
  }

  CCustomButtonMenu menu;
  menu.AddMenuItems({_T("Standard"), _T("DICOM")});

  if (auto res = menu.TrackPopupMenu(); res < 0) {
    return;
  }
  else if (res == 0) {
    SetMsgHandled(FALSE);
    return;
  }

  auto x = CreateObj<ITraceyDicomQuery>();
  if (!x->Query(AfxGetMainWnd()->GetSafeHwnd()))
    return;

  auto patient = x->GetPatient();
  auto work    = x->GetWork();

  if (patient != nullptr) {
    SaveDicomPatient(patient);
  }
}

GUID CmdDicomImport::SaveDicomPatient(const dicom::Patient *dp)
{
  CPatient p;

  auto matching = FindMatchingPatients(dp);
  if (matching.empty()) {
    p = {};
  }
  else if (matching.size() == 1) {
    p = *matching.front();
  }
  else {
    // #todo
    return GUID_NULL;
  }

  Convert(dp, p);
  CDB::Instance()->SavePatient(p);
  App.m_siPatientAdded(&p);

  return p.m_PatientID;
}

auto CmdDicomImport::FindMatchingPatients(const dicom::Patient *dp) -> std::vector<CPatient *>
{
  std::vector<CPatient *> patlist;

  int size = CDB::Instance()->m_Patients.GetSize();
  for (int i = 0; i < size; i++) {
    auto &p = CDB::Instance()->m_Patients[i];
    if (SamePatient(dp, p))
      patlist.push_back(&p);
  }

  return patlist;
}

bool CmdDicomImport::SamePatient(const dicom::Patient *dp, const CPatient &p)
{
  if (dp->patientID.CompareNoCase(p.m_CustomID) != 0)
    return false;

  if (dp->name.last.CompareNoCase(p.m_LastName) != 0)
    return false;

  if (dp->name.first.CompareNoCase(p.m_FirstName) != 0)
    return false;

  if (dp->birthDate.year != p.m_BirthYear || dp->birthDate.month != p.m_BirthMonth || dp->birthDate.day != p.m_BirthDay)
    return false;

  return true;
}

void CmdDicomImport::Convert(const dicom::Patient *dp, CPatient &p)
{
  p.m_LastName   = dp->name.last;
  p.m_FirstName  = dp->name.first;
  p.m_CustomID   = dp->patientID;
  p.m_BirthYear  = dp->birthDate.year;
  p.m_BirthMonth = dp->birthDate.month;
  p.m_BirthDay   = dp->birthDate.day;
  p.m_Sex        = static_cast<int>(dp->sex);
  p.m_IsDicom    = 1;

  if (p.m_PatientID == GUID_NULL) {
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    p.m_RegYear   = st.wYear;
    p.m_RegMonth  = st.wMonth;
    p.m_RegDay    = st.wDay;
    p.m_RegHour   = st.wHour;
    p.m_RegMinute = st.wMinute;

    if (UuidCreate(&p.m_PatientID) != RPC_S_OK)
      TRACEY_THROW("Unable to create patient GUID");
  }
}
