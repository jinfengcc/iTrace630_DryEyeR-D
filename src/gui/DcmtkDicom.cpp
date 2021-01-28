/****************************************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          06-09-2017
Description:   Create a  Dicom class (use DCMTK) for no combo activation (save screen as dicom)
*****************************************************************************************/

#include "StdAfx.h"
#include "DcmtkDicom.h"

#undef UNICODE
#include <dcmtk\dcmdata\dctk.h>
#include <DCMTK\dcmimgle\dcmimage.h>

//***************************************************************************************

CDcmtkDicom::CDcmtkDicom()
{
}

//***************************************************************************************

CDcmtkDicom::~CDcmtkDicom()
{
}

//***************************************************************************************

BOOL CDcmtkDicom::SaveDcmtkDicomFile(CString fileName, CDicomInfo DicomInfo, BYTE* pData, int width, int height)
{
	char uid[100];
	DcmFileFormat fileformat;
	DcmMetaInfo *metainfo = fileformat.getMetaInfo();
	DcmDataset  *dataset = fileformat.getDataset();

	// meta group  
	metainfo->putAndInsertString(DCM_FileMetaInformationVersion, "Tracey Dicom file");
	//metainfo->putAndInsertString(DCM_MediaStorageSOPClassUID, UID_HardcopyColorImageStorage);
	metainfo->putAndInsertString(DCM_MediaStorageSOPInstanceUID, dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT));
	metainfo->putAndInsertString(DCM_TransferSyntaxUID, UID_LittleEndianExplicitTransferSyntax);
	metainfo->putAndInsertString(DCM_ImplementationClassUID, "999.999");
	// Done

	// New    
	//DicomInfo.m_Study_Instance_ID = "000.999.2.19941105.112113";
	//dataset->putAndInsertString(DCM_SeriesInstanceUID, "999.999.2.19941105.112000.3");
	//dataset->putAndInsertString(DCM_SeriesNumber, "3");
	//dataset->putAndInsertString(DCM_AccessionNumber, "");

	// identifying group
//	dataset->putAndInsertString(DCM_SOPClassUID, UID_HardcopyColorImageStorage);
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT));
	// Done

	auto DCM_OtherPatientIDs = DCM_RETIRED_OtherPatientIDs;

	// Patient
	G_As = DicomInfo.m_Patient_Name;
	dataset->putAndInsertString(DCM_PatientName, G_As); 
	G_As = DicomInfo.m_Patient_ID;
	dataset->putAndInsertString(DCM_PatientID, G_As);
	G_As = DicomInfo.m_Patient_Other_ID;
	dataset->putAndInsertString(DCM_OtherPatientIDs, G_As);
	G_As = DicomInfo.m_Patient_BirthDate;
	dataset->putAndInsertString(DCM_PatientBirthDate, G_As);
	G_As = DicomInfo.m_Patient_Sex;
	dataset->putAndInsertString(DCM_PatientSex, G_As);
	// Done
	

	// Study
	G_As = DicomInfo.m_Study_Date;
	dataset->putAndInsertString(DCM_StudyDate, G_As);
	G_As = DicomInfo.m_Study_Time;
	dataset->putAndInsertString(DCM_StudyTime, G_As);
	G_As = DicomInfo.m_StudyDes;
	dataset->putAndInsertString(DCM_StudyDescription, G_As);
	G_As = DicomInfo.m_Study_Instance_ID;
	dataset->putAndInsertString(DCM_StudyInstanceUID, G_As);
	G_As = DicomInfo.m_Study_ID;
	dataset->putAndInsertString(DCM_StudyID, G_As);
	G_As = DicomInfo.m_Physician_Name;
//	dataset->putAndInsertString(DCM_ReferringPhysiciansName, G_As);
	// Done
		
	//Equipment
	G_As = DicomInfo.m_Equi_Manufacturer;
	dataset->putAndInsertString(DCM_Manufacturer, G_As);
	G_As = DicomInfo.m_Equi_Manufacture_Model;
	dataset->putAndInsertString(DCM_ManufacturerModelName, G_As);
	G_As = DicomInfo.m_Equi_Serial_Num;
	dataset->putAndInsertString(DCM_DeviceSerialNumber, G_As);
	G_As = DicomInfo.m_Software_Versions;
	dataset->putAndInsertString(DCM_SoftwareVersions, G_As);
	//Done

	//SC Equipment
	G_As = DicomInfo.m_Modality;
	dataset->putAndInsertString(DCM_Modality, G_As);
	//Done

	//6.2.0 Add 9 other items
	G_As = DicomInfo.m_Conversion_Type;
	dataset->putAndInsertString(DCM_ConversionType, G_As);

	G_As = DicomInfo.m_Physician_Name;
	dataset->putAndInsertString(DCM_PerformingPhysicianName, G_As);

	G_As = DicomInfo.m_Study_ID;
	dataset->putAndInsertString(DCM_StudyID, G_As);

	G_As = DicomInfo.m_Series_Instance_UID;
	dataset->putAndInsertString(DCM_SeriesInstanceUID, G_As);

	G_As = DicomInfo.m_Series_Num;
	dataset->putAndInsertString(DCM_SeriesNumber, G_As);

	G_As = DicomInfo.m_Acess_Num;
	dataset->putAndInsertString(DCM_AccessionNumber, G_As);

	G_As = DicomInfo.m_Laterality;
	dataset->putAndInsertString(DCM_Laterality, G_As);

	G_As = DicomInfo.m_Instance_Num;
	dataset->putAndInsertString(DCM_InstanceNumber, G_As);

	G_As = DicomInfo.m_Patient_Orientation;
	dataset->putAndInsertString(DCM_PatientOrientation, G_As);
	//6.2.0 Add 9 other items Done

	//Image Pixel
	dataset->putAndInsertString(DCM_InstanceNumber, "1");
	dataset->putAndInsertString(DCM_PhotometricInterpretation, "RGB");
	dataset->putAndInsertUint16(DCM_SamplesPerPixel, 3);
	dataset->putAndInsertUint16(DCM_BitsAllocated, 8);
	dataset->putAndInsertUint16(DCM_BitsStored, 8);
	dataset->putAndInsertUint16(DCM_HighBit, 7);
	dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
	dataset->putAndInsertUint16(DCM_PlanarConfiguration, 0);
	dataset->putAndInsertString(DCM_PixelAspectRatio, "4\\3");
	dataset->putAndInsertUint16(DCM_Rows, height);
	dataset->putAndInsertUint16(DCM_Columns, width);
		
	dataset->putAndInsertUint8Array(DCM_PixelData, pData, width *height * 3);
	delete[] pData;
	//Done

	G_As = fileName;
	OFCondition status = fileformat.saveFile(G_As.GetString(), EXS_LittleEndianImplicit, EET_UndefinedLength, EGL_withoutGL);
	if (status.bad())
	{
		::Error("Sorry, the saveing of  the DICOM format is failed");
		return FALSE;
	}

	return TRUE;
}

//***************************************************************************************



