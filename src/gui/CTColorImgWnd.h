//***************************************************************************************

#pragma once

//***************************************************************************************

#include "SumWnd.h"

//***************************************************************************************

class CCTColorImgWnd : public CSumWnd
{
public:

	CCTColorImgWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings);

private:

	CCTExam* m_pCTExam;

	virtual void RepaintMemDC() override;

	DECLARE_MESSAGE_MAP()
};