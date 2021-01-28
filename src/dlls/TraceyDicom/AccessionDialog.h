#pragma once

#include "resource.h"

class CAccessionDialog
  : public CDialogImpl<CAccessionDialog>
  , public CWinDataExchange<CAccessionDialog>

{
public:
  enum { IDD = IDD_ACCESSION };

  CAccessionDialog(dicom::Patient &p)
  {
    m_patName = p.name.first + " " + p.name.last;
  }

  CStringA AccessionNumber() const
  {
    return CStringA(m_accession);
  }

private:
  CString             m_patName;
  CString             m_accession;

  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  void OnOK(UINT uNotifyCode, int nID, CWindow wndCtl);
  void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);

  BEGIN_MSG_MAP_EX(CAccessionDialog)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDOK, OnOK)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
  END_MSG_MAP()

  BEGIN_DDX_MAP(CAccessionDialog)
    DDX_TEXT(IDC_PATIENT, m_patName)
    DDX_TEXT(IDC_ACCESSION, m_accession)
  END_DDX_MAP()
};
