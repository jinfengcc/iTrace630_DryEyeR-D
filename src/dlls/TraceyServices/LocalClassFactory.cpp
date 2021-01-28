#include "pch.h"

#define API __declspec(dllexport)

#include <libs/CommonLib/ClassFactoryImpl.h>
#include "Calibration.h"

#include "Progress.h"

namespace {
  struct LocalClassFactory : ClassFactoryImpl
  {
    LocalClassFactory()
    {
      // clang-format off
      AddCreator(__uuidof(ILogger        ), &LocalClassFactory::CreateLogger);
      AddCreator(__uuidof(ILoggerSettings), &LocalClassFactory::CreateLogger);
      AddCreator(__uuidof(ICalibration   ), &LocalClassFactory::Create<Calibration>);
      AddCreator(__uuidof(IProgress      ), &LocalClassFactory::Create<Progress>);
      // clang-format on
    }

  private:
    static bool CreateLogger(REFIID riid, void **ppv)
    {
      static Logger logger;
      return logger.QueryInterface(riid, ppv) == S_OK;

      //static std::unique_ptr<Logger> logger;
      //if (!logger) {
      //  logger = std::make_unique<Logger>();

      //  logger->Enable(ILogger::Type::Info, true);
      //  logger->Enable(ILogger::Type::Diag, true);
      //  logger->SetOutFile("D:/tracey.log", 8 * 1024);
      //}

      //return logger->QueryInterface(riid, ppv) == S_OK;
    }
  };
} // namespace

extern "C" IClassFactory API *CreateClassFactory()
{
  static LocalClassFactory cf;
  return cf.Factory();
}

