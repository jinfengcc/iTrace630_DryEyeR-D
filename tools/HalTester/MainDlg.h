// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "FlashView.h"
#include <SimpleTabCtrl.h>
#include <ViewContainer.h>

class CMainDlg
  : public CDialogImpl<CMainDlg>
  , public CDynamicDialogLayout<CMainDlg>
  , public CUpdateUI<CMainDlg>
  , public CMessageFilter
  , public CIdleHandler
{
public:
  CMainDlg()
  {
    CreateFonts();
  }

  enum { IDD = IDD_MAINDLG };

private:
  enum class Sect { Tab, Contents };

  BOOL PreTranslateMessage(MSG *pMsg) override;
  BOOL OnIdle() override;

  CRect GetSectRect(Sect s) const;
  void  DrawVersion(CDCHandle &dc, CRect rcc);
  void  CreateFonts();

  std::unique_ptr<CSimpleTabCtrl> m_btnViewGroup;
  CViewContainer                  m_viewContainer{CViewContainer::Animate::UPDN};
  CFont                           m_font;

  template <class View>
  void CreateView(const TCHAR *name)
  {
    auto view = std::make_unique<View>();
    view->Create(m_viewContainer);
    m_viewContainer.AddView(std::move(view));
    m_viewContainer.ShowViewNdx(0);
    m_btnViewGroup->AddButton(name);
  }

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/);

  void InitializeViews();
  void SetVersionInfo();
  void SetIcons();

  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/);
  void    OnDpiChanged(UINT nDpiX, UINT nDpiY, PRECT pRect);
  LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/);
  BOOL    OnEraseBackground(CDCHandle dc);
  HBRUSH  OnCtlColorButton(CDCHandle dc, HWND /*hWnd*/);
  bool VerifyInstrument(bool quiet = true);

  friend class CUpdateUI<CMainDlg>;
  BEGIN_UPDATE_UI_MAP(CMainDlg)
  END_UPDATE_UI_MAP()

  BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    //MSG_WM_DPICHANGED(OnDpiChanged)
    MSG_WM_ERASEBKGND(OnEraseBackground)
    MSG_WM_CTLCOLORBTN(OnCtlColorButton)
    MSG_WM_CTLCOLORSTATIC(OnCtlColorButton)
    CHAIN_MSG_MAP(CDynamicDialogLayout<CMainDlg>)
  END_MSG_MAP()
};
