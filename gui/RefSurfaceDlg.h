//***************************************************************************************

#pragma once

//***************************************************************************************

class CRefSurfaceDlg : public CDialog
{
public:

	CRefSurfaceDlg(CWnd* pParentWnd, real_t ax0_um, real_t q);

	real_t m_ax0_um;
	real_t m_q;

private:

	CEdit m_CentralRadiusEdit;
	CEdit m_AsphericityEdit;

	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;
};

//***************************************************************************************
