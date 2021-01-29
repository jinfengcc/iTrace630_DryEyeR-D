#include "pch.h"
#include "TraceyDicomConfig.h"
#include "SetupSheet.h"
#include "libs/DicomLib/DicomLog.h"

namespace {
  struct RegistrySettings : public TraceyRegistryImpl<RegistrySettings>
  {
    RegistrySettings()
      : TraceyRegistryImpl<RegistrySettings>(L"Dicom")
    {
      Load();
    }

    void Load()
    {
      TraceyRegistryImpl<RegistrySettings>::Load();

      DicomLog::Instance()->SetLevel(m_cfg.loggingLevel);

      if (m_cfg.selectionFlags == 0)
        m_cfg.selectionFlags = CombineEnumFlags(ViewType::CT_Summary, ViewType::WF_Custom);
    }
    void Save()
    {
      TraceyRegistryImpl<RegistrySettings>::Save();
    }

    ITraceyDicomConfig::Data m_cfg;


    // clang-format off
    BEGIN_REGPROP_MAP(RegistrySettings                             )
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
    END_REGPROP_MAP(                                               )
    // clang-format on
  };
} // namespace

auto TraceyDicomConfig::Get() const -> Data
{
  RegistrySettings s;
  return s.m_cfg;
}

void TraceyDicomConfig::Set(const Data &d)
{
  RegistrySettings s;
  s.m_cfg = d;
  s.Save();
}

bool TraceyDicomConfig::IsValid(const Data *d) const
{
  return (d != nullptr) ? IsValidImpl(*d) : IsValidImpl(Get());
}

bool TraceyDicomConfig::Change(HWND hWndParent)
{
  CDicomSetup     ds(this);
  return ds.DoModal(hWndParent);
}

bool TraceyDicomConfig::IsActivated() const
{
  return true;
}

bool TraceyDicomConfig::Activate()
{
  return true;
}