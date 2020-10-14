#include "ExcelSettingsWFpage.h"
#include "ExcelSettingsCTpage.h"
#include "ExcelSettingsWFCTpage.h"
#include <atlimage.h>  //520 121129 For CImage class

//***************************************************************************************

#pragma once

//***************************************************************************************

class CExcelSettingsDlg2 : public CDialog
{
public:

	CExcelSettingsDlg2(CWnd* pParentWnd, BOOL ExcDef);// Add m_ExcDef

private:

	CButton   m_CancelButton;
	CButton   m_OKButton;
	CTabCtrl  m_tab;

	BOOL m_ExcDef;//

	int m_CurSelTab;
	CExcelSettingsWFpage m_page1;
	CExcelSettingsCTpage m_page2;
	CExcelSettingsWFCTpage m_page3;
	CDialog* pDialog[3];

	void SetSavedSetting();
	void SetDefSetting();//  
	void SaveSetting();

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
