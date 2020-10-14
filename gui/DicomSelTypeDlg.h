/***************************************************************************************

Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          11-14-2016
Description:   Create a dialog for the source of import patient selection.
Since customer has their MWL system for DICOM import
verison:       6.2.0

***************************************************************************************/

#pragma once

//***************************************************************************************

class CDicomSelTypeDlg : public CDialog
{
public:

	CDicomSelTypeDlg(CWnd* pParentWnd);

	BOOL  m_FileRadioSel;

private:

	CButton   m_FileRadio;
	CButton   m_MWLRadio;

	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();

	afx_msg void OnFielRadioClicked();
	afx_msg void OnMWLRadioClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************

