#include "pch.h"
#include "Services.h"
#include "PasswordDlg.h"

bool Services::CheckPassword(const char *expected, HWND parent)
{
  CPasswordDlg dlg(expected ? expected : "Tracey");
  return dlg.DoModal(parent) == IDOK;
}
