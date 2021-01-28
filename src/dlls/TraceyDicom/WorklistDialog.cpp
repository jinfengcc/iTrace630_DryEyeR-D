#include "pch.h"
#include "WorklistDialog.h"
#include "interfaces/IProgress.h"

namespace {
  struct Modality
  {
    const TCHAR *name;
    const TCHAR *description;
  };

  std::array modalities = {
    // clang-format off
    Modality{_T("")                                },
    Modality{_T("OPM"), _T("Ophthalmic Mapping")   },
    Modality{_T("AR") , _T("Autorefraction")       },
    Modality{_T("IOL"), _T("Intraocular lens data")},
    Modality{_T("KER"), _T("Keratometry")          },
    // clang-format on
  };

} // namespace

BOOL CWorklistDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  CenterWindow();
  Initialize();
  DoDataExchange(DDX_LOAD);
  SetTimer(1, 100);
  return TRUE; // Let the system set the focus
}

void CWorklistDialog::OnTimer(UINT_PTR nIDEvent)
{
  KillTimer(nIDEvent);
  //PostMessage(WM_COMMAND, IDC_SEARCH);
}

LRESULT CWorklistDialog::OnPatientSelectionChanged(LPNMHDR pnmh)
{
  LoadWorkItems();
  OnWorkItemSelectionChanged(nullptr);
  return 0;
}

LRESULT CWorklistDialog::OnWorkItemSelectionChanged(LPNMHDR pnmh)
{
  int nSel = GetSelection(m_itemListCtrl);
  GetDlgItem(IDOK).EnableWindow(nSel >= 0);

  return LRESULT();
}

void CWorklistDialog::OnSearch(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  if (DoDataExchange(DDX_SAVE))
    LoadWorklist();
}

void CWorklistDialog::OnClear(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  m_filter         = {};
  m_filterDate     = 0;
  m_filterModality = 0;

  DoDataExchange(DDX_LOAD);
  LoadWorklist();
}

void CWorklistDialog::OnSettings(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  if (m_cfg->Change(*this))
    LoadWorklist();
}

void CWorklistDialog::OnOK(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  auto patientSel  = GetSelection(m_patListCtrl);
  auto workItemSel = GetSelection(m_itemListCtrl);

  if (patientSel >= 0 && workItemSel >= 0) {
    m_patient = m_workList[patientSel].patient;
    m_work    = m_workList[patientSel].items[workItemSel];

    DicomSqlite().SaveWorkItem(m_patient, m_work);
    EndDialog(IDOK);
  }
}

void CWorklistDialog::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  EndDialog(nID);
}

void CWorklistDialog::Initialize()
{
  LoadPatientList(true);
  LoadWorkItems(true);

  { // Modality
    CComboBox cb(GetDlgItem(IDC_MODALITY));
    for (auto &i : modalities) {
      if (i.description)
        cb.AddString(fmt::format(L"{} - {}", i.name, i.description).c_str());
      else
        cb.AddString(i.name);
    }
  }
}

void CWorklistDialog::LoadWorklist()
{
  m_patListCtrl.DeleteAllItems();
  m_itemListCtrl.DeleteAllItems();

  try {
    auto wait = CreateObj<IProgress>();
    wait->Initialize(*this, {.title = "Dicom Import", .status = "Please wait. Loading worklist..."});

    auto dq = DicomQuery{m_cfg->Get()};

    const int days[] = {1, 3, 7, -1};
    if (auto d = days[m_filterDate]; d > 0) {
      auto now     = COleDateTime::GetCurrentTime();
      auto tonight = COleDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 23, 59, 59);
      auto start   = tonight - COleDateTimeSpan(d, 0, 0, 0);

      m_filter.dateFrom = {.year = start.GetYear(), .month = start.GetMonth(), .day = start.GetDay()};
      m_filter.dateTo   = {.year = now.GetYear(), .month = now.GetMonth(), .day = now.GetDay()};
    }
    else {
      m_filter.dateFrom = {};
      m_filter.dateTo   = {};
    }

    m_filter.modality = modalities[m_filterModality].name;
    m_workList        = dq.Query(&m_filter);
  }
  catch (const std::exception &e) {
    ::MessageBoxA(*this, e.what(), "DICOM Server Error", MB_ICONSTOP | MB_OK);
  }

  LoadPatientList();
}

void CWorklistDialog::LoadPatientList(bool initListCtrl)
{
  if (initListCtrl) {
    static LV_COLUMN lvc[] = {
      // Mask, Format, Width, Name
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 55, LPTSTR(L"Patient ID")},
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 65, LPTSTR(L"Last Name")},
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 55, LPTSTR(L"First Name")},
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 25, LPTSTR(L"Birthday")},
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 16, LPTSTR(L"Gender")},
    };

    m_patListCtrl = CListViewCtrl(GetDlgItem(IDC_PATIENTS));
    m_patListCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | SORTLV_USESHELLBITMAPS);
    CreateListHeader(m_patListCtrl, lvc);
    return;
  }

  m_patListCtrl.DeleteAllItems();
  m_itemListCtrl.DeleteAllItems();

  for (const auto &wl : m_workList) {
    auto row = m_patListCtrl.GetItemCount();
    auto col = 0;

    m_patListCtrl.AddItem(row, col++, wl.patient.patientID.GetString());
    m_patListCtrl.AddItem(row, col++, wl.patient.name.last.GetString());
    m_patListCtrl.AddItem(row, col++, wl.patient.name.first.GetString());
    m_patListCtrl.AddItem(row, col++, Date2Str(wl.patient.birthDate).c_str());
    m_patListCtrl.AddItem(row, col++, dicom::Sex2Str(wl.patient.sex));

    m_patListCtrl.SetItemData(row, row);
  }
}

void CWorklistDialog::LoadWorkItems(bool initListCtrl)
{
  if (initListCtrl) {
    static LV_COLUMN lvc[] = {
      // Mask, Format, Width, Name
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 45, LPTSTR(L"Date")},
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 45, LPTSTR(L"Accession")},
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 25, LPTSTR(L"Modality")},
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 65, LPTSTR(L"Physician")},
      {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 70, LPTSTR(L"Procedure")},
    };

    m_itemListCtrl = CListViewCtrl(GetDlgItem(IDC_ITEMS));
    m_itemListCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); //  | LVS_EX_CHECKBOXES);
    CreateListHeader(m_itemListCtrl, lvc);
    return;
  }

  m_itemListCtrl.DeleteAllItems();

  int nSel = GetSelection(m_patListCtrl);
  if (nSel < 0)
    return;

  nSel            = m_patListCtrl.GetItemData(nSel);
  const auto &wli = m_workList[nSel].items;

  for (auto &i : wli) {
    auto row = m_itemListCtrl.GetItemCount();
    auto col = 0;

    m_itemListCtrl.AddItem(row, col++, Date2Str(i.studyTime, true).c_str());
    m_itemListCtrl.AddItem(row, col++, i.accession.GetString());
    m_itemListCtrl.AddItem(row, col++, i.modality.GetString());
    m_itemListCtrl.AddItem(row, col++, i.referringPhysician.GetString());
    m_itemListCtrl.AddItem(row, col++, i.procedureDescription.GetString());
  }
}

void CWorklistDialog::CreateListHeader(CListViewCtrl lc, std::span<const LV_COLUMN> cols)
{
  int total = 0;
  for (auto &c : cols)
    total += c.cx;

  CRect rc;
  lc.GetClientRect(&rc);
  rc.right -= GetSystemMetrics(SM_CXVSCROLL) + 1;

  int cx = 0;
  for (unsigned i = 0; i < cols.size(); ++i) {
    auto c = cols[i];
    c.cx   = i == cols.size() - 1 ? (rc.Width() - cx) : (c.cx * rc.Width() / total);

    cx += c.cx;
    lc.InsertColumn(i, &c);
  }
}
