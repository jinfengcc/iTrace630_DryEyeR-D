//***************************************************************************************

#pragma once

//***************************************************************************************

class CSettingsIOLSelDlg : public CDialog
{
public:

	CSettingsIOLSelDlg(CWnd* pParentWnd, BOOL Default, real_t IOLSel_Alpha[2], real_t IOLSel_CHOA, real_t IOLSel_IHOA, real_t IOLSel_Astig, real_t IOLSel_Sph_Abbe[4]);

	real_t  m_IOLSel_Alpha[2];//530 IOL SELECTION threshold
	real_t  m_IOLSel_CHOA;//530 IOL SELECTION Corneal HOA threshold
	real_t  m_IOLSel_Astig;//530 IOL SELECTION threshold
	real_t  m_IOLSel_Sph_Abbe[4];//530 IOL SELECTION threshold
	real_t  m_IOLSel_IHOA;//530 IOL SELECTION Internal HOA threshold

private:

	HACCEL          m_hAccel;

	BOOL    m_Defult;

	BOOL    m_Alpha_Edit1Updata;
	BOOL    m_Alpha_Edit2Updata;
	BOOL    m_Alpha_Edit3Updata;
	BOOL    m_Alpha_Edit4Updata;

	BOOL    m_HOA_CEdit1Updata;
	BOOL    m_HOA_CEdit2Updata;

	BOOL    m_HOA_IEdit1Updata;
	BOOL    m_HOA_IEdit2Updata;

	BOOL    m_Astig_Edit1Updata;
	BOOL    m_Astig_Edit2Updata;

	BOOL    m_Saval_EditUpdata[8];

	CEdit   m_Alpha_Edit1;
	CEdit   m_Alpha_Edit2;
	CEdit   m_Alpha_Edit3;
	CEdit   m_Alpha_Edit4;

	CEdit   m_Hoa_CEdit1;
	CEdit   m_Hoa_CEdit2;

	CEdit   m_Hoa_IEdit1;
	CEdit   m_Hoa_IEdit2;

	CEdit   m_Astig_Edit1;
	CEdit   m_Astig_Edit2;

	CEdit   m_Saval_Edit[8];

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

	afx_msg void UpdateAlphaEdit1();
	afx_msg void UpdateAlphaEdit2();
	afx_msg void UpdateAlphaEdit3();
	afx_msg void UpdateAlphaEdit4();

	afx_msg void UpdateHoaCEdit1();
	afx_msg void UpdateHoaCEdit2();

	afx_msg void UpdateHoaIEdit1();
	afx_msg void UpdateHoaIEdit2();

	afx_msg void UpdateAstigEdit1();
	afx_msg void UpdateAstigEdit2();

	afx_msg void UpdateSavalEdit1();
	afx_msg void UpdateSavalEdit2();
	afx_msg void UpdateSavalEdit3();
	afx_msg void UpdateSavalEdit4();
	afx_msg void UpdateSavalEdit5();
	afx_msg void UpdateSavalEdit6();
	afx_msg void UpdateSavalEdit7();
	afx_msg void UpdateSavalEdit8();

	afx_msg void OnDefault();

	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);//

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
