// MainViewDlg.cpp : implementation of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "FlashView.h"
#include <interfaces/hal/IFlash.h>
#include <interfaces/ICalibration.h>
#include <libs/CommonLib/Guid.h>

BOOL CFlashView::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
  InitDynamicLayout(false, false);
  return TRUE;
}

void CFlashView::OnReadClicked(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  auto flash = CreateObj<hal::IFlash>();

  LoadFlashSize(flash.get());
  LoadFlashContents(flash.get());

  LoadDbCalibration();
}

void CFlashView::OnWriteClicked(UINT uNotifyCode, int nID, CWindow wndCtl)
{
  AtlMessageBox(nullptr, L"Not Implemented yet", L"Oops", MB_ICONSTOP);
}

/////////////////////////////////////////////////////////////////////////////////////////

void CFlashView::LoadFlashSize(IFlash *flash)
{
  auto size = flash->Size();
  SetDlgItemText(IDC_EDIT1, fmt::format(L"{0} (0x{0:x})", size).c_str());
}

void CFlashView::LoadFlashContents(IFlash *flash)
{
  auto cd  = std::make_unique<CALIBRATION_V1>();
  auto cal = CreateObj<ICalibration>();

  if (!cal->Read({flash}, *cd))
    return;

  ATLASSERT(cal->Verify({flash}, *cd));
}

void CFlashView::LoadDbCalibration()
{
  AtlMessageBox(*this, L"Not implemented yet");
}
