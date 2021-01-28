//***************************************************************************************

#pragma once

//***************************************************************************************


//***************************************************************************************

class CToricSelDlg2 : public CDialog
{
public:

	CToricSelDlg2(CWnd* pParentWnd);

	int GetSelection() { return (m_SaveToFile ? 1 : m_SaveToZip ? 2 : m_SaveToBoth ? 3 : 4); }

private:

	BOOL    m_SaveToFile;
	BOOL    m_SaveToZip;
	BOOL    m_SaveToBoth;

	CButton m_SaveToFile_Radio;
	CButton m_SaveToZip_Radio;
	CButton m_SaveToBoth_Radio;
	CButton m_OKButton;


	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;

	afx_msg void OnFileRadioClicked();
	afx_msg void OnZipRadioClicked();
	afx_msg void OnBothRadioClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
