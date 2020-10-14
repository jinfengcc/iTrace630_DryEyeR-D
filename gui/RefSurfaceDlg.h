//***************************************************************************************

#pragma once

//***************************************************************************************

class CRefSurfaceDlg : public CDialog
{
public:

	CRefSurfaceDlg(CWnd* pParentWnd, real ax0_um, real q);

	real m_ax0_um;
	real m_q;

private:

	CEdit m_CentralRadiusEdit;
	CEdit m_AsphericityEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;
};

//***************************************************************************************
