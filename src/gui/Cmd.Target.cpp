#include "stdafx.h"
#include "Cmd.Target.h"

namespace {
  auto &GetList()
  {
    static std::list<CmdTargetBase *> list;
    return list;
  }
}

CmdTargetBase::CmdTargetBase(Attach a)
{
  if (a == Attach::yes)
    GetList().push_back(this);
}


CmdTargetBase::~CmdTargetBase()
{
  GetList().remove(this);
}

BOOL CmdTargetBase::OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
  m_msgHandled = TRUE;
  return CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) && GetMsgHandled();
}

BOOL CmdTargetBase::ForwardCommand(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
  try
  {
    for (auto l : GetList()) {
      l->m_msgHandled = TRUE;
      if (l->OnCmdMsg(nID, 0, nullptr, nullptr) && l->m_msgHandled)
        return TRUE;
    }
  }
  catch (const std::exception &e)
  {
    if (*e.what())
      ::Error(e.what());
  }
  
  return FALSE;
}
