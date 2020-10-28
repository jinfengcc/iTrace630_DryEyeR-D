#include "pch.h"
#include <atomic>
#include <libs/CommonLib/SignalUI.h>
#include <libs/LoggingLib/TkTraceWin.h>

void PumpMessages()
{
  ASSERT_TRUE(sig::UiThread());

  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, TRUE)) {
    DispatchMessage(&msg);
  }
}

GTEST_TEST(Signals, UI1)
{
  int a1    = 12;
  int count = 0;

  sig::SignalUI<int> sig;
  Connect(sig, [&](int x) {
    EXPECT_EQ(a1, x);
    EXPECT_TRUE(sig::UiThread());

    ++count;
    DILASCIA_TRACE("UI1: {}\n", count);
  });

  sig(a1);
  EXPECT_EQ(0, count);

  PumpMessages();
  EXPECT_EQ(1, count);
}

GTEST_TEST(Signals, UI2)
{
  int a1    = 12;
  int count = 0;

  sig::SignalUI<int> sig{false};
  Connect(sig, [&](int x) {
    EXPECT_EQ(a1, x);
    EXPECT_TRUE(sig::UiThread());

    ++count;
    DILASCIA_TRACE("UI2: {}\n", count);
  });

  sig(a1);
  EXPECT_EQ(1, count);
}

GTEST_TEST(Signals, UI3)
{
  int              a1    = 12;
  //std::atomic<int> count = 0;

  int count = 0;

  sig::SignalUI<int, int> sig{false};
  Connect(sig, [&](int, int a) {
    EXPECT_EQ(a1, a);
    EXPECT_TRUE(sig::UiThread());

    ++count;
    DILASCIA_TRACE("UI2: {}\n", count);
  });

  const int NTHREADS = 10;
  const int NSIGPERTHREAD = 1;

  std::vector<std::thread> threads;
  for (int i = 0; i < NTHREADS; ++i) {
    threads.push_back(std::thread([&]() {
      for (int j = 0; j < NSIGPERTHREAD; ++j)
        sig(i, a1);
    }));
  }

  for (auto &t : threads) {
    if (t.joinable())
      t.join();
  }
  EXPECT_EQ(0, count);

  PumpMessages();
  Sleep(100);
  EXPECT_EQ(NTHREADS * NSIGPERTHREAD, count);
}
