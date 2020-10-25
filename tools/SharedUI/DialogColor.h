#pragma once

class DialogColor : public CMessageMap {
public:
  DialogColor(COLORREF rgb = RGB(255, 255, 255))
     : m_rgb(rgb)
  {
    m_brush.CreateSolidBrush(rgb);
  }

private:
  COLORREF m_rgb;
  CBrush   m_brush;

  HBRUSH OnCtlColor(CDCHandle dc, CWindow)
  {
    dc.SetBkColor(m_rgb);
    dc.SelectBrush(m_brush);
    return static_cast<HBRUSH>(m_brush);
  }

  BEGIN_MSG_MAP_EX(DialogColor)
    MSG_WM_CTLCOLORDLG(OnCtlColor)       // For dialog background.
    MSG_WM_CTLCOLORSTATIC(OnCtlColor)    // For static and read only edit.
    MSG_WM_CTLCOLOREDIT(OnCtlColor)      // For edit boxes
    MSG_WM_CTLCOLORBTN(OnCtlColor)       // Owner-drawn only will respond.
    MSG_WM_CTLCOLORLISTBOX(OnCtlColor)   // List and combo.
    MSG_WM_CTLCOLORSCROLLBAR(OnCtlColor) // Scroll bars. No good for edit-box children.
  END_MSG_MAP()

};


