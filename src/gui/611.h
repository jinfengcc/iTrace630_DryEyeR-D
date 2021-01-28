1. Make the text color of settings dialog, IOL settings dialog , new patient dialog darker
2. The OnCtrlE for edit dialog(transform ID/IS, move exam to another patient) crash problem
3. The '2.5mm' alert for wf exam between 2nd shot
4. The activation key problem
5. ...some other bugs
6. The color of 0.175u for complain
7. delete all setting files ans show the default setting




for "
if (::RegCreateKeyEx(ParentKey, KeyName, 0, NULL, REG_OPTION_NON_VOLATILE,
	KEY_READ | KEY_WRITE, NULL, &m_Key, &disp) != ERROR_SUCCESS)" 

	works need run vs 'run as administrator'


Dialog CTRL+T shows the GUI
CTRL+D the order

//-1. find a substring
count = s.Left(i).Find(_T("Alpha", 0);




0.::Licensing.m_LicenseType == LT_HOYA 

real_t t = pRMSWnd->m_Surface.ComputeRms();
should be another function GetMaxRms????????????10152014


'Ctrl + d' Can be used to change the tap order in dialog

µ °

1. m_MemDC.FillSolidRect(&m_LabelRect[0], BLACK);

2. PreTranslateMessage

3. DC.MeasureRect("0000  ", Rect, *m_pFont);
m_MemDC.MeasureRect("0000  ", Rect, *m_pFont);

m_MemDC.MeasureRect("+00.00 D ", Rect, Font);

4. m_ShowScanBtn.SetEnabled(FALSE);

5. 
   LPCRECT prct1
   prct1a = &m_BR[0][9];
   m_WFSumButton[2].MoveWindow(prct1a,1);
   m_WFSumButton[2].MoveWindow(&m_BR[0][9],1);//?

6. RECT rect;
   ::SetRect(&rect, xLeft, yTop, xRight, yBottom);

6.1 
   pTasbarWnd->GetWindowRect(&rect);

7. InvalidateRect(&m_LabelRect[i], FALSE);

8. ::Sleep(600);  

9. m_FontSize = intRound(1.4 * Half_OnoffHeight);

	CMFont Font(m_FontSize, 400, "Arial");

10.  CString FileName =  "C:\\Tracey\\Data\\MSJetEx\\c.zip";

	 if(::PathFileExists(FileName) )

11.  FILE* pFilew = fopen(DemoFileName, "w");			   
				fprintf(pFilew, "%axyzejdou_8%@\n"); 					
				fclose(pFilew);

12.   ::CreateDirectory(m_RecordFileDir, NULL);

13. //Delete files in a folder 

DeleteDirectory((LPSTR)(LPCTSTR)foldername);

    BOOL   CMainWnd::DeleteDirectory(char* sDirName)
{
	CFileFind tempFind; 
    char sTempFileFind[200] ;
    
     sprintf(sTempFileFind,"%s\\*.*",sDirName); 
     BOOL IsFinded = tempFind.FindFile(sTempFileFind);  
    while (IsFinded) 
    { 
		IsFinded = tempFind.FindNextFile(); 
        
        if (!tempFind.IsDots()) 
        { 
            char sFoundFileName[200]; 
             strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 
            
            if (tempFind.IsDirectory()) 
           { 
                char sTempDir[200]; 
                 sprintf(sTempDir,"%s\\%s",sDirName,sFoundFileName); 
                 DeleteDirectory(sTempDir); 
             } 
            else 
            { 
                char sTempFileName[200]; 
                 sprintf(sTempFileName,"%s\\%s",sDirName,sFoundFileName); 
                 DeleteFile(sTempFileName); 
             } 
         } 
     } 
     tempFind.Close(); 
    if(!RemoveDirectory(sDirName)) 
	{ 
        return FALSE; 
     } 
    return TRUE; 
}


14. GUID_NULL

15. CPoint point; 
	GetCursorPos(&point);



16. /* CClientDC dc(this);
  dc.MoveTo(rectL.left, rectL.top);
  dc.LineTo(rectR.right, rectR.bottom);
  dc.MoveTo(rectL.right, rectL.top);
  dc.LineTo(rectR.left, rectR.bottom);
  dc.MoveTo(rectL.left + m_colume / 2, rectL.top + m_row / 2);*/
  //


GUID CDB::LoadFellowID

m_Image.FindVertexAndRings();//the rings missing

OnESDButtonClicked

//*******************************
Version info:

1). const uint SOFTWARE_VERSION   = ;// V V V V Y M D D in StdAfx.cpp

2). iTrace.rc "VALUE "FileVersion", "6.0.1""

3). BOOL CLicensing::LoadSoftwareAK() and BOOL CLicensing::SaveSoftwareAK()

4). \\settings530

5). "Inst600"

//*******************************

PreTranslateMessage



//How many bytes for different types
BOOL 4 bytes
real_t 8 bytes
//Done
//
//Some info about DICOM,
//1. The DICOM used the open source library 'DCMTK'
//2. The debug and release version of the program used different DCMTK library
//3. The debug DCMTK library is under the 'c:/dcmtk/lib/debug'
//4. The relaese DCMTK library is under the 'c:/dcmtk/lib/release' 
//5. Set the library link in the 'Tools->options->vc++ directory -> library files'
//4. Add the folder 'C:\DCMTK\dcmtk-3.5.4-win32-i386\include' to 'Tools->options->vc++ directory -> Included files'


********************************************************************
//*************************//
// How to create the Setup CD
0. Cope new exectuable file 'iTrace510.exe'(release version) to folders setup32 and setup64
1. The setup soluting 'Setup' is used to judge the system is 32 bits or 64 bits
2. Use 'release' to get te executable file setup.exe
3. Save the 'setup.exe' to the 'output' folder inside the folders setup32 and setup64
4. Open the inno file 'iTrace.iss' in two folders seperatly and compile them,
the 'setup.exe' file insdie the 'output' will be the installation file 'setup.exe'.
5. Change the 'setup.exe' file in two 'output' folders as 'setup32.exe' and 'setup64.exe'
6. Burned setup cd as following contents:
   1) 'setup.exe' from C++ solution;
   2) 'autorun.inf'
   3) 'setup32.exe' and 'setup64.ext'
   4) the manual of new version
   5) the new executable file 'iTracexxx.exe' from the code (it is not necessary)
//
// How to create the Setup CD Done
//*************************//
********************************************************************


#define TYPE_TEYE  1//CWFSingleSumWnd(Angle K/A) default 
#define TYPE_TEYC  2
#define TYPE_TSGN  3
#define TYPE_TRSD  4
#define TYPE_TOPM  5
#define TYPE_TWFM 10
#define TYPE_TRFM 11 //Refraction Map
#define TYPE_TPSF 12
#define TYPE_TEEF 13
#define TYPE_TLTR 14
#define TYPE_TMTF 15
#define TYPE_TRMS 16
#define TYPE_TAZR 17
#define TYPE_TANG 18

#define TYPE_CEYE 30
#define TYPE_CAXM 31 //CT: Axial map
#define TYPE_CTNM 32 // local Roc Map
#define TYPE_CRFM 33 // refrective map
#define TYPE_CELM 34 // z elevation
#define TYPE_CWFM 35
#define TYPE_CRMS 36
#define TYPE_CPSF 37 //axial map
#define TYPE_CEEF 38
#define TYPE_CMTF 39
#define TYPE_CLTR 50
#define TYPE_CSKM 51 // Kerotametry map
#define TYPE_CPLN 52

#define TYPE_IWFM 40
#define TYPE_IRMS 41 // RMS
#define TYPE_IPSF 42 // RMS 
#define TYPE_IEEF 43
#define TYPE_IMTF 44
#define TYPE_ILTR 45
#define TYPE_IRFM 46