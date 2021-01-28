#pragma once

#include <vector>

class CCustomMenu : public CMenu
{
public:
  CCustomMenu(int width = 120, int height = 64);
  ~CCustomMenu();

	BOOL LoadMenu(LPCTSTR lpszResourceName);
  BOOL LoadMenu(UINT nIDResource);
  void AddMenuItem(const TCHAR *sz);

  void AddMenuItems(std::initializer_list<const TCHAR *> list)
  {
    for (auto i : list)
      AddMenuItem(i);
  }

  void SetSelection(int n)
  {
    m_selected = n;
  }

	int TrackPopupMenu(int x, int y, CWnd *pWnd = AfxGetMainWnd());

protected:
  void SetSize(SIZE size)
  {
    m_size = size;
  }

private:
  struct MenuData
  {
    CString strCaption;
  };

  CSize                 m_size;
  std::vector<MenuData> m_menuData;
  int                   m_selected{-1};

  void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) override;
  void DrawItem(LPDRAWITEMSTRUCT lpds) override;

  void DrawFrame(Gdiplus::Graphics &g, Gdiplus::RectF &rc, LPDRAWITEMSTRUCT lpds);
  void DrawText(Gdiplus::Graphics &g, const Gdiplus::RectF &rc, unsigned n);
};

class CCustomButtonMenu : public CCustomMenu
{
public:
  CCustomButtonMenu(const CRect &rc)
    : m_rc(rc)
  {
    SetSize(m_rc.Size());
  }

  CCustomButtonMenu()
  {
    HWND hWnd = (HWND)AfxGetThreadState()->m_msgCur.lParam;
    ASSERT(IsWindow(hWnd));
    ::GetWindowRect(hWnd, &m_rc);
    SetSize(m_rc.Size());
  }

 	int TrackPopupMenu(CWnd *pWnd = AfxGetMainWnd())
  {
    return CCustomMenu::TrackPopupMenu(m_rc.left, m_rc.top, pWnd);
  }

private:
  CRect m_rc;
};
