//***************************************************************************************

#include "StdAfx.h"
#include <ntddscsi.h>
#include <Iphlpapi.h>
#include "Registry.h"
#include "INIParser.h" //6.2.0 ini registration

#include "Licensing.h"
#include "interfaces/ITraceyDicom.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CLicensing::GenerateTable()
{
  FILE *pFile = fopen("D:\\Table.txt", "wt");
  BOOL  b[120];

  memset(b, 0, sizeof(b));

  srand((uint)time(NULL));

  int k = 0;

  while (k < 120) {
    int r = 120 * rand() / RAND_MAX;
    r %= 120;
    if (!b[r]) {
      if (k % 10 == 0)
        fprintf(pFile, "\n");
      fprintf(pFile, "%3i, ", r);
      b[r] = TRUE;
      k++;
    }
  }

  fclose(pFile);
}

//***************************************************************************************

BOOL CLicensing::GetHardwareID()
{
  m_HardwareID.Empty();

  CString s;

  if (!GetHardwareString1(s)) {
    if (!GetHardwareString2(s)) {
      ::Error("Failed to obtain hardware ID.");
      return FALSE;
    }
  }

  uint T[20];
  memset(T, 0, sizeof(T));

  for (int i = 0; i < s.GetLength(); i++) {
    T[i % 20] += (int)s[i];
  }

  for (int i = 0; i < 20; i++) {
    uint b = T[i] % 36;
    char c = b < 10 ? '0' + (char)b : 'A' + (char)(b - 10);
    m_HardwareID += c;
  }

  return TRUE;
}

//***************************************************************************************

BOOL CLicensing::LoadSoftwareAK()
{
  m_SoftwareAK.Empty();

  // 6.2.0 ini registration
  CString    keyStr = "Software Activation Key 6.0.0";
  CINIParser IniPar;
  m_SoftwareAK = IniPar.GetStrKey(IniSection, keyStr, IniPath);

  return (m_SoftwareAK == "" ? FALSE : TRUE);
}

//***************************************************************************************

BOOL CLicensing::SaveSoftwareAK()
{
  CINIParser IniPar;
  IniPar.SetStrKey(IniSection, "Software Activation Key 6.0.0", m_SoftwareAK, IniPath); // 6.2.0 ini registration

  return TRUE;
}

//***************************************************************************************

CString CLicensing::LicenseName() const
{
  switch (m_LicenseType) {
  case LT_PRESBIA:
    return "P"; // Presbia
  case LT_DEMO:
    return "D"; // Demo
  case LT_SOLO:
    return "S";
  case LT_COMBO:
    return "C";
  case LT_WORKSTATION:
    return "W";
  case LT_HOYA:
    return "H";
  case LT_HOYAVIEWER:
    return "HSV"; // HOYA Workstataion Viewer
  case LT_COMBOVIEWER:
    return "COMBOV"; // COMBO Viewer
  case LT_COMBODICOM:
    return "CD"; // COMBO DICOM
  }

  return "";
}

//***************************************************************************************

BOOL CLicensing::DetermineLicenseType(const CString &SoftwareAK)
{
  if (!DetermineLicenseTypeImpl(SoftwareAK))
    return FALSE;

  if (m_LicenseType == LT_COMBODICOM) {
    try {
      auto obj = CreateObj<ITraceyDicomConfigRZDCX>();
      obj->Activate();
    }
    catch (const std::exception &e) {
      Error(e.what());
    }
  }

  return TRUE;
}

//***************************************************************************************

BOOL CLicensing::DetermineLicenseTypeImpl(const CString &SoftwareAK)
{
  // m_LicenseType = LT_COMBODICOM;// LT_COMBO;
  // return TRUE;

  // m_LicenseType = LT_COMBO;
  // return TRUE;

  m_LicenseType = LT_NONE;

  if (m_HardwareID.IsEmpty())
    return FALSE;

  CString tSoftwareAK(SoftwareAK);

  if (tSoftwareAK.IsEmpty()) {
    tSoftwareAK = m_SoftwareAK;
  }
  else {
    tSoftwareAK.MakeUpper();
  }

  int LicenseType[9] = {LT_PRESBIA, LT_DEMO, LT_SOLO, LT_COMBO, LT_WORKSTATION, LT_HOYA, LT_HOYAVIEWER, LT_COMBOVIEWER, LT_COMBODICOM};

#if defined(_DEBUG) && 0
  m_LicenseType = LT_SOLO;
  return TRUE;
#endif

  for (int i = 0; i < 9; i++) {
    CString gSoftwareAK;

    if (GenerateSoftwareAK(LicenseType[i], gSoftwareAK)) {
      if (tSoftwareAK == gSoftwareAK) {
        if (m_SoftwareAK != tSoftwareAK) {
          m_SoftwareAK = tSoftwareAK;
          SaveSoftwareAK();
        }

        m_LicenseType = LicenseType[i];

        return TRUE;
      }
      else {
        // If there is only one different letter consider it is correct key
        int len1 = tSoftwareAK.GetLength();
        int len2 = gSoftwareAK.GetLength();

        if (len1 == len2) {
          char *pValue1 = new char[len1];
          G_As          = tSoftwareAK;
          strncpy(pValue1, G_As, len1);

          char *pValue2 = new char[len1];
          G_As          = gSoftwareAK;
          strncpy(pValue2, G_As, len1);

          int t = 0;
          for (int j = 0; j < len1; j++) {
            char c1 = *(pValue1 + j);
            char c2 = *(pValue2 + j);

            if (c1 != c2)
              t++;

            if (t > 1)
              break;
          }

          if (t == 1) {
            //::Info("I'm here");
            if (m_SoftwareAK != tSoftwareAK) {
              m_SoftwareAK = tSoftwareAK;
              SaveSoftwareAK();
            }
            m_LicenseType = LicenseType[i];
            return TRUE;
          }
        }
      }
    }
  }

  return FALSE;
}

//***************************************************************************************

BOOL CLicensing::GenerateSoftwareAK(const int LicenseType, CString &SoftwareAK)
{
  // ote: The q array number can not greater than 120.
  const int q[9][120] = {
    //  SOLO
    {39, 49, 83, 46, 88, 3,   22, 91,  17, 64, 45, 10, 98,  60, 110, 10,  32, 33, 2,  20, 0,  99, 15, 11, 103, 42,  73, 37,  99,  57,
     58, 69, 52, 5,  75, 89,  48, 20,  10, 63, 17, 51, 106, 63, 73,  18,  92, 66, 95, 43, 22, 20, 80, 73, 104, 96,  33, 21,  103, 24,
     17, 82, 7,  35, 96, 117, 84, 102, 67, 15, 74, 8,  109, 49, 39,  104, 22, 87, 99, 8,  54, 53, 58, 57, 79,  39,  85, 34,  49,  81,
     71, 83, 59, 14, 30, 25,  31, 78,  95, 28, 13, 64, 58,  23, 99,  14,  7,  44, 66, 54, 45, 51, 13, 36, 73,  108, 92, 116, 95,  43},
    //  COMBO
    {29, 21,  34, 34, 113, 59, 14,  66,  7,  93, 92, 70, 93, 74, 55,  85, 17, 19, 107, 86, 64, 56,  28, 89, 6,   21,  53, 4,  14,  84,
     77, 57,  94, 27, 56,  28, 62,  114, 42, 39, 18, 55, 10, 38, 113, 86, 54, 99, 42,  4,  15, 19,  10, 38, 93,  100, 87, 99, 42,  4,
     29, 110, 39, 47, 111, 58, 107, 119, 67, 68, 97, 43, 34, 7,  33,  71, 35, 62, 15,  5,  27, 26,  48, 61, 101, 20,  2,  57, 115, 81,
     15, 59,  73, 22, 69,  3,  95,  118, 49, 90, 29, 17, 76, 8,  18,  73, 25, 69, 52,  52, 31, 105, 63, 89, 101, 19,  25, 84, 38,  77},
    // WORKSTATION
    {31, 32,  8,  61,  16,  89, 109, 36,  0,   63, 64, 108, 23, 74, 60, 103, 104, 85, 112, 55, 13, 45,  3,  98,  101, 91, 105, 88,  83, 97,
     29, 110, 39, 47,  111, 58, 107, 119, 67,  68, 79, 34,  43, 70, 44, 17,  53,  26, 51,  50, 46, 65,  82, 9,   106, 12, 35,  40,  41, 48,
     77, 57,  94, 27,  56,  28, 62,  14,  24,  30, 15, 19,  10, 38, 93, 100, 87,  99, 42,  4,  78, 5,   71, 33,  113, 86, 54,  114, 72, 66,
     7,  6,   84, 116, 21,  11, 102, 75,  115, 81, 2,  59,  73, 22, 69, 76,  95,  18, 49,  90, 92, 117, 80, 118, 1,   37, 52,  96,  25, 20},
    // HOYA
    {
      102, 88, 20,  37, 40, 69, 44, 112, 33, 64,  111, 67, 53,  97,  23,  80, 96, 119, 114, 39,  74, 8,  103, 94,  93, 104, 22, 78, 99, 108,
      16,  51, 18,  0,  30, 47, 61, 60,  95, 113, 36,  83, 101, 57,  115, 42, 5,  82,  28,  59,  24, 2,  71,  15,  84, 75,  41, 81, 50, 4,
      56,  14, 107, 7,  91, 27, 52, 100, 65, 116, 11,  46, 72,  86,  21,  58, 87, 38,  9,   92,  77, 12, 10,  110, 3,  79,  73, 68, 70, 98,
      1,   55, 6,   76, 19, 26, 48, 66,  32, 85,  109, 25, 54,  105, 106, 63, 35, 117, 17,  118, 89, 62, 31,  90,  34, 29,  43, 49, 13, 45,
    },
    // DEMO
    {116, 118, 17, 91, 109, 46, 78,  56, 55,  77,  35, 33, 60,  20,  36,  16,  4,  43, 12, 24, 54, 38, 94,  65, 68,  29,  64, 8,  5,   76,
     47,  112, 28, 31, 2,   19, 69,  41, 42,  95,  40, 0,  119, 106, 93,  48,  37, 84, 15, 71, 47, 96, 111, 89, 104, 10,  22, 80, 102, 1,
     41,  18,  52, 75, 66,  63, 115, 90, 47,  30,  43, 6,  107, 72,  11,  110, 61, 88, 59, 25, 43, 14, 74,  32, 105, 100, 40, 82, 97,  7,
     47,  103, 99, 83, 54,  9,  70,  13, 117, 113, 42, 62, 98,  44,  101, 108, 86, 26, 3,  21, 15, 53, 58,  57, 79,  39,  85, 34, 49,  81},
    // PRESBIA
    {113, 44, 95,  32,  99, 4,  70, 33, 86, 45,  20, 54,  109, 24, 112, 103, 91, 41, 53, 114, 1,   28, 97, 52,  69, 117, 104, 48,  76,  51,
     85,  96, 25,  50,  57, 98, 84, 10, 74, 36,  79, 26,  119, 2,  56,  35,  61, 67, 47, 16,  107, 15, 31, 63,  37, 118, 29,  106, 59,  34,
     101, 71, 115, 108, 88, 30, 6,  13, 58, 110, 93, 94,  38,  64, 8,   83,  11, 19, 77, 46,  55,  90, 68, 116, 0,  82,  22,  3,   92,  80,
     49,  78, 65,  21,  60, 72, 43, 27, 89, 18,  9,  100, 66,  81, 105, 14,  7,  73, 5,  75,  87,  39, 62, 23,  42, 17,  40,  12,  111, 102},
    // HOYA Workstation Viewer
    {
      20, 54, 109, 24,  112, 103, 91, 41, 53, 114, 113, 44, 95,  32,  99, 4,  70, 33, 86,  45,  23, 14,  74,  32, 105, 100, 40, 82, 97, 7,
      97, 43, 34,  7,   33,  71,  35, 62, 15, 5,   50,  0,  119, 106, 93, 48, 37, 84, 15,  71,  79, 26,  119, 2,  56,  35,  61, 67, 47, 16,
      93, 94, 38,  64,  8,   83,  11, 19, 77, 46,  101, 71, 115, 108, 88, 30, 6,  13, 58,  110, 49, 78,  65,  21, 60,  72,  43, 27, 89, 18,
      55, 90, 68,  116, 0,   82,  22, 3,  92, 80,  87,  39, 62,  23,  42, 17, 40, 12, 111, 102, 9,  100, 66,  81, 105, 14,  7,  73, 5,  75,
    },
    // COMBO Workstation Viewer
    {
      107, 15, 31, 63, 37, 118, 29,  106, 59, 34, 93,  94, 38,  64, 8,  83,  11, 19,  77, 46,  85, 96, 25, 50, 57,  98, 84, 10, 74, 36,
      1,   28, 97, 52, 69, 117, 104, 48,  76, 51, 107, 15, 31,  63, 37, 118, 29, 106, 59, 34,  45, 53, 58, 57, 79,  39, 85, 34, 49, 81,
      102, 88, 20, 37, 40, 69,  44,  112, 33, 64, 74,  8,  103, 94, 93, 104, 22, 78,  99, 108, 19, 38, 95, 41, 30,  52, 13, 87, 59, 28,
      45,  10, 98, 16, 20, 110, 32,  13,  2,  20, 113, 44, 95,  32, 99, 4,   70, 33,  86, 45,  99, 0,  5,  1,  103, 24, 73, 73, 55, 75,
    },

    // COMBO Dicom
    {
      45, 51, 13, 36, 73,  108, 92, 116, 95,  43, 39, 49, 83,  46, 88, 3,   22, 91, 17, 64, 58, 69, 52, 5,  75,  89, 48, 20, 10, 63,
      17, 82, 7,  35, 96,  117, 84, 102, 67,  15, 17, 51, 106, 63, 73, 18,  92, 66, 95, 43, 54, 53, 58, 57, 79,  39, 85, 34, 49, 81,
      22, 20, 80, 73, 104, 96,  33, 21,  103, 24, 74, 8,  109, 49, 39, 104, 22, 87, 99, 8,  71, 83, 59, 14, 30,  25, 31, 78, 95, 28,
      45, 10, 98, 60, 110, 10,  32, 33,  2,   20, 13, 64, 58,  23, 99, 14,  7,  44, 66, 54, 0,  99, 15, 11, 103, 42, 73, 37, 99, 57,
    }};

  SoftwareAK.Empty();

  int l;
  switch (LicenseType) {
  case LT_SOLO:
    l = 0;
    break;
  case LT_COMBO:
    l = 1;
    break;
  case LT_WORKSTATION:
    l = 2;
    break;
  case LT_HOYA:
    l = 3;
    break;
  case LT_DEMO:
    l = 4;
    break;
  case LT_PRESBIA:
    l = 5;
    break;
  case LT_HOYAVIEWER:
    l = 6;
    break;
  case LT_COMBOVIEWER:
    l = 7;
    break;
  case LT_COMBODICOM:
    l = 8;
    break;
  default:
    return FALSE;
  }

  if (m_HardwareID.GetLength() != 20)
    return FALSE;

  byte bits1[120];

  for (int i = 0; i < 20; i++) {
    char c = m_HardwareID[i];
    byte b = 0;

    if ('0' <= c && c <= '9') {
      b = (byte)((c - '0') + 10);
    }
    if ('A' <= c && c <= 'Z') {
      b = (byte)((c - 'A') + 20);
    }

    for (int j = 0; j < 6; j++) {
      bits1[6 * i + j] = (b >> j) & 1;
    }
  }

  byte bits2[120];

  for (int i = 0; i < 120; i++) {
    bits2[i] = bits1[q[l][i]];
  }

  for (int i = 0; i < 20; i++) {
    byte b = 0;
    for (int j = 0; j < 6; j++) {
      b |= (bits2[6 * i + j] << (5 - j));
    }

    b %= 36;
    char c = b < 10 ? '0' + (char)b : 'A' + (char)(b - 10);
    SoftwareAK += c;
  }

  return TRUE;
}

//***************************************************************************************

#define IOCTL_STORAGE_QUERY_PROPERTY CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

//***************************************************************************************

typedef struct _IDSECTOR
{
  ushort wGenConfig;
  ushort wNumCyls;
  ushort wReserved;
  ushort wNumHeads;
  ushort wBytesPerTrack;
  ushort wBytesPerSector;
  ushort wSectorsPerTrack;
  ushort wVendorUnique[3];
  uchar  sSerialNumber[20];
  ushort wBufferType;
  ushort wBufferSize;
  ushort wECCSize;
  uchar  sFirmwareRev[8];
  uchar  sModelNumber[40];
  ushort wMoreVendorUnique;
  ushort wDoubleWordIO;
  ushort wCapabilities;
  ushort wReserved1;
  ushort wPIOTiming;
  ushort wDMATiming;
  ushort wBS;
  ushort wNumCurrentCyls;
  ushort wNumCurrentHeads;
  ushort wNumCurrentSectorsPerTrack;
  ulong  ulCurrentSectorCapacity;
  ushort wMultSectorStuff;
  ulong  ulTotalAddressableSectors;
  ushort wSingleWordDMA;
  ushort wMultiWordDMA;
  uchar  bReserved[127];

} IDSECTOR;

//***************************************************************************************

BOOL CLicensing::GetHardDriveString(CString &s)
{
  s.Empty();

  for (int i = 0; i < 16; i++) {
    CString Name;
    Name.Format(_T("\\\\.\\PhysicalDrive%i"), i);

    HANDLE hDrive = ::CreateFile(Name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDrive == INVALID_HANDLE_VALUE)
      continue;

    ulong bt;

    SENDCMDINPARAMS * pInParams;
    SENDCMDOUTPARAMS *pOutParams;

    uchar Buffer[1024];
    memset(Buffer, 0, sizeof(Buffer));

    pInParams  = (SENDCMDINPARAMS *)Buffer;
    pOutParams = (SENDCMDOUTPARAMS *)Buffer;

    pInParams->irDriveRegs.bCommandReg = 0xec; // ID_CMD or ATAPI_ID_CMD

    if (::DeviceIoControl(hDrive, SMART_RCV_DRIVE_DATA, Buffer, sizeof(Buffer), Buffer, sizeof(Buffer), &bt, NULL)) {
      IDSECTOR *pData = (IDSECTOR *)pOutParams->bBuffer;

      CString Model;

      for (int k = 0; k < 20; k++) {
        Model += (char)pData->sModelNumber[2 * k + 1];
        Model += (char)pData->sModelNumber[2 * k];
      }

      Model.TrimLeft();
      Model.TrimRight();

      CString Serial;

      for (int k = 0; k < 10; k++) {
        Serial += (char)pData->sSerialNumber[2 * k + 1];
        Serial += (char)pData->sSerialNumber[2 * k];
      }

      Serial.TrimLeft();
      Serial.TrimRight();

      s = Model + ' ' + Serial;

      ::CloseHandle(hDrive);
      return TRUE;
    }

    ::CloseHandle(hDrive);
  }

  return FALSE;
}

//***************************************************************************************

BOOL CLicensing::GetSystemBIOSString(CString &S)
{
  CString       S1, S2;
  CTraceyRegKey Key(HKEY_LOCAL_MACHINE, "Hardware\\Description\\System");

  if (!Key.GetStrValue("SystemBiosDate", S1))
    return FALSE;

  if (!Key.GetStrValue("SystemBiosVersion", S2))
    return FALSE;

  S = S1 + " " + S2;
  return TRUE;
}

//***************************************************************************************

BOOL CLicensing::GetVideoBIOSString(CString &S)
{
  CString       S1, S2;
  CTraceyRegKey Key(HKEY_LOCAL_MACHINE, "Hardware\\Description\\System");

  if (!Key.GetStrValue("VideoBiosDate", S1))
    return FALSE;

  if (!Key.GetStrValue("VideoBiosVersion", S2))
    return FALSE;

  S = S1 + " " + S2;
  return TRUE;
}

//***************************************************************************************

BOOL CLicensing::GetProcessorString(CString &S)
{
  CString       S1, S2;
  CTraceyRegKey Key(HKEY_LOCAL_MACHINE, "Hardware\\Description\\System\\CentralProcessor\\0");

  if (!Key.GetStrValue("ProcessorNameString", S1))
    return FALSE;

  if (!Key.GetStrValue("Identifier", S2))
    return FALSE;

  S = S1 + " " + S2;
  return TRUE;
}

//***************************************************************************************

BOOL CLicensing::GetHardwareString1(CString &s)
{
  s.Empty();

  CString s1, s2, s3, s4;

  int NumGood = 0;

  CINIParser IniPar;
  s1 = IniPar.GetStrKey(IniSection, "S1", IniPath); // 6.2.0 ini registration

  if (s1.IsEmpty()) {
    GetHardDriveString(s1);
    IniPar.SetStrKey(IniSection, "S1", s1, IniPath); // 6.2.0 ini registration
  }

  s2 = IniPar.GetStrKey(IniSection, "S2", IniPath); // 6.2.0 ini registration
  if (s2.IsEmpty()) {
    GetSystemBIOSString(s2);
    IniPar.SetStrKey(IniSection, "S2", s2, IniPath); // 6.2.0 ini registration
  }

  s3 = IniPar.GetStrKey(IniSection, "S3", IniPath); // 6.2.0 ini registration
  if (s3.IsEmpty()) {
    GetVideoBIOSString(s3);
    IniPar.SetStrKey(IniSection, "S3", s3, IniPath); // 6.2.0 ini registration
  }

  s4 = IniPar.GetStrKey(IniSection, "S4", IniPath); // 6.2.0 ini registration
  if (s4.IsEmpty()) {
    GetProcessorString(s4);
    IniPar.SetStrKey(IniSection, "S4", s4, IniPath); // 6.2.0 ini registration
  }

  CTraceyRegKey Key_Read(HKEY_LOCAL_MACHINE, MAIN_REG_KEY);
  CString       FirstTime;
  Key_Read.GetStrValue("Inst600", FirstTime); // 6.2.0 ini registration

  if (!s1.IsEmpty())
    NumGood++;
  if (!s2.IsEmpty())
    NumGood++;
  if (!s3.IsEmpty())
    NumGood++;
  if (!s4.IsEmpty())
    NumGood++;

  if (NumGood < 3)
    return FALSE;

  s = s1 + " " + s2 + " " + s3 + " " + s4 + " " + FirstTime;

  return TRUE;
}

//***************************************************************************************

#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>

//***************************************************************************************

BOOL CLicensing::GetHardwareString2(CString &s)
{
  s.Empty();

  IWbemLocator * pLoc = NULL;
  IWbemServices *pSvc = NULL;

  ::CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

  ::CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);

  pLoc->ConnectServer(BSTR(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);

  ::CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

  IEnumWbemClassObject *pEnumerator = NULL;

  pSvc->ExecQuery(BSTR(L"WQL"), BSTR(L"SELECT * FROM Win32_BIOS"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

  VARIANT v;

  while (pEnumerator) {
    IWbemClassObject *pClassObject = NULL;

    ULONG uReturn = 0;

    if (pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn) != WBEM_S_NO_ERROR)
      break;

    if (uReturn == 0)
      break;

    VariantInit(&v);

    if (pClassObject->Get(L"Name", 0, &v, 0, 0) == WBEM_S_NO_ERROR) {
      int l = wcslen(v.bstrVal);
      for (int i = 0; i < l; i++) {
        s += (char)v.bstrVal[i];
        v.bstrVal[i] = 0;
      }
    }
    VariantClear(&v);

    if (pClassObject->Get(L"Manufacturer", 0, &v, 0, 0) == WBEM_S_NO_ERROR) {
      int l = wcslen(v.bstrVal);
      if (l > 0) {
        if (!s.IsEmpty())
          s += ' ';

        for (int i = 0; i < l; i++) {
          s += (char)v.bstrVal[i];
          v.bstrVal[i] = 0;
        }
      }
    }
    VariantClear(&v);

    if (pClassObject->Get(L"Version", 0, &v, 0, 0) == WBEM_S_NO_ERROR) {
      int l = wcslen(v.bstrVal);

      if (l > 0) {
        if (!s.IsEmpty())
          s += ' ';
        for (int i = 0; i < l; i++) {
          s += (char)v.bstrVal[i];
          v.bstrVal[i] = 0;
        }
      }
    }

    VariantClear(&v);

    if (pClassObject->Get(L"SerialNumber", 0, &v, 0, 0) == WBEM_S_NO_ERROR) {
      int l = wcslen(v.bstrVal);
      if (l > 0) {
        if (!s.IsEmpty())
          s += ' ';

        for (int i = 0; i < l; i++) {
          s += (char)v.bstrVal[i];
          v.bstrVal[i] = 0;
        }
      }
    }

    VariantClear(&v);

    pClassObject->Release();
  }

  pEnumerator->Release();

  pSvc->Release();
  pLoc->Release();

  CString       FirstTime;
  CTraceyRegKey Key(HKEY_LOCAL_MACHINE, MAIN_REG_KEY);
  Key.GetStrValue("Inst600", FirstTime);

  s += FirstTime;

  return TRUE;
}

//***************************************************************************************
