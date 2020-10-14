//***************************************************************************************

#include "StdAfx.h"
#include "Export.h"
#include "Zip.h"
#include "Data.h"
#include "Registry.h"
#include "INIParser.h"//6.2.0 ini registration

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void ExportWavetouch(CWFExam* pWFExam, const Matrix<uchar>& Mem)
{
	const char LF = (char)0x0a; // line feed
	const char CR = (char)0x0d; // carriage return

	if (pWFExam->m_Image.m_le_ok == FALSE)
	{
		::Error("Can't export exam because no lens dots found.");
		return;
	}

	CPatient* pPatient = ::DB.GetPatient(pWFExam->m_Header.m_PatientID);

	CString FolderName;

	CINIParser IniPar;
	FolderName = IniPar.GetStrKey(IniSection, "WaveTouch Folder", IniPath);//6.2.0 ini registration

	if (!::PathFileExists(FolderName))
	{
		FolderName = MAIN_DIR + "\\WaveTouch Lens Orders";
		IniPar.SetStrKey(IniSection, "WaveTouch Folder", FolderName, IniPath);//6.2.0 ini registration
		::CreateDirectory(FolderName, NULL);
	}

	CString ShortFileName(pPatient->m_LastName + pPatient->m_FirstName.Left(1) + pWFExam->m_Header.Eye());
	CString s;
	s.Format(_T("%02i%02i%i"), pWFExam->m_Header.m_Month, pWFExam->m_Header.m_Day, pWFExam->m_Header.m_Year);
	ShortFileName += s;

	ShortFileName.Replace(_T("\\"), _T("_"));
	ShortFileName.Replace(_T("/"), _T("_"));
	ShortFileName.Replace(_T(":"), _T("_"));
	ShortFileName.Replace(_T("*"), _T("_"));
	ShortFileName.Replace(_T("?"), _T("_"));
	ShortFileName.Replace(_T("\""), _T("_"));
	ShortFileName.Replace(_T("<"), _T("_"));
	ShortFileName.Replace(_T(">"), _T("_"));
	ShortFileName.Replace(_T("|"), _T("_"));
	ShortFileName.Replace(_T(","), _T("_"));

	CFileDialog* pDlg = new CFileDialog(FALSE, _T("zip"), ShortFileName, OFN_OVERWRITEPROMPT, _T("ZIP files (*.zip)|*.zip||"), NULL);
	pDlg->m_ofn.lpstrInitialDir = FolderName;

	CString FullFileName;

	BOOL Res = FALSE;
	if (pDlg->DoModal() == IDOK)
	{
		FullFileName = pDlg->GetPathName();
		ShortFileName = pDlg->GetFileName();
		int i = FullFileName.Find(ShortFileName);
		FolderName = FullFileName.Left(i - 1);
		IniPar.SetStrKey(IniSection, "WaveTouch Folder", FolderName, IniPath);//6.2.0 ini registration
		Res = TRUE;
	}
	delete pDlg;

	if (!Res) return;

	HZIP hZip = ::CreateZip(FullFileName, "");

	CString PngShortFileName(ShortFileName.Left(ShortFileName.GetLength() - 3) + "png");
	::ZipAdd(hZip, PngShortFileName, Mem.GetMem(), Mem.GetSize());

	char* Txt = new char[12288];

	CString LensID = pPatient->m_CustomID + (pWFExam->m_Header.m_Eye == EYE_OS ? "2" : "1");
	CString Date;
	Date.Format(_T("%02i%02i%02i"), pWFExam->m_Header.m_Month, pWFExam->m_Header.m_Day, pWFExam->m_Header.m_Year % 100);
	LensID += Date;

	CStringA LensIDA(LensID);//6.2.1

	int k = sprintf(Txt, "LensID: %s", LensIDA);//6.2.1 from LensID to LensIDA
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;

	k += sprintf(Txt + k, "Scan Size: %4.2f um", pWFExam->m_WfSurface.m_r_max_um);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;

	k += sprintf(Txt + k, "Zernike values(0-27): ");
	Txt[k++] = CR; Txt[k++] = LF;

	for (int z = 0; z < 28; z++)
	{
		k += sprintf(Txt + k, "%13.10f", pWFExam->m_WfSurface.GetCUm(z));
		Txt[k++] = CR; Txt[k++] = LF;
	}

	Txt[k++] = CR; Txt[k++] = LF;
	k += sprintf(Txt + k, "Lens rotation angel: %3i", intRound(pWFExam->m_Image.GetLensRotationAngle()));
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;

	real xDis = pWFExam->m_Image.GetLensX0Um() - pWFExam->m_Image.m_pu_x0_um;
	real yDis = pWFExam->m_Image.GetLensY0Um() - pWFExam->m_Image.m_pu_y0_um;

	real Dis = sqrt(xDis*xDis + yDis*yDis);

	k += sprintf(Txt + k, "Lens center to Pupil center: (x: %5.2fum y: %5.2fum)", xDis, yDis);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;

	k += sprintf(Txt + k, "Distance from Lens center to Pupil center: %5.2fum", Dis);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;

	//Dots
	for (int i = 0; i < 3; i++)
	{
		xDis = pWFExam->m_Image.m_le_x_um[i] - pWFExam->m_Image.m_pu_x0_um;
		yDis = pWFExam->m_Image.m_le_y_um[i] - pWFExam->m_Image.m_pu_y0_um;

		Dis = sqrt(xDis*xDis + yDis*yDis);

		k += sprintf(Txt + k, "Dot %i to Pupil center: (x: %5.2fum y: %5.2fum)", i, xDis, yDis);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;

		k += sprintf(Txt + k, "Distance from Dot %i to Pupil center: %5.2fum", i, Dis);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;
	}
	//Dots

	xDis = pWFExam->m_Image.GetLensX0Um() - pWFExam->m_Image.m_ve_x_um;
	yDis = pWFExam->m_Image.GetLensY0Um() - pWFExam->m_Image.m_ve_y_um;

	Dis = sqrt(xDis*xDis + yDis*yDis);

	k += sprintf(Txt + k, "Lens center to Visual axis: (x: %5.2fum y: %5.2fum)", xDis, yDis);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;
	k += sprintf(Txt + k, "Distance from Lens center to Visual axis: %5.2fum", Dis);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;


	//Dots
	for (int i = 0; i < 3; i++)
	{
		xDis = pWFExam->m_Image.m_le_x_um[i] - pWFExam->m_Image.m_ve_x_um;
		yDis = pWFExam->m_Image.m_le_y_um[i] - pWFExam->m_Image.m_ve_y_um;

		Dis = sqrt(xDis*xDis + yDis*yDis);

		k += sprintf(Txt + k, "Dot %i to Visual axis: (x: %5.2fum y: %5.2fum)", i, xDis, yDis);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;

		k += sprintf(Txt + k, "Distance from Dot %i to Visual axis: %5.2fum", i, Dis);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;
	}
	//Dots


	k += sprintf(Txt + k, "Lens Power: %.2f", pWFExam->m_WavetouchLensPower);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;
	k += sprintf(Txt + k, "Lens Base Curve: %i", pWFExam->m_WavetouchLensBaseCurve);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;

	//6.2.1
	CStringA LastNameA(pPatient->m_LastName);
	CStringA FirstNameA(pPatient->m_FirstName);
	CStringA DateA(Date);
	CStringA CustomIDA(pPatient->m_CustomID);
	CStringA EyeA(pWFExam->m_Header.Eye());

	k += sprintf(Txt + k, "Patient Name: %s %s", LastNameA, FirstNameA);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;
	k += sprintf(Txt + k, "Exam date: %s", DateA);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;
	k += sprintf(Txt + k, "Custom ID: %s", CustomIDA);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;
	k += sprintf(Txt + k, "OD/OS: %s", EyeA);
	Txt[k++] = CR; Txt[k++] = LF;
	Txt[k++] = CR; Txt[k++] = LF;

	CClinic* pClinic = ::DB.GetClinic(pWFExam->m_Header.m_ClinicID);
	if (pClinic)
	{
		CStringA ClinicNameA(pClinic->m_Name);
		CStringA AddLine1A(pClinic->m_Address.m_Line1);
		CStringA AddLine2A(pClinic->m_Address.m_Line2);
		CStringA AddCityA(pClinic->m_Address.m_City);
		CStringA AddStateA(pClinic->m_Address.m_State);
		CStringA AddZipA(pClinic->m_Address.m_ZIP);

		k += sprintf(Txt + k, "Clinic: %s", ClinicNameA);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;
		k += sprintf(Txt + k, "Add: %s", AddLine1A);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;
		k += sprintf(Txt + k, "%s", AddLine2A);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;
		k += sprintf(Txt + k, "%s", AddCityA);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;
		k += sprintf(Txt + k, "%s", AddStateA);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;
		k += sprintf(Txt + k, "%s", AddZipA);
		Txt[k++] = CR; Txt[k++] = LF;
		Txt[k++] = CR; Txt[k++] = LF;
	}
	//6.2.1

	CString TxtShortFileName(ShortFileName.Left(ShortFileName.GetLength() - 3) + "txt");
	::ZipAdd(hZip, TxtShortFileName, Txt, k);

	delete[] Txt;

	::CloseZipZ(hZip);

	::Info("Order exported successfully.");
}
//***************************************************************************************
