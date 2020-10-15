#pragma once

#include <chrono>

class Benchmark
{
public:
  Benchmark(const char *name = nullptr, const char *traceTabName = "Timing")
    : _name(name)
    , _tabName(traceTabName)
  {
  }
  ~Benchmark()
  {
    if (_name)
      ReportTime(nullptr);
  }

  void Start()
  {
    _startTime = clock_t::now();
  }
  auto Stop()
  {
    return clock_t::now() - _startTime;
  }
  void ReportTime(const char *label = nullptr);

private:
  using clock_t = std::chrono::high_resolution_clock;

  const char *        _name;
  const char *        _tabName;
  clock_t::time_point _startTime = clock_t::now();
};
