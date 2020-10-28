#include "pch.h"
#include <libs/CommonLib/Signal.h>

GTEST_TEST(Signals, ConnectDisconnect)
{
  Signal<int, const char *> sig;

  int         a1    = 12;
  const char *a2    = "testArg2";
  int         count = 0;

  for (int i = 0; i < 100; ++i) {
    Connect(sig, [&](int x, const char *y) {
      EXPECT_EQ(a1, x);
      EXPECT_EQ(a2, y);

      ++count;
    });

    sig(a1, a2);
    EXPECT_EQ(1, count);

    auto id = Connect(sig, [&](int x, const char *y) {
      EXPECT_EQ(a1, x);
      EXPECT_EQ(a2, y);

      ++count;
    });

    count = 0;
    sig(a1, a2);
    EXPECT_EQ(2, count);

    sig::Disconnect(sig, id);
    // Disconnect(sig, id);
    count = 0;
    sig(a1, a2);
    EXPECT_EQ(1, count);

    DisconnectAll(sig, sig::yes_i_am_sure());
    count = 0;
    sig(a1, a2);
    EXPECT_EQ(0, count);
  }
}

GTEST_TEST(Signals, Enable)
{
  int a1 = 12;
  int count{};

  sig::Signal<int> sig;
  Connect(sig, [&](int x) {
    EXPECT_EQ(a1, x);
    ++count;
  });

  sig(a1);
  EXPECT_EQ(1, count);

  sig.Enable(false);
  sig(a1);
  EXPECT_EQ(1, count);
}

GTEST_TEST(Signals, Slot)
{
  int a1 = 12;
  int count{};

  sig::Signal<int> sig;
  Connect(sig, [&](int x) {
    EXPECT_EQ(a1, x);
    ++count;
  });

  sig(a1);
  EXPECT_EQ(1, count);

  count = 0;
  {
    auto slot = ConnectEx(sig, [&](int x) {
      EXPECT_EQ(a1, x);
      ++count;
    });

    sig(a1);
    EXPECT_EQ(2, count);
  }

  count = 0;
  sig(a1);
  EXPECT_EQ(1, count);
}

