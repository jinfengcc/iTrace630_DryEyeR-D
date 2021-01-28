//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Settings.h"

//***************************************************************************************

class CCustomColorsDlg : public CDialog
{
public:

	CCustomColorsDlg(CScale* pExtScale, CWnd* pParent = NULL);

private:

	CScale* m_pExtScale;
	CScale m_Scale;

	CButton m_ColorButtons[21];

	void OnColor(const int i);
	afx_msg void OnColor01ButtonClicked() { OnColor(1); }
	afx_msg void OnColor02ButtonClicked() { OnColor(2); }
	afx_msg void OnColor03ButtonClicked() { OnColor(3); }
	afx_msg void OnColor04ButtonClicked() { OnColor(4); }
	afx_msg void OnColor05ButtonClicked() { OnColor(5); }
	afx_msg void OnColor06ButtonClicked() { OnColor(6); }
	afx_msg void OnColor07ButtonClicked() { OnColor(7); }
	afx_msg void OnColor08ButtonClicked() { OnColor(8); }
	afx_msg void OnColor09ButtonClicked() { OnColor(9); }
	afx_msg void OnColor10ButtonClicked() { OnColor(10); }
	afx_msg void OnColor11ButtonClicked() { OnColor(11); }
	afx_msg void OnColor12ButtonClicked() { OnColor(12); }
	afx_msg void OnColor13ButtonClicked() { OnColor(13); }
	afx_msg void OnColor14ButtonClicked() { OnColor(14); }
	afx_msg void OnColor15ButtonClicked() { OnColor(15); }
	afx_msg void OnColor16ButtonClicked() { OnColor(16); }
	afx_msg void OnColor17ButtonClicked() { OnColor(17); }
	afx_msg void OnColor18ButtonClicked() { OnColor(18); }
	afx_msg void OnColor19ButtonClicked() { OnColor(19); }
	afx_msg void OnColor20ButtonClicked() { OnColor(20); }
	afx_msg void OnColor21ButtonClicked() { OnColor(21); }

	afx_msg void OnNewButtonClicked();
	afx_msg void OnOpenButtonClicked();
	afx_msg void OnSaveButtonClicked();

	afx_msg void OnPaint();

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
