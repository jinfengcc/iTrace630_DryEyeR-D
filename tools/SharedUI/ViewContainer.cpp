#include "pch.h"
#include "ViewContainer.h"
#include "SimpleTabCtrl.h"

////////////////////////////////////////////////////////////////////////////////////////////////

CViewContainer::~CViewContainer()
{
}

BOOL CViewContainer::CreateContainer(HWND hWndParent, UINT nID)
{
  CWindow parent = hWndParent;
  CWindow wnd    = parent.GetDlgItem(nID);
  if (wnd == 0)
    return FALSE;

  CRect rc;
  wnd.GetWindowRect(&rc);
  parent.ScreenToClient(&rc);

  if (!BaseClass::Create(hWndParent, &rc, _T("Container"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT, nID))
    return FALSE;

  SetWindowPos(wnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  wnd.DestroyWindow();

  // register object for idle updates
  if (CMessageLoop *pLoop = _Module.GetMessageLoop())
    pLoop->AddIdleHandler(this);

  return TRUE;
}

void CViewContainer::Shutdown()
{
  if (m_curWndNdx >= 0 && m_curWndNdx < m_vdl.size())
    m_vdl[m_curWndNdx]->OnTerminating();
}

void CViewContainer::AddView(ViewPtr view)
{
  CRect rc;
  GetWindowRect(&rc);
  ::MoveWindow(view->GetHWnd(), 0, 0, rc.Width(), rc.Height(), FALSE);

  m_vdl.push_back(std::move(view));
}

bool CViewContainer::ShowViewNdx(unsigned nextWnd)
{
  if (m_curWndNdx == nextWnd)
    false;

  auto vf = [this](unsigned id) { return id < m_vdl.size() ? m_vdl[id].get() : nullptr; };

  auto curView  = vf(m_curWndNdx);
  auto nextView = vf(nextWnd);

  if (curView && !curView->OnHide(nextWnd > m_curWndNdx))
    return false;

  m_prevWndNdx = m_curWndNdx;
  m_curWndNdx  = nextWnd;

  if (m_curWndNdx < m_vdl.size() && m_prevWndNdx < m_vdl.size()) {
    auto mode = m_prevWndNdx < m_curWndNdx ? m_animUp : m_animDn;
    m_wa.Animate(m_vdl[m_prevWndNdx]->GetHWnd(), m_vdl[m_curWndNdx]->GetHWnd(), mode);
  }
  else {
    ::ShowWindow(m_vdl[m_curWndNdx]->GetHWnd(), SW_SHOW);
  }

  if (nextView)
    nextView->OnShow();

  return true;
}

void CViewContainer::OnSize(UINT nType, CSize size)
{
  HWND hWnd = GetTopWindow();
  while (hWnd != nullptr) {
    CWindow w = hWnd;
    w.MoveWindow(0, 0, size.cx, size.cy);
    w.Invalidate();
    w.Detach();

    hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
  }
}

BOOL CViewContainer::OnIdle()
{
  return FALSE;
}
