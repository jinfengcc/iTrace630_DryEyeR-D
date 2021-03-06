#pragma once

#include "resource.h"

class CWorklistDialog
  : public CDialogImpl<CWorklistDialog>
  , public CDynamicDialogLayout<CWorklistDialog>
  , public CWinDataExchange<CWorklistDialog>

{
public:
  enum
  {
    IDD = IDD_WORKLIST
  };

  CWorklistDialog(ITraceyDicomConfig *cfg, dicom::Patient &p, dicom::Work &w)
    : m_cfg(cfg)
    , m_patient(p)
    , m_work(w)
  {
  }

private:
  ITraceyDicomConfig *m_cfg;
  dicom::Patient &    m_patient;
  dicom::Work &       m_work;
  static inline int   m_filterDate{};
  DicomQuery::Filter  m_filter;
  int                 m_filterModality{};
  CListViewCtrl       m_patListCtrl;
  CListViewCtrl       m_workitemListCtrl;
  dicom::WorkList     m_workList;
  int                 m_clientWidth;

  BOOL    OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL & /*bHandled*/);
  void    OnTimer(UINT_PTR nIDEvent);
  LRESULT OnPatientSelectionChanged(LPNMHDR pnmh);
  LRESULT OnWorkItemSelectionChanged(LPNMHDR pnmh);
  void    OnSearch(UINT uNotifyCode, int nID, CWindow wndCtl);
  void    OnClear(UINT uNotifyCode, int nID, CWindow wndCtl);
  void    OnSettings(UINT uNotifyCode, int nID, CWindow wndCtl);
  void    OnOK(UINT uNotifyCode, int nID, CWindow wndCtl);
  void    OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);

  void Initialize();
  void LoadWorklist();
  void LoadPatientList(bool initListCtrl = false);
  void LoadWorkItems(bool initListCtrl = false);
  void EndDialog(int nID);

  static int GetSelection(CListViewCtrl lvc)
  {
    return lvc.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
  }
  static void SetSelection(CListViewCtrl lvc, int sel)
  {
    const DWORD dw = LVIS_SELECTED | LVIS_FOCUSED;

    for (int curSel = -1; (curSel = lvc.GetNextItem(curSel, LVNI_ALL | LVNI_SELECTED)) >= 0;) {
      if (curSel != sel)
        lvc.SetItemState(curSel, 0, dw);
    }

    lvc.SetItemState(sel, dw, dw);
  }

  static auto Date2Str(const dicom::Date &d)
  {
    return fmt::format(L"{:02d}-{:02d}-{:02d}", d.year, d.month, d.day);
  }
  static auto Date2Str(const dicom::DateTime &d, bool dateOnly = false)
  {
    fmt::wmemory_buffer buf;

    fmt::format_to(buf, L"{:02d}-{:02d}-{:02d}", d.year, d.month, d.day);
    if (!dateOnly && (d.hour > 0 || d.minute > 0 || d.second > 0))
      fmt::format_to(buf, L" {:02d}:{:02d}:{:02d}", d.hour, d.minute, d.second);

    return fmt::to_string(buf);
  }

  BEGIN_MSG_MAP_EX(CWorklistDialog)
    MSG_WM_INITDIALOG(OnInitDialog)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MSG_WM_TIMER(OnTimer)
    NOTIFY_HANDLER_EX(IDC_PATIENTS, LVN_ITEMCHANGED, OnPatientSelectionChanged)
    NOTIFY_HANDLER_EX(IDC_ITEMS, LVN_ITEMCHANGED, OnWorkItemSelectionChanged)
    COMMAND_ID_HANDLER_EX(IDC_SEARCH, OnSearch)
    COMMAND_ID_HANDLER_EX(IDC_CLEAR, OnClear)
    COMMAND_ID_HANDLER_EX(IDC_SETTINGS, OnSettings)
    COMMAND_ID_HANDLER_EX(IDOK, OnOK)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
    CHAIN_MSG_MAP(CDynamicDialogLayout<CWorklistDialog>)
  END_MSG_MAP()

  BEGIN_DDX_MAP(CWorklistDialog)
    DDX_RADIO(IDC_DATE_RADIO, m_filterDate)
    DDX_TEXT(IDC_PATIENT_NAME, m_filter.lastName)
    DDX_TEXT(IDC_PATIENT_ID, m_filter.patientID)
    DDX_TEXT(IDC_ACCESSION, m_filter.accession)
    DDX_COMBO_INDEX(IDC_MODALITY, m_filterModality)
  END_DDX_MAP()
};
