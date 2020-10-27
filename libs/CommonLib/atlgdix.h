#ifndef __ATLGDIX_H__
#define __ATLGDIX_H__

/////////////////////////////////////////////////////////////////////////////
// Additional GDI/USER wrappers
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2001-2002 Bjarke Viksoe.
// Thanks to Daniel Bowen for COffscreenDrawRect.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

/*****
* Modified by TK
****/

#pragma once


namespace WTL {

  /////////////////////////////////////////////////////////////////////////////
  // COffscreenDraw

  // To use it, derive from it and chain it in the message map.
  template <class T>
  class COffscreenDraw
  {
  public:
    BEGIN_MSG_MAP(COffscreenDraw)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
    END_MSG_MAP()

    LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL & /*bHandled*/)
    {

      T *pT = static_cast<T *>(this);
      if (wParam != NULL) {
        RECT rc;
        pT->GetClientRect(&rc);

        CMemoryDC memdc((HDC)wParam, rc);
        pT->DoPaint(memdc.m_hDC);
      }
      else {
        CPaintDC dc(pT->m_hWnd);
        CMemoryDC memdc(dc.m_hDC, dc.m_ps.rcPaint);
        pT->DoPaint(memdc.m_hDC);
      }
      return 0;
    }
    LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
    {
      return 1; // handled; no need to erase background; do it in DoPaint();
    }
    void DoPaint(CDCHandle dc)
    {
      ATLASSERT(false); // must override this
    }
  };


  /////////////////////////////////////////////////////////////////////////////
  // CSaveDC

  class CSaveDC
  {
  public:
    HDC m_hDC;
    int m_iState;

    CSaveDC(HDC hDC)
      : m_hDC(hDC)
    {
      ATLASSERT(::GetObjectType(m_hDC) == OBJ_DC || ::GetObjectType(m_hDC) == OBJ_MEMDC);
      m_iState = ::SaveDC(hDC);
      ATLASSERT(m_iState != 0);
    }
    ~CSaveDC()
    {
      Restore();
    }
    void Restore()
    {
      if (m_iState == 0)
        return;
      ATLASSERT(::GetObjectType(m_hDC) == OBJ_DC || ::GetObjectType(m_hDC) == OBJ_MEMDC);
      ::RestoreDC(m_hDC, m_iState);
      m_iState = 0;
    }
  };

  /////////////////////////////////////////////////////////////////////////////
  // Mouse Hover helper

#ifndef NOTRACKMOUSEEVENT

#ifndef WM_MOUSEENTER
#define WM_MOUSEENTER WM_USER + 253
#endif // WM_MOUSEENTER

  // To use it, derive from it and chain it in the message map.
  // Make sure to set bHandled to FALSE when handling WM_MOUSEMOVE or
  // the WM_MOUSELEAVE message!
  template <class T>
  class CMouseHover
  {
  public:
    bool m_fMouseOver;        // Internal mouse-over state
    bool m_fMouseForceUpdate; // Update window immediately on event

    CMouseHover()
      : m_fMouseOver(false)
      , m_fMouseForceUpdate(true)
    {
    }

    void SetForceMouseOverUpdate(bool bForce = false)
    {
      m_fMouseForceUpdate = bForce;
    }

    BEGIN_MSG_MAP(CMouseHover)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
    END_MSG_MAP()

    LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
    {
      T *pT = static_cast<T *>(this);
      if (!m_fMouseOver) {
        m_fMouseOver = true;
        pT->SendMessage(WM_MOUSEENTER, wParam, lParam);
        if (m_fMouseForceUpdate) {
          pT->Invalidate();
          pT->UpdateWindow();
        }
        _StartTrackMouseLeave(pT->m_hWnd);
      }
      bHandled = FALSE;
      return 0;
    }
    LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
    {
      T *pT = static_cast<T *>(this);
      if (m_fMouseOver) {
        m_fMouseOver = false;
        if (m_fMouseForceUpdate) {
          pT->Invalidate();
          pT->UpdateWindow();
        }
      }
      bHandled = FALSE;
      return 0;
    }
    BOOL _StartTrackMouseLeave(HWND hWnd) const
    {
      ATLASSERT(::IsWindow(hWnd));
      TRACKMOUSEEVENT tme = {0};
      tme.cbSize          = sizeof(tme);
      tme.dwFlags         = TME_LEAVE;
      tme.hwndTrack       = hWnd;
      return _TrackMouseEvent(&tme);
    }
    BOOL _CancelTrackMouseLeave(HWND hWnd) const
    {
      TRACKMOUSEEVENT tme = {0};
      tme.cbSize          = sizeof(tme);
      tme.dwFlags         = TME_LEAVE | TME_CANCEL;
      tme.hwndTrack       = hWnd;
      return _TrackMouseEvent(&tme);
    }
  };

#endif // NOTRACKMOUSEEVENT

}; // namespace WTL

#endif // __ATLGDIX_H__
