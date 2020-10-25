#include "pch.h"
#include "WinHelpers.h"

CRect GetSameAspectRect(CRect rc, const CRect &refRect)
{
  int nHeight = MulDiv(rc.Width(), refRect.Height(), refRect.Width());
  if (nHeight == rc.Height())
    return rc;

  if (nHeight < rc.Height()) {
    // Tall
    rc.top    = (rc.top + rc.bottom) / 2 - nHeight / 2;
    rc.bottom = rc.top + nHeight;
  }
  else {
    // Wide
    int nWidth = MulDiv(rc.Height(), refRect.Width(), refRect.Height());
    rc.left    = (rc.left + rc.right) / 2 - nWidth / 2;
    rc.right   = rc.left + nWidth;
  }

  return rc;
}

std::vector<HWND> WindowAreaControls(HWND hWnd, int nIDMarker)
{
  std::vector<HWND> ws;

  if (HWND hWndMarker = ::GetDlgItem(hWnd, nIDMarker)) {
    CRect rcDefArea;
    ::GetWindowRect(hWndMarker, rcDefArea);
    rcDefArea.InflateRect(-1, -1);

    for( hWnd = ::GetTopWindow(hWnd); hWnd; hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT)) {
      CRect rc;
      ::GetWindowRect(hWnd, rc); // screen coordinates

      if (rcDefArea.PtInRect(rc.TopLeft()))
        ws.push_back(hWnd);
    }
  }

  return ws;
}

std::unique_ptr<Gdiplus::Bitmap> Mat2Bitmap(const cv::Mat &img)
{
  auto size   = img.size();
  auto stride = img.step1();

  auto bmp = std::make_unique<Gdiplus::Bitmap>(size.width, size.height, stride, PixelFormat24bppRGB, img.data);

  // Gdiplus bitmap cannot cop with any stride value, so sometimes direct copying of pixels is required
  if (bmp->GetLastStatus() != Gdiplus::Ok) {
    using namespace Gdiplus;

    bmp = std::make_unique<Bitmap>(size.width, size.height, PixelFormat24bppRGB);
    BitmapData bmData;
    if (bmp->LockBits(NULL, ImageLockModeWrite | ImageLockModeRead, PixelFormat32bppARGB, &bmData) == Ok) {
      for (size_t iy = 0; iy < bmData.Height; iy++) {
        auto pPixels = static_cast<DWORD *>(bmData.Scan0) + iy * bmData.Stride / 4;
        memcpy(pPixels, img.ptr(iy), bmData.Stride);
      }
      bmp->UnlockBits(&bmData);
    }
  }
  return bmp;
}

void DrawImage(CDCHandle dc, const RECT &rc, const cv::Mat &img)
{
  Gdiplus::Graphics g(dc);
  if (!img.empty()) {
    DrawImage(g, rc, img);
  }
  else {
    Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0), 5.0f);
    g.DrawLine(&pen, (int)rc.left, (int)rc.top, (int)rc.right, (int)rc.bottom);
    g.DrawLine(&pen, (int)rc.left, (int)rc.bottom, (int)rc.right, (int)rc.top);
  }
}

void DrawImage(Gdiplus::Graphics &g, const RECT &rc, const cv::Mat &img)
{
  auto bmp = Mat2Bitmap(img);
  auto rc1 = GetSameAspectRect(rc, CRect(0, 0, bmp->GetWidth(), bmp->GetHeight()));
  g.DrawImage(bmp.get(), rc1.left, rc1.top, rc1.Width(), rc1.Height());
}
