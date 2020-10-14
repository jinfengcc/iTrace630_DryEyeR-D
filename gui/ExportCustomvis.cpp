//***************************************************************************************

#include "StdAfx.h"
#include "Export.h"
#include "Data.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void ExportCustomvis(const CString& FolderName, CExam* pExam)
{
	CPatient* pPatient = ::DB.GetPatient(pExam->m_Header.m_PatientID);
	CGroup* pGroup = ::DB.GetGroup(pPatient->m_GroupID);

	::DB.LoadEyes(pPatient->m_PatientID);

	CEye* pOD = NULL;
	CEye* pOS = NULL;

	for (int i = 0; i < ::DB.m_Eyes.GetSize(); i++)
	{
		if (ISOD(::DB.m_Eyes[i].m_Type)) pOD = &::DB.m_Eyes[i];
		else pOS = &::DB.m_Eyes[i];
	}

	CStringA AFolderName(FolderName);
	FILE* pFile = fopen(AFolderName + "\\patient.txt", "wt");

	if (pFile)
	{
		fprintf(pFile, "[Personal data]\n\n");
		fprintf(pFile, "Last name   = %s\n", pPatient->m_LastName);
		fprintf(pFile, "First name  = %s\n", pPatient->m_FirstName);
		fprintf(pFile, "Middle name = %s\n", pPatient->m_MiddleName);
		fprintf(pFile, "Year        = %04i\n", pPatient->m_BirthYear);
		fprintf(pFile, "Month       = %02i\n", pPatient->m_BirthMonth);
		fprintf(pFile, "Day         = %02i\n", pPatient->m_BirthDay);
		fprintf(pFile, "Sex         = %s\n", pPatient->m_Sex == SEX_MALE ? "male" : (pPatient->m_Sex == SEX_FEMALE ? "female" : "unknown"));
		fprintf(pFile, "\n");
		fprintf(pFile, "[Miscellaneous data]\n\n");
		fprintf(pFile, "ID          = %s\n", pPatient->m_CustomID);
		fprintf(pFile, "Group       = %s\n", pGroup != NULL ? pGroup->m_Name : "");
		fprintf(pFile, "Note        = %s\n", pPatient->m_Note);
		fprintf(pFile, "\n");

		fprintf(pFile, "[Manifest refraction data]\n\n");

		if (pOD)
		{
			fprintf(pFile, "OD vertex   = %.2f\n", pOD->m_ManifestVertex);
			fprintf(pFile, "OD sphere   = %.2f\n", pOD->m_ManifestSphere);
			fprintf(pFile, "OD cylinder = %.2f\n", pOD->m_ManifestCylinder);
			fprintf(pFile, "OD axis     = %i\n", pOD->m_ManifestAxis);
		}

		if (pOS)
		{
			fprintf(pFile, "OS vertex   = %.2f\n", pOS->m_ManifestVertex);
			fprintf(pFile, "OS sphere   = %.2f\n", pOS->m_ManifestSphere);
			fprintf(pFile, "OS cylinder = %.2f\n", pOS->m_ManifestCylinder);
			fprintf(pFile, "OS axis     = %i\n", pOS->m_ManifestAxis);
		}

		fprintf(pFile, "\n");

		fprintf(pFile, "[Keratometry data]\n\n");

		if (pOD)
		{
			fprintf(pFile, "OD K1       = %.2f\n", pOD->m_K1);
			fprintf(pFile, "OD K2       = %.2f\n", pOD->m_K2);
			fprintf(pFile, "OD K2 axis  = %i\n", pOD->m_K2Axis);
		}

		if (pOS)
		{
			fprintf(pFile, "OS K1       = %.2f\n", pOS->m_K1);
			fprintf(pFile, "OS K2       = %.2f\n", pOS->m_K2);
			fprintf(pFile, "OS K2 axis  = %i\n", pOS->m_K2Axis);
		}

		fclose(pFile);
	}

	pFile = fopen(AFolderName + "\\photo_scale.txt", "wt");

	if (pFile)
	{
		fprintf(pFile, "[Ratio, pixels/micrometers]\n\n");
		fprintf(pFile, "X = %12.10f\n", pExam->m_Image.m_w / pExam->m_Calibration.VideoWidthMicrons);
		fprintf(pFile, "Y = %12.10f\n", pExam->m_Image.m_h / pExam->m_Calibration.VideoHeightMicrons);
		fclose(pFile);
	}

	if (pExam->m_Header.m_Type == EXAM_TYPE_WF)
	{
		CWFExam* pWFExam = (CWFExam*)pExam;

		pFile = fopen(AFolderName + "\\exam.txt", "wt");
		if (pFile)
		{
			fprintf(pFile, "[Exam info]\n\n");
			fprintf(pFile, "Year          = %04i\n", pExam->m_Header.m_Year);
			fprintf(pFile, "Month         = %02i\n", pExam->m_Header.m_Month);
			fprintf(pFile, "Day           = %02i\n", pExam->m_Header.m_Day);
			fprintf(pFile, "Hour          = %02i\n", pExam->m_Header.m_Hour);
			fprintf(pFile, "Minute        = %02i\n", pExam->m_Header.m_Minute);
			fprintf(pFile, "Second        = %02i\n", pExam->m_Header.m_Second);
			fprintf(pFile, "Comment       = %s\n", pExam->m_Header.m_Note);
			fprintf(pFile, "OS/OD         = %s\n", pExam->m_Header.Eye());
			fprintf(pFile, "Scan size, um = %4i\n", intRound(pWFExam->m_ScanDiameter));
			fprintf(pFile, "Pre/Post      = %s\n", pExam->m_Header.Preop());
			fprintf(pFile, "Target at, D  = %4.2f\n", pWFExam->m_AccommTargetPosition);
			fclose(pFile);
		}

		pExam->m_Image.SaveIntoFile(FolderName + "\\eye_mono.bmp");
		if (!pWFExam->m_ColorImage.SaveIntoFile(FolderName + "\\eye.bmp"))
		{
			pExam->m_Image.SaveIntoFile(FolderName + "\\eye.bmp");
		}

		if (pExam->m_Image.m_pu_ok)
		{
			pFile = fopen(AFolderName + "\\pupil.txt", "wt");
			if (pFile != NULL)
			{
				fprintf(pFile, "[Pupil center position, micrometers]\n\n");
				fprintf(pFile, "X = %9.3f\n", pExam->m_Image.m_pu_x0_um);
				fprintf(pFile, "Y = %9.3f\n", pExam->m_Image.m_pu_y0_um);
				fprintf(pFile, "\n");
				fprintf(pFile, "[Distance from pupil center to pupil edge, micrometers]\n\n");

				for (int a = 0; a < 360; a++)
				{
					fprintf(pFile, "%3i: %9.3f\n", a, pExam->m_Image.m_pu_r_um[a]);
				}
				fclose(pFile);
			}
		}

		if (pExam->m_Image.m_ve_ok)
		{
			pFile = fopen(AFolderName + "\\vertex.txt", "wt");

			if (pFile != NULL)
			{
				fprintf(pFile, "[Vertex position, micrometers]\n\n");
				fprintf(pFile, "X = %9.3f\n", pExam->m_Image.m_ve_x_um);
				fprintf(pFile, "Y = %9.3f\n", pExam->m_Image.m_ve_y_um);
				fclose(pFile);
			}
		}

		CString s;
		s.Format(_T("\\zernike_%04i.txt"), intRound(2.0 * pWFExam->m_WfSurface.m_r_max_um));
		CStringA Afile2(FolderName + s);
		pFile = fopen(Afile2, "wt");

		if (pFile != NULL)
		{
			fprintf(pFile, "[Normalized Zernike coefficients, micrometers]\n\n");
			for (int z = 0; z < ZERNIKE_NUM_MODES; z++) {
				fprintf(pFile, "%2i: %13.10f\n", z, pWFExam->m_WfSurface.GetCUm(z));
			}
			fclose(pFile);
		}
	}

	else if (pExam->m_Header.m_Type == EXAM_TYPE_CT)
	{
		CCTExam* pCTExam = (CCTExam*)pExam;

		pFile = fopen(AFolderName + "\\exam.txt", "wt");
		if (pFile)
		{
			fprintf(pFile, "[Exam info]\n\n");
			fprintf(pFile, "Year          = %04i\n", pExam->m_Header.m_Year);
			fprintf(pFile, "Month         = %02i\n", pExam->m_Header.m_Month);
			fprintf(pFile, "Day           = %02i\n", pExam->m_Header.m_Day);
			fprintf(pFile, "Hour          = %02i\n", pExam->m_Header.m_Hour);
			fprintf(pFile, "Minute        = %02i\n", pExam->m_Header.m_Minute);
			fprintf(pFile, "Second        = %02i\n", pExam->m_Header.m_Second);
			fprintf(pFile, "Comment       = %s\n", pExam->m_Header.m_Note);
			fprintf(pFile, "OS/OD         = %s\n", pExam->m_Header.Eye());
			fclose(pFile);
		}

		pExam->m_Image.SaveIntoFile(FolderName + "\\eye.bmp");

		pFile = fopen(AFolderName + "\\vertex.txt", "wt");

		if (pFile)
		{
			fprintf(pFile, "[Vertex position, micrometers]\n\n");
			fprintf(pFile, "X = %9.3f\n", pExam->m_Image.m_ve_x_um);
			fprintf(pFile, "Y = %9.3f\n", pExam->m_Image.m_ve_y_um);
			fclose(pFile);
		}

		pFile = fopen(AFolderName + "\\rings.txt", "wt");
		if (pFile)
		{
			for (int a = 0; a < 360; a++)
			{
				fprintf(pFile, "%3u:", a);
				for (int r = 0; r < pExam->m_Calibration.NumRings; r++)
				{
					if (pExam->m_Image.m_ri_r_um[r][a] != INVALID_VALUE)
					{
						fprintf(pFile, " %9.3f", pExam->m_Image.m_ri_r_um[r][a]);
					}
					else {
						fprintf(pFile, " ---------");
					}
				}
				fprintf(pFile, "\n");
			}
			fclose(pFile);
		}

		pFile = fopen(AFolderName + "\\rad_ht_deriv.txt", "wt");
		if (pFile)
		{
			for (int a = 0; a < 360; a++)
			{
				fprintf(pFile, "%3u:", a);

				for (int r = 0; r < pExam->m_Calibration.NumRings; r++)
				{
					real v = pCTExam->m_dh_dr[r][a];

					if (v != INVALID_VALUE)
					{
						fprintf(pFile, " %12.10f", v);
					}
					else
					{
						fprintf(pFile, " ------------");
					}
				}
				fprintf(pFile, "\n");
			}
			fclose(pFile);
		}

		pFile = fopen(AFolderName + "\\z_elev.txt", "wt");

		if (pFile)
		{
			for (int a = 0; a < 360; a++)
			{
				fprintf(pFile, "%3u:", a);
				for (int r = 0; r < pExam->m_Calibration.NumRings; r++)
				{
					real r_um = pExam->m_Image.m_ri_r_um[r][a];
					if (r_um != INVALID_VALUE && r_um <= CT_R_MAX_UM)
					{
						real ht_um;
						pCTExam->m_HtZrSurface.GetAt(r_um, a * _Pi_180, &ht_um, NULL, NULL, NULL, NULL, NULL);
						if (ht_um != INVALID_VALUE)
						{
							fprintf(pFile, " %16.10f", ht_um);
						}
						else {
							fprintf(pFile, " ----------------");
						}
					}
					else
					{
						fprintf(pFile, " ----------------");
					}
				}
				fprintf(pFile, "\n");
			}
			fclose(pFile);
		}

		for (int b = 0; b < 4; b++)
		{
			CString FileName;
			FileName.Format(_T("%s\\ball_%04i.txt"), FolderName, intRound(pExam->m_Calibration.BallsAx[b]));

			CStringA AFileName(FileName);
			pFile = fopen(AFileName, "wt");

			if (pFile)
			{
				for (int a = 0; a < 360; a++)
				{
					fprintf(pFile, "%3i:", a);
					for (int r = 0; r < pExam->m_Calibration.NumRings; r++)
					{
						if (pExam->m_Calibration.BallsRi[b][r][a] != INVALID_VALUE)
						{
							fprintf(pFile, " %9.3f", pExam->m_Calibration.BallsRi[b][r][a]);
						}
						else
						{
							fprintf(pFile, " ---------");
						}
					}
					fprintf(pFile, "\n");
				}
				fclose(pFile);
			}
		}
	}

	::Info("Exam exported successfully.");
}

//***************************************************************************************
