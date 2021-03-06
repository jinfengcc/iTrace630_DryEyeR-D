#pragma once

#include <UIAnimation.h>
#include <libs/CommonLib/Implements.h>

namespace Animation {
  class Manager;

  class Storyboard
  {
  public:
    Storyboard(Manager *m);
    ~Storyboard();

    enum class MODE { AccelDesel, SmoothStop };
    void AnimateVariable(IUIAnimationVariable *pv, double endValue, double duration, MODE mode = MODE::AccelDesel);

  private:
    CComPtr<IUIAnimationStoryboard> m_sb;
  };

  class Manager
  {
    friend class Storyboard;

  public:
    ~Manager();

    static Manager *Instance();

    void SetTimerEventHandler(IUIAnimationTimerEventHandler *p);

    CComPtr<IUIAnimationVariable>   CreateAnimationVariable(double initialValue);
    CComPtr<IUIAnimationStoryboard> CreateStoryBoard();
    CComPtr<IUIAnimationVariable>   CreateAndAnimateVariable(double start, double end, double duration);

  private:
    Manager();
    void CheckObjs()
    {
      ATLASSERT(m_animObjs);
      if (m_animObjs == 0)
        throw std::runtime_error("Animation not supported in this platform");
    }

    struct AnimObj
    {
      CComPtr<IUIAnimationManager>           animationManager;
      CComPtr<IUIAnimationTimer>             animationTimer;
      CComPtr<IUIAnimationTransitionLibrary> transitionLibrary;
    };

    std::unique_ptr<AnimObj> m_animObjs;
  };
} // namespace Animation

class CTimerEventHandler : public ImplementsStatic<IUIAnimationTimerEventHandler>
{
public:
  CTimerEventHandler(HWND hWnd)
    : m_hWnd(hWnd)
  {
  }

  // IUIAnimationTimerEventHandler
  IFACEMETHODIMP OnPreUpdate()
  {
    return S_OK;
  }

  IFACEMETHODIMP OnPostUpdate()
  {
    ::InvalidateRect(m_hWnd, nullptr, FALSE);
    return S_OK;
  }

  IFACEMETHODIMP OnRenderingTooSlow(UINT32 /* fps */)
  {
    return S_OK;
  }

protected:
  HWND m_hWnd;
};

class CTimerEventHandlerImpl : public ImplementsStatic<IUIAnimationTimerEventHandler>
{
public:
  // IUIAnimationTimerEventHandler
  IFACEMETHODIMP OnPreUpdate() override
  {
    return S_OK;
  }

  IFACEMETHODIMP OnRenderingTooSlow(UINT32 /* fps */) override
  {
    return S_OK;
  }
};
