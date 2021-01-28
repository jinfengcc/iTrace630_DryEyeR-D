#include "pch.h"
#include "Benchmark.h"
#include <libs/LoggingLib/TkTraceWin.h>

void Benchmark::ReportTime(const char *label)
{
  using namespace std::chrono;

  if (!DILASCIA_VISIBLE())
    return;

  auto now         = clock_t::now();
  auto timeElapsed = now - _startTime;
  auto ms          = duration_cast<milliseconds>(timeElapsed).count();

  if (label) {
    if (_name)
      DILASCIA_TRACE_EX(_tabName, "{}({}): {:2}s\n", label, _name, ms / 1000.0);
    else
      DILASCIA_TRACE_EX(_tabName, "{}: {:2}s\n", label, ms / 1000.0);
  }
  else {
    if (_name)
      DILASCIA_TRACE_EX(_tabName, "{}: {:2}s\n", _name, ms / 1000.0);
    else
      DILASCIA_TRACE_EX(_tabName, "{:2}s\n", ms / 1000.0);
  }
}

