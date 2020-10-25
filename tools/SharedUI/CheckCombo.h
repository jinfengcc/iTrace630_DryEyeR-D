#if !defined(AFX_CHECKCOMBO_H__20030420_CFDE_36CE_899B_0080AD509054__INCLUDED_)
#define AFX_CHECKCOMBO_H__20030420_CFDE_36CE_899B_0080AD509054__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CheckComboCtrl - ComboBox with checkboxes
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2003 Bjarke Viksoe.
//
// Add the following macro to the parent's message map:
//   REFLECT_NOTIFICATIONS()
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __cplusplus
#error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLCTRLS_H__
#error CheckCombo.h requires atlctrls.h to be included first
#endif

#if (_WIN32_IE < 0x0400)
#error CheckCombo.h requires _WIN32_IE >= 0x0400
#endif

/////////////////////////////////////////////////////////////////////////////
//

#define CCBS_EX_NONE             0x00000000
#define CCBS_EX_CLOSEIMMEDIATELY 0x00000001
#define CCBS_EX_ADJUSTEDITHEIGHT 0x00000002
#define CCBS_EX_FIRST_ITEM_ALL   0x00000004
#define CCBS_EX_ALL_DETAILS      0x00000008

template <class T, class TBase = CComboBox, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CCheckComboImpl
  : public CWindowImpl<T, TBase, TWinTraits>
  , public COwnerDraw<T>
{
public:
  DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

  CCheckComboImpl(DWORD dwExtStyle)
    : m_ctrlList(this, 1)
    , m_dwExtStyle(dwExtStyle)
  {
  }

  CContainedWindowT<CListBox> m_ctrlList;
  DWORD                       m_dwExtStyle;

  // Operations

  BOOL SubclassWindow(HWND hWnd)
  {
    ATLASSERT(m_hWnd == NULL);
    ATLASSERT(::IsWindow(hWnd));
    BOOL bRet = CWindowImpl<T, TBase, TWinTraits>::SubclassWindow(hWnd);
    if (bRet)
      _Init();
    return bRet;
  }

  DWORD SetExtendedComboStyle(DWORD dwStyle)
  {
    ATLASSERT(::IsWindow(m_hWnd));
    DWORD dwOldStyle = m_dwExtStyle;
    m_dwExtStyle     = dwStyle /* | CCBS_EX_FIRST_ITEM_ALL*/;
    _Init();
    return dwOldStyle;
  }

  int AddString(LPCTSTR pstrText, BOOL bCheck = FALSE, BOOL bEnable = TRUE)
  {
    ATLASSERT(::IsWindow(m_hWnd));
    int iRes = TBase::AddString(pstrText);
    if (iRes >= 0)
      SetItemDataImpl(iRes, bCheck, bEnable);
    return iRes;
  }

  void SetEnableState(int iIndex, BOOL enable)
  {
    SetItemDataImpl(iIndex, -1, enable);
    Invalidate();
  }

  bool GetEnableState(int iIndex) const
  {
    return GetItemDataImpl(iIndex).bits.enabled;
  }

  std::function<void()>                      m_cbOnItemsChanged;
  std::function<void(int ndx, bool checked)> m_cbOnItemChecked;

  void SetCheckState(int iIndex, BOOL bCheck)
  {
    if (!GetEnableState(iIndex))
      return;

    SetItemDataImpl(iIndex, bCheck, -1);
    if (m_cbOnItemChecked)
      m_cbOnItemChecked(iIndex, bCheck ? true : false);

    Invalidate();
  }
  BOOL GetCheckState(int iIndex) const
  {
    return GetItemDataImpl(iIndex).bits.checked;
  }

  // Deprecated methods

  int GetCurSel() const
  {
    ATLASSERT(false);
    return -1;
  }
  int SetCurSel(int /*iIndex*/)
  {
    ATLASSERT(false);
    return -1;
  }
  LPARAM GetItemData(int /*iIndex*/) const
  {
    ATLASSERT(false);
    return -1;
  }
  DWORD_PTR SetItemData(int /*iIndex*/, DWORD_PTR /*dwItemData*/)
  {
    ATLASSERT(false);
    return -1;
  }

  // Implementation

  void _Init()
  {
    ATLASSERT(::IsWindow(m_hWnd));
    // Need to set these in resource editor
    ATLASSERT((GetStyle() & 0x0F) == CBS_DROPDOWNLIST);
    ATLASSERT((GetStyle() & CBS_OWNERDRAWFIXED) != 0);
    ATLASSERT((GetStyle() & CBS_HASSTRINGS) != 0);

    if ((m_dwExtStyle & CCBS_EX_ADJUSTEDITHEIGHT) != 0) {
      MEASUREITEMSTRUCT mis;
      MeasureItem(&mis);
      SetItemHeight(-1, mis.itemHeight);
    }

    Invalidate();
  }

  // Message map and handlers

  BEGIN_MSG_MAP(CCheckComboImpl)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
    MESSAGE_HANDLER(WM_CHAR, OnChar)
    CHAIN_MSG_MAP_ALT(COwnerDraw<T>, 1)
    ALT_MSG_MAP(1)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnListLButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnListRButtonDown)
    MESSAGE_HANDLER(WM_CHAR, OnListChar)
  END_MSG_MAP()

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
  {
    LRESULT lRes = DefWindowProc();
    _Init();
    return lRes;
  }
  LRESULT OnCtlColorListBox(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &bHandled)
  {
    if (!m_ctrlList.IsWindow())
      m_ctrlList.SubclassWindow((HWND)lParam);
    bHandled = FALSE;
    return 0;
  }
  LRESULT OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL &bHandled)
  {
    if (wParam == _T(' ')) {
      if (!m_ctrlList.IsWindow() || !m_ctrlList.IsWindowVisible()) {
        ShowDropDown(TRUE);
        return 0;
      }
    }
    bHandled = FALSE;
    return 0;
  }
  LRESULT OnListLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &bHandled)
  {
    CRect rc;
    m_ctrlList.GetClientRect(&rc);
    POINT pt      = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    int   iHeight = GetItemHeight(0);
    if (!rc.PtInRect(pt)) { // pt.x > iHeight ) {
      bHandled = FALSE;
      return 0;
    }
    int iTopIndex = GetTopIndex();
    int iIndex    = (pt.y / iHeight) + iTopIndex;
    SetCheckState(iIndex, !GetCheckState(iIndex));
    if (m_dwExtStyle & CCBS_EX_FIRST_ITEM_ALL) {
      if (iIndex == 0) {
        for (int i = 1; i < GetCount(); i++)
          SetCheckState(i, GetCheckState(0));
      }
      else {
        SetCheckState(0, AllChecked());
      }
    }

    if (m_cbOnItemsChanged)
      m_cbOnItemsChanged();
    m_ctrlList.Invalidate();
    if ((m_dwExtStyle & CCBS_EX_CLOSEIMMEDIATELY) != 0)
      bHandled = FALSE;
    return 0;
  }
  LRESULT OnListRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &bHandled)
  {
    POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    for (int i = 0; i < GetCount(); ++i)
      SetCheckState(i, TRUE);

    if (m_cbOnItemsChanged)
      m_cbOnItemsChanged();
    m_ctrlList.Invalidate();

    if ((m_dwExtStyle & CCBS_EX_CLOSEIMMEDIATELY) != 0)
      bHandled = FALSE;
    return 0;
  }
  LRESULT OnListChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL &bHandled)
  {
    if (wParam == _T(' ')) {
      int iIndex = m_ctrlList.GetCaretIndex();
      SetCheckState(iIndex, !GetCheckState(iIndex));
      m_ctrlList.Invalidate();
      if ((m_dwExtStyle & CCBS_EX_CLOSEIMMEDIATELY) != 0)
        ShowDropDown(FALSE);
      return 0;
    }
    bHandled = FALSE;
    return 0;
  }

  // COwnerDraw

  void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
  {
    CClientDC  dc(m_hWnd);
    TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);
    lpMeasureItemStruct->itemHeight = tm.tmHeight + 1;
  }

  std::function<std::wstring(int ndx, const TCHAR *sz)> m_cbAbrevFunc;

  void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
  {
    CDCHandle dc        = lpDrawItemStruct->hDC;
    RECT      rcItem    = lpDrawItemStruct->rcItem;
    bool      bFocus    = (lpDrawItemStruct->itemState & ODS_FOCUS) != 0;
    bool      bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;

    // Never paint selected color for combobox itself (edit part)
    if ((lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT) != 0) {
      std::wstring title;
      if (m_cbAbrevFunc)
        title = m_cbAbrevFunc(-1, nullptr);

      if (title.size() == 0) {
        if (AllChecked()) // NumChecked() > 1)
        {
          if (m_dwExtStyle & CCBS_EX_FIRST_ITEM_ALL) {
            TCHAR sz[512];
            GetLBText(0, sz);
            title = sz;
          }
          else if ((m_dwExtStyle & CCBS_EX_ALL_DETAILS) == 0) {
            title = AllChecked() ? _T("All") : _T("Multiple");
          }
        }

        if (title.empty()) {
          const size_t maxLen = 512;
          title = GetCheckedList(maxLen);
        }
      }
      dc.SetBkMode(TRANSPARENT);
      dc.FillRect(&rcItem, ::GetSysColorBrush(bSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW));
      dc.SetTextColor(::GetSysColor(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));
      dc.DrawText(title.c_str(), title.size(), &rcItem, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
      return;
    }

    if (lpDrawItemStruct->itemID == -1)
      return;

    // Erase background
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    dc.FillRect(&rcItem, ::GetSysColorBrush(COLOR_WINDOW));
    rcItem.left += 2;

    int  cx       = rcItem.bottom - rcItem.top;
    RECT rcButton = {rcItem.left, rcItem.top + 1, rcItem.left + cx, rcItem.bottom - 1};
    if (lpDrawItemStruct->itemID == 0 && (m_dwExtStyle & CCBS_EX_FIRST_ITEM_ALL) != 0) {
      if (AllChecked())
        dc.DrawFrameControl(&rcButton, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_CHECKED);
      else if (NumChecked() == 0)
        dc.DrawFrameControl(&rcButton, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT);
      else
        dc.DrawFrameControl(&rcButton, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_INACTIVE);
    }
    else {
      ItemData itemData;
      itemData.itemData = lpDrawItemStruct->itemData;
      dc.DrawFrameControl(&rcButton, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT | (itemData.bits.checked ? DFCS_CHECKED : 0));
    }
    rcItem.left += cx + 2;

    DrawLBTextLine(dc, lpDrawItemStruct->itemID, rcItem);

    if (bFocus) {
      RECT rcFocus = rcItem;
      // dc.DrawText(szItem, -1, &rcFocus, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_CALCRECT);
      // rcFocus.bottom += 1;
      //::InflateRect(&rcFocus, 2, 0);
      dc.DrawFocusRect(&rcFocus);
    }
  }

  std::wstring GetCheckedList(const size_t maxLen = std::numeric_limits<size_t>::max())
  {
    std::wstring title;
    for (int i = 0; i < GetCount() && title.size() < maxLen; i++) {
      if (!GetCheckState(i))
        continue;

      // Get item text
      TCHAR szItem[256];
      int   nItemLen = GetLBTextLen(i);
      ATLASSERT(nItemLen < sizeof(szItem) / sizeof(TCHAR));
      if (nItemLen >= sizeof(szItem) / sizeof(TCHAR))
        break;
      GetLBText(i, szItem);
      if (m_cbAbrevFunc != 0) {
        auto str = m_cbAbrevFunc(i, szItem);
        if (str.size() != 0)
          _tcscpy_s(szItem, str.c_str());
      }

      if (title.size() > 0)
        title += L", ";
      title += szItem;
    }

    return title;
  }

  std::function<bool(CDCHandle dc, int itemNdx, RECT &rcItem, const TCHAR *szItem)> m_drawFunc;
  void DrawLBTextLine(CDCHandle dc, int itemNdx, RECT &rcItem) const
  {
    TCHAR szItem[256] = {0};
    ATLASSERT(GetLBTextLen(itemNdx) < _countof(szItem));
    GetLBText(itemNdx, szItem);
    if (m_drawFunc == nullptr || !m_drawFunc(dc, itemNdx, rcItem, szItem)) {
      dc.DrawText(szItem, -1, &rcItem, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
    }
  }

  bool AllChecked() const
  {
    const int n = GetCount();
    return n > 0 && n == NumChecked();
  }
  int NumChecked() const
  {
    int n = 0;
    for (int i = 0; i < GetCount(); i++) {
      if (GetCheckState(i))
        ++n;
    }
    return n;
  }

  DWORD GetCheckedMask() const
  {
    DWORD n = 0;
    for (int i = 0; i < GetCount(); i++) {
      if (GetCheckState(i))
        n |= (1 << i);
    }
    return n;
  }
  void SetCheckedMask(DWORD n)
  {
    for (int i = 0; i < GetCount(); i++)
      SetCheckState(i, n & (1 << i));
  }

private:
  union ItemData {
    DWORD itemData;
    struct
    {
      DWORD
      checked : 1, enabled : 1;

    } bits;
  };

  void SetItemDataImpl(int idx, BOOL checked, BOOL enabled)
  {
    ItemData d = GetItemDataImpl(idx);

    if (checked >= 0)
      d.bits.checked = checked != 0;

    if (enabled >= 0)
      d.bits.enabled = enabled != 0;

    TBase::SetItemData(idx, d.itemData);
  }

  ItemData GetItemDataImpl(int idx) const
  {
    ATLASSERT(::IsWindow(m_hWnd));

    ItemData d;
    d.itemData = TBase::GetItemData(idx);
    return d;
  }
};

class CCheckComboCtrl
  : public CCheckComboImpl<CCheckComboCtrl, CComboBox, CWinTraitsOR<CBS_OWNERDRAWVARIABLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS>>
{
public:
  CCheckComboCtrl(DWORD dwExtStyle)
    : CCheckComboImpl(dwExtStyle)
  {
  }

  DECLARE_WND_SUPERCLASS(_T("WTL_CheckedComboBox"), GetWndClassName())
};

#endif // !defined(AFX_CHECKCOMBO_H__20030420_CFDE_36CE_899B_0080AD509054__INCLUDED_)
