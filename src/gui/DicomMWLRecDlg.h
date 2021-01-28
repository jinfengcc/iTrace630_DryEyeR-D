/***************************************************************************************

Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          
Description:   Create a dialog for the source of import patient selection.
Since customer has their MWL system for DICOM import
read '//dicomiseasy.blogspot.com/2012/04/dicom-modality-worklist.html'
and '//dicomiseasy.blogspot.com/2011/12/introduction-to-dicom-chapter-5-solving.html'
verison:       6.2.0

***************************************************************************************/

#pragma once

//***************************************************************************************

class CDicomMWLRecDlg : public CDialog
{
public:

	CDicomMWLRecDlg(CWnd* pParentWnd);

	int m_RecCount;
	CDicomInfo m_DicomInfo[1000];
	BOOL       m_SelDicom[1000];

private:

	CEdit      m_Modality;
	CEdit      m_IPEdit;
	CEdit      m_PortEdit;
	CEdit      m_TarGetAEEdit;
	CEdit      m_LocalAEEdit;
	CEdit      m_FNameEdit;
	CEdit      m_LNameEdit; 
	CEdit      m_PaIDEdit;
	CEdit      m_FromEdit;
	CEdit      m_ToEdit;
	CComboBox  m_ModalityCombo;
	CButton    m_QueryBtn;

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();


	//virtual void OnOK() override;

	afx_msg void OnQueryClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************


