//***************************************************************************************

#pragma once

//***************************************************************************************

#define TYPE_CLINIC        0//1
#define TYPE_EXAMHEADER    1
#define TYPE_EYE           2//1
#define TYPE_GROUP         3//1
#define TYPE_OPERATOR      4//1
#define TYPE_PATIENT       5//1
#define TYPE_PHYSICIAN     6//1

//***************************************************************************************

class CDatabaseMergeDlg : public CDialog
{
public:

	CDatabaseMergeDlg(CWnd* pParentWnd, CString DefaultDabaseFile);

private:
	int      m_tClinics;
	int      m_tExamHeaders;
	int      m_tEyes;
	int      m_tGroups;
	int      m_tOperators;
	int      m_tPatients;
	int      m_tPhysicians;

	CDB          m_ThisDB;
	CDB          m_MergeDB;

	CClinic      m_pClinics[1000];
	CExamHeader  m_pExamHeaders[50000];
	CEye         m_pEyes[1000];
	CGroup       m_pGroups[1000];
	COperator    m_pOperators[1000];
	CPatient     m_pPatients[5000];
	CPhysician   m_pPhysicians[1000];

	CString m_DestOriFileName;
	CString m_FileNames[5];
	CEdit m_FileNameEdit[5];

	CButton m_FileBrowseButton1;
	CButton m_FileBrowseButton2;
	CButton m_FileBrowseButton3;
	CButton m_FileBrowseButton4;
	CButton m_FileBrowseButton5;

	BOOL     MergeDatabases();
	void     GetFolderandFileName(CString PathName, CString &Folder, CString &FileName);
	BOOL     CreateTdbFile(CString DestTdbFileName, CString DestOriFileName);
	BOOL     NotInsideDataSet(GUID ImportID, int setNumber, int type);

	BOOL     SaveClinics(int i);
	BOOL     SaveGroups(int i);
	BOOL     SavePatients(int i);
	BOOL     SaveOperators(int i);
	BOOL     SavePhysicians(int i);
	BOOL     SaveExamHeaders(int i);
	BOOL     SaveEyes(int i);

	void     SaveMergedData();

	afx_msg void OnFileBrowseButton1Clicked();
	afx_msg void OnFileBrowseButton2Clicked();
	afx_msg void OnFileBrowseButton3Clicked();
	afx_msg void OnFileBrowseButton4Clicked();
	afx_msg void OnFileBrowseButton5Clicked();

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
