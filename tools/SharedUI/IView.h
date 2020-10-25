#pragma once

class IView
{
public:
  virtual ~IView() = default;

  virtual HWND GetHWnd() const = 0;
  virtual bool OnHide(bool /*movingForward*/)
  {
    return true;
  }
  virtual void OnShow()
  {
  }
  virtual void OnTerminating()
  {
  }
};

