/***************************************************************************************

Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          08-16-2017
Description:   Create a dialog for the source of recvied MWL record selection.
verison:       6.2.0

***************************************************************************************/

#pragma once

#include "Data.h"
#include "XPListCtrl.h"
//***************************************************************************************

class CDicomMWLResDlg : public CDialog
{
public:

	CDicomMWLResDlg(CWnd* pParentWnd);

	int m_RecCount;
	CDicomInfo m_DicomInfo[1000];

	int  m_SelCount;
	BOOL m_SelDicom[1000];

private:

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

	BOOL         m_SelAll;
	CXPListCtrl  m_RecordList;
	CButton      m_SelAllChk;

	void     GetFiLaMiName(CString FullName, CString & FirstName, CString & LastName, CString & MiddleName);//6.2.0 For Dicom	

	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();

	virtual void OnOK() override;

	afx_msg void OnSelAllChkClicked();
	afx_msg void OnListClicked(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnListRClicked(NMHDR* pNMHDR, LRESULT* pRes);

	afx_msg void UpdatePatientsList();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************


