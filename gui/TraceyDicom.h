/****************************************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          05-09-2016
Description:   Create a Tracey Dicom class by using Merge Dicom toolkit
*****************************************************************************************/

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Data.h"// for import the patient's info for dicom file saveing

//***************************************************************************************

#define AddTagS( a )  { el->Init( (int)a ); p->insertElement(el); }
#define AddTagS2( a )  { el->Init( (int)a ); p2->insertElement(el); }
#define AddTag( a )  { el->Init( (int)a ); o->insertElement(el); }
#define AddTagV( a,v )  { el->Init( (int)a ); el->Value=v; o->insertElement(el); }


class CTraceyDicom
{
public:
	CTraceyDicom();
	~CTraceyDicom();

	CDicomInfo m_DicomInfo[1000];

	void OutputDicomImage(CString filename, CDicomInfo DicomInfo, CHAR* pData, int width, int height);

	void InputChinseeDicom(wchar_t* filename, CDicomInfo & DicomInfo);

	void InputDicom(char* filename, CDicomInfo & DicomInfo);

	void  ReadDicomImage(char* filename, CDicomInfo &OutDicomInfo);

	BOOL  Query(CString ModalityStr, CString host, unsigned short port, CString CallAETitle, CString CalledTitle, CString PatientID, CString FirstName, CString LastName, CString DateFrom, CString DateTo, int & count);// 6.2.0 new Dicom

	BOOL  TestActivation();

	void  FirstRun();
	
private:
	
	BOOL GetStandDate(CString OriDate, CString &NewDate, CString NewNote);

	void  GetFiLaMiName(CString FullName, CString & FirstName, CString & LastName, CString & MiddleName);

};

