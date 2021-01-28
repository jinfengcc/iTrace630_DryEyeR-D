#include "pch.h"
#include "Flash.h"
//#include <libs\CommonLib\Compression.h>

namespace {
  const std::uint32_t FLASH_START_ADDRESS = 0;
}

Flash::Flash(THW *thw)
  : Implements<hal::IFlash>("IFlash")
{
  // clang-format off
  thw->GetFunc( "Lynx_GetDataFlashSize", &m_sizeFunc);
  thw->GetFunc( "Lynx_ReadDataFlash"   , &m_readFunc);
  thw->GetFunc( "Lynx_WriteDataFlash"  , &m_writeFunc);
  thw->GetFunc( "Lynx_IsDataFlashBusy" , &m_busyFunc);
  // clang-format on
}

unsigned Flash::Size() const
{
  ulong size;
  if (!Invoke(m_sizeFunc, &size))
    TRACEY_THROW("Unable to read EEPROM size");
  return gsl::narrow<unsigned>(size);
}

void Flash::Read(Addr start, void *buf, unsigned size) const
{
  if (start + size > Size())
    TRACEY_THROW("Trying to read pass the end of EEPROM");

  ReadImpl(FLASH_START_ADDRESS + start, buf, size);
}

void Flash::Write(Addr start, const void *data, unsigned size)
{
  if (start + size > Size())
    TRACEY_THROW("Trying to write pass the end of EEPROM");

  WriteImpl(FLASH_START_ADDRESS + start, data, size);
}

bool Flash::Verify(Addr start, const void *data, unsigned size) const
{
  std::vector<std::uint8_t> read(size);
  Read(start, &read[0], size);
  return memcmp(data, &read[0], size) == 0;
}

void Flash::ReadImpl(std::uint32_t startAddress, void *data, unsigned size) const
{
  ulong sizeRead  = 0;
  ulong errorCode = 0xffffffff;

  WaitForNotBusy();
  if (!Invoke(m_readFunc, reinterpret_cast<std::uint8_t *>(data), startAddress, size, &sizeRead, &errorCode))
    TRACEY_THROW("Unable to read EEPROM data");

  if (errorCode != 0)
    TRACEY_THROW("Read EEPROM returned with error: {}", errorCode);

  if (size != sizeRead)
    TRACEY_THROW("Unable to read EEPROM data (expected size: {}, actual size: {})", size, sizeRead);
}

void Flash::WriteImpl(std::uint32_t startAddress, const void *data, unsigned size)
{
  ulong errorCode = 0xffffffff;

  WaitForNotBusy();

  auto buf = const_cast<void *>(data);
  if (Invoke(m_writeFunc, static_cast<std::uint8_t *>(buf), startAddress, size, &errorCode))
    TRACEY_THROW("Unable to write EEPROM data");

  if (errorCode != 0)
    TRACEY_THROW("Read EEPROM returned with error: {}", errorCode);

  WaitForNotBusy();

  std::vector<std::uint8_t> verify(size);
  ReadImpl(startAddress, &verify[0], verify.size());
  if (memcmp(data, &verify[0], size) != 0)
    TRACEY_THROW("Data saved into EEPROM and data read back from it are not the same.");
}

void Flash::WaitForNotBusy(unsigned maxWait) const
{
  const unsigned N = 100;
  for (unsigned i = 0; i < N; ++i) {
    BOOL busy{TRUE};

    if (!Invoke(m_busyFunc, &busy))
      TRACEY_THROW("Flash Busy function failed");

    if (!busy)
      return;

    Sleep(maxWait / N);
  }

  TRACEY_THROW("Flash busy for {}secs", maxWait / 1000.0);
}
