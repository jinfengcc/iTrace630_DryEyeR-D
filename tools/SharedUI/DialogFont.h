#pragma once

#include <gsl/gsl>

template <class T>
class CDialogFont
{
public:
  enum class FontType { BOLD, ITALIC, BOLD_ITALIC};

  void SetCtrlFont(int idc, FontType type)
  {
    T *  pWnd = static_cast<T *>(this);
    auto font = GetFontCreateIfNeeded(type);
    pWnd->SendDlgItemMessage(idc, WM_SETFONT, (WPARAM)font, FALSE);
  }

  void SetCtrlFont(std::initializer_list<int> idcs, FontType type)
  {
    for (auto i : idcs)
      SetCtrlFont(i, type);
  }

  void SetCtrlFont(gsl::span<const int> idcs, FontType type)
  {
    for (auto i : idcs)
      SetCtrlFont(i, type);
  }

  CFontHandle GetDialogFont(FontType type)
  {
    return GetFontCreateIfNeeded(type);
  }

private:
  using Map = std::map<FontType, CFont>;
  Map m_map;

  HFONT GetFontCreateIfNeeded(FontType type)
  {
    if (!m_map.contains(type)) {
      LOGFONT lf = LogFont();
      if (type == FontType::BOLD || type == FontType::BOLD_ITALIC)
        lf.lfWeight = FW_BOLD;
      if (type == FontType::ITALIC || type == FontType::BOLD_ITALIC)
        lf.lfItalic = true;

      m_map[type].CreateFontIndirect(&lf);
    }
    return m_map[type];
  }

  LOGFONT LogFont()
  {
    CFontHandle font = static_cast<T *>(this)->GetFont();

    LOGFONT lf;
    font.GetLogFont(&lf);

    return lf;
  }

};
