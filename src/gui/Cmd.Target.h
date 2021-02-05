#pragma once

class CmdTargetBase : public CCmdTarget
{
  BOOL m_msgHandled{};

protected:
  enum class Attach {no, yes};
  CmdTargetBase(Attach a = Attach::yes);

  void SetMsgHandled(BOOL handled)
  {
    m_msgHandled = handled;
  }
  BOOL GetMsgHandled() const
  {
    return m_msgHandled;
  }

  static CRect GetButtonRect()
  {
    CRect rc;
    if (HWND hWnd = (HWND)AfxGetThreadState()->m_msgCur.lParam; IsWindow(hWnd))
      ::GetWindowRect(hWnd, &rc);
    return rc;
  }

public:
  virtual ~CmdTargetBase();

  BOOL OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo) override;

  static BOOL ForwardCommand(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo);
};

inline BOOL IsShiftKeyPressed()
{
  return ::GetAsyncKeyState(VK_SHIFT) < 0;
}

inline BOOL IsCtrlKeyPressed()
{
  return ::GetAsyncKeyState(VK_CONTROL) < 0;
}
