#pragma once

class Flash : public Implements<hal::IFlash>
{
public:
  Flash(THW *thw);

  unsigned Size() const override;
  void     Read(Addr start, void *buf, unsigned size) const override;
  void     Write(Addr start, const void *data, unsigned size) override;
  bool     Verify(Addr start, const void *data, unsigned size) const override;

private:
  void ReadImpl(std::uint32_t startAddress, void *data, unsigned size) const;
  void WriteImpl(std::uint32_t startAddress, const void *data, unsigned size);
  void WaitForNotBusy(unsigned maxWait = 10000) const;

private:
  typedef BOOL(_cdecl *TDllGetDataFlashSize)(ulong *);
  typedef BOOL(_cdecl *TDllReadDataFlash)(uchar *, ulong, ulong, ulong *, ulong *);
  typedef BOOL(_cdecl *TDllWriteDataFlash)(uchar *, ulong, ulong, ulong *);
  typedef BOOL(_cdecl *TDllIsDataFlashBusy)(BOOL *);

  TDllGetDataFlashSize m_sizeFunc;
  TDllReadDataFlash    m_readFunc;
  TDllWriteDataFlash   m_writeFunc;
  TDllIsDataFlashBusy  m_busyFunc;
};
