#include "pch.h"
#include <atomic>
#include <libs/CommonLib/Signal.h>

GTEST_TEST(Signals, Thread)
{
  int              a1 = 12;
  std::atomic<int> count{};

  sig::Signal<int> sig;
  Connect(sig, [&](int x) {
    EXPECT_EQ(a1, x);
    ++count;
  });

  const int NTHREADS      = 10;
  const int NSIGPERTHREAD = 100;

  std::vector<std::thread> threads;
  for (int i = 0; i < NTHREADS; ++i) {
    threads.push_back(std::thread([&]() {
      for (int j = 0; j < NSIGPERTHREAD; ++j)
        sig(a1);
    }));
  }

  for (auto &t : threads) {
    if (t.joinable())
      t.join();
  }

  EXPECT_EQ(NTHREADS * NSIGPERTHREAD, count);
}
