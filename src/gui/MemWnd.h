//***************************************************************************************

#pragma once

//***************************************************************************************

#include "MGdi.h"
#include "Data.h"//520 for import the patient's info for dicom file saveing

//***************************************************************************************

class CMemWnd : public CWnd
{
public:

	CMemWnd();
	virtual ~CMemWnd();

	int m_w;
	int m_h;
	CMDC m_MemDC;

	BOOL SpecialBorder;//6.2.0 NEAR VISION

	int  HighLightOutline[4];//6.2.0 NEAR VISION

	void CreateWnd(RECT& Rect, CWnd* pParentWnd, int show = SW_SHOW);
	void ClearMemDC();
	virtual void RepaintMemDC() {}
	virtual void PaintControls() {};//6.2.1
	void Repaint();

	BOOL SaveIntoMem(Matrix<uchar>& Mem, int CompressionType);
	void SaveIntoFile(CDicomInfo DicomInfo);
	void Print();

	void SetPrint();//6.2.1

protected:

	static BOOL m_Printing;

	static BOOL m_Saving;

	CMRgn m_Rgn;

	void Border();

	void BackupMemDC();
	void RestoreMemDC();

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnRButtonDown(uint nFlags, CPoint Point);
	afx_msg void OnRButtonUp(uint nFlags, CPoint Point);

private:

	uchar* m_pBackup;

	void SaveDicomFile(CString fileName, CDicomInfo DicomInfo);//520

	DECLARE_MESSAGE_MAP()
};

//***************************************************************************************
