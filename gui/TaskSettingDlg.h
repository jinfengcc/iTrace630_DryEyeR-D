//***************************************************************************************

#pragma once

//***************************************************************************************
#include "Settings.h"

class CTaskSettinigDlg : public CDialog
{
public:

	CTaskSettinigDlg(CWnd* pParentWnd, CTaskSetting OriTask, int WhichTask);

	CTaskSetting m_SettedTask;

private:

	int      m_t;
	int      m_WhichTask;
	BOOL     m_Enable;

	BOOL     m_Order_Text_Show[28];
	int      m_Order_Int[28];
	int      m_Order_OriInt[28];
	int      m_LastSort;

	BOOL     m_SetDefault;
	BOOL     m_Changed;

	CTaskSetting m_OriTask;

	CEdit    m_TaskName_Edit;

	CButton  m_TaskEn_Radio;
	CButton  m_TaskDis_Radio;

	CButton  m_Wfra_Check;
	CButton  m_Wfaa_Check;
	CButton  m_Vfa_Check;
	CButton  m_Wfva_Check;
	CButton  m_Wfrms_Check;
	CButton  m_Wfka_Check;
	CButton  m_Wfac_Check;

	CButton  m_Ctds_Check;
	CButton  m_Ctsu_Check;
	CButton  m_Ctke_Check;
	CButton  m_Ct3d_Check;
	CButton  m_Ctcm_Check;//n

	CButton  m_Wcch_Check;
	CButton  m_Wcsu_Check;
	CButton  m_Wcva_Check;
	CButton  m_Wcou_Check;
	CButton  m_Wciol_Check;
	CButton  m_Dysfun_Check;//531
	CButton  m_AstiSou_Check;//531

	CButton  m_Tor_Check;

	CButton  m_Def_Check[8];
	CEdit    m_Def_Edit[8];

	CStatic  m_Order_Text[28];//n

	CStatic  m_Wfra_Text;
	CStatic  m_Wfaa_Text;
	CStatic  m_Vfa_Text;
	CStatic  m_Wfva_Text;
	CStatic  m_Wfrms_Text;
	CStatic  m_Wfka_Text;
	CStatic  m_Wfac_Text;

	CStatic  m_Ctds_Text;
	CStatic  m_Ctsu_Text;
	CStatic  m_Ctke_Text;
	CStatic  m_Ct3d_Text;
	CStatic  m_Ctcm_Text;//n

	CStatic  m_Wcch_Text;
	CStatic  m_Wcsu_Text;
	CStatic  m_Wcva_Text;
	CStatic  m_Wcou_Text;
	CStatic  m_Wciol_Text;
	CStatic  m_DysFun_Text;//531
	CStatic  m_Astisou_Text;//531

	CStatic  m_Tor_Text;

	CStatic  m_Other_Text;
	CStatic  m_UserDef_Text;

	CStatic  m_sn_Text;

	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;

	void    CreateOriOrder();
	CString GetStrN(int n);
	int     GetNStr(int n);
	void    SetDlg(CTaskSetting Task);
	void    GetDlg();
	void    SetDlgAble();

	CTaskSetting  SetDefTask0();
	CTaskSetting  SetDefTask1();
	CTaskSetting  SetDefTask2();
	CTaskSetting  SetDefTask3();
	CTaskSetting  SetDefTask4();

	BOOL   CheckDefEdit();

	void   OnCheckBoxClick(int whichone);

	afx_msg void OnDefault();

	afx_msg void OnWfraClicked() { OnCheckBoxClick(0); };
	afx_msg void OnWfaaClicked() { OnCheckBoxClick(1); };
	afx_msg void OnVfaClicked() { OnCheckBoxClick(2); };
	afx_msg void OnWfvaClicked() { OnCheckBoxClick(3); };
	afx_msg void OnWfrmsClicked() { OnCheckBoxClick(4); };
	afx_msg void OnWfkaClicked() { OnCheckBoxClick(5); };
	afx_msg void OnWfacClicked() { OnCheckBoxClick(6); };

	afx_msg void OnCtdsClicked() { OnCheckBoxClick(7); };
	afx_msg void OnCtsuClicked() { OnCheckBoxClick(8); };
	afx_msg void OnCtkeClicked() { OnCheckBoxClick(9); };
	afx_msg void OnCt3DClicked() { OnCheckBoxClick(10); };

	afx_msg void OnWcchClicked() { OnCheckBoxClick(11); };
	afx_msg void OnWcsuClicked() { OnCheckBoxClick(12); };
	afx_msg void OnWcvaClicked() { OnCheckBoxClick(13); };
	afx_msg void OnWcouClicked() { OnCheckBoxClick(14); };
	afx_msg void OnWciolClicked() { OnCheckBoxClick(15); };
	afx_msg void OnDysfunClicked() { OnCheckBoxClick(26); };//531
	afx_msg void OnAstiSoufunClicked() { OnCheckBoxClick(27); };//531


	afx_msg void OnTorClicked() { OnCheckBoxClick(16); };

	afx_msg void OnDef1ChekClicked() { OnCheckBoxClick(17); };
	afx_msg void OnDef2ChekClicked() { OnCheckBoxClick(18); };
	afx_msg void OnDef3ChekClicked() { OnCheckBoxClick(19); };
	afx_msg void OnDef4ChekClicked() { OnCheckBoxClick(20); };
	afx_msg void OnDef5ChekClicked() { OnCheckBoxClick(21); };
	afx_msg void OnDef6ChekClicked() { OnCheckBoxClick(22); };
	afx_msg void OnDef7ChekClicked() { OnCheckBoxClick(23); };
	afx_msg void OnDef8ChekClicked() { OnCheckBoxClick(24); };

	afx_msg void OnCtcmClicked() { OnCheckBoxClick(25); };//n


	afx_msg void OnEnableClicked();
	afx_msg void OnDisableClicked();

	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
