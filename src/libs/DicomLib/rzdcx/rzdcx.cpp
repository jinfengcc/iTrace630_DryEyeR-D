#include "pch.h"
#include "rzdcx.h"
#include <libs/CommonLib/AppFiles.h>

using rzdcxLib::IDCXOBJ;

RZDCX::RZDCX()
{
  auto rzdcx = app::GetAppPath(app::Type::exeFolder) / "rzdcx.dll";
  m_rzdcxLib.Load(rzdcx.wstring().c_str());

  //m_rzdcxLib.Load(LR"(C:\RZDCX\rzdcx.dll)");
}

void RZDCX::SetLogging(LogLevel logLevel, const fs::path &logFile)
{
  if (logLevel == m_logLevel && logFile == m_logFile)
    return;

  m_logLevel = logLevel;
  m_logFile = logFile;
  if (logLevel != LogLevel::None) {
    m_app = CreateInstance<IDCXAPP>();

    m_app->LogLevel = static_cast<rzdcxLib::LOG_LEVEL>(logLevel);
    m_app->StartLogging(logFile.string().c_str());
  }
  else {
    m_app = nullptr;
  }
}

bool RZDCX::TestActivation()
{
  const int N = 100 + 10;

  for (int i = 0; i < N; i++) {
    try {
      auto obj = CreateInstance<IDCXOBJ>();
    }
    catch (const _com_error) {
      return false;
    }
  }

  return true;
}

