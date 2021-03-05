#include "pch.h"
#include "WorklistDialog.h"
#include "interfaces/IProgress.h"
#include "libs/CommonLib/TraceyRegistry.h"

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

#define LVC_FIXED   0x1000
#define LVC_DYNAMIC 0x0000

  // clang-format off
  LV_COLUMN lvcPatientList[] = {
    // Mask, Format, Width, Name
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT            , 55, LPTSTR(L"Patient ID")},
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT            , 65, LPTSTR(L"Last Name")},
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT            , 55, LPTSTR(L"First Name")},
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT | LVC_FIXED, 25, LPTSTR(L"Birthday")},
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT | LVC_FIXED, 16, LPTSTR(L"Gender")},
  };

  LV_COLUMN lvcWorkItems[] = {
    // Mask, Format, Width, Name
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT | LVC_FIXED, 45, LPTSTR(L"Date")},
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT | LVC_FIXED, 45, LPTSTR(L"Accession")},
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT | LVC_FIXED, 25, LPTSTR(L"Modality")},
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT            , 65, LPTSTR(L"Physician")},
    {LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT            , 70, LPTSTR(L"Procedure")},
  };
  // clang-format on
  void CreateListHeader(CListViewCtrl &lc, std::span<const LV_COLUMN> cols);
  void ResizeListHeader(CListViewCtrl &lc, gsl::span<const LV_COLUMN> lva);

} // namespace

namespace {
  struct RegistrySettings : public TraceyRegistryImpl<RegistrySettings>
  {
    static inline int VERSION = 1;

    RegistrySettings()
      : TraceyRegistryImpl<RegistrySettings>(L"Dicom")
    {
      Load();
    }

    explicit operator bool() const
    {
      return m_version == VERSION &&m_winSize.cx > 100 && m_winSize.cy > 100;
    }

    void Save(CWindow w)
    {
      CRect rc;
      w.GetWindowRect(rc);
      m_winSize = rc.Size();
      m_version = VERSION;
      TraceyRegistryImpl<RegistrySettings>::Save();
    }

    int   m_version{0};
    CSize m_winSize;

    BEGIN_REGPROP_MAP(RegistrySettings)
      REG_PROPERTY(L"WinSizeVer", m_version)
      REG_PROPERTY(L"WinSizeCX", m_winSize.cx)
      REG_PROPERTY(L"WinSizeCY", m_winSize.cy)
    END_REGPROP_MAP()
  };
} // namespace

BOOL CWorklistDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  InitDynamicLayout();

  Initialize();
  DoDataExchange(DDX_LOAD);

  if (RegistrySettings rs; rs) {
    SetWindowPos(nullptr, 0, 0, rs.m_winSize.cx, rs.m_winSize.cy, SWP_NOMOVE | SWP_NOZORDER);
  }

  CenterWindow();

  SetTimer(1, 500);
  return TRUE; // Let the system set the focus
}

LRESULT CWorklistDialog::OnSize(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &handled)
{
  auto ret = CDynamicDialogLayout<CWorklistDialog>::OnSize(msg, wParam, lParam, handled);

  ResizeListHeader(m_patListCtrl, lvcPatientList);
  ResizeListHeader(m_itemListCtrl, lvcWorkItems);

  return ret;
}

void CWorklistDialog::OnTimer(UINT_PTR nIDEvent)
{
  // CRect rc;
  // GetClientRect(rc);
  // if (rc.Width() != m_clientWidth) {
  //  m_clientWidth = rc.Width();

  //  ResizeListHeader(m_patListCtrl, lvcPatientList);
  //  ResizeListHeader(m_itemListCtrl, lvcWorkItems);
  //}
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

    RegistrySettings().Save(*this);
    EndDialog(IDOK);
  }
}

void CWorklistDialog::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  RegistrySettings().Save(*this);
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
    m_patListCtrl = CListViewCtrl(GetDlgItem(IDC_PATIENTS));
    m_patListCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | SORTLV_USESHELLBITMAPS);
    CreateListHeader(m_patListCtrl, lvcPatientList);
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
    m_itemListCtrl = CListViewCtrl(GetDlgItem(IDC_ITEMS));
    m_itemListCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); //  | LVS_EX_CHECKBOXES);
    CreateListHeader(m_itemListCtrl, lvcWorkItems);
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

namespace {
  void CreateListHeader(CListViewCtrl &lc, std::span<const LV_COLUMN> cols)
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

      c.fmt &= ~0xFF;
      c.cx = i == cols.size() - 1 ? (rc.Width() - cx) : (c.cx * rc.Width() / total);
      cx += c.cx;

      lc.InsertColumn(i, &c);
    }
  }

  void ResizeListHeader(CListViewCtrl &lc, gsl::span<const LV_COLUMN> lva)
  {
    CRect rc;
    lc.GetClientRect(&rc);

    auto width = rc.right - (GetSystemMetrics(SM_CXVSCROLL) + 1);

    int total = 0, fixed = 0;
    for (unsigned i = 0; i < lva.size(); i++) {
      auto &c = lva[i];
      if ((c.fmt & LVC_FIXED) == 0)
        total += c.cx;
      else
        width -= lc.GetColumnWidth(i);
    }
    // for (int i = 0; i < header.GetItemCount(); i++) {
    //  if ((lva[i].fmt & LVC_FIXED) != 0)
    //    fixed += lc.GetColumnWidth(i);
    //  else
    //    total += lc.GetColumnWidth(i);
    //}

    for (unsigned i = 0; i < lva.size(); i++) {
      if ((lva[i].fmt & LVC_FIXED) == 0) {
        auto cx = lva[i].cx * width / total;
        lc.SetColumnWidth(i, cx);
      }
    }
  }

  // void ResizeListHeader(CListViewCtrl &lc, gsl::span<LV_COLUMN> lva)
  //{
  //  CRect rc;
  //  lc.GetClientRect(&rc);
  //  rc.right -= GetSystemMetrics(SM_CXVSCROLL) + 1;

  //  int total = 0;

  //  for (const auto &c : lva)
  //    total += c.cx;

  //  for (unsigned i = 0; i < lva.size(); i++) {
  //    LVCOLUMN col;

  //    col.mask = LVCF_WIDTH;

  //    if (lc.GetColumn(i, &col)) {
  //      if (auto x = lva[i].cx * rc.Width() / total; x != col.cx) {
  //        col.cx = x;
  //        lc.SetColumn(i, &col);
  //      }
  //    }
  //  }
  //}
} // namespace
