#pragma once

class TraceyDicomConfig : public Implements<ITraceyDicomConfig, ITraceyDicomConfigRZDCX>
{
public:
  TraceyDicomConfig()
    : Implements<ITraceyDicomConfig, ITraceyDicomConfigRZDCX>("ITraceyDicomConfig/ITraceyDicomConfigRZDCX")
  {
  }

  // ITraceyDicomConfig
  Data Get() const override;
  void Set(const Data &d) override;
  bool IsValid(const Data *d) const override;
  bool ChangeConfig(HWND hWndParent) override;

  // ITraceyDicomConfigRZDCX
  bool IsActivated() const override;
  void Activate() override;
  bool ChangeActivation(HWND hWndParent) override;

private:
  static bool IsValidImpl(const Data &d)
  {
    return d.selectionFlags != 0 && IsValidImpl(d.mwl) && IsValidImpl(d.store);
  }
  static bool IsValidImpl(const dicom::Server &d)
  {
    if (d.host.IsEmpty())
      return false;

    if (d.remoteAETitle.IsEmpty())
      return false;

    return true;
  }
};
