//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"

//***************************************************************************************

#define RES_BAYNERA  6 //Rayner Aspheric, MI 60, or Spherical IOL
#define RES_HOYABC2  7 //Hoya ABC
#define RES_HOYABC3  8 //Hoya ABC and Acrysof IQ
#define RES_ACRYSOF  9 //Acrysof IQ
#define RES_TECNISI  10//Tecnis IOL

class CWFCTIolWnd : public CSumWnd
{
public:

	CWFCTIolWnd(CWnd *pWnd, RECT &WndRect, CPatient *pPatient, CWFExam *pWFExam, CCTExam *pCTExam, CWndSettings *pWndSettings,
              int show = SW_SHOW);

private:

	int  m_Scale;//521
	BOOL SameOU();//521

	real_t m_vals[4];

	BOOL m_TraceySet;
	BOOL m_CorHoa;
	BOOL m_Inter;

	int  m_Alpha_Dec;     //Alpha Decsion; AlphaRum >= 0.65 : 0 ; 0.5 ~ 0.65 : 1;  <= 0.5 : 2
	int  m_TCHoa_Dec;     //Total Corneal Decision
	int  m_TCHoa_Dec1;    //Total Corneal Decision
	int  m_Cor_Ast_Dec;   //Corneal Astigmatism Decision
	int  m_Cor_SphAb_Dec;//Corneal Spherical Abberration (SA) at 6.00 mm Decision

	CString m_Con_Res1;
	CString m_Con_Res2;
	CString m_Con_Res3;//new
	CString m_Con_Res4;//new

	RECT m_TitleRect[2];
	RECT m_InfoRect[2];
	RECT m_ComplaintRect[2];

	CBtn     m_IOLSetBtn;
	CButton  m_TraceyRecYes;
	CButton  m_TraceyRecNo;

	CButton  m_HOAYes;
	CButton  m_HOANo;

	CButton  m_EyeCorYes;
	CButton  m_EyeCorNo;

	CWFExam* m_pWFExam;

	CCTExam* m_pCTExam;

	void    GetConRes();

	virtual CString GetNameImpl() const override { return "WF&&CT IOL Selection Analysis"; }
	virtual CWndSettings* GetWndSettings() override { return m_pWndSettings + (m_d % 3); }

	virtual void CreateMenu() override;

	virtual void RepaintMemDC() override;
	virtual void CreateChildWnd() override;

	void    GetVals();
	int     GetGridScale(real_t r_um);
	afx_msg void OnShowDefaultItemClicked();

	afx_msg LRESULT OnChangeCentMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeStepMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeMaxMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTraceYesClicked();
	afx_msg void OnTraceNoClicked();

	afx_msg void OnHoaYesClicked();
	afx_msg void OnHoaNoClicked();

	afx_msg void OnEyeCorYesClicked();
	afx_msg void OnEyeCorNoClicked();

	afx_msg void OnSettingClicked();

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
