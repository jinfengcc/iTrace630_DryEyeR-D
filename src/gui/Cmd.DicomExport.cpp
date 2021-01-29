#include "stdafx.h"
#include "Cmd.Target.h"
#include "Resource.h"
#include "BatchExportDicom.h"
#include "CustomMenu.h"

class CmdDicomExport : public CmdTargetBase
{
  DECLARE_MESSAGE_MAP()
  afx_msg void OnPatientExport();
};

static CmdDicomExport cmd;

BEGIN_MESSAGE_MAP(CmdDicomExport, CCmdTarget)
  ON_COMMAND(IDC_DICOMEXPORT_BUTTON, OnPatientExport)
END_MESSAGE_MAP()

void CmdDicomExport::OnPatientExport()
{
  BatchExportDicom bde;
  bde.Export();
}
