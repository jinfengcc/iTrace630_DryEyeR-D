#include "pch.h"
#include "TraceyDicomConfig.h"
#include "SetupSheet.h"
#include "libs/DicomLib/DicomLog.h"
#include "libs/DicomLib/rzdcx/rzdcxActivate.h"
#include "RzdcxLicenseDlg.h"

namespace {
  struct WorklistDialogSettings : public TraceyRegistryImpl<WorklistDialogSettings>
  {
    WorklistDialogSettings()
      : TraceyRegistryImpl<WorklistDialogSettings>(L"Dicom")
    {
      Load();
    }

    void Load()
    {
      TraceyRegistryImpl<WorklistDialogSettings>::Load();

      DicomLog::Instance()->SetLevel(m_cfg.loggingLevel);

      if (m_cfg.selectionFlags == 0)
        m_cfg.selectionFlags = CombineEnumFlags(ViewType::CT_Summary, ViewType::WF_VisualAcuity);
    }
    void Save()
    {
      TraceyRegistryImpl<WorklistDialogSettings>::Save();
    }

    ITraceyDicomConfig::Data m_cfg;


    // clang-format off
    BEGIN_REGPROP_MAP(WorklistDialogSettings)
      REG_PROPERTY(L"MWLhost"           , m_cfg.mwl.host           )
      REG_PROPERTY(L"MWLport"           , m_cfg.mwl.port           )
      REG_PROPERTY(L"MWLcallAETitle"    , m_cfg.mwl.remoteAETitle  )
      REG_PROPERTY(L"MWLcallingTitle"   , m_cfg.mwl.localAETitle   )
      REG_PROPERTY(L"Storehost"         , m_cfg.store.host         )
      REG_PROPERTY(L"Storeport"         , m_cfg.store.port         )
      REG_PROPERTY(L"StorecallAETitle"  , m_cfg.store.remoteAETitle)
      REG_PROPERTY(L"StorecallingTitle" , m_cfg.store.localAETitle )
      REG_PROPERTY(L"ViewSelection"     , m_cfg.selectionFlags     )
      REG_PROPERTY(L"LoggingLevel"      , m_cfg.loggingLevel       )
    END_REGPROP_MAP()
    // clang-format on
  };
} // namespace

auto TraceyDicomConfig::Get() const -> Data
{
  WorklistDialogSettings s;
  return s.m_cfg;
}

void TraceyDicomConfig::Set(const Data &d)
{
  WorklistDialogSettings s;
  s.m_cfg = d;
  s.Save();
}

bool TraceyDicomConfig::IsValid(const Data *d) const
{
  return (d != nullptr) ? IsValidImpl(*d) : IsValidImpl(Get());
}

bool TraceyDicomConfig::ChangeConfig(HWND hWndParent)
{
  CDicomSetup     ds(this);
  return DoModal(ds, hWndParent);
}

bool TraceyDicomConfig::IsActivated() const
{
  return rzdcxIsActivated();
}

void TraceyDicomConfig::Activate()
{
  rzdcxActivate();    // It will throw on error
}

bool TraceyDicomConfig::ChangeActivation(HWND hWndParent)
{
  CRzdcxLicenseDlg dlg;
  return DoModal(dlg, hWndParent) == IDOK;
}
