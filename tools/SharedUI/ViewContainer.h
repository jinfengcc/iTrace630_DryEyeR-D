#pragma once

#include "WindowAnimate.h"
#include "IView.h"

class CViewContainer
  : public CWindowImpl<CViewContainer>
  , public CIdleHandler
{
  typedef CWindowImpl<CViewContainer> BaseClass;

public:
  using ViewPtr = std::unique_ptr<IView>;

  enum Animate {UPDN, LEFTRIGHT};
  CViewContainer(Animate mode)
    : m_animUp(mode == Animate::LEFTRIGHT ? CWindowAnimate::MODE::LEFT : CWindowAnimate::MODE::UP)
    , m_animDn(mode == Animate::LEFTRIGHT ? CWindowAnimate::MODE::RIGHT : CWindowAnimate::MODE::DN)
  {
  }
  
  ~CViewContainer();

  BOOL CreateContainer(HWND hWndParent, UINT nID);
  void Shutdown();
  void AddView(ViewPtr view);
  bool ShowViewNdx(unsigned ndx);

  int GetViewCount() const
  {
    return static_cast<int>(m_vdl.size());
  }
  
  int GetCurViewNdx() const
  {
    return m_curWndNdx < m_vdl.size() ? static_cast<int>(m_curWndNdx) : -1;
  }

protected:
  void OnSize(UINT nType, CSize size);

  BEGIN_MSG_MAP(CViewContainer)
    MSG_WM_SIZE(OnSize)
  END_MSG_MAP()

  BOOL OnIdle() override;

private:
  unsigned             m_curWndNdx  = 1000;
  unsigned             m_prevWndNdx = 1000;
  std::vector<ViewPtr> m_vdl;
  CWindowAnimate       m_wa;
  CWindowAnimate::MODE m_animUp, m_animDn;
};
