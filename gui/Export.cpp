//***************************************************************************************

#include "StdAfx.h"
#include "Export.h"
#include "Settings.h"
#include "Data.h"
#include "Registry.h"
#include "FolderDlg.h"
#include "INIParser.h"//6.2.0 ini registration

#include "TraceyDicom.h"//6.2.0 For Dicom Toolkit
#include "DcmtkDicom.h"//6.2.0 For Dicom Toolkit

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void SafeInteRelease(int **ppT, int Row)
{
	if (ppT != NULL)
	{
		for (int i = 0; i<Row; i++)
		{
			if (ppT[i] != NULL)
			{
				delete[](ppT[i]);
				ppT[i] = NULL;
			}
		}
		delete[](ppT);
		ppT = NULL;
	}
}

//****************************************************************************************

void ExportScreen(CExam* pExam1, CExam* pExam2, const Matrix<uchar>& Mem, CDicomInfo DicomInfo, uchar* RGBData, int w, int h)
{
	CPatient* pPatient = ::DB.GetPatient(pExam1->m_Header.m_PatientID);

	if (!pPatient)
	{
		::Error("No patient associated with the exam.");
		return;
	}

	CString s;

	CExam* pExams[2] = { pExam1, pExam2 };

	CString ExportFolderName;
	CString PatientFolderName;
	CString ExamFolderName;
	CString ScreenFolderName;

	CINIParser IniPar;
	ExportFolderName = IniPar.GetStrKey(IniSection, "Export Folder", IniPath);//6.2.0 ini registration  

	CString sPatientID;
	::GUIDToStr(pPatient->m_PatientID, sPatientID);

	s = pPatient->m_LastName + " " + pPatient->m_FirstName + " " + sPatientID;
	s.Replace(_T("\\"), _T("_"));
	s.Replace(_T("/"), _T("_"));
	s.Replace(_T(":"), _T("_"));
	s.Replace(_T("*"), _T("_"));
	s.Replace(_T("?"), _T("_"));
	s.Replace(_T("\""), _T("_"));
	s.Replace(_T("<"), _T("_"));
	s.Replace(_T(">"), _T("_"));
	s.Replace(_T("|"), _T("_"));
	s.Replace(_T(","), _T("_"));

	PatientFolderName = ExportFolderName + "\\" + s;

	::CreateDirectory(PatientFolderName, NULL);

	ExamFolderName = PatientFolderName;

	CExamHeader* pHeader = &pExams[0]->m_Header;
	s.Format(_T("\\%s_%s_%i%02i%02i_%02i%02i%02i"),
		pHeader->IsOD() ? _T("OD") : _T("OS"),
		pHeader->m_Type == EXAM_TYPE_WF ? _T("WF") : _T("CT"),
		pHeader->m_Year, pHeader->m_Month, pHeader->m_Day,
		pHeader->m_Hour, pHeader->m_Minute, pHeader->m_Second);
	ExamFolderName += s;

	if (pExams[1])
	{
		CExamHeader* pHeader = &pExams[1]->m_Header;
		s.Format(_T(" & %s_%s_%i%02i%02i_%02i%02i%02i"),
			pHeader->IsOD() ? _T("OD") : _T("OS"),
			pHeader->m_Type == EXAM_TYPE_WF ? _T("WF") : _T("CT"),
			pHeader->m_Year, pHeader->m_Month, pHeader->m_Day,
			pHeader->m_Hour, pHeader->m_Minute, pHeader->m_Second);
		ExamFolderName += s;
	}

	::CreateDirectory(ExamFolderName, NULL);

	SYSTEMTIME Time;
	::GetLocalTime(&Time);
	s.Format(_T("\\%i%02i%02i_%02i%02i%02i"), Time.wYear, Time.wMonth, Time.wDay, Time.wHour, Time.wMinute, Time.wSecond);

	ScreenFolderName = ExamFolderName + s;

	::CreateDirectory(ScreenFolderName, NULL);

	CStringA Astr(ScreenFolderName + "\\Patient.xml");
	FILE* pFile = fopen(Astr, "wt");
	if (pFile)
	{
		G_As = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
		fprintf(pFile, G_As);

		G_As = "<Patient>\n";
		fprintf(pFile, G_As);

		G_As = sPatientID;//6.2.1
		fprintf(pFile, "<GUID>%s</GUID>\n", G_As);
		G_As = pPatient->m_CustomID;//6.2.1
		fprintf(pFile, "<ID>%s</ID>\n", G_As);

		G_As = pPatient->m_FirstName;//6.2.1
		fprintf(pFile, "<FirstName>%s</FirstName>\n", G_As);
		G_As = pPatient->m_MiddleName;//6.2.1
		fprintf(pFile, "<MiddleName>%s</MiddleName>\n", G_As);
		G_As = pPatient->m_LastName;//6.2.1
		fprintf(pFile, "<LastName>%s</LastName>\n", G_As);
		fprintf(pFile, "<BirthDate>%4i-%02i-%02i</BirthDate>\n",
			pPatient->m_BirthYear, pPatient->m_BirthMonth, pPatient->m_BirthDay);
		fprintf(pFile, "<Sex>%s</Sex>\n",
			pPatient->m_Sex == SEX_MALE ? "Male" : pPatient->m_Sex == SEX_FEMALE ? "Female" : "Not specified");
		fprintf(pFile, "<Address>\n");
		G_As = pPatient->m_Address.m_Line1;//6.2.1
		fprintf(pFile, "<Line1>%s</Line1>\n", G_As);
		G_As = pPatient->m_Address.m_Line2;//6.2.1
		fprintf(pFile, "<Line2>%s</Line2>\n", G_As);
		G_As = pPatient->m_Address.m_City;//6.2.1
		fprintf(pFile, "<City>%s</City>\n", G_As);
		G_As = pPatient->m_Address.m_State;//6.2.1
		fprintf(pFile, "<State>%s</State>\n", G_As);
		G_As = pPatient->m_Address.m_ZIP;//6.2.1
		fprintf(pFile, "<ZIP>%s</ZIP>\n", G_As);
		G_As = pPatient->m_Address.m_Country;//6.2.1
		fprintf(pFile, "<Country>%s</Country>\n", G_As);
		fprintf(pFile, "</Address>\n");
		G_As = pPatient->m_WorkPhone;//6.2.1
		fprintf(pFile, "<WorkPhone>%s</WorkPhone>\n", G_As);
		G_As = pPatient->m_HomePhone;//6.2.1
		fprintf(pFile, "<HomePhone>%s</HomePhone>\n", G_As);
		G_As = pPatient->m_EMail;//6.2.1
		fprintf(pFile, "<EMail>%s</EMail>\n", G_As);
		G_As = pPatient->m_Note;//6.2.1
		fprintf(pFile, "<Note>%s</Note>\n", G_As);

		// group
		CGroup* pGroup = ::DB.GetGroup(pPatient->m_GroupID);
		CString sGroupID;

		if (pGroup)
		{
			::GUIDToStr(pPatient->m_GroupID, sGroupID);
		}

		fprintf(pFile, "<Group>\n");
		G_As = sGroupID;//6.2.1
		fprintf(pFile, "<GUID>%s</GUID>\n", G_As);

		if (pGroup)
		{
			G_As = pGroup->m_Name;//6.2.1
		}
		else
		{
			G_As = "";
		}

		fprintf(pFile, "<Name>%s</Name>\n", pGroup ? G_As : "");
		fprintf(pFile, "</Group>\n");

		fprintf(pFile, "</Patient>\n");

		fclose(pFile);
	}
	else
	{
		::Error("Failed to create patient's file.");
		//return;
	}

	for (int e = 0; e < 2; e++)
	{

		CExam* pExam = pExams[e]; if (!pExam) continue;

		CExamHeader* pHeader = &pExam->m_Header;

		s = ScreenFolderName + "\\" + (pExam->m_Header.m_Type == EXAM_TYPE_WF ? "WF" : "CT") + "Exam";

		if (pExams[0] && pExams[1])
		{
			if (pExams[0]->m_Header.m_Type == pExams[1]->m_Header.m_Type)
			{
				CString s1;
				s1.Format(_T("%i"), e + 1);
				s += s1;
			}
		}

		s += ".xml";

		G_As = s;
		pFile = fopen(G_As, "wt");

		if (pFile)
		{
			fprintf(pFile, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
			fprintf(pFile, "<Exam>\n");

			::GUIDToStr(pHeader->m_ExamID, s);
			G_As = s;//6.2.1
			fprintf(pFile, "<GUID>%s</GUID>\n", G_As);

			fprintf(pFile, "<Calibration>\n");
			::GUIDToStr(pHeader->m_CalibrationID, s);

			G_As = s;//6.2.1
			fprintf(pFile, "<GUID>%s</GUID>\n", G_As);
			fprintf(pFile, "</Calibration>\n");

			G_As = pHeader->Type();//6.2.1
			fprintf(pFile, "<Type>%s</Type>\n", G_As);

			G_As = pHeader->Mode();//6.2.1
			fprintf(pFile, "<Mode>%s</Mode>\n", G_As);

			fprintf(pFile, "<Date>%4i-%02i-%02i</Date>\n", pHeader->m_Year, pHeader->m_Month, pHeader->m_Day);
			fprintf(pFile, "<Time>%02i:%02i:%02i</Time>\n", pHeader->m_Hour, pHeader->m_Minute, pHeader->m_Second);

			G_As = pHeader->Eye();//6.2.1
			fprintf(pFile, "<Eye>%s</Eye>\n", G_As);

			G_As = pHeader->Preop();//6.2.1
			fprintf(pFile, "<Preop>%s</Preop>\n", G_As);

			G_As = pHeader->SoftwareVersion();//6.2.1
			fprintf(pFile, "<SoftwareVersion>%s</SoftwareVersion>\n", G_As);

			// clinic
			CClinic* pClinic = ::DB.GetClinic(pHeader->m_ClinicID);
			CString sClinicID;
			if (pClinic) ::GUIDToStr(pClinic->m_ClinicID, sClinicID);
			fprintf(pFile, "<Clinic>\n");

			G_As = sClinicID;//6.2.1
			fprintf(pFile, "<GUID>%s</GUID>\n", G_As);

			if (pClinic) s = pClinic->m_Name; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Name>%s</Name>\n", G_As);

			fprintf(pFile, "<Address>\n");

			if (pClinic) s = pClinic->m_Address.m_Line1; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Line1>%s</Line1>\n", G_As);

			if (pClinic) s = pClinic->m_Address.m_Line2; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Line2>%s</Line2>\n", G_As);

			if (pClinic) s = pClinic->m_Address.m_City; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<City>%s</City>\n", G_As);

			if (pClinic) s = pClinic->m_Address.m_State; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<State>%s</State>\n", G_As);

			if (pClinic) s = pClinic->m_Address.m_ZIP; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<ZIP>%s</ZIP>\n", G_As);

			if (pClinic) s = pClinic->m_Address.m_Country; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Country>%s</Country>\n", G_As);

			fprintf(pFile, "</Address>\n");
			fprintf(pFile, "</Clinic>\n");

			// physician
			CPhysician* pPhysician = ::DB.GetPhysician(pHeader->m_PhysicianID);
			CString sPhysicianID;

			if (pPhysician) ::GUIDToStr(pPhysician->m_PhysicianID, sPhysicianID);
			fprintf(pFile, "<Physician>\n");
			G_As = sPhysicianID;//6.2.1
			fprintf(pFile, "<GUID>%s</GUID>\n", G_As);

			if (pPhysician) s = pPhysician->m_FirstName; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<FirstName>%s</FirstName>\n", G_As);

			if (pPhysician) s = pPhysician->m_LastName; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<LastName>%s</LastName>\n", G_As);

			fprintf(pFile, "</Physician>\n");

			// operator
			COperator* pOperator = ::DB.GetOperator(pHeader->m_OperatorID);
			CString sOperatorID;

			if (pOperator) ::GUIDToStr(pOperator->m_OperatorID, sOperatorID);
			fprintf(pFile, "<Operator>\n");
			G_As = sOperatorID;//6.2.1
			fprintf(pFile, "<GUID>%s</GUID>\n", G_As);

			if (pOperator) s = pOperator->m_FirstName; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<FirstName>%s</FirstName>\n", G_As);

			if (pOperator) s = pOperator->m_LastName; else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<LastName>%s</LastName>\n", G_As);

			fprintf(pFile, "</Operator>\n");

			// note
			G_As = pHeader->m_Note;//6.2.1
			fprintf(pFile, "<Note>%s</Note>\n", G_As);

			//Coordinates of Center of image	
			fprintf(pFile, "<ImageCenter>\n");

			if (pHeader->m_Type == EXAM_TYPE_WF && pExam->m_Image.m_ve_ok)
			{
				real x = -1 * 0.001 * pExam->m_Image.m_ve_x_um;
				real y = -1 * 0.001 * pExam->m_Image.m_ve_y_um;

				s.Format(_T("%.10f"), x);
				G_As = s;//6.2.1
				fprintf(pFile, "<x_coordinate>%s</x_coordinate>\n", G_As);

				s.Format(_T("%.10f"), y);
				G_As = s;//6.2.1
				fprintf(pFile, "<y_coordinate>%s</y_coordinate>\n", G_As);

				real dis = sqrt(x*x + y*y);

				s.Format(_T("%.10f"), dis);
				G_As = s;//6.2.1
				fprintf(pFile, "<Distance_mm>%s</Distance_mm>\n", G_As);

				int Angle = intRound(_180_Pi * angle(-1 * pExam->m_Image.m_ve_y_um, -1 * pExam->m_Image.m_ve_x_um)) % 360;
				s.Format(_T("%i"), Angle);
				G_As = s;//6.2.1
				fprintf(pFile, "<Angle_deg>%s</Angle_deg>\n", G_As);
			}

			fprintf(pFile, "</ImageCenter>\n");
			//Coordinates of Center of image Done

			// angle kappa
			fprintf(pFile, "<AngleKappa>\n");

			if (pHeader->m_AngleKappaOK) s.Format(_T("%.10f"), 0.001 * pHeader->m_AngleKappaRUm); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Distance_mm>%s</Distance_mm>\n", G_As);

			if (pHeader->m_AngleKappaOK) s.Format(_T("%i"), pHeader->m_AngleKappaADg); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Angle_deg>%s</Angle_deg>\n", G_As);

			fprintf(pFile, "</AngleKappa>\n");

			// angle alpha
			fprintf(pFile, "<AngleAlpha>\n");

			if (pHeader->m_AngleAlphaOK) s.Format(_T("%.10f"), 0.001 * pHeader->m_AngleAlphaRUm); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Distance_mm>%s</Distance_mm>\n", G_As);

			if (pHeader->m_AngleAlphaOK) s.Format(_T("%i"), pHeader->m_AngleAlphaADg); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Angle_deg>%s</Angle_deg>\n", G_As);

			fprintf(pFile, "</AngleAlpha>\n");

			CEyeImage* pImage = &pExam->m_Image;

			// limbus
			fprintf(pFile, "<Limbus>\n");
			fprintf(pFile, "<Center>\n");

			if (pImage->m_li_ok) s.Format(_T("%.10f"), 0.001 * pImage->m_li_x0_um); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<X_mm>%s</X_mm>\n", G_As);

			if (pImage->m_li_ok) s.Format(_T("%.10f"), 0.001 * pImage->m_li_y0_um); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Y_mm>%s</Y_mm>\n", G_As);

			fprintf(pFile, "</Center>\n");

			if (pImage->m_li_ok) s.Format(_T("%.10f"), 0.002 * pImage->m_li_r_mean_um); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Diameter_mm>%s</Diameter_mm>\n", G_As);

			fprintf(pFile, "</Limbus>\n");

			// pupil
			fprintf(pFile, "<Pupil>\n");
			fprintf(pFile, "<Center>\n");

			if (pImage->m_pu_ok) s.Format(_T("%.10f"), 0.001 * pImage->m_pu_x0_um); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<X_mm>%s</X_mm>\n", G_As);

			if (pImage->m_pu_ok) s.Format(_T("%.10f"), 0.001 * pImage->m_pu_y0_um); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Y_mm>%s</Y_mm>\n", G_As);

			fprintf(pFile, "</Center>\n");

			if (pImage->m_pu_ok) s.Format(_T("%.10f"), 0.002 * pImage->m_pu_r_mean_um); else s.Empty();
			G_As = s;//6.2.1
			fprintf(pFile, "<Diameter_mm>%s</Diameter_mm>\n", G_As);

			fprintf(pFile, "</Pupil>\n");


			if (pExam->m_Header.m_Type == EXAM_TYPE_WF)
			{
				CWFExam* pWFExam = (CWFExam*)pExam;

				s.Format(_T("%.2f"), 0.001 * pWFExam->m_ScanDiameter);
				G_As = s;//6.2.1
				fprintf(pFile, "<ScanDiameter_mm>%s</ScanDiameter_mm>\n", G_As);

				BOOL OK[5] = { 0, 0, 0, 0, 0 };
				CZernikeSurface Surface[5];
				real ZoneRUm[5] =
				{
					2000.0,
					0.5 * ::Settings.m_ZoneDiameterUm[0],
					0.5 * ::Settings.m_ZoneDiameterUm[1],
					0.5 * ::Settings.m_ZoneDiameterUm[2],
					pWFExam->m_WfSurface.m_r_max_um
				};
				real SphEq[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
				real Sph[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
				real Cyl[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
				int Axis[5] = { 0, 0, 0, 0, 0 };

				// Tracey refraction
				Surface[0] = pWFExam->m_WfSurface;
				Surface[0].ChangeRMaxUm(ZoneRUm[0]);
				CWFExam::GetSpheqSphCylAxis(Surface[0], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
					SphEq[0], Sph[0], Cyl[0], Axis[0]);
				Sph[0] = round8(Sph[0]);
				Cyl[0] = round8(Cyl[0]);
				OK[0] = TRUE;

				// Multizonal refractions
				for (int i = 1; i < 5; i++)
				{
					if (ZoneRUm[i] <= pWFExam->m_WfSurface.m_r_max_um)
					{
						Surface[i] = pWFExam->m_WfSurface;
						Surface[i].ChangeRMaxUm(ZoneRUm[i]);
						CWFExam::GetSpheqSphCylAxis(Surface[i], ::Settings.m_VertexDistanceUm, ::Settings.m_PositiveCylinder,
							SphEq[i], Sph[i], Cyl[i], Axis[i]);
						OK[i] = TRUE;
					}
				}
							

				fprintf(pFile, "<Refractions>\n");
				for (int i = 0; i < 5; i++)
				{
					CString s1;
					if (i == 0) s1 = "TraceyRefraction";
					else s1.Format(_T("Refraction%i"), i);

					G_As = s1;//6.2.1
					fprintf(pFile, "<%s>\n", G_As);

					if (i > 0)
					{
						s.Format(_T("%.2f"), 0.002 * ZoneRUm[i]);
						G_As = s;//6.2.1
						fprintf(pFile, "<ZoneDiameter_mm>%s</ZoneDiameter_mm>\n", G_As);
					}

					if (OK[i]) s.Format(_T("%.2f"), Sph[i]); else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<Sph_dpt>%s</Sph_dpt>\n", G_As);

					if (OK[i]) s.Format(_T("%.2f"), Cyl[i]); else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<Cyl_dpt>%s</Cyl_dpt>\n", G_As);

					if (OK[i]) s.Format(_T("%i"), Axis[i]); else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<Axis_deg>%s</Axis_deg>\n", G_As);

					if (OK[i]) s.Format(_T("%.2f"), 0.001 * ::Settings.m_VertexDistanceUm); else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<VertexDistance_mm>%s</VertexDistance_mm>\n", G_As);

					G_As = s1;//6.2.1
					fprintf(pFile, "</%s>\n", G_As);
				}
				fprintf(pFile, "</Refractions>\n");

				// Zernike
				fprintf(pFile, "<ZernikeSets>\n");
				for (int i = 1; i < 5; i++)
				{
					fprintf(pFile, "<ZernikeSet%i>\n", i);

					s.Format(_T("%.2f"), 0.002 * ZoneRUm[i]);
					G_As = s;//6.2.1
					fprintf(pFile, "<ZoneDiameter_mm>%s</ZoneDiameter_mm>\n", G_As);

					fprintf(pFile, "<Coefficients>\n");

					for (int z = 0; z < ZERNIKE_NUM_MODES; z++)
					{
						if (OK[i]) s.Format(_T("%.10f"), Surface[i].GetCUm(z)); else s.Empty();
						G_As = s;//6.2.1
						fprintf(pFile, "<C%i_um>%s</C%i_um>\n", z, G_As, z);
					}
					fprintf(pFile, "</Coefficients>\n");

					fprintf(pFile, "</ZernikeSet%i>\n", i);
				}

				fprintf(pFile, "</ZernikeSets>\n");

				// points
				if (::Settings.m_IncludeLADs)
				{
					fprintf(pFile, "<ScanPoints>\n");
					CScanPoint* pPoint = pWFExam->m_Points.MoveFirst();
					int p = 0;
					while (pPoint) {
						fprintf(pFile, "<ScanPoint%i>\n", p + 1);

						// good or bad
						fprintf(pFile, "<Good>%s</Good>\n", pPoint->m_Bad ? "No" : "Yes");

						// coordinates
						fprintf(pFile, "<Radius_mm>%.10f</Radius_mm>\n", 0.001 * pPoint->m_r_um);
						fprintf(pFile, "<Angle_rad>%.10f</Angle_rad>\n", pPoint->m_a_rd);

						// transversal aberrations
						fprintf(pFile, "<DX_um>%.10f</DX_um>\n", pPoint->m_dx_um);
						fprintf(pFile, "<DY_um>%.10f</DY_um>\n", pPoint->m_dy_um);

						// linear arrays data
						for (int l = 0; l < 2; l++)
						{
							CLAD* pLAD = l == 0 ? &pPoint->m_LADX : &pPoint->m_LADY;

							CString s1(l == 0 ? "DetectorX" : "DetectorY");
							G_As = s1;//6.2.1
							fprintf(pFile, "<%s>\n", G_As);

							fprintf(pFile, "<MaxValue>%i</MaxValue>\n", pLAD->m_Max);
							fprintf(pFile, "<MaxPosition>%i</MaxPosition>\n", pLAD->m_MaxPos);
							fprintf(pFile, "<CentroidPosition>%.10f</CentroidPosition>\n", pLAD->m_CentroidPos);
							fprintf(pFile, "<Baseline>%.10f</Baseline>\n", pLAD->m_Baseline);
							fprintf(pFile, "<AreaUnderCurveAboveBaseline>%.10f</AreaUnderCurveAboveBaseline>\n", pLAD->m_Area);
							fprintf(pFile, "<Curve>");

							for (int k = 0; k < 512; k++)
							{
								fprintf(pFile, "%u ", pLAD->m_Signal[k]);
							}

							fprintf(pFile, "</Curve>\n");
							fprintf(pFile, "</%s>\n", G_As);
						}
						fprintf(pFile, "</ScanPoint%i>\n", p + 1);

						pPoint = pWFExam->m_Points.MoveNext();
						p++;
					}
					fprintf(pFile, "</ScanPoints>\n");
				}
			}
			else if (pExam->m_Header.m_Type == EXAM_TYPE_CT)
			{
				CCTExam* pCTExam = (CCTExam*)pExam;

				s.Format(_T("%.10f"), pCTExam->m_ax0_dp);
				G_As = s;//6.2.1
				fprintf(pFile, "<CentralPower_dpt>%s</CentralPower_dpt>\n", G_As);

				s.Format(_T("%.10f"), 0.001 * pCTExam->m_ax0_um);
				G_As = s;//6.2.1
				fprintf(pFile, "<CentralRadius_mm>%s</CentralRadius_mm>\n", G_As);

				fprintf(pFile, "<BestFitSphere>\n");

				s.Format(_T("%.10f"), 0.001 * pCTExam->m_HtSpSurface.m_ax0_um);
				G_As = s;//6.2.1
				fprintf(pFile, "<Radius_mm>%s</Radius_mm>\n", G_As);

				fprintf(pFile, "</BestFitSphere>\n");

				fprintf(pFile, "<BestFitConicoid>\n");

				s.Format(_T("%.10f"), 0.001 * pCTExam->m_HtCnSurface.m_ax0_um);
				G_As = s;//6.2.1
				fprintf(pFile, "<CentralRadiusOfCurvature_mm>%s</CentralRadiusOfCurvature_mm>\n", G_As);

				s.Format(_T("%.10f"), pCTExam->m_HtCnSurface.GetEccentricity());
				G_As = s;//6.2.1
				fprintf(pFile, "<Eccentricity>%s</Eccentricity>\n", G_As);

				s.Format(_T("%.10f"), pCTExam->m_HtCnSurface.m_q);
				G_As = s;//6.2.1
				fprintf(pFile, "<Asphericity>%s</Asphericity>\n", G_As);

				fprintf(pFile, "</BestFitConicoid>\n");

				fprintf(pFile, "<RefractivePowerAtDLessOrEqual3Mm>\n");

				fprintf(pFile, "<Steep>\n");

				if (pCTExam->m_rf_ok) s.Format(_T("%.10f"), pCTExam->m_rf_st_dp);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

				if (pCTExam->m_rf_ok) s.Format(_T("%i"), pCTExam->m_rf_st_dg);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Degrees>%s</Degrees>\n", G_As);

				fprintf(pFile, "</Steep>\n");

				fprintf(pFile, "<Flat>\n");

				if (pCTExam->m_rf_ok) s.Format(_T("%.10f"), pCTExam->m_rf_fl_dp);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

				if (pCTExam->m_rf_ok) s.Format(_T("%i"), pCTExam->m_rf_fl_dg);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Degrees>%s</Degrees>\n", G_As);

				fprintf(pFile, "</Flat>\n");

				fprintf(pFile, "<Effective>\n");

				if (pCTExam->m_rf_ok) s.Format(_T("%.10f"), pCTExam->m_rf_av_dp);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

				fprintf(pFile, "</Effective>\n");

				fprintf(pFile, "<Astigmatism>\n");

				if (pCTExam->m_rf_ok) s.Format(_T("%.10f"), pCTExam->m_rf_df_dp);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

				if (pCTExam->m_rf_ok) s.Format(_T("%i"), pCTExam->m_rf_st_dg);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Degrees>%s</Degrees>\n", G_As);

				fprintf(pFile, "</Astigmatism>\n");

				fprintf(pFile, "</RefractivePowerAtDLessOrEqual3Mm>\n");

				fprintf(pFile, "<SimKAtDEqual3Mm>\n");

				fprintf(pFile, "<Steep>\n");

				if (pCTExam->m_sk_ok) s.Format(_T("%.10f"), pCTExam->m_sk_st_dp);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

				if (pCTExam->m_sk_ok) s.Format(_T("%.10f"), 0.001 * pCTExam->m_sk_st_um);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Millimeters>%s</Millimeters>\n", G_As);

				if (pCTExam->m_sk_ok) s.Format(_T("%i"), pCTExam->m_sk_st_dg);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Degrees>%s</Degrees>\n", G_As);

				fprintf(pFile, "</Steep>\n");

				fprintf(pFile, "<Flat>\n");

				if (pCTExam->m_sk_ok) s.Format(_T("%.10f"), pCTExam->m_sk_fl_dp);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

				if (pCTExam->m_sk_ok) s.Format(_T("%.10f"), 0.001 * pCTExam->m_sk_fl_um);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Millimeters>%s</Millimeters>\n", G_As);

				if (pCTExam->m_sk_ok) s.Format(_T("%i"), pCTExam->m_sk_fl_dg);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Degrees>%s</Degrees>\n", G_As);

				fprintf(pFile, "</Flat>\n");

				fprintf(pFile, "<Average>\n");

				if (pCTExam->m_sk_ok) s.Format(_T("%.10f"), pCTExam->m_sk_av_dp);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

				if (pCTExam->m_sk_ok) s.Format(_T("%.10f"), 0.001 * pCTExam->m_sk_av_um);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Millimeters>%s</Millimeters>\n", G_As);

				fprintf(pFile, "</Average>\n");

				fprintf(pFile, "<Delta>\n");

				if (pCTExam->m_sk_ok) s.Format(_T("%.10f"), pCTExam->m_sk_df_dp);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

				if (pCTExam->m_sk_ok) s.Format(_T("%i"), pCTExam->m_sk_st_dg);
				else s.Empty();
				G_As = s;//6.2.1
				fprintf(pFile, "<Degrees>%s</Degrees>\n", G_As);

				fprintf(pFile, "</Delta>\n");

				fprintf(pFile, "</SimKAtDEqual3Mm>\n");

				if (pCTExam->m_is_ok)
				{
					s.Format(_T("%.10f"), pCTExam->m_is_df_dp);
				}

				G_As = s;//6.2.1
				fprintf(pFile, "<InferiorMinusSuperiorAxialPowerAtDEqual6Mm_dpt>%s</InferiorMinusSuperiorAxialPowerAtDEqual6Mm_dpt>\n", G_As);

				for (int i = 0; i <= 10; i++)
				{
					if (pCTExam->m_zk_ok[i]) s.Format(_T("%.10f"), pCTExam->m_zk_av_dp[i]); else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<AverageAxialPowerAtDEqual%iMm_dpt>%s</AverageAxialPowerAtDEqual%iMm_dpt>\n", i, G_As, i);
				}

				char* c[3] =
				{
					"LessOrEqual3",
					"EqualOrGreater3MmAndLessOrEqual5Mm",
					"EqualOrGreater5MmAndLessOrEqual7Mm"
				};

				for (int i = 0; i < 3; i++)
				{
					fprintf(pFile, "<AxialPowerAtD%s>\n", c[i]);
					for (int j = 0; j < 2; j++)
					{
						fprintf(pFile, "<SteepSemimeridian%i>\n", j + 1);

						if (pCTExam->m_kr_ok[i]) s.Format(_T("%.10f"), pCTExam->m_kr_st_dp[i][j]);
						else s.Empty();
						G_As = s;//6.2.1
						fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

						if (pCTExam->m_kr_ok[i]) s.Format(_T("%.10f"), 0.001 * pCTExam->m_kr_st_um[i][j]);
						else s.Empty();
						G_As = s;//6.2.1
						fprintf(pFile, "<Millimeters>%s</Millimeters>\n", G_As);

						if (pCTExam->m_kr_ok[i]) s.Format(_T("%i"), pCTExam->m_kr_st_dg[i][j]);
						else s.Empty();
						G_As = s;//6.2.1
						fprintf(pFile, "<Degrees>%s</Degrees>\n", G_As);
						fprintf(pFile, "</SteepSemimeridian%i>\n", j + 1);
					}
					for (int j = 0; j < 2; j++)
					{
						fprintf(pFile, "<FlatSemimeridian%i>\n", j + 1);

						if (pCTExam->m_kr_ok[i]) s.Format(_T("%.10f"), pCTExam->m_kr_fl_dp[i][j]);
						else s.Empty();
						G_As = s;//6.2.1
						fprintf(pFile, "<Diopters>%s</Diopters>\n", G_As);

						if (pCTExam->m_kr_ok[i]) s.Format(_T("%.10f"), 0.001 * pCTExam->m_kr_fl_um[i][j]);
						else s.Empty();
						G_As = s;//6.2.1
						fprintf(pFile, "<Millimeters>%s</Millimeters>\n", G_As);

						if (pCTExam->m_kr_ok[i]) s.Format(_T("%i"), pCTExam->m_kr_fl_dg[i][j]);
						else s.Empty();
						G_As = s;//6.2.1
						fprintf(pFile, "<Degrees>%s</Degrees>\n", G_As);

						fprintf(pFile, "</FlatSemimeridian%i>\n", j + 1);
					}
					fprintf(pFile, "</AxialPowerAtD%s>\n", c[i]);
				}

				s.Format(_T("%.10f"), pCTExam->m_sph_6mm);
				G_As = s;//6.2.1
				fprintf(pFile, "<SphericalAberrationAtDEqual6Mm_um>%s</SphericalAberrationAtDEqual6Mm_um>\n", G_As);
			}

			fprintf(pFile, "</Exam>\n");

			fclose(pFile);
		}
		else
		{
			::Error("Failed to create exam's file.");
			//return;
		}
	}

	//Export color image xml
	for (int e = 0; e < 2; e++)
	{
		CExam* pExam = pExams[e]; if (!pExam) continue;

		CExamHeader* pHeader = &pExam->m_Header;

		if (pExam->m_Header.m_Type == EXAM_TYPE_CT)
		{
			CCTExam* cExam = (CCTExam*)pExam;

			if (cExam->m_ColorImage.m_RGBData.GetMem() != NULL)
			{
				//Calculate the pos
				CEyeImage*  pImage = &cExam->m_ColorImage;
				pImage->SwapImageInfo(&cExam->m_Image);

				if (!pImage->m_ve_ok)
				{
					pImage->FindWFVertex();
				}
				if (!pImage->m_li_ok)
				{
					pImage->FindLimbus();
					//FindClearLimbus(m_pImage,m_pCTExam->m_Image.m_li_x0_um, m_pCTExam->m_Image.m_li_y0_um, m_pCTExam->m_Image.m_li_r_mean_um);			
				}
				if (pImage->m_pu_r_mean_um > 0) pImage->m_pu_ok = TRUE;
				if (!pImage->m_pu_ok)
				{
					pImage->FindPupil();

					//Judge wether the finding pupil is correct or not
					if (pImage->m_ve_ok)
					{
						real x0 = pImage->m_pu_x0_um;
						real x1 = pImage->m_ve_x_um;
						real y0 = pImage->m_pu_y0_um;
						real y1 = pImage->m_ve_y_um;
						real puveDisum = sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1));

						if (puveDisum > 1000)
						{
							pImage->ClearPupil();
							pImage->m_pu_ok = FindColorPupil(pImage);
							int a = 0;
						}
					}
				}
				//m_pImage->SaveIntoFile("C://1.jpg");

				//Recalculate the visual axis by useing ct gray image
				CEyeImage*  GrayImage;

				GrayImage = &cExam->m_Image;

				real xGrayDis = GrayImage->m_li_x0_um - GrayImage->m_ve_x_um;
				real yGrayDis = GrayImage->m_li_y0_um - GrayImage->m_ve_y_um;

				pImage->m_ve_x_um = pImage->m_li_x0_um - xGrayDis;
				pImage->m_ve_y_um = pImage->m_li_y0_um - yGrayDis;
				//ecalculate the visual axis by useing ct gray image Done

				CStringA  AFileName(ScreenFolderName + "\\Color.xml");
				FILE* pFile = fopen(AFileName, "wt");
				if (pFile)
				{
					fprintf(pFile, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
					fprintf(pFile, "<Color Image>\n");

					//Image center; Limbus center; angle k/a; pupil;
					fprintf(pFile, "<ImageCenter>\n");
					if (pImage->m_ve_ok)
					{
						real x = -1 * 0.001 * pImage->m_ve_x_um;
						real y = -1 * 0.001 * pImage->m_ve_y_um;

						s.Format(_T("%.10f"), x);
						G_As = s;//6.2.1
						fprintf(pFile, "<x_coordinate>%s</x_coordinate>\n", G_As);
						s.Format(_T("%.10f"), y);
						G_As = s;//6.2.1
						fprintf(pFile, "<y_coordinate>%s</y_coordinate>\n", G_As);

						real dis = sqrt(x*x + y*y);
						s.Format(_T("%.10f"), dis);
						G_As = s;//6.2.1
						fprintf(pFile, "<Distance_mm>%s</Distance_mm>\n", G_As);

						int Angle = intRound(_180_Pi * angle(-1 * pImage->m_ve_y_um, -1 * pImage->m_ve_x_um)) % 360;
						s.Format(_T("%i"), Angle);
						G_As = s;//6.2.1
						fprintf(pFile, "<Angle_deg>%s</Angle_deg>\n", G_As);
					}
					fprintf(pFile, "</ImageCenter>\n");

					//angle kappa
					fprintf(pFile, "<AngleKappa>\n");

					if (pImage->m_pu_ok && pImage->m_ve_ok)
					{
						real AngleKappaXUm = pImage->m_pu_x0_um - pImage->m_ve_x_um;
						real AngleKappaYUm = pImage->m_pu_y0_um - pImage->m_ve_y_um;
						real AngleKappaRUm = hyp(AngleKappaYUm, AngleKappaXUm);
						real AngleKappaADg = intRound(_180_Pi * angle(AngleKappaYUm, AngleKappaXUm)) % 360;

						s.Format(_T("%.10f"), 0.001 * AngleKappaRUm);
						G_As = s;//6.2.1
						fprintf(pFile, "<Distance_mm>%s</Distance_mm>\n", G_As);
						s.Format(_T("%i"), AngleKappaADg);
						G_As = s;//6.2.1
						fprintf(pFile, "<Angle_deg>%s</Angle_deg>\n", G_As);
					}

					fprintf(pFile, "</AngleKappa>\n");

					// angle alpha
					fprintf(pFile, "<AngleAlpha>\n");

					if (pImage->m_li_ok && pImage->m_ve_ok)
					{
						real AngleAlphaXUm = pImage->m_li_x0_um - pImage->m_ve_x_um;
						real AngleAlphaYUm = pImage->m_li_y0_um - pImage->m_ve_y_um;
						real AngleAlphaRUm = hyp(AngleAlphaYUm, AngleAlphaXUm);
						real AngleAlphaADg = intRound(_180_Pi * angle(AngleAlphaYUm, AngleAlphaXUm)) % 360;

						s.Format(_T("%.10f"), 0.001 * AngleAlphaRUm);
						G_As = s;//6.2.1
						fprintf(pFile, "<Distance_mm>%s</Distance_mm>\n", G_As);
						s.Format(_T("%i"), AngleAlphaADg);
						G_As = s;//6.2.1
						fprintf(pFile, "<Angle_deg>%s</Angle_deg>\n", G_As);
					}

					fprintf(pFile, "</AngleAlpha>\n");

					// limbus
					fprintf(pFile, "<Limbus>\n");
					fprintf(pFile, "<Center>\n");

					if (pImage->m_li_ok) s.Format(_T("%.10f"), 0.001 * pImage->m_li_x0_um);
					else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<X_mm>%s</X_mm>\n", G_As);

					if (pImage->m_li_ok) s.Format(_T("%.10f"), 0.001 * pImage->m_li_y0_um);
					else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<Y_mm>%s</Y_mm>\n", G_As);
					fprintf(pFile, "</Center>\n");

					if (pImage->m_li_ok) s.Format(_T("%.10f"), 0.002 * pImage->m_li_r_mean_um);
					else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<Diameter_mm>%s</Diameter_mm>\n", G_As);
					fprintf(pFile, "</Limbus>\n");

					// pupil
					fprintf(pFile, "<Pupil>\n");
					fprintf(pFile, "<Center>\n");

					if (pImage->m_pu_ok) s.Format(_T("%.10f"), 0.001 * pImage->m_pu_x0_um);
					else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<X_mm>%s</X_mm>\n", G_As);

					if (pImage->m_pu_ok) s.Format(_T("%.10f"), 0.001 * pImage->m_pu_y0_um);
					else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<Y_mm>%s</Y_mm>\n", G_As);
					fprintf(pFile, "</Center>\n");

					if (pImage->m_pu_ok) s.Format(_T("%.10f"), 0.002 * pImage->m_pu_r_mean_um);
					else s.Empty();
					G_As = s;//6.2.1
					fprintf(pFile, "<Diameter_mm>%s</Diameter_mm>\n", G_As);
					fprintf(pFile, "</Pupil>\n");

					fprintf(pFile, "</Color Image>\n");

					fclose(pFile);
				}
				else
				{
					::Error("Failed to create color image's file.");
					//return;
				}
			}
		}
	}
	//Done

	s = ScreenFolderName + "\\Screenshot." + (::Settings.m_UseJPGFormat ? "jpg" : "png");

	HANDLE hFile = CreateFile(s, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ulong bt;
		WriteFile(hFile, Mem.GetMem(), Mem.GetSize(), &bt, NULL);
		CloseHandle(hFile);
	}
	else
	{
		::Error("Failed to create screenshot file.");
		//return;
	}

	//DICOM File
	s = ScreenFolderName + "\\ScreenDicom.dcm";

	G_As = s;
	char* CharFileName = G_As.GetBuffer(s.GetLength());

	CBasicImage Image;

	Image.m_w = w;
	Image.m_h = h;
	Image.m_RGBData.Create(h, LINE_SIZE(w), RGBData);
	
	if (m_isComboDicom)
	{		
		CTraceyDicom TraceyDicom;

		CHAR* pData = new CHAR[(w + 200)*h * 3];
		memset(pData, 0, (w + 200)*h * 3);
		int R, G, B;
		int count = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = -150; x < (w + 50); x++)
			{
				if (x >= 0 && x < w)
				{
					R = Image.GetRAt(x, h - y - 1);
					G = Image.GetGAt(x, h - y - 1);
					B = Image.GetBAt(x, h - y - 1);
				}
				else
				{
					R = 0;
					G = 0;
					B = 0;
				}

				pData[count] = R;
				count++;
				pData[count] = G;
				count++;
				pData[count] = B;
				count++;
			}
		}

		TraceyDicom.OutputDicomImage(CharFileName, DicomInfo, pData, w + 200, h);

		delete[] pData;		

		CString Note = "Data has been exported to '" + ExportFolderName + "' successfully.";
		::Info(Note);
	}
	else//DCMTK
	{
		CDcmtkDicom DcmtkDicom;

		BYTE* pData = new BYTE[(w + 200)*h * 3];
		memset(pData, 0, (w + 200)*h * 3);
		int R, G, B;
		int count = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = -150; x<(w + 50); x++)
			{
				if (x >= 0 && x < w)
				{
					R = Image.GetRAt(x, h - y - 1);
					G = Image.GetGAt(x, h - y - 1);
					B = Image.GetBAt(x, h - y - 1);
				}
				else
				{
					R = 0;
					G = 0;
					B = 0;
				}

				pData[count] = R;
				count++;
				pData[count] = G;
				count++;
				pData[count] = B;
				count++;
			}
		}

		BOOL Suc = DcmtkDicom.SaveDcmtkDicomFile(CharFileName, DicomInfo, pData, w + 200, h);

		//delete pData;//This one shows error and make software crash

		if (Suc)
		{
			CString Note = "Data has been exported to '" + ExportFolderName + "' successfully.";
			::Info(Note);
		}
	}
	//DICOM File Done
}

//***************************************************************************************

BOOL FindColorPupil(CEyeImage* OriImage)
{
	CEyeImage* TestImage = new CEyeImage();
	int h_h = TestImage->m_h = OriImage->m_h;
	int w_w = TestImage->m_w = OriImage->m_w;
	TestImage->m_h_um = OriImage->m_h_um;
	TestImage->m_w_um = OriImage->m_w_um;

	TestImage->m_RGBData.Create(OriImage->m_h, (int)OriImage->m_w * 3, OriImage->m_RGBData.GetMem());

	if (TestImage->m_RGBData.GetMem() == NULL) return FALSE;

	int ** m_pixels = (int**)calloc(w_w, sizeof(int));
	real** m_g = (real**)calloc(4, sizeof(real));
	int ** m_BlurPixels = (int**)calloc(w_w, sizeof(int));
	int ** m_gxy = (int**)calloc(w_w, sizeof(int));
	int ** m_gx = (int**)calloc(w_w, sizeof(int));
	int ** m_gy = (int**)calloc(w_w, sizeof(int));
	int ** m_dis = (int**)calloc(w_w, sizeof(int));
	int ** m_degree = (int**)calloc(w_w, sizeof(int));
	int ** m_gR = (int**)calloc(w_w, sizeof(int));

	for (int i = 0; i<w_w; i++)
	{
		if (i<4)  m_g[i] = (real*)calloc(4, sizeof(real));
		m_pixels[i] = (int*)calloc(h_h, sizeof(int));
		m_BlurPixels[i] = (int*)calloc(h_h, sizeof(int));

		m_gxy[i] = (int*)calloc(h_h, sizeof(int));
		m_gx[i] = (int*)calloc(h_h, sizeof(int));
		m_gy[i] = (int*)calloc(h_h, sizeof(int));
		m_dis[i] = (int*)calloc(h_h, sizeof(int));
		m_degree[i] = (int*)calloc(h_h, sizeof(int));

		m_gR[i] = (int*)calloc(h_h, sizeof(int));
	}


	//Set the Tracking Region
	int SRegion_StX = intRound(0.3*w_w);// Search region start x
	int SRegion_EndX = intRound(0.7*w_w);// Search region end x
	int SRegion_StY = intRound(0.3*h_h);// Search region start x
	int SRegion_EndY = intRound(0.7*h_h);// Search region end x	

	int StartR = intRound(0.05*w_w);
	int EndR = intRound(0.15*w_w);

	real de = 1.4;

	//*******************************************************************************
	//The arrays definition
	//*******************************************************************************

	int tt = 0;
	int ave = 0;
	for (int i = SRegion_StX + 1; i < SRegion_EndX; i++)
	{
		for (int j = SRegion_StY + 1; j < SRegion_EndY; j++)
		{
			int R = TestImage->m_RGBData(j, i * 3 + 2);
			int G = TestImage->m_RGBData(j, i * 3 + 1);
			int B = TestImage->m_RGBData(j, i * 3);

			ave += intRound(0.3*R + 0.59*G + 0.11*B);
			tt++;
		}
	}
	ave = ave / tt;

	int Gray = 0;
	int dif = 0;
	for (int i = SRegion_StX + 1; i < SRegion_EndX; i++)
	{
		for (int j = SRegion_StY + 1; j < SRegion_EndY; j++)
		{
			int R = TestImage->m_RGBData(j, i * 3 + 2);
			int G = TestImage->m_RGBData(j, i * 3 + 1);
			int B = TestImage->m_RGBData(j, i * 3);

			Gray = intRound(0.3*R + 0.59*G + 0.11*B);
			dif = abs(Gray - ave) + dif;
		}
	}
	dif = dif / tt;


	int R;
	int G;
	int B;

	for (int i = 0; i<w_w; i++)
	{
		for (int j = 0; j<h_h; j++)
		{
			//For Gaussian Blur		
			if (i < 4 && j < 4)
			{
				m_g[i][j] = Gaussian(i, j, de);
			}

			if (i > SRegion_StX && i < SRegion_EndX && j > SRegion_StY && j < SRegion_EndY)
			{
				R = TestImage->m_RGBData(j, i * 3 + 2);
				G = TestImage->m_RGBData(j, i * 3 + 1);
				B = TestImage->m_RGBData(j, i * 3);

				if (dif > 25)// Lighter eye has larger variance
				{
					m_pixels[i][j] = intRound(0.3*R + 0.59*G + 0.11*B);
					//m_pixels[i][j] = intRound(sqrt(((real)B*R + (real)B*G + (real)R*G)/3.0));
				}
				else//Darker eye use the color for judgement
				{
					HSI res = ChangeRGBtoHSI(R, G, B);//improve the w/b comparison of pupil for iTrace color image
					m_pixels[i][j] = intRound(res.H);
					if (0 < res.H && res.H <= 30)
					{
						m_pixels[i][j] = 10;
					}
					else if (30 < res.H && res.H <= 60)
					{
						m_pixels[i][j] = 40;
					}
					else if (60 < res.H && res.H <= 90)
					{
						m_pixels[i][j] = 70;
					}
					else if (90 < res.H && res.H <= 120)
					{
						m_pixels[i][j] = 100;
					}
					else if (120 < res.H && res.H <= 150)
					{
						m_pixels[i][j] = 130;
					}
					else if (150 < res.H && res.H <= 180)
					{
						m_pixels[i][j] = 160;
					}
					else if (180 < res.H && res.H <= 240)
					{
						m_pixels[i][j] = 190;
					}
					else if (240 < res.H && res.H <= 270)
					{
						m_pixels[i][j] = 220;
					}
					else if (270 < res.H && res.H <= 300)
					{
						m_pixels[i][j] = 250;
					}
					else if (300 < res.H && res.H <= 330)
					{
						m_pixels[i][j] = 40;
					}
					else if (330 < res.H && res.H <= 360)
					{
						m_pixels[i][j] = 10;
					}
				}
			}
			else m_pixels[i][j] = 0;

			m_BlurPixels[i][j] = 0;
			//Done


			//For Canney Edge
			m_gxy[i][j] = 0;
			m_gx[i][j] = 0;
			m_gy[i][j] = 0;
			m_degree[i][j] = 0;
			m_dis[i][j] = 0;
			m_gR[i][j] = 0;
			//Done
		}
	}
	//*******************************************************************************
	//arrays definition Done
	//*******************************************************************************





	//*******************************************************************************
	//First step is the Gassusian blur
	//*******************************************************************************	
	real Mid = 0;
	for (int h = SRegion_StY + 3; h < SRegion_EndY - 3; h++)
	{
		for (int v = SRegion_StX + 3; v < SRegion_EndX - 3; v++)
		{
			Mid = 0;

			//1
			int R0 = m_pixels[v - 3][h - 3];
			int R1 = m_pixels[v - 2][h - 3];
			int R2 = m_pixels[v - 1][h - 3];
			int R3 = m_pixels[v][h - 3];
			int R4 = m_pixels[v + 1][h - 3];
			int R5 = m_pixels[v + 2][h - 3];
			int R6 = m_pixels[v + 2][h - 3];

			Mid = (R0)*m_g[3][3] + (R1)*m_g[2][3] + (R2)*m_g[1][3] + (R3)*m_g[0][3] + (R4)*m_g[3][3] + (R5)*m_g[3][2] + (R6)*m_g[3][1];

			//2
			R0 = m_pixels[v - 3][h - 2];
			R1 = m_pixels[v - 2][h - 2];
			R2 = m_pixels[v - 1][h - 2];
			R3 = m_pixels[v][h - 2];
			R4 = m_pixels[v + 1][h - 2];
			R5 = m_pixels[v + 2][h - 2];
			R6 = m_pixels[v + 2][h - 2];

			Mid += (R0)*m_g[3][2] + (R1)*m_g[2][2] + (R2)*m_g[1][2] + (R3)*m_g[0][2] + (R4)*m_g[3][2] + (R5)*m_g[2][2] + (R6)*m_g[2][1];

			//3
			R0 = m_pixels[v - 3][h - 1];
			R1 = m_pixels[v - 2][h - 1];
			R2 = m_pixels[v - 1][h - 1];
			R3 = m_pixels[v][h - 1];
			R4 = m_pixels[v + 1][h - 1];
			R5 = m_pixels[v + 2][h - 1];
			R6 = m_pixels[v + 2][h - 1];

			Mid += (R0)*m_g[3][1] + (R1)*m_g[2][1] + (R2)*m_g[1][1] + (R3)*m_g[0][1] + (R4)*m_g[1][3] + (R5)*m_g[1][2] + (R6)*m_g[1][1];

			//4
			R0 = m_pixels[v - 3][h];
			R1 = m_pixels[v - 2][h];
			R2 = m_pixels[v - 1][h];
			R3 = m_pixels[v][h];
			R4 = m_pixels[v + 1][h];
			R5 = m_pixels[v + 2][h];
			R6 = m_pixels[v + 2][h];

			Mid += (R0)*m_g[3][0] + (R1)*m_g[2][0] + (R2)*m_g[1][0] + (R3)*m_g[0][0] + (R4)*m_g[0][3] + (R5)*m_g[0][2] + (R6)*m_g[0][1];

			//5
			R0 = m_pixels[v - 3][h + 1];
			R1 = m_pixels[v - 2][h + 1];
			R2 = m_pixels[v - 1][h + 1];
			R3 = m_pixels[v][h + 1];
			R4 = m_pixels[v + 1][h + 1];
			R5 = m_pixels[v + 2][h + 1];
			R6 = m_pixels[v + 2][h + 1];

			Mid += (R0)*m_g[3][1] + (R1)*m_g[2][1] + (R2)*m_g[1][1] + (R3)*m_g[0][1] + (R4)*m_g[1][3] + (R5)*m_g[1][2] + (R6)*m_g[1][1];

			//6
			R0 = m_pixels[v - 3][h + 2];
			R1 = m_pixels[v - 2][h + 2];
			R2 = m_pixels[v - 1][h + 2];
			R3 = m_pixels[v][h + 2];
			R4 = m_pixels[v + 1][h + 2];
			R5 = m_pixels[v + 2][h + 2];
			R6 = m_pixels[v + 2][h + 2];

			Mid += (R0)*m_g[3][2] + (R1)*m_g[2][2] + (R2)*m_g[1][2] + (R3)*m_g[0][2] + (R4)*m_g[3][2] + (R5)*m_g[2][2] + (R6)*m_g[2][1];

			//7
			R0 = m_pixels[v - 3][h + 3];
			R1 = m_pixels[v - 2][h + 3];
			R2 = m_pixels[v - 1][h + 3];
			R3 = m_pixels[v][h + 3];
			R4 = m_pixels[v + 1][h + 3];
			R5 = m_pixels[v + 2][h + 3];
			R6 = m_pixels[v + 2][h + 3];

			Mid += (R0)*m_g[3][3] + (R1)*m_g[2][3] + (R2)*m_g[1][3] + (R3)*m_g[0][3] + (R4)*m_g[3][3] + (R5)*m_g[3][2] + (R6)*m_g[3][1];

			m_BlurPixels[v][h] = intRound(Mid);
		}
	}
	//*******************************************************************************
	//Gassusian blur Done
	//*******************************************************************************




	//*******************************************************************************
	//Second step is Canney Edge detection
	//*******************************************************************************
	int Range = EndR - StartR;
	int maxGradient = 0;
	real fmean = 0;
	int t = 0;
	double mid = 180 / 3.1415926;

	for (int h = SRegion_StY + 1; h < SRegion_EndY - 1; h++)
	{
		for (int v = SRegion_StX + 1; v < SRegion_EndX - 1; v++)
		{
			m_gx[v][h] = (m_BlurPixels[v - 1][h - 1] * (-1) + m_BlurPixels[v + 1][h + 1] * 1 + m_BlurPixels[v - 1][h] * (-2) + m_BlurPixels[v + 1][h] * 2 + m_BlurPixels[v - 1][h + 1] * (-1) + m_BlurPixels[v + 1][h + 1] * 1);
			m_gy[v][h] = (m_BlurPixels[v - 1][h - 1] * (1) + m_BlurPixels[v][h - 1] * 2 + m_BlurPixels[v + 1][h + 1] * 1 + m_BlurPixels[v - 1][h + 1] * (-1) + m_BlurPixels[v][h + 1] * (-2) + m_BlurPixels[v + 1][h + 1] * (-1));

			m_gxy[v][h] = intRound(sqrt((real)m_gx[v][h] * m_gx[v][h] + (real)m_gy[v][h] * m_gy[v][h]));

			fmean += (real)m_gxy[v][h];
			t++;
			if (m_gxy[v][h]>maxGradient) maxGradient = m_gxy[v][h];

			if (m_gx[v][h] == 0)
			{
				m_degree[v][h] = ((m_gy[v][h] == 0) ? 0 : 90);
			}
			else
			{
				double div = (double)m_gy[v][h] / (double)m_gx[v][h];
				double ang = 0;

				if (div < 0)
				{
					ang = 180 - atan(-div) * mid;
				}
				else
				{
					ang = atan(div) * mid;
				}

				//(0 to 22.5 & 157.5 to 180 degrees) =>0
				//22.5 to 67.5 degrees =>45
				//67.5 to 112.5 degrees =>90
				//112.5 to 157.5 degrees =>135				
				if (ang < 22.5)        m_degree[v][h] = 0;
				else if (ang < 67.5)   m_degree[v][h] = 45;
				else if (ang < 112.5)  m_degree[v][h] = 90;
				else if (ang < 157.5)  m_degree[v][h] = 135;
				else ang = 0;
			}
		}
	}


	int leftPixel;
	int rightPixel;
	for (int h = SRegion_StY + 1; h < SRegion_EndY - 1; h++)
	{
		for (int v = SRegion_StX + 1; v < SRegion_EndX - 1; v++)
		{
			switch (m_degree[v][h])
			{
			case 0:
				leftPixel = m_gxy[v - 1][h];
				rightPixel = m_gxy[v + 1][h];
				break;
			case 45:
				leftPixel = m_gxy[v - 1][h + 1];
				rightPixel = m_gxy[v + 1][h - 1];
				break;
			case 90:
				leftPixel = m_gxy[v][h + 1];
				rightPixel = m_gxy[v][h - 1];
				break;
			case 135:
				leftPixel = m_gxy[v + 1][h + 1];
				rightPixel = m_gxy[v - 1][h - 1];
				break;
			}
			if (m_gxy[v][h] < leftPixel || m_gxy[v][h] < rightPixel)
			{
				m_dis[v][h] = 0;
			}
			else
			{
				m_dis[v][h] = intRound((real)m_gxy[v][h] / (real)maxGradient * 255.0);
			}
		}
	}

	fmean = fmean / t;
	fmean = fmean / maxGradient * 255;
	int highThreshold = intRound(fmean);
	int lowThreshold = intRound(0.4 * highThreshold);

	for (int y = SRegion_StY; y < SRegion_EndY; y++)
	{
		for (int x = SRegion_StX; x < SRegion_EndX; x++)
		{
			if (m_dis[x][y] != 0)
			{
				if (m_dis[x][y] < highThreshold)
				{
					if (m_dis[x][y] < lowThreshold)
					{
						m_dis[x][y] = 0;
					}
					else
					{
						if ((m_dis[x - 1][y]   < highThreshold) &&
							(m_dis[x + 1][y]   < highThreshold) &&
							(m_dis[x - 1][y - 1] < highThreshold) &&
							(m_dis[x][y - 1]   < highThreshold) &&
							(m_dis[x + 1][y - 1] < highThreshold) &&
							(m_dis[x - 1][y + 1] < highThreshold) &&
							(m_dis[x][y + 1]   < highThreshold) &&
							(m_dis[x + 1][y + 1] < highThreshold))
						{
							m_dis[x][y] = 0;
						}
					}
				}
			}
		}
	}
	//*******************************************************************************
	//Canney Edge detection Done
	//******************************************************************************* 






	//*******************************************************************************
	//Third step is hough circle detection
	//*******************************************************************************
	int EdgeX = 0;
	int EdgeY = 0;

	OnePoint CircleCen;
	CircleCen.x = 0;
	CircleCen.y = 0;

	int max = 0;
	int FindR = 0;

	for (int CircleR = StartR; CircleR <= EndR; CircleR++)
	{
		for (int h = SRegion_StY + 5; h < SRegion_EndY - 5; h++)
		{
			for (int v = SRegion_StX + 5; v < SRegion_EndX - 5; v++)
			{
				if (m_dis[v][h] != 0 && m_dis[v][h] != -3)
				{
					for (int r = 0; r< 360; r++)
					{
						EdgeX = v + ::MutiCOS[CircleR][r];
						EdgeY = h + ::MutiSIN[CircleR][r];

						if (EdgeX > SRegion_StX && EdgeX < SRegion_EndX && EdgeY > SRegion_StY && EdgeY < SRegion_EndY)
						{
							m_gR[EdgeX][EdgeY]++;
							m_gR[EdgeX - 1][EdgeY]++;
							m_gR[EdgeX + 1][EdgeY]++;
							m_gR[EdgeX - 1][EdgeY - 1]++;
							m_gR[EdgeX][EdgeY - 1]++;
							m_gR[EdgeX + 1][EdgeY - 1]++;
							m_gR[EdgeX - 1][EdgeY + 1]++;
							m_gR[EdgeX][EdgeY + 1]++;
							m_gR[EdgeX + 1][EdgeY + 1]++;
						}
					}
				}
			}
		}

		//Find the circle
		for (int h = 0; h<h_h; h++)
		{
			for (int v = 0; v<w_w; v++)
			{
				if (m_gR[v][h] != 0)
				{
					if (m_gR[v][h] > max)
					{
						max = m_gR[v][h];
						FindR = CircleR;
						CircleCen.x = v;
						CircleCen.y = h;
					}
					m_gR[v][h] = 0;
				}
			}
		}
	}

	if (max < 50)
	{
		return FALSE;
	}


	//Save the result  	
	{
		int R_um = intRound(FindR * TestImage->m_w_um / w_w);

		int cx = intRound(0.5*w_w);
		int cy = intRound(0.5*h_h);

		OriImage->m_pu_ok = TRUE;
		OriImage->m_pu_x0_um = (CircleCen.x - cx)*TestImage->m_w_um / w_w;
		OriImage->m_pu_y0_um = (CircleCen.y - cy)*TestImage->m_h_um / h_h;

		for (int a = 0; a < 360; a++)
		{
			OriImage->m_pu_r_um[a] = R_um;
		}

		OriImage->m_pu_r_min_um = R_um;
		OriImage->m_pu_r_max_um = R_um;
		OriImage->m_pu_r_mean_um = R_um;
	}

	//*******************************************************************************
	//hough circle detection Done
	//*******************************************************************************

	delete TestImage;

	//free memory	
	if (m_g != NULL) {
		for (int i = 0; i<3; i++) {
			if (m_g[i] != NULL) {
				delete[](m_g[i]);
				m_g[i] = NULL;
			}
		}
		delete[](m_g);
		m_g = NULL;
	}

	SafeInteRelease(m_pixels, w_w);
	SafeInteRelease(m_BlurPixels, w_w);
	SafeInteRelease(m_gxy, w_w);
	SafeInteRelease(m_gx, w_w);
	SafeInteRelease(m_gy, w_w);
	SafeInteRelease(m_dis, w_w);
	SafeInteRelease(m_degree, w_w);

	SafeInteRelease(m_gR, w_w);

	return TRUE;
}

//*****************************************************************************************

double Gaussian(int x, int y, double de)
{
	double value = 0;
	double pai = 3.1415926;
	double e = 2.718281828;

	value = (-1)*((x*x + y*y) / (2 * de*de));
	value = (1 / (2 * pai*de*de))*pow(e, value);

	return value;
}

//*****************************************************************************************

HSI ChangeRGBtoHSI(int R, int G, int B)
{

	HSI res;
	res.I = (real)(R + G + B) / 3;

	//find the minixum of RGB for the S value
	real min = (real)R;
	if (min>G) min = G;
	if (min>B) min = B;
	//
	res.S = 1 - (min / res.I);

	//find the S
	real sita;
	real mid1 = 0.5*(2 * R - G - B);
	real mid2 = pow((R - G)*(R - G) + (R - B)*(G - B), 0.5) + 0.0000000001;
	mid1 = mid1 / mid2;
	sita = acos(mid1);

	if (B>G) sita = 2 * _Pi - sita;

	real angle = sita * 180 / _Pi;

	res.H = angle;

	return res;
}

//*****************************************************************************************
