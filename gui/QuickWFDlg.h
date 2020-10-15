//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Hardware.h"
#include "MGDI.h"
#include "XPListCtrl.h"
#include "Settings.h"

//***************************************************************************************

class CQuickWFDlg : public CDialog
{
public:

	CQuickWFDlg(CHW* pHW, CALIBRATION* pCalibration, CWnd* pParentWnd);

private:
	CHW*         m_pHW;
	CALIBRATION* m_pCalibration;

	HACCEL       m_hAccel;

	int          m_w;
	int          m_h;

	BOOL         m_WFClick;
	CString      m_res[300];
	int          m_count;

	//Backup of releated setting values
	int       m_Bk_VertexDistanceUm;
	int       m_Bk_ZoneDiameterUm[3];
	int       m_Bk_RefractionRounding;
	int       m_Bk_AutoScanDiameterUm;
	int       m_Bk_ManualScanDiameterUm;
	//Done

	//Backup of releated Calibration values
	real_t m_backC3;
	real_t m_backC4;
	real_t m_backC5;
	int  m_backExposureTime;
	BOOL m_backLADSwapXY;
	BOOL m_backADXNoFlip;
	real_t m_backLADXFocalDistance;
	BOOL m_backLADYNoFlip;
	real_t m_backLADYFocalDistance;
	int  m_backOptometerEnabled;
	//Done

	//Record of releated values
	int  m_Saved_VertexDistanceUm[300];
	int  m_Saved_ZoneDiameterUm[300];
	int  m_Saved_RefractionRounding[300];
	int  m_Saved_ScanDiameterUm[300];

	real_t m_Saved_C3[300];
	real_t m_Saved_C4[300];
	real_t m_Saved_C5[300];
	int  m_Saved_ExposureTime[300];
	BOOL m_Saved_LADSwapXY[300];
	BOOL m_Saved_ADXNoFlip[300];
	real_t m_Saved_LADXFocalDistance[300];
	BOOL m_Saved_LADYNoFlip[300];
	real_t m_Saved_LADYFocalDistance[300];
	int  m_Saved_OptometerEnabled[300];
	//Done


	CComboBox m_VertexComboBox;
	CComboBox m_ZonesComboBox;

	CButton   m_Round8Radio;
	CButton   m_Round100Radio;

	CEdit     m_ScanDiameterEdit;

	CEdit     m_C3Edit;
	CEdit     m_C4Edit;
	CEdit     m_C5Edit;

	CEdit     m_LADExpTimeEdit;
	CButton   m_SwapLADsCheck;
	CButton   m_FlipLADXCheck;
	CEdit     m_LADXFocalDistanceEdit;
	CButton   m_FlipLADYCheck;
	CEdit     m_LADYFocalDistanceEdit;

	CButton   m_OptometerCheck;

	CButton   m_QuickWFBtn;
	CButton   m_SaveWFBtn;
	CButton   m_DeleteWFBtn;
	CButton   m_PrintBtn;

	CStatic   m_ResText;
	CStatic   m_Opt_Enable_Text;

	CXPListCtrl m_WFResultList;

	CButton   m_CalSaveButton;//

	BOOL    CalChanged();
	void    ChangeCal();
	void    GetBkSetVal();

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

	afx_msg void OnQuickWFBtnClicked();
	afx_msg void OnSaveBtnClicked();
	afx_msg void OnDeleteBtnClicked();
	afx_msg void OnPrintBtnClicked();
	afx_msg void OnOptometerCheckClicked();

	afx_msg void OnListDblClicked(NMHDR* pNMHDR, LRESULT* pRes);

	afx_msg void OnCalSaveBtnClicked();//

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel()     override;
	virtual void OnOK()         override;

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
