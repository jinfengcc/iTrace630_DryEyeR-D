#pragma once

#ifndef SIMPLETABCTRL_IMPL
#error This file should not be included here
#endif

#include "SimpleTabCtrl.h"
#include "IView.h"
#include "AnimationHelper.h"

class CSimpleTabCtrlVert
   : public CBufferedPaintWindowImpl<CSimpleTabCtrlVert>
   , public CSimpleTabCtrl
   , private CTimerEventHandlerImpl
{
public:
   CSimpleTabCtrlVert( HWND hParentWnd, int idc, std::function<bool(unsigned ndx)> func);

   void  AddButton ( const TCHAR *psz) override;
   void  AddSeparator() override;
   void  EnableView(bool b) override;
   void  SelectItem(size_t ndx) override;

   void DoPaint(CDCHandle dc, RECT& rect);

   void    OnMouseMove  ( UINT nFlags, CPoint point);
   void    OnLButtonDown( UINT nFlags, CPoint point);

private:
   enum class Mode
   {
      NORMAL, HOVER, SELECTED,
   };
   struct BtnData
   {
      std::wstring  title;
      CRect         rc;
      Mode          mode = Mode::NORMAL;
      bool          separatorAbove{ false };
   };

   HWND                               m_hParentWnd;
   bool                               m_enabled = true;
   bool                               m_separatorPending{ false };
   std::function<bool(unsigned ndx)>  m_func;
   std::vector<BtnData>               m_bvl;
   CComPtr<IUIAnimationVariable>      m_animColor;

   using ColorPair = std::pair<COLORREF, COLORREF>;

   void        PrepareToDraw(CDCHandle &dc, CRect rc, Mode mode);
   void        DoPaintImpl  (HDC dc, CRect rc);
   ColorPair   GetColor     (Mode mode);

   IFACEMETHODIMP OnPostUpdate()
   {
      ::InvalidateRect(m_hWnd, NULL, FALSE);
      return S_OK;
   }
   static COLORREF Interpolate(COLORREF a0, COLORREF a1, double p)
   {
      BYTE r = static_cast<BYTE>(GetRValue(a0) * p + GetRValue(a1) * (1 - p));
      BYTE g = static_cast<BYTE>(GetGValue(a0) * p + GetGValue(a1) * (1 - p));
      BYTE b = static_cast<BYTE>(GetBValue(a0) * p + GetBValue(a1) * (1 - p));

      return RGB(r, g, b);
   }

   BEGIN_MSG_MAP_EX(CSimpleTabCtrlVert)
     MSG_WM_LBUTTONDOWN(OnLButtonDown)
     MSG_WM_MOUSEMOVE(OnMouseMove)
     CHAIN_MSG_MAP(CBufferedPaintWindowImpl<CSimpleTabCtrlVert>)
   END_MSG_MAP()
};
