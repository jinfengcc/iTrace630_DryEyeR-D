//***************************************************************************************

#include "StdAfx.h"
#include "CTColorImgWnd.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BEGIN_MESSAGE_MAP(CCTColorImgWnd, CSumWnd)


END_MESSAGE_MAP()

//***************************************************************************************

CCTColorImgWnd::CCTColorImgWnd(CWnd* pWnd, RECT& WndRect, CPatient* pPatient, CCTExam* pCTExam, CWndSettings* pWndSettings) :
	CSumWnd(pWnd, WndRect, pPatient, pWndSettings)
{
	//---------------------------------------------------- 
	m_pCTExam = pCTExam;

	Repaint();

	ShowWindow(SW_SHOW);
}

//***************************************************************************************

void CCTColorImgWnd::RepaintMemDC()
{
	CEyeImage* pImage = &m_pCTExam->m_ColorImage;
	if (pImage == NULL) return;
	if (pImage->m_RGBData.GetMem() == NULL) return;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;

	real_t x0_um = pImage->m_ve_x_um;
	real_t y0_um = pImage->m_ve_y_um;

	real_t h_um = 12000.0;
	real_t w_um = h_um * m_w / m_h;
	real_t x_px_um = m_w / w_um;
	real_t y_px_um = m_h / h_um;

	for (int y = 0; y < m_h; y++)
	{
		real_t y_um = y0_um - (cy - y) / y_px_um;
		for (int x = 0; x < m_w; x++)
		{
			real_t x_um = x0_um + (x - cx) / x_px_um;
			real_t r, g, b;
			if (pImage->GetRGBAtUm(x_um, y_um, &r, &g, &b))
			{
				m_MemDC.SetPixel(x, m_h - y - 1, (int)r, (int)g, (int)b);
			}
		}
	}
}

//***************************************************************************************


