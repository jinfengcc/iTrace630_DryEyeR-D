
//***************************************************************************************

#include "StdAfx.h"
#include "Resource.h"
#include "ExcelSettingsDlg2.h"
#include "FolderDlg.h"
#include "Settings.h"
#include "Registry.h"
#include "Settings.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CExcelSettingsDlg2, CDialog)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CExcelSettingsDlg2::OnTcnSelchangeTab1)

END_MESSAGE_MAP()

//***************************************************************************************

CExcelSettingsDlg2::CExcelSettingsDlg2(CWnd* pParentWnd, BOOL ExcDef) : CDialog(IDD_EXCEL_SETTING_DLG2, pParentWnd)
{
	m_ExcDef = ExcDef;
}

//***************************************************************************************

void CExcelSettingsDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}



//***************************************************************************************

BOOL CExcelSettingsDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_tab.InsertItem(0, _T("WF Parameters"));
	m_tab.InsertItem(1, _T("CT Parameters"));
	m_tab.InsertItem(2, _T("WF and CT Parameters"));

	m_page1.Create(IDD_EXCEL_WF_PAGE, &m_tab);
	m_page2.Create(IDD_EXCEL_CT_PAGE, &m_tab);
	m_page3.Create(IDD_EXCEL_WFCT_PAGE, &m_tab);

	//
	if (m_ExcDef)
		SetDefSetting(); //
	else
		SetSavedSetting();

	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 25;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_page1.MoveWindow(&rc);
	m_page2.MoveWindow(&rc);
	m_page3.MoveWindow(&rc);

	pDialog[0] = &m_page1;
	pDialog[1] = &m_page2;
	pDialog[2] = &m_page3;

	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	pDialog[2]->ShowWindow(SW_HIDE);


	m_CurSelTab = 0;
	////
	CRect rect;
	GetWindowRect(&rect);

	RECT Rect;
	GetWindowRect(&Rect);
	int ww = Rect.right - Rect.left;
	int wh = Rect.bottom - Rect.top;

	if (m_isSolo) {
		ww = intRound(0.660 * ww);
		wh = intRound(0.770 * wh);
		SetWindowPos(NULL, 0, 0, ww, wh, SWP_NOZORDER);
	}
	CenterWindow();

	//Set the required item in tabs
	CImage image;
	image.Load(_T("C:\\Projects\\iTrace 6.0.1\\Main\\Res\\Accept0.png"));
	m_page1.m_ImgRequired1.SetBitmap((HBITMAP)image);
	m_page1.m_ImgRequired2.SetBitmap((HBITMAP)image);
	m_page1.m_ImgRequired3.SetBitmap((HBITMAP)image);

	m_page2.m_ImgRequired1.SetBitmap((HBITMAP)image);
	m_page2.m_ImgRequired2.SetBitmap((HBITMAP)image);
	m_page2.m_ImgRequired3.SetBitmap((HBITMAP)image);

	m_page3.m_ImgRequired1.SetBitmap((HBITMAP)image);
	m_page3.m_ImgRequired2.SetBitmap((HBITMAP)image);
	m_page3.m_ImgRequired3.SetBitmap((HBITMAP)image);
	//

	return TRUE;
}

//***************************************************************************************

void CExcelSettingsDlg2::OnOK()
{
	SaveSetting();
	CDialog::OnOK();
}

//***************************************************************************************

void CExcelSettingsDlg2::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);

	m_CurSelTab = m_tab.GetCurSel();

	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);

	*pResult = 0;
}

//***************************************************************************************

void CExcelSettingsDlg2::SaveSetting()
{
	//WF excel setting	
	if (m_page1.m_CheckUserID.GetCheck())
		::Settings.m_EXCEL_WF_SETS[0] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[0] = '0';

	if (m_page1.m_CheckFName.GetCheck())
		::Settings.m_EXCEL_WF_SETS[1] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[1] = '0';

	if (m_page1.m_CheckMName.GetCheck())
		::Settings.m_EXCEL_WF_SETS[2] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[2] = '0';

	if (m_page1.m_CheckLName.GetCheck())
		::Settings.m_EXCEL_WF_SETS[3] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[3] = '0';

	if (m_page1.m_CheckDOB.GetCheck())
		::Settings.m_EXCEL_WF_SETS[4] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[4] = '0';

	if (m_page1.m_CheckGender.GetCheck())
		::Settings.m_EXCEL_WF_SETS[5] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[5] = '0';

	if (m_page1.m_CheckAddLine1.GetCheck())
		::Settings.m_EXCEL_WF_SETS[6] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[6] = '0';

	if (m_page1.m_CheckAddLine2.GetCheck())
		::Settings.m_EXCEL_WF_SETS[7] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[7] = '0';

	if (m_page1.m_CheckCity.GetCheck())
		::Settings.m_EXCEL_WF_SETS[8] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[8] = '0';

	if (m_page1.m_CheckState.GetCheck())
		::Settings.m_EXCEL_WF_SETS[9] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[9] = '0';

	if (m_page1.m_CheckZip.GetCheck())
		::Settings.m_EXCEL_WF_SETS[10] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[10] = '0';

	if (m_page1.m_CheckCountry.GetCheck())
		::Settings.m_EXCEL_WF_SETS[11] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[11] = '0';

	if (m_page1.m_CheckWPhone.GetCheck())
		::Settings.m_EXCEL_WF_SETS[12] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[12] = '0';

	if (m_page1.m_CheckHPhone.GetCheck())
		::Settings.m_EXCEL_WF_SETS[13] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[13] = '0';

	if (m_page1.m_CheckEmail.GetCheck())
		::Settings.m_EXCEL_WF_SETS[14] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[14] = '0';

	if (m_page1.m_CheckNote.GetCheck())
		::Settings.m_EXCEL_WF_SETS[15] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[15] = '0';

	if (m_page1.m_CheckGroupGUID.GetCheck())
		::Settings.m_EXCEL_WF_SETS[16] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[16] = '0';

	if (m_page1.m_CheckGroupName.GetCheck())
		::Settings.m_EXCEL_WF_SETS[17] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[17] = '0';

	if (m_page1.m_CheckCalID.GetCheck())
		::Settings.m_EXCEL_WF_SETS[18] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[18] = '0';

	if (m_page1.m_CheckMode.GetCheck())
		::Settings.m_EXCEL_WF_SETS[19] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[19] = '0';

	if (m_page1.m_CheckDate.GetCheck())
		::Settings.m_EXCEL_WF_SETS[20] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[20] = '0';

	if (m_page1.m_CheckTime.GetCheck())
		::Settings.m_EXCEL_WF_SETS[21] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[21] = '0';

	if (m_page1.m_CheckEye.GetCheck())
		::Settings.m_EXCEL_WF_SETS[22] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[22] = '0';

	if (m_page1.m_CheckPrePost.GetCheck())
		::Settings.m_EXCEL_WF_SETS[23] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[23] = '0';

	if (m_page1.m_CheckSW.GetCheck())
		::Settings.m_EXCEL_WF_SETS[24] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[24] = '0';

	if (m_page1.m_CheckPhysicianID.GetCheck())
		::Settings.m_EXCEL_WF_SETS[25] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[25] = '0';

	if (m_page1.m_CheckPhysFName.GetCheck())
		::Settings.m_EXCEL_WF_SETS[26] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[26] = '0';

	if (m_page1.m_CheckPhysLName.GetCheck())
		::Settings.m_EXCEL_WF_SETS[27] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[27] = '0';

	if (m_page1.m_CheckOperatorID.GetCheck())
		::Settings.m_EXCEL_WF_SETS[28] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[28] = '0';

	if (m_page1.m_CheckOperFName.GetCheck())
		::Settings.m_EXCEL_WF_SETS[29] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[29] = '0';

	if (m_page1.m_CheckOperLName.GetCheck())
		::Settings.m_EXCEL_WF_SETS[30] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[30] = '0';

	if (m_page1.m_CheckExamNote.GetCheck())
		::Settings.m_EXCEL_WF_SETS[31] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[31] = '0';

	if (m_page1.m_CheckAngelKDist.GetCheck())
		::Settings.m_EXCEL_WF_SETS[32] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[32] = '0';

	if (m_page1.m_CheckAngelKDeg.GetCheck())
		::Settings.m_EXCEL_WF_SETS[33] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[33] = '0';

	/*if(m_page1.m_CheckAngelADist.GetCheck())
	::Settings.m_EXCEL_WF_SETS[34] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[34] = '0';

	if(m_page1.m_CheckAngelADeg.GetCheck())
	::Settings.m_EXCEL_WF_SETS[35] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[35] = '0';

	if(m_page1.m_CheckLimbalX.GetCheck())
	::Settings.m_EXCEL_WF_SETS[36] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[36] = '0';

	if(m_page1.m_CheckLimbalY.GetCheck())
	::Settings.m_EXCEL_WF_SETS[37] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[37] = '0';

	if(m_page1.m_CheckLimbalD.GetCheck())
	::Settings.m_EXCEL_WF_SETS[38] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[38] = '0';*/

	if (m_page1.m_CheckPupilX.GetCheck())
		::Settings.m_EXCEL_WF_SETS[39] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[39] = '0';

	if (m_page1.m_CheckPupilY.GetCheck())
		::Settings.m_EXCEL_WF_SETS[40] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[40] = '0';

	if (m_page1.m_CheckPupilD.GetCheck())
		::Settings.m_EXCEL_WF_SETS[41] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[41] = '0';

	if (m_page1.m_CheckScanD.GetCheck())
		::Settings.m_EXCEL_WF_SETS[42] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[42] = '0';

	if (m_page1.m_CheckTraceRef.GetCheck())
		::Settings.m_EXCEL_WF_SETS[43] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[43] = '0';

	if (m_page1.m_CheckRef1Zone.GetCheck())
		::Settings.m_EXCEL_WF_SETS[44] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[44] = '0';

	if (m_page1.m_CheckRef2Zone.GetCheck())
		::Settings.m_EXCEL_WF_SETS[45] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[45] = '0';

	if (m_page1.m_CheckRef3Zone.GetCheck())
		::Settings.m_EXCEL_WF_SETS[46] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[46] = '0';

	if (m_page1.m_CheckRef4Zone.GetCheck())
		::Settings.m_EXCEL_WF_SETS[47] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[47] = '0';

	if (m_page1.m_CheckEntireEyeZC1.GetCheck())
		::Settings.m_EXCEL_WF_SETS[48] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[48] = '0';

	if (m_page1.m_CheckEntireEyeZC2.GetCheck())
		::Settings.m_EXCEL_WF_SETS[49] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[49] = '0';

	if (m_page1.m_CheckEntireEyeZC3.GetCheck())
		::Settings.m_EXCEL_WF_SETS[50] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[50] = '0';

	if (m_page1.m_CheckEntireEyeZCF.GetCheck())
		::Settings.m_EXCEL_WF_SETS[51] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[51] = '0';

	if (m_page1.m_CheckClinic.GetCheck())
		::Settings.m_EXCEL_WF_SETS[52] = '1';
	else  ::Settings.m_EXCEL_WF_SETS[52] = '0';

	//
	if (m_page1.m_CheckOpacityGrade.GetCheck())
		::NewSettings.m_NEW_EXCEL_WF_SETS[0] = '1';
	else  ::NewSettings.m_NEW_EXCEL_WF_SETS[0] = '0';
	//
	//Done





	//CT excel setting	
	if (m_page2.m_CheckUserID.GetCheck())
		::Settings.m_EXCEL_CT_SETS[0] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[0] = '0';

	if (m_page2.m_CheckFName.GetCheck())
		::Settings.m_EXCEL_CT_SETS[1] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[1] = '0';

	if (m_page2.m_CheckMName.GetCheck())
		::Settings.m_EXCEL_CT_SETS[2] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[2] = '0';

	if (m_page2.m_CheckLName.GetCheck())
		::Settings.m_EXCEL_CT_SETS[3] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[3] = '0';

	if (m_page2.m_CheckDOB.GetCheck())
		::Settings.m_EXCEL_CT_SETS[4] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[4] = '0';

	if (m_page2.m_CheckGender.GetCheck())
		::Settings.m_EXCEL_CT_SETS[5] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[5] = '0';

	if (m_page2.m_CheckAddLine1.GetCheck())
		::Settings.m_EXCEL_CT_SETS[6] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[6] = '0';

	if (m_page2.m_CheckAddLine2.GetCheck())
		::Settings.m_EXCEL_CT_SETS[7] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[7] = '0';

	if (m_page2.m_CheckCity.GetCheck())
		::Settings.m_EXCEL_CT_SETS[8] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[8] = '0';

	if (m_page2.m_CheckState.GetCheck())
		::Settings.m_EXCEL_CT_SETS[9] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[9] = '0';

	if (m_page2.m_CheckZip.GetCheck())
		::Settings.m_EXCEL_CT_SETS[10] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[10] = '0';

	if (m_page2.m_CheckCountry.GetCheck())
		::Settings.m_EXCEL_CT_SETS[11] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[11] = '0';

	if (m_page2.m_CheckWPhone.GetCheck())
		::Settings.m_EXCEL_CT_SETS[12] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[12] = '0';

	if (m_page2.m_CheckHPhone.GetCheck())
		::Settings.m_EXCEL_CT_SETS[13] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[13] = '0';

	if (m_page2.m_CheckEmail.GetCheck())
		::Settings.m_EXCEL_CT_SETS[14] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[14] = '0';

	if (m_page2.m_CheckNote.GetCheck())
		::Settings.m_EXCEL_CT_SETS[15] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[15] = '0';

	if (m_page2.m_CheckGroupGUID.GetCheck())
		::Settings.m_EXCEL_CT_SETS[16] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[16] = '0';

	if (m_page2.m_CheckGroupName.GetCheck())
		::Settings.m_EXCEL_CT_SETS[17] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[17] = '0';

	if (m_page2.m_CheckCalID.GetCheck())
		::Settings.m_EXCEL_CT_SETS[18] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[18] = '0';

	if (m_page2.m_CheckMode.GetCheck())
		::Settings.m_EXCEL_CT_SETS[19] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[19] = '0';

	if (m_page2.m_CheckDate.GetCheck())
		::Settings.m_EXCEL_CT_SETS[20] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[20] = '0';

	if (m_page2.m_CheckTime.GetCheck())
		::Settings.m_EXCEL_CT_SETS[21] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[21] = '0';

	if (m_page2.m_CheckEye.GetCheck())
		::Settings.m_EXCEL_CT_SETS[22] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[22] = '0';

	if (m_page2.m_CheckPrePost.GetCheck())
		::Settings.m_EXCEL_CT_SETS[23] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[23] = '0';

	if (m_page2.m_CheckSW.GetCheck())
		::Settings.m_EXCEL_CT_SETS[24] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[24] = '0';

	if (m_page2.m_CheckPhysicianID.GetCheck())
		::Settings.m_EXCEL_CT_SETS[25] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[25] = '0';

	if (m_page2.m_CheckPhysFName.GetCheck())
		::Settings.m_EXCEL_CT_SETS[26] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[26] = '0';

	if (m_page2.m_CheckPhysLName.GetCheck())
		::Settings.m_EXCEL_CT_SETS[27] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[27] = '0';

	if (m_page2.m_CheckOperatorID.GetCheck())
		::Settings.m_EXCEL_CT_SETS[28] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[28] = '0';

	if (m_page2.m_CheckOperFName.GetCheck())
		::Settings.m_EXCEL_CT_SETS[29] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[29] = '0';

	if (m_page2.m_CheckOperLName.GetCheck())
		::Settings.m_EXCEL_CT_SETS[30] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[30] = '0';

	if (m_page2.m_CheckExamNote.GetCheck())
		::Settings.m_EXCEL_CT_SETS[31] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[31] = '0';

	if (m_page2.m_CheckLimbalX.GetCheck())
		::Settings.m_EXCEL_CT_SETS[32] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[32] = '0';

	if (m_page2.m_CheckLimbalY.GetCheck())
		::Settings.m_EXCEL_CT_SETS[33] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[33] = '0';

	if (m_page2.m_CheckLimbalD.GetCheck())
		::Settings.m_EXCEL_CT_SETS[34] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[34] = '0';

	if (m_page2.m_CheckCentralPwr.GetCheck())
		::Settings.m_EXCEL_CT_SETS[35] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[35] = '0';

	if (m_page2.m_CheckBestFitSph.GetCheck())
		::Settings.m_EXCEL_CT_SETS[36] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[36] = '0';

	if (m_page2.m_CheckBestConCen.GetCheck())
		::Settings.m_EXCEL_CT_SETS[37] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[37] = '0';

	if (m_page2.m_CheckBestConEcc.GetCheck())
		::Settings.m_EXCEL_CT_SETS[38] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[38] = '0';

	if (m_page2.m_CheckBestConAsp.GetCheck())
		::Settings.m_EXCEL_CT_SETS[39] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[39] = '0';

	if (m_page2.m_CheckRefPwrStp.GetCheck())
		::Settings.m_EXCEL_CT_SETS[40] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[40] = '0';

	if (m_page2.m_CheckRefPwrFlt.GetCheck())
		::Settings.m_EXCEL_CT_SETS[41] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[41] = '0';

	if (m_page2.m_CheckEffRef.GetCheck())
		::Settings.m_EXCEL_CT_SETS[42] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[42] = '0';

	if (m_page2.m_CheckRefPwrAst.GetCheck())
		::Settings.m_EXCEL_CT_SETS[43] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[43] = '0';

	if (m_page2.m_CheckSimKStp.GetCheck())
		::Settings.m_EXCEL_CT_SETS[44] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[44] = '0';

	if (m_page2.m_CheckSimKFlt.GetCheck())
		::Settings.m_EXCEL_CT_SETS[45] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[45] = '0';

	if (m_page2.m_CheckSimKAvg.GetCheck())
		::Settings.m_EXCEL_CT_SETS[46] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[46] = '0';

	if (m_page2.m_CheckSimKDel.GetCheck())
		::Settings.m_EXCEL_CT_SETS[47] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[47] = '0';

	if (m_page2.m_CheckIS.GetCheck())
		::Settings.m_EXCEL_CT_SETS[48] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[48] = '0';

	if (m_page2.m_CheckAvePwr.GetCheck())
		::Settings.m_EXCEL_CT_SETS[49] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[49] = '0';

	if (m_page2.m_CheckStpSemi1.GetCheck())
		::Settings.m_EXCEL_CT_SETS[50] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[50] = '0';

	if (m_page2.m_CheckStpSemi2.GetCheck())
		::Settings.m_EXCEL_CT_SETS[51] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[51] = '0';

	if (m_page2.m_CheckFltSemi1.GetCheck())
		::Settings.m_EXCEL_CT_SETS[52] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[52] = '0';

	if (m_page2.m_CheckFltSemi2.GetCheck())
		::Settings.m_EXCEL_CT_SETS[53] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[53] = '0';

	if (m_page2.m_CheckStpSemi12.GetCheck())
		::Settings.m_EXCEL_CT_SETS[54] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[54] = '0';

	if (m_page2.m_CheckStpSemi22.GetCheck())
		::Settings.m_EXCEL_CT_SETS[55] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[55] = '0';

	if (m_page2.m_CheckFltSemi12.GetCheck())
		::Settings.m_EXCEL_CT_SETS[56] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[56] = '0';

	if (m_page2.m_CheckFltSemi22.GetCheck())
		::Settings.m_EXCEL_CT_SETS[57] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[57] = '0';

	if (m_page2.m_CheckStpSemi13.GetCheck())
		::Settings.m_EXCEL_CT_SETS[58] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[58] = '0';

	if (m_page2.m_CheckStpSemi23.GetCheck())
		::Settings.m_EXCEL_CT_SETS[59] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[59] = '0';

	if (m_page2.m_CheckFltSemi13.GetCheck())
		::Settings.m_EXCEL_CT_SETS[60] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[60] = '0';

	if (m_page2.m_CheckFltSemi23.GetCheck())
		::Settings.m_EXCEL_CT_SETS[61] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[61] = '0';

	if (m_page2.m_CheckCornealSph.GetCheck())
		::Settings.m_EXCEL_CT_SETS[62] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[62] = '0';

	if (m_page2.m_CheckCornealZer1.GetCheck())
		::Settings.m_EXCEL_CT_SETS[63] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[63] = '0';

	if (m_page2.m_CheckCornealZer2.GetCheck())
		::Settings.m_EXCEL_CT_SETS[64] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[64] = '0';

	if (m_page2.m_CheckCornealZer3.GetCheck())
		::Settings.m_EXCEL_CT_SETS[65] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[65] = '0';

	if (m_page2.m_CheckCornealZerF.GetCheck())
		::Settings.m_EXCEL_CT_SETS[66] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[66] = '0';

	if (m_page2.m_CheckClinic.GetCheck())
		::Settings.m_EXCEL_CT_SETS[67] = '1';
	else  ::Settings.m_EXCEL_CT_SETS[67] = '0';
	//Done






	//WF and CT excel setting	
	if (m_page3.m_CheckUserID.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[0] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[0] = '0';

	if (m_page3.m_CheckFName.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[1] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[1] = '0';

	if (m_page3.m_CheckMName.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[2] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[2] = '0';

	if (m_page3.m_CheckLName.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[3] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[3] = '0';

	if (m_page3.m_CheckDOB.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[4] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[4] = '0';

	if (m_page3.m_CheckGender.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[5] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[5] = '0';

	if (m_page3.m_CheckAddLine1.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[6] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[6] = '0';

	if (m_page3.m_CheckAddLine2.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[7] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[7] = '0';

	if (m_page3.m_CheckCity.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[8] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[8] = '0';

	if (m_page3.m_CheckState.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[9] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[9] = '0';

	if (m_page3.m_CheckZip.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[10] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[10] = '0';

	if (m_page3.m_CheckCountry.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[11] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[11] = '0';

	if (m_page3.m_CheckWPhone.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[12] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[12] = '0';

	if (m_page3.m_CheckHPhone.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[13] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[13] = '0';

	if (m_page3.m_CheckEmail.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[14] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[14] = '0';

	if (m_page3.m_CheckNote.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[15] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[15] = '0';

	if (m_page3.m_CheckGroupGUID.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[16] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[16] = '0';

	if (m_page3.m_CheckGroupName.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[17] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[17] = '0';

	if (m_page3.m_CheckCalID.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[18] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[18] = '0';

	if (m_page3.m_CheckMode.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[19] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[19] = '0';

	if (m_page3.m_CheckDate.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[20] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[20] = '0';

	if (m_page3.m_CheckTime.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[21] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[21] = '0';

	if (m_page3.m_CheckEye.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[22] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[22] = '0';

	if (m_page3.m_CheckPrePost.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[23] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[23] = '0';

	if (m_page3.m_CheckSW.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[24] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[24] = '0';

	if (m_page3.m_CheckPhysicianID.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[25] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[25] = '0';

	if (m_page3.m_CheckPhysFName.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[26] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[26] = '0';

	if (m_page3.m_CheckPhysLName.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[27] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[27] = '0';

	if (m_page3.m_CheckOperatorID.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[28] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[28] = '0';

	if (m_page3.m_CheckOperFName.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[29] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[29] = '0';

	if (m_page3.m_CheckOperLName.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[30] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[30] = '0';

	if (m_page3.m_CheckExamNote.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[31] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[31] = '0';

	if (m_page3.m_CheckAngelKDist.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[32] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[32] = '0';

	if (m_page3.m_CheckAngelKDeg.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[33] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[33] = '0';

	if (m_page3.m_CheckAngelADist.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[34] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[34] = '0';

	if (m_page3.m_CheckAngelADeg.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[35] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[35] = '0';

	if (m_page3.m_CheckLimbalX.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[36] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[36] = '0';

	if (m_page3.m_CheckLimbalY.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[37] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[37] = '0';

	if (m_page3.m_CheckLimbalD.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[38] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[38] = '0';

	if (m_page3.m_CheckPupilX.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[39] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[39] = '0';

	if (m_page3.m_CheckPupilY.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[40] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[40] = '0';

	if (m_page3.m_CheckPupilD.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[41] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[41] = '0';

	if (m_page3.m_CheckScanD.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[42] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[42] = '0';

	if (m_page3.m_CheckTraceRef.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[43] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[43] = '0';

	if (m_page3.m_CheckRef1Zone.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[44] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[44] = '0';

	if (m_page3.m_CheckRef2Zone.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[45] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[45] = '0';

	if (m_page3.m_CheckRef3Zone.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[46] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[46] = '0';

	if (m_page3.m_CheckRef4Zone.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[47] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[47] = '0';

	if (m_page3.m_CheckEntireEyeZC1.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[48] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[48] = '0';

	if (m_page3.m_CheckEntireEyeZC2.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[49] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[49] = '0';

	if (m_page3.m_CheckEntireEyeZC3.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[50] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[50] = '0';

	if (m_page3.m_CheckEntireEyeZCF.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[51] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[51] = '0';

	if (m_page3.m_CheckCentralPwr.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[52] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[52] = '0';

	if (m_page3.m_CheckBestFitSph.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[53] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[53] = '0';

	if (m_page3.m_CheckBestConCen.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[54] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[54] = '0';

	if (m_page3.m_CheckBestConEcc.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[55] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[55] = '0';

	if (m_page3.m_CheckBestConAsp.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[56] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[56] = '0';

	if (m_page3.m_CheckRefPwrStp.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[57] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[57] = '0';

	if (m_page3.m_CheckRefPwrFlt.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[58] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[58] = '0';

	if (m_page3.m_CheckEffRef.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[59] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[59] = '0';

	if (m_page3.m_CheckRefPwrAst.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[60] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[60] = '0';

	if (m_page3.m_CheckSimKStp.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[61] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[61] = '0';

	if (m_page3.m_CheckSimKFlt.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[62] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[62] = '0';

	if (m_page3.m_CheckSimKAvg.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[63] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[63] = '0';

	if (m_page3.m_CheckSimKDel.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[64] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[64] = '0';

	if (m_page3.m_CheckIS.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[65] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[65] = '0';

	if (m_page3.m_CheckAvePwr.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[66] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[66] = '0';

	if (m_page3.m_CheckStpSemi1.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[67] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[67] = '0';

	if (m_page3.m_CheckStpSemi2.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[68] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[68] = '0';

	if (m_page3.m_CheckFltSemi1.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[69] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[69] = '0';

	if (m_page3.m_CheckFltSemi2.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[70] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[70] = '0';

	if (m_page3.m_CheckStpSemi12.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[71] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[71] = '0';

	if (m_page3.m_CheckStpSemi22.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[72] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[72] = '0';

	if (m_page3.m_CheckFltSemi12.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[73] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[73] = '0';

	if (m_page3.m_CheckFltSemi22.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[74] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[74] = '0';

	if (m_page3.m_CheckStpSemi13.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[75] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[75] = '0';

	if (m_page3.m_CheckStpSemi23.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[76] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[76] = '0';

	if (m_page3.m_CheckFltSemi13.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[77] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[77] = '0';

	if (m_page3.m_CheckFltSemi23.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[78] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[78] = '0';

	if (m_page3.m_CheckCornealSph.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[79] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[79] = '0';

	if (m_page3.m_CheckCornealZer1.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[80] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[80] = '0';

	if (m_page3.m_CheckCornealZer2.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[81] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[81] = '0';

	if (m_page3.m_CheckCornealZer3.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[82] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[82] = '0';

	if (m_page3.m_CheckCornealZerF.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[83] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[83] = '0';

	if (m_page3.m_CheckInternalZer1.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[84] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[84] = '0';

	if (m_page3.m_CheckInternalZer2.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[85] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[85] = '0';

	if (m_page3.m_CheckInternalZer3.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[86] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[86] = '0';

	if (m_page3.m_CheckInternalZerF.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[87] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[87] = '0';

	if (m_page3.m_CheckClinic.GetCheck())
		::Settings.m_EXCEL_WFCT_SETS[88] = '1';
	else  ::Settings.m_EXCEL_WFCT_SETS[88] = '0';

	//
	if (m_page3.m_CheckDli.GetCheck())
		::NewSettings.m_NEW_EXCEL_WFCT_SETS[0] = '1';
	else  ::NewSettings.m_NEW_EXCEL_WFCT_SETS[0] = '0';

	if (m_page3.m_CheckOpacityGrade.GetCheck())
		::NewSettings.m_NEW_EXCEL_WFCT_SETS[1] = '1';
	else  ::NewSettings.m_NEW_EXCEL_WFCT_SETS[1] = '0';
	//
	//Done	
}

//***************************************************************************************

void CExcelSettingsDlg2::SetSavedSetting()
{
	BOOL ShowAllChecked = TRUE;
	//WF excel setting	
	if (::Settings.m_EXCEL_WF_SETS[0] != '1')
	{
		m_page1.m_CheckUserID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckUserID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[1] != '1')
	{
		m_page1.m_CheckFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[2] != '1')
	{
		m_page1.m_CheckMName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckMName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[3] != '1')
	{
		m_page1.m_CheckLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[4] != '1')
	{
		m_page1.m_CheckDOB.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckDOB.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[5] != '1')
	{
		m_page1.m_CheckGender.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckGender.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[6] != '1')
	{
		m_page1.m_CheckAddLine1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckAddLine1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[7] != '1')
	{
		m_page1.m_CheckAddLine2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckAddLine2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[8] != '1')
	{
		m_page1.m_CheckCity.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckCity.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[9] != '1')
	{
		m_page1.m_CheckState.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckState.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[10] != '1')
	{
		m_page1.m_CheckZip.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckZip.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[11] != '1')
	{
		m_page1.m_CheckCountry.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckCountry.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[12] != '1')
	{
		m_page1.m_CheckWPhone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckWPhone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[13] != '1')
	{
		m_page1.m_CheckHPhone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckHPhone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[14] != '1')
	{
		m_page1.m_CheckEmail.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckEmail.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[15] != '1')
	{
		m_page1.m_CheckNote.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckNote.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[16] != '1')
	{
		m_page1.m_CheckGroupGUID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckGroupGUID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[17] != '1')
	{
		m_page1.m_CheckGroupName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckGroupName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[18] != '1')
	{
		m_page1.m_CheckCalID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckCalID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[19] != '1')
	{
		m_page1.m_CheckMode.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckMode.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[20] != '1')
	{
		m_page1.m_CheckDate.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckDate.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[21] != '1')
	{
		m_page1.m_CheckTime.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckTime.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[22] != '1')
	{
		m_page1.m_CheckEye.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckEye.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[23] != '1')
	{
		m_page1.m_CheckPrePost.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckPrePost.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[24] != '1')
	{
		m_page1.m_CheckSW.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckSW.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[25] != '1')
	{
		m_page1.m_CheckPhysicianID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckPhysicianID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[26] != '1')
	{
		m_page1.m_CheckPhysFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckPhysFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[27] != '1')
	{
		m_page1.m_CheckPhysLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckPhysLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[28] != '1')
	{
		m_page1.m_CheckOperatorID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckOperatorID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[29] != '1')
	{
		m_page1.m_CheckOperFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckOperFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[30] != '1')
	{
		m_page1.m_CheckOperLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckOperLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[31] != '1')
	{
		m_page1.m_CheckExamNote.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckExamNote.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[32] != '1')
	{
		m_page1.m_CheckAngelKDist.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckAngelKDist.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[33] != '1')
	{
		m_page1.m_CheckAngelKDeg.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckAngelKDeg.SetCheck(TRUE);

	/*if(::Settings.m_EXCEL_WF_SETS[34] != '1')
	{
	m_page1.m_CheckAngelADist.SetCheck(FALSE);
	ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckAngelADist.SetCheck(TRUE);

	if(::Settings.m_EXCEL_WF_SETS[35] != '1')
	{
	m_page1.m_CheckAngelADeg.SetCheck(FALSE);
	ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckAngelADeg.SetCheck(TRUE);

	if(::Settings.m_EXCEL_WF_SETS[36] != '1')
	{
	m_page1.m_CheckLimbalX.SetCheck(FALSE);
	ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckLimbalX.SetCheck(TRUE);

	if(::Settings.m_EXCEL_WF_SETS[37] != '1')
	{
	m_page1.m_CheckLimbalY.SetCheck(FALSE);
	ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckLimbalY.SetCheck(TRUE);

	if(::Settings.m_EXCEL_WF_SETS[38] != '1')
	{
	m_page1.m_CheckLimbalD.SetCheck(FALSE);
	ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckLimbalD.SetCheck(TRUE);*/

	if (::Settings.m_EXCEL_WF_SETS[39] != '1')
	{
		m_page1.m_CheckPupilX.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckPupilX.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[40] != '1')
	{
		m_page1.m_CheckPupilY.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckPupilY.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[41] != '1')
	{
		m_page1.m_CheckPupilD.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckPupilD.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[42] != '1')
	{
		m_page1.m_CheckScanD.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckScanD.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[43] != '1')
	{
		m_page1.m_CheckTraceRef.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckTraceRef.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[44] != '1')
	{
		m_page1.m_CheckRef1Zone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckRef1Zone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[45] != '1')
	{
		m_page1.m_CheckRef2Zone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckRef2Zone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[46] != '1')
	{
		m_page1.m_CheckRef3Zone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckRef3Zone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[47] != '1')
	{
		m_page1.m_CheckRef4Zone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckRef4Zone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[48] != '1')
	{
		m_page1.m_CheckEntireEyeZC1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckEntireEyeZC1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[49] != '1')
	{
		m_page1.m_CheckEntireEyeZC2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckEntireEyeZC2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[50] != '1')
	{
		m_page1.m_CheckEntireEyeZC3.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckEntireEyeZC3.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[51] != '1')
	{
		m_page1.m_CheckEntireEyeZCF.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckEntireEyeZCF.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WF_SETS[52] != '1')
	{
		m_page1.m_CheckClinic.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckClinic.SetCheck(TRUE);

	//
	if (::NewSettings.m_NEW_EXCEL_WF_SETS[0] != '1')
	{
		m_page1.m_CheckOpacityGrade.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page1.m_CheckOpacityGrade.SetCheck(TRUE);
	//

	m_page1.m_CheckAll.SetCheck(ShowAllChecked);
	ShowAllChecked = TRUE;
	//Done



	//CT excel setting	
	if (::Settings.m_EXCEL_CT_SETS[0] != '1')
	{
		m_page2.m_CheckUserID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckUserID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[1] != '1')
	{
		m_page2.m_CheckFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[2] != '1')
	{
		m_page2.m_CheckMName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckMName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[3] != '1')
	{
		m_page2.m_CheckLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[4] != '1')
	{
		m_page2.m_CheckDOB.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckDOB.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[5] != '1')
	{
		m_page2.m_CheckGender.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckGender.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[6] != '1')
	{
		m_page2.m_CheckAddLine1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckAddLine1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[7] != '1')
	{
		m_page2.m_CheckAddLine2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckAddLine2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[8] != '1')
	{
		m_page2.m_CheckCity.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCity.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[9] != '1')
	{
		m_page2.m_CheckState.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckState.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[10] != '1')
	{
		m_page2.m_CheckZip.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckZip.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[11] != '1')
	{
		m_page2.m_CheckCountry.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCountry.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[12] != '1')
	{
		m_page2.m_CheckWPhone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckWPhone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[13] != '1')
	{
		m_page2.m_CheckHPhone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckHPhone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[14] != '1')
	{
		m_page2.m_CheckEmail.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckEmail.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[15] != '1')
	{
		m_page2.m_CheckNote.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckNote.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[16] != '1')
	{
		m_page2.m_CheckGroupGUID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckGroupGUID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[17] != '1')
	{
		m_page2.m_CheckGroupName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckGroupName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[18] != '1')
	{
		m_page2.m_CheckCalID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCalID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[19] != '1')
	{
		m_page2.m_CheckMode.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckMode.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[20] != '1')
	{
		m_page2.m_CheckDate.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckDate.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[21] != '1')
	{
		m_page2.m_CheckTime.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckTime.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[22] != '1')
	{
		m_page2.m_CheckEye.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckEye.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[23] != '1')
	{
		m_page2.m_CheckPrePost.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckPrePost.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[24] != '1')
	{
		m_page2.m_CheckSW.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckSW.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[25] != '1')
	{
		m_page2.m_CheckPhysicianID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckPhysicianID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[26] != '1')
	{
		m_page2.m_CheckPhysFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckPhysFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[27] != '1')
	{
		m_page2.m_CheckPhysLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckPhysLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[28] != '1')
	{
		m_page2.m_CheckOperatorID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckOperatorID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[29] != '1')
	{
		m_page2.m_CheckOperFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckOperFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[30] != '1')
	{
		m_page2.m_CheckOperLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckOperLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[31] != '1')
	{
		m_page2.m_CheckExamNote.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckExamNote.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[32] != '1')
	{
		m_page2.m_CheckLimbalX.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckLimbalX.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[33] != '1')
	{
		m_page2.m_CheckLimbalY.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckLimbalY.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[34] != '1')
	{
		m_page2.m_CheckLimbalD.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckLimbalD.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[35] != '1')
	{
		m_page2.m_CheckCentralPwr.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCentralPwr.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[36] != '1')
	{
		m_page2.m_CheckBestFitSph.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckBestFitSph.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[37] != '1')
	{
		m_page2.m_CheckBestConCen.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckBestConCen.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[38] != '1')
	{
		m_page2.m_CheckBestConEcc.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckBestConEcc.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[39] != '1')
	{
		m_page2.m_CheckBestConAsp.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckBestConAsp.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[40] != '1')
	{
		m_page2.m_CheckRefPwrStp.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckRefPwrStp.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[41] != '1')
	{
		m_page2.m_CheckRefPwrFlt.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckRefPwrFlt.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[42] != '1')
	{
		m_page2.m_CheckEffRef.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckEffRef.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[43] != '1')
	{
		m_page2.m_CheckRefPwrAst.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckRefPwrAst.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[44] != '1')
	{
		m_page2.m_CheckSimKStp.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckSimKStp.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[45] != '1')
	{
		m_page2.m_CheckSimKFlt.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckSimKFlt.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[46] != '1')
	{
		m_page2.m_CheckSimKAvg.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckSimKAvg.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[47] != '1')
	{
		m_page2.m_CheckSimKDel.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckSimKDel.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[48] != '1')
	{
		m_page2.m_CheckIS.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckIS.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[49] != '1')
	{
		m_page2.m_CheckAvePwr.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckAvePwr.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[50] != '1')
	{
		m_page2.m_CheckStpSemi1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckStpSemi1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[51] != '1')
	{
		m_page2.m_CheckStpSemi2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckStpSemi2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[52] != '1')
	{
		m_page2.m_CheckFltSemi1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckFltSemi1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[53] != '1')
	{
		m_page2.m_CheckFltSemi2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckFltSemi2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[54] != '1')
	{
		m_page2.m_CheckStpSemi12.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckStpSemi12.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[55] != '1')
	{
		m_page2.m_CheckStpSemi22.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckStpSemi22.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[56] != '1')
	{
		m_page2.m_CheckFltSemi12.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckFltSemi12.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[57] != '1')
	{
		m_page2.m_CheckFltSemi22.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckFltSemi22.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[58] != '1')
	{
		m_page2.m_CheckStpSemi13.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckStpSemi13.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[59] != '1')
	{
		m_page2.m_CheckStpSemi23.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckStpSemi23.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[60] != '1')
	{
		m_page2.m_CheckFltSemi13.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckFltSemi13.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[61] != '1')
	{
		m_page2.m_CheckFltSemi23.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckFltSemi23.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[62] != '1')
	{
		m_page2.m_CheckCornealSph.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCornealSph.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[63] != '1')
	{
		m_page2.m_CheckCornealZer1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCornealZer1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[64] != '1')
	{
		m_page2.m_CheckCornealZer2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCornealZer2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[65] != '1')
	{
		m_page2.m_CheckCornealZer3.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCornealZer3.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[66] != '1')
	{
		m_page2.m_CheckCornealZerF.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckCornealZerF.SetCheck(TRUE);

	if (::Settings.m_EXCEL_CT_SETS[67] != '1')
	{
		m_page2.m_CheckClinic.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page2.m_CheckClinic.SetCheck(TRUE);

	m_page2.m_CheckAll.SetCheck(ShowAllChecked);
	ShowAllChecked = TRUE;
	//Done






	//WF and CT excel setting	
	if (::Settings.m_EXCEL_WFCT_SETS[0] != '1')
	{
		m_page3.m_CheckUserID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckUserID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[1] != '1')
	{
		m_page3.m_CheckFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[2] != '1')
	{
		m_page3.m_CheckMName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckMName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[3] != '1')
	{
		m_page3.m_CheckLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[4] != '1')
	{
		m_page3.m_CheckDOB.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckDOB.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[5] != '1')
	{
		m_page3.m_CheckGender.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckGender.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[6] != '1')
	{
		m_page3.m_CheckAddLine1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckAddLine1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[7] != '1')
	{
		m_page3.m_CheckAddLine2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckAddLine2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[8] != '1')
	{
		m_page3.m_CheckCity.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCity.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[9] != '1')
	{
		m_page3.m_CheckState.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckState.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[10] != '1')
	{
		m_page3.m_CheckZip.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckZip.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[11] != '1')
	{
		m_page3.m_CheckCountry.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCountry.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[12] != '1')
	{
		m_page3.m_CheckWPhone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckWPhone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[13] != '1')
	{
		m_page3.m_CheckHPhone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckHPhone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[14] != '1')
	{
		m_page3.m_CheckEmail.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckEmail.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[15] != '1')
	{
		m_page3.m_CheckNote.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckNote.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[16] != '1')
	{
		m_page3.m_CheckGroupGUID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckGroupGUID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[17] != '1')
	{
		m_page3.m_CheckGroupName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckGroupName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[18] != '1')
	{
		m_page3.m_CheckCalID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCalID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[19] != '1')
	{
		m_page3.m_CheckMode.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckMode.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[20] != '1')
	{
		m_page3.m_CheckDate.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckDate.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[21] != '1')
	{
		m_page3.m_CheckTime.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckTime.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[22] != '1')
	{
		m_page3.m_CheckEye.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckEye.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[23] != '1')
	{
		m_page3.m_CheckPrePost.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckPrePost.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[24] != '1')
	{
		m_page3.m_CheckSW.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckSW.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[25] != '1')
	{
		m_page3.m_CheckPhysicianID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckPhysicianID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[26] != '1')
	{
		m_page3.m_CheckPhysFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckPhysFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[27] != '1')
	{
		m_page3.m_CheckPhysLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckPhysLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[28] != '1')
	{
		m_page3.m_CheckOperatorID.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckOperatorID.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[29] != '1')
	{
		m_page3.m_CheckOperFName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckOperFName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[30] != '1')
	{
		m_page3.m_CheckOperLName.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckOperLName.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[31] != '1')
	{
		m_page3.m_CheckExamNote.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckExamNote.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[32] != '1')
	{
		m_page3.m_CheckAngelKDist.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckAngelKDist.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[33] != '1')
	{
		m_page3.m_CheckAngelKDeg.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckAngelKDeg.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[34] != '1')
	{
		m_page3.m_CheckAngelADist.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckAngelADist.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[35] != '1')
	{
		m_page3.m_CheckAngelADeg.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckAngelADeg.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[36] != '1')
	{
		m_page3.m_CheckLimbalX.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckLimbalX.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[37] != '1')
	{
		m_page3.m_CheckLimbalY.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckLimbalY.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[38] != '1')
	{
		m_page3.m_CheckLimbalD.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckLimbalD.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[39] != '1')
	{
		m_page3.m_CheckPupilX.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckPupilX.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[40] != '1')
	{
		m_page3.m_CheckPupilY.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckPupilY.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[41] != '1')
	{
		m_page3.m_CheckPupilD.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckPupilD.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[42] != '1')
	{
		m_page3.m_CheckScanD.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckScanD.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[43] != '1')
	{
		m_page3.m_CheckTraceRef.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckTraceRef.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[44] != '1')
	{
		m_page3.m_CheckRef1Zone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckRef1Zone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[45] != '1')
	{
		m_page3.m_CheckRef2Zone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckRef2Zone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[46] != '1')
	{
		m_page3.m_CheckRef3Zone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckRef3Zone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[47] != '1')
	{
		m_page3.m_CheckRef4Zone.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckRef4Zone.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[48] != '1')
	{
		m_page3.m_CheckEntireEyeZC1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckEntireEyeZC1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[49] != '1')
	{
		m_page3.m_CheckEntireEyeZC2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckEntireEyeZC2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[50] != '1')
	{
		m_page3.m_CheckEntireEyeZC3.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckEntireEyeZC3.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[51] != '1')
	{
		m_page3.m_CheckEntireEyeZCF.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckEntireEyeZCF.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[52] != '1')
	{
		m_page3.m_CheckCentralPwr.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCentralPwr.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[53] != '1')
	{
		m_page3.m_CheckBestFitSph.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckBestFitSph.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[54] != '1')
	{
		m_page3.m_CheckBestConCen.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckBestConCen.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[55] != '1')
	{
		m_page3.m_CheckBestConEcc.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckBestConEcc.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[56] != '1')
	{
		m_page3.m_CheckBestConAsp.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckBestConAsp.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[57] != '1')
	{
		m_page3.m_CheckRefPwrStp.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckRefPwrStp.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[58] != '1')
	{
		m_page3.m_CheckRefPwrFlt.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckRefPwrFlt.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[59] != '1')
	{
		m_page3.m_CheckEffRef.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckEffRef.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[60] != '1')
	{
		m_page3.m_CheckRefPwrAst.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckRefPwrAst.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[61] != '1')
	{
		m_page3.m_CheckSimKStp.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckSimKStp.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[62] != '1')
	{
		m_page3.m_CheckSimKFlt.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckSimKFlt.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[63] != '1')
	{
		m_page3.m_CheckSimKAvg.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckSimKAvg.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[64] != '1')
	{
		m_page3.m_CheckSimKDel.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckSimKDel.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[65] != '1')
	{
		m_page3.m_CheckIS.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckIS.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[66] != '1')
	{
		m_page3.m_CheckAvePwr.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckAvePwr.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[67] != '1')
	{
		m_page3.m_CheckStpSemi1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckStpSemi1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[68] != '1')
	{
		m_page3.m_CheckStpSemi2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckStpSemi2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[69] != '1')
	{
		m_page3.m_CheckFltSemi1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckFltSemi1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[70] != '1')
	{
		m_page3.m_CheckFltSemi2.SetCheck(FALSE);

		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckFltSemi2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[71] != '1')
	{
		m_page3.m_CheckStpSemi12.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckStpSemi12.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[72] != '1')
	{
		m_page3.m_CheckStpSemi22.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckStpSemi22.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[73] != '1')
	{
		m_page3.m_CheckFltSemi12.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckFltSemi12.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[74] != '1')
	{
		m_page3.m_CheckFltSemi22.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckFltSemi22.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[75] != '1')
	{
		m_page3.m_CheckStpSemi13.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckStpSemi13.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[76] != '1')
	{
		m_page3.m_CheckStpSemi23.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckStpSemi23.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[77] != '1')
	{
		m_page3.m_CheckFltSemi13.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckFltSemi13.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[78] != '1')
	{
		m_page3.m_CheckFltSemi23.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckFltSemi23.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[79] != '1')
	{
		m_page3.m_CheckCornealSph.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCornealSph.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[80] != '1')
	{
		m_page3.m_CheckCornealZer1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCornealZer1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[81] != '1')
	{
		m_page3.m_CheckCornealZer2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCornealZer2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[82] != '1')
	{
		m_page3.m_CheckCornealZer3.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCornealZer3.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[83] != '1')
	{
		m_page3.m_CheckCornealZerF.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckCornealZerF.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[84] != '1')
	{
		m_page3.m_CheckInternalZer1.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckInternalZer1.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[85] != '1')
	{
		m_page3.m_CheckInternalZer2.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckInternalZer2.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[86] != '1')
	{
		m_page3.m_CheckInternalZer3.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckInternalZer3.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[87] != '1')
	{
		m_page3.m_CheckInternalZerF.SetCheck(FALSE);

		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckInternalZerF.SetCheck(TRUE);

	if (::Settings.m_EXCEL_WFCT_SETS[88] != '1')
	{
		m_page3.m_CheckClinic.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckClinic.SetCheck(TRUE);

	//
	if (::NewSettings.m_NEW_EXCEL_WFCT_SETS[0] != '1')
	{
		m_page3.m_CheckDli.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckDli.SetCheck(TRUE);

	if (::NewSettings.m_NEW_EXCEL_WFCT_SETS[1] != '1')
	{
		m_page3.m_CheckOpacityGrade.SetCheck(FALSE);
		ShowAllChecked = FALSE;
	}
	else  m_page3.m_CheckOpacityGrade.SetCheck(TRUE);
	//

	m_page3.m_CheckAll.SetCheck(ShowAllChecked);

	//Done	
}

//***************************************************************************************

void CExcelSettingsDlg2::SetDefSetting()
{
	m_page1.m_CheckAll.SetCheck(FALSE);
	m_page2.m_CheckAll.SetCheck(FALSE);
	m_page3.m_CheckAll.SetCheck(FALSE);

	//WF excel setting	
	m_page1.m_CheckUserID.SetCheck(FALSE);  //1
	m_page1.m_CheckFName.SetCheck(FALSE);   //2
	m_page1.m_CheckMName.SetCheck(FALSE);   //3
	m_page1.m_CheckLName.SetCheck(FALSE);   //4
	m_page1.m_CheckDOB.SetCheck(FALSE);     //5
	m_page1.m_CheckGender.SetCheck(FALSE);  //6
	m_page1.m_CheckAddLine1.SetCheck(FALSE);//7
	m_page1.m_CheckAddLine2.SetCheck(FALSE);//8
	m_page1.m_CheckCity.SetCheck(FALSE);    //9
	m_page1.m_CheckState.SetCheck(FALSE);   //10

	m_page1.m_CheckZip.SetCheck(FALSE);      //11
	m_page1.m_CheckCountry.SetCheck(FALSE);  //12
	m_page1.m_CheckWPhone.SetCheck(FALSE);   //13
	m_page1.m_CheckHPhone.SetCheck(FALSE);   //14
	m_page1.m_CheckEmail.SetCheck(FALSE);    //15
	m_page1.m_CheckNote.SetCheck(FALSE);     //16
	m_page1.m_CheckGroupGUID.SetCheck(FALSE);//17
	m_page1.m_CheckGroupName.SetCheck(FALSE);//18
	m_page1.m_CheckCalID.SetCheck(FALSE);    //19
	m_page1.m_CheckMode.SetCheck(FALSE);     //20

	m_page1.m_CheckDate.SetCheck(FALSE);       //21
	m_page1.m_CheckTime.SetCheck(FALSE);       //22
	m_page1.m_CheckEye.SetCheck(FALSE);        //23
	m_page1.m_CheckPrePost.SetCheck(FALSE);    //24
	m_page1.m_CheckSW.SetCheck(FALSE);         //25
	m_page1.m_CheckPhysicianID.SetCheck(FALSE);//26
	m_page1.m_CheckPhysFName.SetCheck(FALSE);  //27
	m_page1.m_CheckPhysLName.SetCheck(FALSE);  //28
	m_page1.m_CheckOperatorID.SetCheck(FALSE); //29
	m_page1.m_CheckOperFName.SetCheck(FALSE);  //30

	m_page1.m_CheckOperLName.SetCheck(FALSE);  //31
	m_page1.m_CheckExamNote.SetCheck(FALSE);   //32
	m_page1.m_CheckAngelKDist.SetCheck(FALSE); //33
	m_page1.m_CheckAngelKDeg.SetCheck(FALSE);  //34							 
	m_page1.m_CheckPupilX.SetCheck(FALSE);     //40

	m_page1.m_CheckPupilY.SetCheck(FALSE);     //41
	m_page1.m_CheckPupilD.SetCheck(FALSE);     //42
	m_page1.m_CheckScanD.SetCheck(FALSE);      //43
	m_page1.m_CheckTraceRef.SetCheck(FALSE);   //44
	m_page1.m_CheckRef1Zone.SetCheck(FALSE);   //45
	m_page1.m_CheckRef2Zone.SetCheck(FALSE);   //46
	m_page1.m_CheckRef3Zone.SetCheck(FALSE);   //47
	m_page1.m_CheckRef4Zone.SetCheck(FALSE);   //48
	m_page1.m_CheckEntireEyeZC1.SetCheck(FALSE); //49
	m_page1.m_CheckEntireEyeZC2.SetCheck(FALSE); //50
	m_page1.m_CheckEntireEyeZC3.SetCheck(FALSE); //51
	m_page1.m_CheckEntireEyeZCF.SetCheck(FALSE); //52

	m_page1.m_CheckClinic.SetCheck(FALSE); //53

	m_page1.m_CheckOpacityGrade.SetCheck(FALSE);
	//Done



	//CT excel setting	
	m_page2.m_CheckUserID.SetCheck(FALSE);  //1
	m_page2.m_CheckFName.SetCheck(FALSE);   //2
	m_page2.m_CheckMName.SetCheck(FALSE);   //3
	m_page2.m_CheckLName.SetCheck(FALSE);   //4
	m_page2.m_CheckDOB.SetCheck(FALSE);     //5
	m_page2.m_CheckGender.SetCheck(FALSE);  //6
	m_page2.m_CheckAddLine1.SetCheck(FALSE);//7
	m_page2.m_CheckAddLine2.SetCheck(FALSE);//8
	m_page2.m_CheckCity.SetCheck(FALSE);    //9
	m_page2.m_CheckState.SetCheck(FALSE);   //10

	m_page2.m_CheckZip.SetCheck(FALSE);      //11
	m_page2.m_CheckCountry.SetCheck(FALSE);  //12
	m_page2.m_CheckWPhone.SetCheck(FALSE);   //13
	m_page2.m_CheckHPhone.SetCheck(FALSE);   //14
	m_page2.m_CheckEmail.SetCheck(FALSE);    //15
	m_page2.m_CheckNote.SetCheck(FALSE);     //16
	m_page2.m_CheckGroupGUID.SetCheck(FALSE);//17
	m_page2.m_CheckGroupName.SetCheck(FALSE);//18
	m_page2.m_CheckCalID.SetCheck(FALSE);    //19
	m_page2.m_CheckMode.SetCheck(FALSE);     //20

	m_page2.m_CheckDate.SetCheck(FALSE);       //21
	m_page2.m_CheckTime.SetCheck(FALSE);       //22
	m_page2.m_CheckEye.SetCheck(FALSE);        //23
	m_page2.m_CheckPrePost.SetCheck(FALSE);    //24
	m_page2.m_CheckSW.SetCheck(FALSE);         //25
	m_page2.m_CheckPhysicianID.SetCheck(FALSE);//26
	m_page2.m_CheckPhysFName.SetCheck(FALSE);  //27
	m_page2.m_CheckPhysLName.SetCheck(FALSE);  //28
	m_page2.m_CheckOperatorID.SetCheck(FALSE); //29
	m_page2.m_CheckOperFName.SetCheck(FALSE);  //30

	m_page2.m_CheckOperLName.SetCheck(FALSE);  //31
	m_page2.m_CheckExamNote.SetCheck(FALSE);   //32
	m_page2.m_CheckLimbalX.SetCheck(FALSE);    //33
	m_page2.m_CheckLimbalY.SetCheck(FALSE);    //34
	m_page2.m_CheckLimbalD.SetCheck(FALSE);    //35
	m_page2.m_CheckCentralPwr.SetCheck(FALSE); //36
	m_page2.m_CheckBestFitSph.SetCheck(FALSE); //37
	m_page2.m_CheckBestConCen.SetCheck(FALSE); //38
	m_page2.m_CheckBestConEcc.SetCheck(FALSE); //39
	m_page2.m_CheckBestConAsp.SetCheck(FALSE); //40

	m_page2.m_CheckRefPwrStp.SetCheck(FALSE);  //41
	m_page2.m_CheckRefPwrFlt.SetCheck(FALSE);  //42
	m_page2.m_CheckEffRef.SetCheck(FALSE);     //43
	m_page2.m_CheckRefPwrAst.SetCheck(FALSE);  //44
	m_page2.m_CheckSimKStp.SetCheck(FALSE);    //45
	m_page2.m_CheckSimKFlt.SetCheck(FALSE);    //46
	m_page2.m_CheckSimKAvg.SetCheck(FALSE);    //47
	m_page2.m_CheckSimKDel.SetCheck(FALSE);    //48
	m_page2.m_CheckIS.SetCheck(FALSE);         //49
	m_page2.m_CheckAvePwr.SetCheck(FALSE);     //50

	m_page2.m_CheckStpSemi1.SetCheck(FALSE); //51
	m_page2.m_CheckStpSemi2.SetCheck(FALSE); //52
	m_page2.m_CheckFltSemi1.SetCheck(FALSE); //53
	m_page2.m_CheckFltSemi2.SetCheck(FALSE); //54
	m_page2.m_CheckStpSemi12.SetCheck(FALSE); //55
	m_page2.m_CheckStpSemi22.SetCheck(FALSE); //56
	m_page2.m_CheckFltSemi12.SetCheck(FALSE); //57
	m_page2.m_CheckFltSemi22.SetCheck(FALSE); //58
	m_page2.m_CheckStpSemi13.SetCheck(FALSE); //59
	m_page2.m_CheckStpSemi23.SetCheck(FALSE); //60

	m_page2.m_CheckFltSemi13.SetCheck(FALSE); //61
	m_page2.m_CheckFltSemi23.SetCheck(FALSE); //62
	m_page2.m_CheckCornealSph.SetCheck(FALSE); //63
	m_page2.m_CheckCornealZer1.SetCheck(FALSE); //64
	m_page2.m_CheckCornealZer2.SetCheck(FALSE); //65
	m_page2.m_CheckCornealZer3.SetCheck(FALSE); //66
	m_page2.m_CheckCornealZerF.SetCheck(FALSE); //67

	m_page2.m_CheckClinic.SetCheck(FALSE); //68
										   //Done






										   //WF and CT excel setting	
	m_page3.m_CheckUserID.SetCheck(FALSE);  //1
	m_page3.m_CheckFName.SetCheck(FALSE);   //2
	m_page3.m_CheckMName.SetCheck(FALSE);   //3
	m_page3.m_CheckLName.SetCheck(FALSE);   //4
	m_page3.m_CheckDOB.SetCheck(FALSE);     //5
	m_page3.m_CheckGender.SetCheck(FALSE);  //6
	m_page3.m_CheckAddLine1.SetCheck(FALSE);//7
	m_page3.m_CheckAddLine2.SetCheck(FALSE);//8
	m_page3.m_CheckCity.SetCheck(FALSE);    //9
	m_page3.m_CheckState.SetCheck(FALSE);   //10

	m_page3.m_CheckZip.SetCheck(FALSE);      //11
	m_page3.m_CheckCountry.SetCheck(FALSE);  //12
	m_page3.m_CheckWPhone.SetCheck(FALSE);   //13
	m_page3.m_CheckHPhone.SetCheck(FALSE);   //14
	m_page3.m_CheckEmail.SetCheck(FALSE);    //15
	m_page3.m_CheckNote.SetCheck(FALSE);     //16
	m_page3.m_CheckGroupGUID.SetCheck(FALSE);//17
	m_page3.m_CheckGroupName.SetCheck(FALSE);//18
	m_page3.m_CheckCalID.SetCheck(FALSE);    //19
	m_page3.m_CheckMode.SetCheck(FALSE);     //20

	m_page3.m_CheckDate.SetCheck(FALSE);       //21
	m_page3.m_CheckTime.SetCheck(FALSE);       //22
	m_page3.m_CheckEye.SetCheck(FALSE);        //23
	m_page3.m_CheckPrePost.SetCheck(FALSE);    //24
	m_page3.m_CheckSW.SetCheck(FALSE);         //25
	m_page3.m_CheckPhysicianID.SetCheck(FALSE);//26
	m_page3.m_CheckPhysFName.SetCheck(FALSE);  //27
	m_page3.m_CheckPhysLName.SetCheck(FALSE);  //28
	m_page3.m_CheckOperatorID.SetCheck(FALSE); //29
	m_page3.m_CheckOperFName.SetCheck(FALSE);  //30

	m_page3.m_CheckOperLName.SetCheck(FALSE);  //31
	m_page3.m_CheckExamNote.SetCheck(FALSE);   //32
	m_page3.m_CheckAngelKDist.SetCheck(FALSE); //33
	m_page3.m_CheckAngelKDeg.SetCheck(FALSE);  //34
	m_page3.m_CheckAngelADist.SetCheck(FALSE); //35
	m_page3.m_CheckAngelADeg.SetCheck(FALSE);  //36
	m_page3.m_CheckLimbalX.SetCheck(FALSE);    //37
	m_page3.m_CheckLimbalY.SetCheck(FALSE);    //38
	m_page3.m_CheckLimbalD.SetCheck(FALSE);    //39
	m_page3.m_CheckPupilX.SetCheck(FALSE);     //40

	m_page3.m_CheckPupilY.SetCheck(FALSE);     //41
	m_page3.m_CheckPupilD.SetCheck(FALSE);     //42
	m_page3.m_CheckScanD.SetCheck(FALSE);      //43
	m_page3.m_CheckTraceRef.SetCheck(FALSE);   //44
	m_page3.m_CheckRef1Zone.SetCheck(FALSE);   //45
	m_page3.m_CheckRef2Zone.SetCheck(FALSE);   //46
	m_page3.m_CheckRef3Zone.SetCheck(FALSE);   //47
	m_page3.m_CheckRef4Zone.SetCheck(FALSE);   //48
	m_page3.m_CheckEntireEyeZC1.SetCheck(FALSE); //49
	m_page3.m_CheckEntireEyeZC2.SetCheck(FALSE); //50

	m_page3.m_CheckEntireEyeZC3.SetCheck(FALSE); //51
	m_page3.m_CheckEntireEyeZCF.SetCheck(FALSE); //52
	m_page3.m_CheckCentralPwr.SetCheck(FALSE); //53
	m_page3.m_CheckBestFitSph.SetCheck(FALSE); //54
	m_page3.m_CheckBestConCen.SetCheck(FALSE); //55
	m_page3.m_CheckBestConEcc.SetCheck(FALSE); //56
	m_page3.m_CheckBestConAsp.SetCheck(FALSE); //57
	m_page3.m_CheckRefPwrStp.SetCheck(FALSE);  //58
	m_page3.m_CheckRefPwrFlt.SetCheck(FALSE);  //59
	m_page3.m_CheckEffRef.SetCheck(FALSE);     //60

	m_page3.m_CheckRefPwrAst.SetCheck(FALSE);  //61
	m_page3.m_CheckSimKStp.SetCheck(FALSE);    //62
	m_page3.m_CheckSimKFlt.SetCheck(FALSE);    //63
	m_page3.m_CheckSimKAvg.SetCheck(FALSE);    //64
	m_page3.m_CheckSimKDel.SetCheck(FALSE);    //65
	m_page3.m_CheckIS.SetCheck(FALSE);         //66
	m_page3.m_CheckAvePwr.SetCheck(FALSE);     //67
	m_page3.m_CheckStpSemi1.SetCheck(FALSE); //68
	m_page3.m_CheckStpSemi2.SetCheck(FALSE); //69
	m_page3.m_CheckFltSemi1.SetCheck(FALSE); //70

	m_page3.m_CheckFltSemi2.SetCheck(FALSE); //71
	m_page3.m_CheckStpSemi12.SetCheck(FALSE); //72
	m_page3.m_CheckStpSemi22.SetCheck(FALSE); //73
	m_page3.m_CheckFltSemi12.SetCheck(FALSE); //74
	m_page3.m_CheckFltSemi22.SetCheck(FALSE); //75
	m_page3.m_CheckStpSemi13.SetCheck(FALSE); //76
	m_page3.m_CheckStpSemi23.SetCheck(FALSE); //77
	m_page3.m_CheckFltSemi13.SetCheck(FALSE); //78
	m_page3.m_CheckFltSemi23.SetCheck(FALSE); //79
	m_page3.m_CheckCornealSph.SetCheck(FALSE); //80

	m_page3.m_CheckCornealZer1.SetCheck(FALSE); //81
	m_page3.m_CheckCornealZer2.SetCheck(FALSE); //82
	m_page3.m_CheckCornealZer3.SetCheck(FALSE); //83
	m_page3.m_CheckCornealZerF.SetCheck(FALSE); //84

	m_page3.m_CheckInternalZer1.SetCheck(FALSE); //85
	m_page3.m_CheckInternalZer2.SetCheck(FALSE); //86
	m_page3.m_CheckInternalZer3.SetCheck(FALSE); //87
	m_page3.m_CheckInternalZerF.SetCheck(FALSE); //88

	m_page3.m_CheckClinic.SetCheck(FALSE); //89

	m_page3.m_CheckDli.SetCheck(FALSE); //90 
	m_page3.m_CheckOpacityGrade.SetCheck(FALSE); //91 
												 //Done	
}