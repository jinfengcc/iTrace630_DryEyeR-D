//***************************************************************************************

#pragma once

//***************************************************************************************

#define LT_NONE        -1
#define LT_SOLO         0
#define LT_COMBO        1
#define LT_WORKSTATION  2
#define LT_HOYA         3
#define LT_DEMO         4
#define LT_PRESBIA      5
#define LT_HOYAVIEWER   6
#define LT_COMBOVIEWER  7
#define LT_COMBODICOM   8//6.2.0

//***************************************************************************************

class CLicensing
{
public:

	BOOL IsValid()
	{
		return m_LicenseType == LT_SOLO || m_LicenseType == LT_COMBO || m_LicenseType == LT_WORKSTATION || m_LicenseType == LT_HOYA || m_LicenseType == LT_DEMO || m_LicenseType == LT_PRESBIA || m_LicenseType == LT_HOYAVIEWER || m_LicenseType == LT_COMBOVIEWER || m_LicenseType == LT_COMBODICOM; // 530
	}

	BOOL IsViewer()
	{
		return m_LicenseType == LT_HOYAVIEWER;
	}

	BOOL IsDemo()
	{
		return m_LicenseType == LT_DEMO;
	}

	BOOL IsPresbia()
	{
		return m_LicenseType == LT_PRESBIA;
	}

	BOOL IsComboViewer()
	{
		return m_LicenseType == LT_COMBOVIEWER;
	}

	BOOL IsSolo()
	{
		return m_LicenseType == LT_SOLO;
	}

	BOOL IsCombo()
	{
		return m_LicenseType == LT_COMBO;
	}

	BOOL IsComboDICOM()
	{
		return m_LicenseType == LT_COMBODICOM;
	}

	BOOL IsWorkstation()
	{
		return m_LicenseType == LT_WORKSTATION || m_LicenseType == LT_HOYA;
	}

	BOOL IsHoya()
	{
		return m_LicenseType == LT_HOYA;
	}

	int m_LicenseType;
	BOOL DetermineLicenseType(const CString& SoftwareAK);
	CString LicenseName() const;

	CString m_HardwareID;
	BOOL GetHardwareID();
	BOOL GetHardwareString1(CString& s);
	BOOL GetHardwareString2(CString& s);

	BOOL GetHardDriveString(CString& s);
	BOOL GetSystemBIOSString(CString& s);
	BOOL GetVideoBIOSString(CString& s);
	BOOL GetProcessorString(CString& s);

	CString m_SoftwareAK;
	BOOL LoadSoftwareAK();
	BOOL SaveSoftwareAK();
	BOOL GenerateSoftwareAK(const int LicenseType, CString& SoftwareAK);

	void GenerateTable();
};

//***************************************************************************************

extern CLicensing Licensing;

//***************************************************************************************
