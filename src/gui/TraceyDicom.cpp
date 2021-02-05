/****************************************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          05-09-2016
Description:   Create a Tracey Dicom class by using Merge Dicom toolkit
*****************************************************************************************/



//***************************************************************************************

#include "StdAfx.h"
#include <string>
#include <list>
#include <fstream>
#include "GlobalFunctions.h"

#include "TraceyDicom.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

// Rzdcx Dicom ToolKit .h
#import "C:\RZDCX\rzdcx.tlb" rename_namespace ("rzdcxLib")
//#import "C:\RZDCX\RZDCX_DEVPACK_2.0.5.9\Win64\rzdcx.tlb" rename_namespace ("rzdcxLib")

using namespace rzdcxLib;
using namespace std;
// Dicom ToolKit .h Done


//***************************************************************************************

CTraceyDicom::CTraceyDicom()
{
}

//***************************************************************************************

CTraceyDicom::~CTraceyDicom()
{
}

//***************************************************************************************

void CTraceyDicom::FirstRun()
{
	IDCXOBJPtr obj(__uuidof(DCXOBJ));
}

//***************************************************************************************

BOOL CTraceyDicom::TestActivation()
{
	/*int count = 0;

	for (int i = 0; i < 100; i++)
	{
		try
		{
			IDCXOBJPtr obj(__uuidof(DCXOBJ));
		}
		catch (const _com_error)
		{
			int a = 0;
		}

		count = i;
	}

	return TRUE;*/

#if 0
	int count = 0;

	for (int i = 0; i < 100; i++)
	{
		try
		{
			IDCXOBJPtr obj(__uuidof(DCXOBJ));
		}
		catch (const _com_error)
		{
			count = 1;
			break;
		}
	}

	if (count != 0) return FALSE;
#endif

	return TRUE;
}

//***************************************************************************************
void CTraceyDicom::OutputDicomImage(CString filename, CDicomInfo DicomInfo, CHAR* pData, int width, int height)
{
	HRESULT hr = ::CoInitialize(NULL);

	if (!SUCCEEDED(hr)) {
		::CoUninitialize();
		return;
	}

	IDCXOBJPtr obj(__uuidof(DCXOBJ));

	IDCXELMPtr el(__uuidof(DCXELM));

	IDCXUIDPtr id(__uuidof(DCXUID));

	// Set the basic elements - patient name and ID, study, series and instance UID's
	// You don't have to create an element every time,
	// just initialize it.

	CString s;
	s = DicomInfo.m_Patient_Name;
	G_As = s;

	char* sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::patientName);
	el->Value = sChar;
	//el->PutCStringPtr((int)pValue);
	obj->insertElement(el);

	//----------------------------------

	sChar = G_As.GetBuffer(s.GetLength());
	s = DicomInfo.m_Patient_ID;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::patientID);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	s = DicomInfo.m_Patient_Other_ID;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());
	el->Init(rzdcxLib::OtherPatientIDs);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	s = DicomInfo.m_Patient_BirthDate;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::PatientBirthDate);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	s = DicomInfo.m_Patient_Sex;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());;

	el->Init(rzdcxLib::PatientSex);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	el->Init(rzdcxLib::studyInstanceUID);
	el->Value = id->CreateUID(UID_TYPE_STUDY);
	obj->insertElement(el);

	//----------------------------------

	s = DicomInfo.m_StudyDes;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());;

	el->Init(rzdcxLib::StudyDescription);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	s = DicomInfo.m_Study_Date;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());;

	el->Init(rzdcxLib::StudyDate);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	s = DicomInfo.m_Study_Time;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());;

	el->Init(rzdcxLib::StudyTime);
	el->Value = sChar;
	obj->insertElement(el);

	////----------------------------------

	el->Init(rzdcxLib::sopInstanceUID);
	el->Value = id->CreateUID(UID_TYPE_INSTANCE);
	obj->insertElement(el);

	//----------------------------------

	el->Init(rzdcxLib::sopClassUid);
	el->Value = "1.2.840.10008.5.1.4.1.1.7"; // Secondary Capture
	obj->insertElement(el);

	//----------------------------------
	s = DicomInfo.m_Modality;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::Modality);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	s = DicomInfo.m_Equi_Manufacturer;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::Manufacturer);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	s = DicomInfo.m_Equi_Manufacture_Model;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::ManufacturerModelName);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------

	//6.2.0 Add 9 other items
	s = DicomInfo.m_Physician_Name;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::PerformingPhysicianName);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------
	s = DicomInfo.m_Study_ID;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::StudyID);
	el->Value = sChar;
	obj->insertElement(el);


	//----------------------------------
	s = DicomInfo.m_Series_Instance_UID;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::seriesInstanceUID);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------
	s = DicomInfo.m_Series_Num;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::SeriesNumber);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------
	s = DicomInfo.m_Acess_Num;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::AccessionNumber);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------
	s = DicomInfo.m_Laterality;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::Laterality);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------
	s = DicomInfo.m_Conversion_Type;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::ConversionType);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------
	s = DicomInfo.m_Instance_Num;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::InstanceNumber);
	el->Value = sChar;
	obj->insertElement(el);

	//----------------------------------
	s = DicomInfo.m_Patient_Orientation;
	G_As = s;
	sChar = G_As.GetBuffer(s.GetLength());

	el->Init(rzdcxLib::PatientOrientation);
	el->Value = sChar;
	obj->insertElement(el);
	//6.2.0 Add 9 other items Done

	//----------------------------------
	// Setting the image pixel
	int ROWS = height;
	int COLUMNS = width;
	int SAMPLES_PER_PIXEL = 3;
	int BITS_ALLOCATED = 8;
	int BITS_STORED = 8;
	int NUMBER_OF_FRAMES = 1;
	int RESCALE_INTERCEPT = 0;
	const char* PHOTOMETRIC_INTERPRETATION = "RGB";

	el->Init(rzdcxLib::Rows);
	el->Value = ROWS;
	obj->insertElement(el);

	el->Init(rzdcxLib::Columns);
	el->Value = COLUMNS;
	obj->insertElement(el);

	el->Init(rzdcxLib::SamplesPerPixel);
	el->Value = SAMPLES_PER_PIXEL;
	obj->insertElement(el);

	el->Init(rzdcxLib::PhotometricInterpretation);
	el->Value = PHOTOMETRIC_INTERPRETATION;
	obj->insertElement(el);

	if (string("RGB") == PHOTOMETRIC_INTERPRETATION)
	{
		el->Init(rzdcxLib::PlanarConfiguration);
		el->Value = 0;
		obj->insertElement(el);
	}

	el->Init(rzdcxLib::BitsAllocated);
	el->Value = BITS_ALLOCATED;
	obj->insertElement(el);

	el->Init(rzdcxLib::BitsStored);
	el->Value = BITS_STORED;
	obj->insertElement(el);

	el->Init(rzdcxLib::HighBit);
	el->Value = BITS_STORED - 1;
	obj->insertElement(el);

	el->Init(rzdcxLib::PixelRepresentation);
	el->Value = 0;
	obj->insertElement(el);

	el->Init(rzdcxLib::PixelData);
	el->ValueRepresentation = VR_CODE_OB;

	// Create a dummy pixel
	int frameSize = ROWS*COLUMNS*SAMPLES_PER_PIXEL;

	s = ::MAIN_DIR + "\\pixel.data";
	G_As = s;
	CHAR *charTempFile = G_As.GetBuffer(s.GetLength());

	ofstream sf(charTempFile, ios_base::binary);
	for (int i = 0; i<NUMBER_OF_FRAMES; i++)
	{
		sf.write(pData, frameSize);
	}
	sf.close();

	int pixelsLength = frameSize*NUMBER_OF_FRAMES;
	el->SetValueFromFile(charTempFile, 0, pixelsLength);
	obj->insertElement(el);

	// Save the dcm file
	obj->saveFile(_bstr_t(filename));

	s = ::MAIN_DIR + "\\pixel.data";
	DeleteFile(s);

	::CoUninitialize();
}
//***************************************************************************************

void  CTraceyDicom::InputDicom(char* filename, CDicomInfo & DicomInfo)
{
	// Create a DCXOBJ
	IDCXOBJPtr obj(__uuidof(DCXOBJ));

	obj->openFile(filename);

	// You need to hold a IDCXELMPtr in order to get the elements
	IDCXELMPtr elem;

	//Patient name
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(patientName);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Patient_Name = elem->Value;
	}

	//Patient ID
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(patientID);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Patient_ID = elem->Value;
	}


	//Patient GUID ID
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(OtherPatientIDs);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Patient_Other_ID = elem->Value;
	}

	//Patient BirthDay
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(PatientBirthDate);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Patient_BirthDate = elem->Value;
	}

	//Patient Sex
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(PatientSex);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Patient_Sex = elem->Value;
	}

	//Patient m_Equi_Manufacturer
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(Manufacturer);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Equi_Manufacturer = elem->Value;
	}

	//Patient m_Equi_Manufacturer Model
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(ManufacturerModelName);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Equi_Manufacture_Model = elem->Value;
	}

	//Note
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(StudyDescription);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_StudyDes = elem->Value;
	}
}

//***************************************************************************************

void  CTraceyDicom::InputChinseeDicom(wchar_t* filename, CDicomInfo & DicomInfo)
{
	// Create a DCXOBJ
	IDCXOBJPtr obj(__uuidof(DCXOBJ));

	obj->openFile(filename);

	// You need to hold a IDCXELMPtr in order to get the elements
	IDCXELMPtr elem;

	//Patient name
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(patientName);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		//DicomInfo.m_Patient_Name = G_ReadChinese(elem->Value);
		DicomInfo.m_Patient_Name = elem->Value;
	}

	//Patient ID
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(patientID);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Patient_ID = elem->Value;
	}

	//Patient BirthDay
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(PatientBirthDate);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Patient_BirthDate = elem->Value;
	}

	//Patient Sex
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(PatientSex);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Patient_Sex = elem->Value;
	}

	//Patient m_Equi_Manufacturer
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(Manufacturer);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Equi_Manufacturer = elem->Value;
	}

	//Patient m_Equi_Manufacturer Model
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(ManufacturerModelName);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_Equi_Manufacture_Model = elem->Value;
	}

	//Note
	try
	{
		elem = NULL;
		elem = obj->getElementByTag(StudyDescription);
	}
	catch (const _com_error)
	{
	}

	if (elem != NULL)
	{
		DicomInfo.m_StudyDes = elem->Value;
	}
}

//***************************************************************************************
void  CTraceyDicom::ReadDicomImage(char* filename, CDicomInfo &OutDicomInfo)
{
	//// Create a DCXOBJ
	//IDCXOBJPtr obj(__uuidof(DCXOBJ));

	//obj->openFile(filename);

	//// You need to hold a IDCXELMPtr in order to get the elements
	//IDCXELMPtr elem;

	////Patient name
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(patientName);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Patient_Name = elem->Value;
	//}

	////Patient ID
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(patientID);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Patient_ID = elem->Value;
	//}

	////Patient BirthDay
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(PatientBirthDate);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Patient_BirthDate = elem->Value;
	//}

	////Patient Sex
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(PatientSex);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Patient_Sex = elem->Value;
	//}
	//
	////Study Date
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(StudyDate);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Study_Date = elem->Value;
	//}

	////Study Time
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(StudyTime);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Study_Time = elem->Value;
	//}
	//
	////Study ID
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(studyInstanceUID);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Study_ID = elem->Value;
	//}

	////Study description
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(StudyDescription);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_StudyDes = elem->Value;
	//}
	//

	////Sop SopInstanceUID
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(sopInstanceUID);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_SopInstanceUID = elem->Value;
	//}
	//
	////Sop SopClassUid UID
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(sopClassUid);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_SopClassUid = elem->Value;
	//}

	////Modality
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(Modality);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Modality = elem->Value;
	//}

	////Manufacturer
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(Manufacturer);
	//}
	//catch (const _com_error)
	//{
	//}
	//
	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Equi_Manufacturer = elem->Value;
	//}
	//

	////Manufacturer Model Name
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(ManufacturerModelName);
	//}
	//catch (const _com_error)
	//{
	//}

	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Equi_Manufacture_Model = elem->Value;
	//}

	//
	////Equi Institution Model Name
	//try
	//{
	//	elem = NULL;
	//	elem = obj->getElementByTag(InstitutionName);
	//}
	//catch (const _com_error)
	//{
	//}

	//if (elem != NULL)
	//{
	//	OutDicomInfo.m_Equi_Institution_Name = elem->Value;
	//}
	//
	//   //Images
	////Done
}
//***************************************************************************************
//For date filtering you can add Scheduled Procedure Step Start Date(0040, 0002) to the query(you currently don’t
//have this field in the query I think, but you seem to have added it to your Import dialog below, so I’m not
//exactly sure what is going on).
//
//The date format is fixed at YYYYMMDD(it must be 8 and only 8 characters),
//HOWEVER for this field you can also specify a range in the form “YYYYYMMDD - “, or “ - YYYYMMDD ”, or “YYYYMMDD - YYYYMMDD ”
// 6.2.0 new Dicom
//http://dicomiseasy.blogspot.co.il/2012/04/dicom-modality-worklist.htm
BOOL  CTraceyDicom::Query(CString ModalityStr, CString host, unsigned short port, CString CallAETitle, CString CalledTitle, CString PatientID, CString FirstName, CString LastName, CString DateFrom, CString DateTo, int & count)
{
	try
	{

		_bstr_t hCallAETitle(CallAETitle);


		//Build query
		// Fill the query object
		IDCXOBJPtr rp(__uuidof(DCXOBJ));
		IDCXOBJPtr sps(__uuidof(DCXOBJ));
		IDCXELMPtr el(__uuidof(DCXELM));
		IDCXOBJIteratorPtr spsIt(__uuidof(DCXOBJIterator));

		// Build the Scheduled procedure Step (SPS) item
		el->Init(ScheduledStationAETitle);
		el->Value = hCallAETitle;//07202018
		sps->insertElement(el);


		//Schedule Start Time and End Time
		el->Init(ScheduledProcedureStepStartDate);
		//07202018
		CString ParsedDateFrom;
		if (GetStandDate(DateFrom, ParsedDateFrom, "/"))
		{
			CString ParsedDateTo;
			if (GetStandDate(DateTo, ParsedDateTo, "/"))
			{
				_bstr_t hDate(ParsedDateFrom + "-" + ParsedDateTo + " ");
				el->Value = hDate;
			}
		}
		//07202018

		sps->insertElement(el);

		//el->Init(ScheduledProcedureStepEndDate);
		//el->Value = "20180723";
		////////07202018
		////CString ParsedDateTo;
		////if (GetStandDate(DateTo, ParsedDateTo, "/"))
		////{
		////	_bstr_t hEndDate = ParsedDateTo;
		////	el->Value = hEndDate;
		////}
		//////07202018
		//sps->insertElement(el);

		//Search fields
		el->Init(Modality);
		//07202018
		_bstr_t hModality(ModalityStr);
		el->Value = hModality;
		//07202018
		sps->insertElement(el);

		//Following is to search attributs
		el->Init(PatientsName);
		rp->insertElement(el);

		el->Init(patientID);
		rp->insertElement(el);

		//Add more items here
		el->Init(PatientsSex);
		rp->insertElement(el);

		el->Init(StudyDescription);
		rp->insertElement(el);

		el->Init(PatientsBirthDate);
		rp->insertElement(el);

		//Add more items here Done

		/// Add the Requested Procedure attributes that we would like to get
		el->Init(RequestedProcedureID);
		rp->insertElement(el);

		el->Init(RequestedProcedureDescription);
		rp->insertElement(el);

		el->Init(ReferringPhysiciansName);
		rp->insertElement(el);

		el->Init(studyInstanceUID);
		rp->insertElement(el);

		el->Init(AccessionNumber);
		rp->insertElement(el);

		// Ask for SPS description
		el->Init(ScheduledProcedureStepDescription);
		sps->insertElement(el);

		// Ask for SPS ID
		el->Init(ScheduledProcedureStepID);
		sps->insertElement(el);

		// Now we put it as an item to sequence
		spsIt->Insert(sps);

		// and add the sequence Scheduled Procedure Step Sequence to the requested procedure (parent) object
		el->Init(ScheduledProcedureStepSequence);
		el->PutValue(_variant_t((IUnknown*)spsIt, false));
		rp->insertElement(el);

		//Build query Done

		//------------------------------------------------

		//Query
		_bstr_t hhost(host);
		_bstr_t hCalledTitle(CalledTitle);

		IDCXREQPtr req(__uuidof(DCXREQ));
		IDCXOBJIteratorPtr it(__uuidof(DCXOBJIterator));

		//req->OnQueryResponseRecieved += new IDCXREQEvents_OnQueryResponseRecievedEventHandler(OnQueryResponseRecievedAction);
		//HRESULT hr = req->Echo(hCallAETitle, hCalledTitle, hhost, port);

		try
		{
			it = req->Query(hCallAETitle, hCalledTitle, hhost, port, "1.2.840.10008.5.1.4.31" /* The MWL SOP Class UID */, rp);
			//::Info("Query works!");
		}
		catch (const _com_error)
		{
			::Info("Query failed: No DICOM worklist server found.");

			return FALSE;
		}
		//Query Done

		//------------------------------------------------

		//Get results
		count = 0;
		for (; !it->AtEnd(); it->Next())
		{
			IDCXOBJPtr r(__uuidof(DCXOBJ));
			r = it->Get();

			IDCXELMPtr elem;

			elem = NULL;
			elem = r->getElementByTag(Modality);

			if (elem != NULL)
			{
				CString Mstr = elem->Value;

				if (Mstr == ModalityStr || ModalityStr == "")
				{
					BOOL SameName = TRUE;
					BOOL DateRange = TRUE;
					BOOL PatientIDOK = TRUE;

					CString  Patient_Name, DicomPID;

					//Check patient name
					elem = NULL;
					elem = r->GetElement(patientName);
					if (elem != NULL)
					{
						if (FirstName != "" || LastName != "")
						{
							CString  DicomFirstName, DicomLastName, DicomMidName;

							Patient_Name = elem->Value;
							GetFiLaMiName(Patient_Name, DicomFirstName, DicomLastName, DicomMidName);

							if (FirstName != "")//&& FirstName != DicomFirstName)
							{
								CString LowerFirstName = FirstName;
								LowerFirstName.MakeLower();

								CString LowerDicomFirstName = DicomFirstName;
								LowerDicomFirstName.MakeLower();

								int count = LowerDicomFirstName.Find(LowerFirstName, 0);

								if (count == -1)
								{
									SameName = FALSE;
								}
							}

							if (LastName != "")
							{
								CString LowerLastName = LastName;
								LowerLastName.MakeLower();

								CString LowerDicomLastName = DicomLastName;
								LowerDicomLastName.MakeLower();

								int count = LowerDicomLastName.Find(LowerLastName, 0);

								if (count == -1)
								{
									SameName = FALSE;
								}
							}
						}
					}
					//Check patient name Done

					//Check patient id
					elem = NULL;
					elem = r->GetElement(patientID);
					if (elem != NULL)
					{
						DicomPID = elem->Value;

						if (PatientID != "" && DicomPID != PatientID)
						{
							PatientIDOK = FALSE;
						}
					}
					//Check patient id Done

					//if (SameName && DateRange && PatientIDOK)
					if (SameName && DateRange && PatientIDOK)
					{
						BOOL exist = FALSE;

						CDicomInfo OneDicomInfo;

						OneDicomInfo.m_Patient_ID = DicomPID;

						elem = NULL;
						elem = r->getElementByTag(PatientSex);

						if (elem != NULL)
						{
							OneDicomInfo.m_Patient_Sex = elem->Value;
						}

						elem = NULL;
						elem = r->GetElement(StudyDescription);

						if (elem != NULL)
						{
							OneDicomInfo.m_StudyDes = elem->Value;
						}

						elem = NULL;
						elem = r->GetElement(PatientsBirthDate);
						if (elem != NULL)
						{
							CString NewDate;

							if (!GetStandDate(elem->Value, NewDate, "/"))
							{
								GetStandDate(elem->Value, NewDate, "-");
							}

							OneDicomInfo.m_Patient_BirthDate = NewDate;
						}


						elem = NULL;
						elem = r->GetElement(patientName);
						if (elem != NULL)
						{
							CString PreName = elem->Value;

							OneDicomInfo.m_Patient_Name = G_TransformEurope(PreName);
						}

						/*//08212019 Test for 'Study Instance UID'
						elem = NULL;
						elem = r->GetElement(studyInstanceUID);
						if (elem != NULL)
						{
							CString StudyInId = elem->Value;

							OneDicomInfo.m_Study_Instance_ID = G_TransformEurope(StudyInId);
						}
						//08212019 Test for 'Study Instance UID'*/

						//Merge same patients if there is
						for (int i = 0; i < count; i++)
						{
							if ((OneDicomInfo.m_Patient_ID == m_DicomInfo[i].m_Patient_ID) &&
								(OneDicomInfo.m_Patient_Name == m_DicomInfo[i].m_Patient_Name) &&
								(OneDicomInfo.m_Patient_BirthDate == m_DicomInfo[i].m_Patient_BirthDate) &&
								(OneDicomInfo.m_Patient_Sex == m_DicomInfo[i].m_Patient_Sex)
								)
							{
								exist = TRUE;
								break;
							}
						}


						//Merge same patient Done
						if (!exist)
						{
							m_DicomInfo[count] = OneDicomInfo;
							count++;
						}
					}
				}
			}
		}
		//Get results Done

		//------------------------------------------------
	}
	catch (exception ex)
	{
		::Info("Query failed.");

		return FALSE;
	}

	return TRUE;
}

//***************************************************************************************

BOOL CTraceyDicom::GetStandDate(CString OriDate, CString &NewDate, CString NewNote)
{
	NewDate = OriDate;
	CString ProDate = OriDate;

	CString Year, Month, Day,ValStr;
	CStringA ValStrA;
	int     Val;

	int count = ProDate.Find(NewNote, 0);

	if (count != -1)
	{
		if (count == 1)
		{
			int Len = ProDate.GetLength();
			Month = "0" + ProDate.Left(1);

			ProDate = ProDate.Right(Len - 2);

			count = ProDate.Find(NewNote, 0);

			if (count != -1)
			{
				if (count == 1)
				{
					Len = ProDate.GetLength();
					Day = "0" + ProDate.Left(1);

					Year = ProDate.Right(Len - 2);

					NewDate = Year + Month + Day;
				}
				else if (count == 2)
				{
					Len = ProDate.GetLength();
					Day = ProDate.Left(2);

					Year = ProDate.Right(4);

					NewDate = Year + Month + Day;
				}
			}
		}
		else if (count == 2)
		{
			ValStr = ProDate.Left(2);

			ValStrA = ValStr;

			Val = atoi(ValStrA);

			if (Val > 12)
			{
				Day = ValStr;

				int Len = ProDate.GetLength();

				ProDate = ProDate.Right(Len - 3);

				count = ProDate.Find(NewNote, 0);

				if (count == 1)
				{
					Len = ProDate.GetLength();
					Month = "0" + ProDate.Left(1);

					Year = ProDate.Right(4);
					NewDate = Year + Month + Day;
				}
				else if (count == 2)
				{
					Len = ProDate.GetLength();
					Month = ProDate.Left(2);

					Year = ProDate.Right(4);

					NewDate = Year + Month + Day;
				}
			}
			else
			{
				Month = ValStr;

				int Len = ProDate.GetLength();

				ProDate = ProDate.Right(Len - 3);

				count = ProDate.Find(NewNote, 0);

				if (count == 1)
				{
					Len = ProDate.GetLength();
					Day = "0" + ProDate.Left(1);

					Year = ProDate.Right(4);
					NewDate = Year + Month + Day;
				}
				else if (count == 2)
				{
					Len = ProDate.GetLength();
					Day = ProDate.Left(2);

					Year = ProDate.Right(4);
					NewDate = Year + Month + Day;
				}
			}
		}
		else
		{
			NewDate.Replace(NewNote, _T(""));
		}

		return TRUE;
	}
	else return FALSE;
}
//********************************************************************************************

//6.2.0 For Dicom
/*
Rev. John Robert Quincy Adams, B.A. M.Div.

"Adams^John Robert Quincy^^Rev.^B.A. M.Div."

[One family name; three given names; no middle name; one prefix; two suffixes.]

Susan Morrison-Jones, Ph.D., Chief Executive Officer

"Morrison-Jones^Susan^^^Ph.D., Chief Executive Officer"

[Two family names; one given name; no middle name; no prefix; two suffixes.]

John Doe

"Doe^John"

[One family name; one given name; no middle name, prefix, or suffix. Delimiters have been omitted for the three trailing null components.]

(for examples of the encoding of Person Names using multi-byte character sets see Annex H)

"Smith^Fluffy"

[A cat, rather than a human, whose responsible party family name is Smith, and whose own name is Fluffy]

"ABC Farms^Running on Water"

[A horse whose responsible organization is named ABC Farms, and whose name is "Running On Water"]
*/
void  CTraceyDicom::GetFiLaMiName(CString FullName, CString & FirstName, CString & LastName, CString & MiddleName)
{
	CString s[4];
	int Len = FullName.GetLength();

	char *pValue = new char[Len];

	G_As = FullName;
	strncpy(pValue, G_As, Len);

	int t = 0;

	for (int i = 0; i < 4; i++)
	{
		s[i] = "";
	}

	//0x41 A; 0x42 B; 0x43 C; 44 D; 45 E; 46 F; 47 G; 48 H; 49 I; 4A J; 4B K; 4C L;
	//0x4D M; 0x4E N; 0x4F O; 50 P; 51 Q; 52 R; 53 S; 54 T; 55 U; 56 V; 57 W; 58 X;
	//0x59 Y; 0x5A Z;
	for (int i = 0; i < Len; i++)
	{
		char c = *(pValue + i);

		if (c == '^')
		{
			t++;
		}
		else
		{
			s[t] += c;
		}
	}



	for (int i = 0; i <= t; i++)
	{
		s[i].Trim();
	}

	LastName = s[0];
	FirstName = s[1];
	MiddleName = s[2];
}
//6.2.0 For Dicom

//***************************************************************************************
