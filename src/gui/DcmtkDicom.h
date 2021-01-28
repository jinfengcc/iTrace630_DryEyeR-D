/****************************************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          06-09-2017
Description:   Create a  Dicom class (use DCMTK) for no combo activation (save screen as dicom)
*****************************************************************************************/

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"// for import the patient's info for dicom file saveing

//***************************************************************************************

class CDcmtkDicom
{
public:
	CDcmtkDicom();
	~CDcmtkDicom();

	CDicomInfo m_DicomInfo[1000];

	BOOL SaveDcmtkDicomFile(CString fileName, CDicomInfo DicomInfo, BYTE* pData, int width, int height);

private:

};
