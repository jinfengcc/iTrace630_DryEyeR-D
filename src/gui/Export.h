//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"
#include "Settings.h"

//***************************************************************************************

void ExportScreen(CExam* pExam1, CExam* pExam2, const Matrix<uchar>& Mem, CDicomInfo DicomInfo, uchar* RGBData, int w, int h); // 520, Add ', CDicomInfo DicomInfo, uchar* m_RGBData, int w, int h ' For DICOM export

void ExportCustomvis(const CString& FolderName, CExam* pExam);

void ExportWavetouch(CWFExam* pWFExam, const Matrix<uchar>& Mem);

void ExportTopconCV5000(CWFExam* pWFODExam, CCTExam* pCTODExam, CWFExam* pWFOSExam, CCTExam* pCTOSExam);

BOOL FindColorPupil(CEyeImage* OriImage);

double Gaussian(int x, int y, double de);

HSI ChangeRGBtoHSI(int R, int G, int B);
