#pragma once

#include <vector>
#include <interfaces/TraceyObj.h>

namespace hal {
  TRACEYOBJ("C1210F48-76C8-4475-8697-816A6C249B3F") IFlash : public IUnknown
  {
    using Addr = std::uint32_t;

    virtual unsigned Size() const                                              = 0;
    virtual void     Read(Addr start, void *buf, unsigned size) const          = 0;
    virtual void     Write(Addr start, const void *data, unsigned size)        = 0;
    virtual bool     Verify(Addr start, const void *data, unsigned size) const = 0;
  };

} // namespace hal
