#pragma once

class CmdTargetBase : public CCmdTarget
{
  BOOL m_bHandled{};

protected:
  CmdTargetBase();

  void SetMsgHandled(BOOL handled)
  {
    m_bHandled = handled;
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

  static BOOL ForwardCommand(UINT nID);
};

inline BOOL IsShiftKeyPressed()
{
  return ::GetAsyncKeyState(VK_SHIFT) < 0;
}

inline BOOL IsCtrlKeyPressed()
{
  return ::GetAsyncKeyState(VK_CONTROL) < 0;
}
