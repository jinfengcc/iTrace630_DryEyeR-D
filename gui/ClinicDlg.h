//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"

//***************************************************************************************

class CClinicDlg : public CDialog
{
public:

	CClinicDlg(CWnd* pParentWnd, CClinic* pClinic);

	CClinic* m_pClinic;

private:

	CEdit m_NameEdit;
	CEdit m_Line1Edit;
	CEdit m_Line2Edit;
	CEdit m_CityEdit;
	CComboBox m_StateComboBox;
	CEdit m_ZipEdit;
	CEdit m_CountryEdit;

	//CButton  m_DeleteButton;//[cjf***05082012]
	//afx_msg void OnDeleteButtonClicked();//[cjf***05082012]

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()//[cjf***05082012]
};

//***************************************************************************************
