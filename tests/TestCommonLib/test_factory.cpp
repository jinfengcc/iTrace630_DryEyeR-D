#include "pch.h"
#include <libs/CommonLib/CreateObj.h>
#include <libs/CommonLib/AppClassFactory.h>
#include <libs/CommonLib/ClassFactoryImpl.h>

// {60874FBF-328D-4FFE-923E-F23E1412C330}
static const GUID guid1 = {0x60874fbf, 0x328d, 0x4ffe, {0x92, 0x3e, 0xf2, 0x3e, 0x14, 0x12, 0xc3, 0x30}};

#define API __declspec(dllexport)

// clang-format off
struct __declspec(uuid("528f2a9e-7a64-442f-954c-7336e9a14d3b"))
ITestObject1 : public IUnknown
{
};
struct __declspec(uuid("3b9f1340-8137-4983-aa15-f9f7d2b837f9"))
ITestObject2 : public IUnknown
{
};
// clang-format on

struct TestObject1 : Implements<ITestObject1>
{
  TestObject1()
    : Implements<ITestObject1>("TestObject")
  {
  }
};

namespace {
  struct LocalClassFactory : ClassFactoryImpl
  {
    LocalClassFactory()
  {
      AddCreator(__uuidof(ITestObject1), &LocalClassFactory::Create);
  }

  private:
    static bool Create(REFIID riid, void **ppv)
  {
      auto obj = new TestObject1();
      return obj->QueryInterface(riid, ppv) == S_OK;
  }
  };
} // namespace

extern "C" IClassFactory API *CreateClassFactory()
{
  static LocalClassFactory cf;
  return cf.Factory();
}

GTEST_TEST(AppClassFactory, Test1)
{
  AppClassFactory f;

  auto obj = CreateObj<ITestObject1>(&f);
  EXPECT_TRUE(obj);
}

GTEST_TEST(AppClassFactory, Test2)
{
  AppClassFactory f;

  auto obj = CreateObj<ITestObject2>(&f);
  EXPECT_TRUE(!obj);
}
