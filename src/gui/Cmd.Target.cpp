#include "stdafx.h"
#include "Cmd.Target.h"

namespace {
  auto &GetList()
  {
    static std::list<CmdTargetBase *> list;
    return list;
  }
}

CmdTargetBase::CmdTargetBase()
{
  GetList().push_back(this);
}


CmdTargetBase::~CmdTargetBase()
{
  GetList().remove(this);
}

BOOL CmdTargetBase::ForwardCommand(UINT nID)
{
  for (auto l : GetList()) {
    l->m_bHandled = TRUE;
    if (l->OnCmdMsg(nID, 0, nullptr, nullptr) && l->m_bHandled)
      return TRUE;
  }
  return FALSE;
}
