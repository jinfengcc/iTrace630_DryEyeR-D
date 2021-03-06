//***************************************************************************************
#include "StdAfx.h"
#include "Image.h"
#include "Sort.h"
#include "resource.h"

// Debug
#include <fstream>

// clang-format off

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

BOOL operator==(const CNode& a, const CNode& b)
{
	return a.m_x == b.m_x && a.m_y == b.m_y;
}

BOOL operator!=(const CNode& a, const CNode& b)
{
	return a.m_x != b.m_x || a.m_y != b.m_y;
}

//***************************************************************************************

CBasicImage::CBasicImage()
{
	m_w = 0;
	m_h = 0;
}

//***************************************************************************************

void CBasicImage::Destroy()
{
	m_w = 0;
	m_h = 0;

	m_RGBData.Destroy();
	m_AData.Destroy();
}

//***************************************************************************************

BOOL CBasicImage::LoadFromRes(int ResourceID)
{
	HRSRC hResInfo = ::FindResource(NULL, MAKEINTRESOURCE(ResourceID), _T("PNG"));

	if (hResInfo == NULL) return FALSE;

	HGLOBAL hRes = ::LoadResource(NULL, hResInfo);

	if (hRes == NULL) return FALSE;

	int ResSize = ::SizeofResource(NULL, hResInfo);

	uchar* pResData = (uchar*)::LockResource(hRes);

	return LoadFromMem(ResSize, pResData);
}

//***************************************************************************************

BOOL CBasicImage::LoadFromSpcRes(int ResourceID, CString SpcType)
{
	HRSRC hResInfo = ::FindResource(NULL, MAKEINTRESOURCE(ResourceID), SpcType);

	if (hResInfo == NULL) return FALSE;

	HGLOBAL hRes = ::LoadResource(NULL, hResInfo);

	if (hRes == NULL) return FALSE;

	int ResSize = ::SizeofResource(NULL, hResInfo);

	uchar* pResData = (uchar*)::LockResource(hRes);

	return LoadFromMem(ResSize, pResData);
}

//***************************************************************************************

BOOL CBasicImage::LoadFromMem(int Size, uchar* pData)
{
	const GUID EncoderBMP = { 0x557CF400, 0x1A04, 0x11D3, 0x9A, 0x73, 0x00, 0x00, 0xF8, 0x1E, 0xF3, 0x2E };

	Destroy();

	HGLOBAL	hMem1 = ::GlobalAlloc(GMEM_MOVEABLE, Size);
	HGLOBAL	hMem2 = ::GlobalAlloc(GMEM_MOVEABLE, 0);

	CComPtr<IStream> pStream = NULL;
	CComPtr<IStream> pBMPStream = NULL;

	uchar* pMem = (uchar*)::GlobalLock(hMem1);

	memcpy(pMem, pData, Size);

	::GlobalUnlock(hMem1);

	if (::CreateStreamOnHGlobal(hMem1, FALSE, &pStream) != S_OK)
	{
		::GlobalFree(hMem1);
		::GlobalFree(hMem2);
		return FALSE;
	}

	if (::CreateStreamOnHGlobal(hMem2, FALSE, &pBMPStream) != S_OK)
	{
		::GlobalFree(hMem1);
		::GlobalFree(hMem2);
		return FALSE;
	}

	Gdiplus::Image Image(pStream);

	Gdiplus::Status Status = Image.Save(pBMPStream, &EncoderBMP);

	if (Status != Gdiplus::Ok)
	{
		::GlobalFree(hMem1);
		::GlobalFree(hMem2);
		return FALSE;
	}

	pMem = (uchar*)::GlobalLock(hMem2);

	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*)(pMem + sizeof(BITMAPFILEHEADER));

	m_w = pBIH->biWidth;
	m_h = pBIH->biHeight;
	m_RGBData.Create(m_h, LINE_SIZE(m_w));

	uchar* p = pMem + sizeof(BITMAPFILEHEADER) + pBIH->biSize;

	if (pBIH->biBitCount == 8)
	{
	}
	else if (pBIH->biBitCount == 24)
	{
		m_RGBData.CopyMem(p);
	}
	else if (pBIH->biBitCount == 32)
	{
		m_AData.Create(m_h, m_w);

		for (int y = 0; y < m_h; y++)
		{
			for (int x = 0; x < m_w; x++)
			{
				int xx = 4 * (y * m_w + x);
				SetBAt(x, y, p[xx + 0]);
				SetGAt(x, y, p[xx + 1]);
				SetRAt(x, y, p[xx + 2]);
				SetAAt(x, y, p[xx + 3]);
			}
		}
	}

	::GlobalUnlock(hMem2);

	::GlobalFree(hMem1);
	::GlobalFree(hMem2);
	return TRUE;
}

//***************************************************************************************

BOOL CBasicImage::SaveIntoMem(int* pSize, uchar** ppData, int CompressionType)
{
	const GUID EncoderJPG = { 0x557CF401, 0x1A04, 0x11D3, 0x9A, 0x73, 0x00, 0x00, 0xF8, 0x1E, 0xF3, 0x2E };
	const GUID EncoderPNG = { 0x557CF406, 0x1A04, 0x11D3, 0x9A, 0x73, 0x00, 0x00, 0xF8, 0x1E, 0xF3, 0x2E };

	if (CompressionType != COMPRESSION_JPG && CompressionType != COMPRESSION_PNG) return FALSE;

	if (m_RGBData.GetMem() == NULL) return FALSE;

	int BMPSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_RGBData.GetSize();

	HGLOBAL	hMem1 = ::GlobalAlloc(GMEM_MOVEABLE, BMPSize);
	HGLOBAL	hMem2 = ::GlobalAlloc(GMEM_MOVEABLE, 0);

	CComPtr<IStream> pBMPStream = NULL;
	CComPtr<IStream> pStream = NULL;

	uchar* pMem = (uchar*)::GlobalLock(hMem1);

	BITMAPFILEHEADER* pBFH = (BITMAPFILEHEADER*)pMem;
	pBFH->bfType = 0x4d42;
	pBFH->bfSize = BMPSize;
	pBFH->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	pBFH->bfReserved1 = 0;
	pBFH->bfReserved2 = 0;

	BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*)(pMem + sizeof(BITMAPFILEHEADER));
	pBIH->biSize = sizeof(BITMAPINFOHEADER);
	pBIH->biWidth = m_w;
	pBIH->biHeight = m_h;
	pBIH->biPlanes = 1;
	pBIH->biBitCount = 24;
	pBIH->biCompression = BI_RGB;
	pBIH->biSizeImage = 0;
	pBIH->biClrUsed = 0;
	pBIH->biClrImportant = 0;
	pBIH->biXPelsPerMeter = 0;
	pBIH->biYPelsPerMeter = 0;

	memcpy((pMem + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), m_RGBData.GetMem(), m_RGBData.GetSize());

	::GlobalUnlock(hMem1);

	if (::CreateStreamOnHGlobal(hMem1, FALSE, &pBMPStream) != S_OK)
	{
		::GlobalFree(hMem1);
		::GlobalFree(hMem2);
		return FALSE;
	}

	if (::CreateStreamOnHGlobal(hMem2, FALSE, &pStream) != S_OK)
	{
		::GlobalFree(hMem1);
		::GlobalFree(hMem2);
		return FALSE;
	}

	Gdiplus::Image Img(pBMPStream);

	CLSID* pEncoder;
	Gdiplus::EncoderParameters* pEncoderParameters;
	Gdiplus::EncoderParameters EncoderParameters;
	if (CompressionType == COMPRESSION_JPG)
	{
		ulong Quality = 90;
		EncoderParameters.Count = 1;
		EncoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
		EncoderParameters.Parameter[0].NumberOfValues = 1;
		EncoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
		EncoderParameters.Parameter[0].Value = &Quality;
		pEncoder = (CLSID*)&EncoderJPG;
		pEncoderParameters = &EncoderParameters;
	}
	else
	{
		pEncoder = (CLSID*)&EncoderPNG;
		pEncoderParameters = NULL;
	}

	Gdiplus::Status Status = Img.Save(pStream, pEncoder, pEncoderParameters);

	if (Status != Gdiplus::Ok)
	{
		::GlobalFree(hMem1);
		::GlobalFree(hMem2);
		return FALSE;
	}

	*pSize = ::GlobalSize(hMem2);

	*ppData = (uchar*)malloc(*pSize);

	pMem = (uchar*)::GlobalLock(hMem2);

	memcpy(*ppData, pMem, *pSize);

	::GlobalUnlock(hMem2);

	::GlobalFree(hMem1);
	::GlobalFree(hMem2);
	return TRUE;
}

//***************************************************************************************

BOOL CBasicImage::LoadFromFile(const CString& FileName)
{
	HANDLE hFile = ::CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE) return FALSE;

	ulong bt;

	int Size = ::GetFileSize(hFile, NULL);

	uchar* pData = (uchar*)malloc(Size);

	::ReadFile(hFile, pData, Size, &bt, NULL);

	::CloseHandle(hFile);

	LoadFromMem(Size, pData);

	free(pData);

	return TRUE;
}

//***************************************************************************************

BOOL CBasicImage::SaveIntoFile(const CString& FileName)
{
	BOOL Res = FALSE;

	CString Extension(FileName.Right(3).MakeLower());

	if (Extension == "bmp")
	{
		HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			BITMAPFILEHEADER BFH =
			{
				0x4d42,
				sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_RGBData.GetSize(),
				0, 0,
				sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
			};

			BITMAPINFOHEADER BIH;
			BIH.biSize = sizeof(BITMAPINFOHEADER);
			BIH.biWidth = m_w;
			BIH.biHeight = m_h;
			BIH.biPlanes = 1;
			BIH.biBitCount = 24;
			BIH.biCompression = BI_RGB;
			BIH.biSizeImage = 0;
			BIH.biClrUsed = 0;
			BIH.biClrImportant = 0;
			BIH.biXPelsPerMeter = 0;
			BIH.biYPelsPerMeter = 0;

			ulong bt;
			::WriteFile(hFile, &BFH, sizeof(BITMAPFILEHEADER), &bt, NULL);
			::WriteFile(hFile, &BIH, sizeof(BITMAPINFOHEADER), &bt, NULL);
			::WriteFile(hFile, m_RGBData.GetMem(), m_RGBData.GetSize(), &bt, NULL);

			::CloseHandle(hFile);

			Res = TRUE;
		}
	}
	else if (Extension == "jpg" || Extension == "png")
	{
		int CompressionType = Extension == "jpg" ? COMPRESSION_JPG : COMPRESSION_PNG;

		int Size = 0;
		uchar* pData = NULL;

		if (SaveIntoMem(&Size, &pData, CompressionType))
		{
			HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				ulong bt;
				::WriteFile(hFile, pData, Size, &bt, NULL);

				::CloseHandle(hFile);

				Res = TRUE;
			}

			free(pData);
		}
	}

	return Res;
}

//***************************************************************************************

CProductLogo::CProductLogo(int i)
{
	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int ResourceID;

	if (W >= 1920)
	{
		ResourceID = i == 0 ? IDR_PRODUCT_LOGO_BLACK_3 : IDR_PRODUCT_LOGO_WHITE_3;
	}
	else if (W >= 1600)
	{
		ResourceID = i == 0 ? IDR_PRODUCT_LOGO_BLACK_2 : IDR_PRODUCT_LOGO_WHITE_2;
	}
	else
	{
		ResourceID = i == 0 ? IDR_PRODUCT_LOGO_BLACK_1 : IDR_PRODUCT_LOGO_WHITE_1;
	}

	LoadFromRes(ResourceID);
}

//***************************************************************************************

CiTraceLogo::CiTraceLogo(int i)
{
	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int ResourceID;

	if (m_isCombo || !m_isValid)
	{
		if (W >= 1920)
		{
			ResourceID = i == 0 ? IDR_ITRACE_LOGO_BLACK_3 : IDR_ITRACE_LOGO_WHITE_3;
		}
		else if (W >= 1600)
		{
			ResourceID = i == 0 ? IDR_ITRACE_LOGO_BLACK_2 : IDR_ITRACE_LOGO_WHITE_2;
		}
		else
		{
			ResourceID = i == 0 ? IDR_ITRACE_LOGO_BLACK_1 : IDR_ITRACE_LOGO_WHITE_1;
		}
	}
	else
	{
		if (W >= 1920)
		{
			ResourceID = i == 0 ? IDR_ITRACE_SOLO_BLACK_3 : IDR_ITRACE_SOLO_WHITE_3;
		}
		else if (W >= 1600)
		{
			ResourceID = i == 0 ? IDR_ITRACE_SOLO_BLACK_2 : IDR_ITRACE_SOLO_WHITE_2;
		}
		else
		{
			ResourceID = i == 0 ? IDR_ITRACE_SOLO_BLACK_1 : IDR_ITRACE_SOLO_WHITE_1;
		}
	}

	LoadFromRes(ResourceID);
}

//***************************************************************************************

CTraceyLogo::CTraceyLogo(int i)
{
	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int ResourceID;

	if (W >= 1920)
	{
		ResourceID = i == 0 ? IDR_TRACEY_LOGO_BLACK_3 : IDR_TRACEY_LOGO_WHITE_3;
	}
	else if (W >= 1600)
	{
		ResourceID = i == 0 ? IDR_TRACEY_LOGO_BLACK_2 : IDR_TRACEY_LOGO_WHITE_2;
	}
	else
	{
		ResourceID = i == 0 ? IDR_TRACEY_LOGO_BLACK_1 : IDR_TRACEY_LOGO_WHITE_1;
	}

	LoadFromRes(ResourceID);
}

//***************************************************************************************

//7.0.0
CAcqNoExamLogo::CAcqNoExamLogo()
{
	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int ResourceID;

	if (W >= 1920)
	{
		ResourceID = IDB_NOEXAM;
	}
	else if (W >= 1600)
	{
		ResourceID = IDB_NOEXAM;
	}
	else
	{
		ResourceID = IDB_NOEXAM;
	}

	LoadFromRes(ResourceID);
}

//***************************************************************************************

//7.0.0
CAcqResLogo::CAcqResLogo(int type)
{
	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int ResourceID;

	if (W >= 1920)
	{
		switch (type)
		{
		case ACQRES_CG:
			ResourceID = IDB_ACQRES_CG;
			break;
		case ACQRES_CG_IG:
			ResourceID = IDB_ACQRES_CG_IG;
			break;
		case ACQRES_CG_IR:
			ResourceID = IDB_ACQRES_CG_IR;
			break;
		case ACQRES_CG_IY:
			ResourceID = IDB_ACQRES_CG_IY;
			break;
		case ACQRES_CR:
			ResourceID = IDB_ACQRES_CR;
			break;
		case ACQRES_CR_IG:
			ResourceID = IDB_ACQRES_CR_IG;
			break;
		case ACQRES_CR_IR:
			ResourceID = IDB_ACQRES_CR_IR;
			break;
		case ACQRES_CR_IY:
			ResourceID = IDB_ACQRES_CR_IY;
			break;
		case ACQRES_CY:
			ResourceID = IDB_ACQRES_CY;
			break;
		case ACQRES_CY_IG:
			ResourceID = IDB_ACQRES_CY_IG;
			break;
		case ACQRES_CY_IR:
			ResourceID = IDB_ACQRES_CY_IR;
			break;
		case ACQRES_CY_IY:
			ResourceID = IDB_ACQRES_CY_IY;
			break;
		case ACQRES_GRAY:
			ResourceID = IDB_ACQRES_GRAY;
			break;
		default:
			break;
		}

	}
	else if (W >= 1600)
	{
		switch (type)
		{
		case ACQRES_CG:
			ResourceID = IDB_ACQRES_CG;
			break;
		case ACQRES_CG_IG:
			ResourceID = IDB_ACQRES_CG_IG;
			break;
		case ACQRES_CG_IR:
			ResourceID = IDB_ACQRES_CG_IR;
			break;
		case ACQRES_CG_IY:
			ResourceID = IDB_ACQRES_CG_IY;
			break;
		case ACQRES_CR:
			ResourceID = IDB_ACQRES_CR;
			break;
		case ACQRES_CR_IG:
			ResourceID = IDB_ACQRES_CR_IG;
			break;
		case ACQRES_CR_IR:
			ResourceID = IDB_ACQRES_CR_IR;
			break;
		case ACQRES_CR_IY:
			ResourceID = IDB_ACQRES_CR_IY;
			break;
		case ACQRES_CY:
			ResourceID = IDB_ACQRES_CY;
			break;
		case ACQRES_CY_IG:
			ResourceID = IDB_ACQRES_CY_IG;
			break;
		case ACQRES_CY_IR:
			ResourceID = IDB_ACQRES_CY_IR;
			break;
		case ACQRES_CY_IY:
			ResourceID = IDB_ACQRES_CY_IY;
			break;
		case ACQRES_GRAY:
			ResourceID = IDB_ACQRES_GRAY;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (type)
		{
		case ACQRES_CG:
			ResourceID = IDB_ACQRES_CG;
			break;
		case ACQRES_CG_IG:
			ResourceID = IDB_ACQRES_CG_IG;
			break;
		case ACQRES_CG_IR:
			ResourceID = IDB_ACQRES_CG_IR;
			break;
		case ACQRES_CG_IY:
			ResourceID = IDB_ACQRES_CG_IY;
			break;
		case ACQRES_CR:
			ResourceID = IDB_ACQRES_CR;
			break;
		case ACQRES_CR_IG:
			ResourceID = IDB_ACQRES_CR_IG;
			break;
		case ACQRES_CR_IR:
			ResourceID = IDB_ACQRES_CR_IR;
			break;
		case ACQRES_CR_IY:
			ResourceID = IDB_ACQRES_CR_IY;
			break;
		case ACQRES_CY:
			ResourceID = IDB_ACQRES_CY;
			break;
		case ACQRES_CY_IG:
			ResourceID = IDB_ACQRES_CY_IG;
			break;
		case ACQRES_CY_IR:
			ResourceID = IDB_ACQRES_CY_IR;
			break;
		case ACQRES_CY_IY:
			ResourceID = IDB_ACQRES_CY_IY;
			break;
		case ACQRES_GRAY:
			ResourceID = IDB_ACQRES_GRAY;
			break;
		default:
			break;
		}
	}

	LoadFromRes(ResourceID);
}
//7.0.0

//***************************************************************************************

CWavetouchLogo::CWavetouchLogo(int i)
{
	int ResourceID = i == 0 ? IDR_WAVETOUCH_LOGO_BLACK : IDR_WAVETOUCH_LOGO_WHITE;

	LoadFromRes(ResourceID);
}

//***************************************************************************************

CHoyaLogo::CHoyaLogo()
{
	int W = ::GetSystemMetrics(SM_CXSCREEN);

	int ResourceID;

	if (W >= 1920)
	{
		ResourceID = IDR_HOYA_LOGO_3;
	}
	else if (W >= 1600)
	{
		ResourceID = IDR_HOYA_LOGO_2;
	}
	else
	{
		ResourceID = IDR_HOYA_LOGO_1;
	}

	LoadFromRes(ResourceID);
}

//***************************************************************************************

CEyeImage::CEyeImage()
{
	m_w_um = 0.0;
	m_h_um = 0.0;

	ClearPupil();
	ClearLimbus();
	ClearVertex0();
	ClearVertexAndRings();
	ClearLensDots();

	//7.0.0
	m_FocusedValue = 0;
	m_Focused = FALSE;
	m_IRDotReady = FALSE;
	m_Centration = FALSE;
	//7.0.0
}

//***************************************************************************************

void CEyeImage::Destroy()
{
	CBasicImage::Destroy();

	m_w_um = 0.0;
	m_h_um = 0.0;

	ClearPupil();
	ClearLimbus();
	ClearVertex0();
	ClearVertexAndRings();
	ClearLensDots();

	//7.0.0
	m_FocusedValue = 0;
	m_Centration = FALSE;
	m_IRDotReady = FALSE;
	m_Focused = FALSE;
	//7.0.0
}

//***************************************************************************************

BOOL CEyeImage::GetRGBAtUm(real_t x_um, real_t y_um, real_t* pR, real_t* pG, real_t* pB)
{
	real_t xr = m_w * (0.5 + x_um / m_w_um);
	real_t yr = m_h * (0.5 + y_um / m_h_um);

	int xi = (int)xr; if (xi < 0 || xi > m_w - 2) return FALSE;
	int yi = (int)yr; if (yi < 0 || yi > m_h - 2) return FALSE;

	if (pR)
	{
		int R0 = GetRAt(xi, yi);
		int R1 = GetRAt(xi + 1, yi);
		int R2 = GetRAt(xi + 1, yi + 1);
		int R3 = GetRAt(xi, yi + 1);
		real_t Rl = R0 + (R3 - R0) * (yr - yi);
		real_t Rr = R1 + (R2 - R1) * (yr - yi);
		*pR = Rl + (Rr - Rl) * (xr - xi);
	}

	if (pG)
	{
		int G0 = GetGAt(xi, yi);
		int G1 = GetGAt(xi + 1, yi);
		int G2 = GetGAt(xi + 1, yi + 1);
		int G3 = GetGAt(xi, yi + 1);
		real_t Gl = G0 + (G3 - G0) * (yr - yi);
		real_t Gr = G1 + (G2 - G1) * (yr - yi);
		*pG = Gl + (Gr - Gl) * (xr - xi);
	}

	if (pB)
	{
		int B0 = GetBAt(xi, yi);
		int B1 = GetBAt(xi + 1, yi);
		int B2 = GetBAt(xi + 1, yi + 1);
		int B3 = GetBAt(xi, yi + 1);
		real_t Bl = B0 + (B3 - B0) * (yr - yi);
		real_t Br = B1 + (B2 - B1) * (yr - yi);
		*pB = Bl + (Br - Bl) * (xr - xi);
	}

	return TRUE;
}

//***************************************************************************************

void CEyeImage::ClearPupil()
{
	m_pu_ok = FALSE;
	m_pu_x0_um = 0.0;
	m_pu_y0_um = 0.0;
	for (int a = 0; a < 360; a++) m_pu_r_um[a] = INVALID_VALUE;
	m_pu_r_min_um = 0.0;
	m_pu_r_max_um = 0.0;
	m_pu_r_mean_um = 0.0;

	m_pu_IsCircle = FALSE;//7.0.0
}

//***************************************************************************************

void CEyeImage::ClearLimbus()
{
	m_li_ok = FALSE;
	m_li_x0_um = 0.0;
	m_li_y0_um = 0.0;
	for (int a = 0; a < 360; a++) m_li_r_um[a] = INVALID_VALUE;
	m_li_r_min_um = 0.0;
	m_li_r_max_um = 0.0;
	m_li_r_mean_um = 0.0;
}

//***************************************************************************************

void CEyeImage::ClearVertex0()
{
	m_ve0_ok = FALSE;
	m_ve0_x = 0;
	m_ve0_y = 0;

	m_la_ok = FALSE;
	m_Target_ok = FALSE;
	m_la_x = 0;
	m_la_v = 0;
}

//***************************************************************************************

void CEyeImage::ClearVertex()
{
	m_ve_ok = FALSE;
	m_ve_x_um = 0.0;
	m_ve_y_um = 0.0;

	m_ve_r = 0;
}

//***************************************************************************************

void CEyeImage::ClearLensDots()
{
	m_le_ok = FALSE;
	for (int i = 0; i < 3; i++)
	{
		m_le_x_um[i] = 0.0;
		m_le_y_um[i] = 0.0;
	}
}

//***************************************************************************************

void CEyeImage::ClearVertexAndRings()
{
	m_ve_ok = FALSE;
	m_ve_x_um = 0.0;
	m_ve_y_um = 0.0;

	m_ri_ok = FALSE;
	for (int r = 0; r < MAX_NUM_RINGS; r++)
	{
		for (int a = 0; a < 360; a++)
		{
			m_ri_r_um[r][a] = INVALID_VALUE;
		}
	}
}

//***************************************************************************************

void CEyeImage::ClearRuler()
{
	m_Ra_ok = FALSE;
	m_Ra_x_um = 0.0;
	m_Ra_y_um = 0.0;
	m_Ra_r_um = 0.0;
}

//***************************************************************************************

void CEyeImage::ClearLRuler()
{
	m_LRa_ok = FALSE;
	m_LRa_x0_um = 0.0;
	m_LRa_y0_um = 0.0;
	m_LRa_x1_um = 0.0;
	m_LRa_y1_um = 0.0;
}

//***************************************************************************************

//7.0.0
void CEyeImage::TestChe(int order, CString filename, CString filename2)
{
	if (m_RGBData.GetMem() == NULL) return;

	int x, y;

	if (!m_pu_ok || !m_ve_ok)
	{
		for (int i = 0; i <= 100; i++)
		{
			for (int j = 10; j <= 50; j++)
			{
				SetRAt(i, j, 250);
				SetGAt(i, j, 0);
				SetBAt(i, j, 0);
			}
		}

		SaveIntoFile(filename);
		return;
	}


	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	//Show Pupil
	int x0, y0;

	x = intRound(m_pu_x0_um * x_px_um + cx);
	y = intRound(m_pu_y0_um * y_px_um + cy);

	for (int a = 0; a < 360; a++)
	{
		int InR = intRound(m_pu_r_um[a] * x_px_um);
		int OutR = intRound((m_pu_r_um[a] + 300) * x_px_um);

		for (int r = InR; r <= OutR; r++)
		{
			x0 = intRound(x + r * ::COS[a]);
			y0 = intRound(y + r * ::SIN[a]);

			if ((x0 >= 0 && x0 < m_w) && (y0 >= 0 && y0 < m_h))
			{
				SetRAt(x0, y0, 250);
				SetGAt(x0, y0, 0);
				SetBAt(x0, y0, 0);
			}
		}
	}
	//Show Pupil Done

	//Show Vertex and row
	x = intRound(m_ve_x_um * x_px_um + cx);
	y = intRound(m_ve_y_um * y_px_um + cy);


	////--------------------------

	int Inner_R = m_ve_r - 6;
	int Outline_R = m_ve_r + 6;

	//Ensure Inside is not iR_dot blurry
	CString oneStr;
	if (m_ve_r != 0)
	{
		int aveR_O = 0;
		int aveR_I = 0;

		int FindPos;
		int t_O = 0;
		int t_I = 0;

		CString In_Point_O_Str = "";
		CString In_Point_I_Str = "";
		CString s;

		for (int a = 0; a < 360; a++)
		{
			for (int r = Inner_R; r <= Outline_R; r++)
			{
				x0 = intRound(x + r * ::COS[a]);
				y0 = intRound(y + r * ::SIN[a]);

				s.Format(_T("*%i,%i*"), x0, y0);

				FindPos = In_Point_O_Str.Find(s, 0);

				if (FindPos == -1)
				{
					aveR_O += GetRAt(x0, y0);
					t_O++;
					In_Point_O_Str += s;
				}
			}

			for (int r = 1; r < Inner_R; r++)
			{
				x0 = intRound(x + r * ::COS[a]);
				y0 = intRound(y + r * ::SIN[a]);

				s.Format(_T("*%i,%i*"), x0, y0);

				FindPos = In_Point_I_Str.Find(s, 0);

				if (FindPos == -1)
				{
					aveR_I += GetRAt(x0, y0);
					t_I++;
					In_Point_I_Str += s;
				}
			}
		}

		aveR_O = intRound((real_t)aveR_O / (real_t)t_O);
		aveR_I = intRound((real_t)aveR_I / (real_t)t_I);

		real_t Rat = (real_t)aveR_O / (real_t)aveR_I; //1.94 390; 1.55 316

		if (Rat > 1.4)
		{

		}


		int IntRat = (intRound)(Rat * 100);
		oneStr.Format(_T("%i"), IntRat);

		filename = filename + oneStr + ".jpg";

		int a = 0;
		//Ensure Inside is not iR_dot

		SaveIntoFile(filename);
		//Show Vertex

		for (int i = -3; i <= 3; i++)
		{
			for (int j = -3; j <= 3; j++)
			{
				SetRAt(x + i, y + j, 250);
				SetGAt(x + i, y + j, 0);
				SetBAt(x + i, y + j, 0);
			}
		}

		for (int a = 1; a < 360; a++)
		{
			for (int r = Inner_R; r <= Outline_R; r++)
			{
				x0 = intRound(x + r * ::COS[a]);
				y0 = intRound(y + r * ::SIN[a]);

				SetRAt(x0, y0, 250);
				SetGAt(x0, y0, 0);
				SetBAt(x0, y0, 0);
			}
		}

		SaveIntoFile(filename2);
		//Show Vertex Done
	}
	else
	{
		int a = 0;
	}

	int a = 0;
}

//***************************************************************************************

//7.0.0 check the clearance of eye image
//User the out circle of pupil
real_t CEyeImage::CheckFocus()
{
	if (m_RGBData.GetMem() == NULL) return 0;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	//ave value [each 10 degree as a group]
	real_t aveR[36];
	real_t varianceR[36];
	int  nt[36];

	for (int i = 0; i < 36; i++)
	{
		aveR[i] = 0;
		varianceR[i] = 0;
		nt[i] = 0;
	}
	//ave

	//Pupil
	int x, y;

	int x0 = intRound(m_pu_x0_um * x_px_um + cx);
	int y0 = intRound(m_pu_y0_um * y_px_um + cy);

	int FindPos;
	CString O_Str = "";
	CString s;

	int t = 0;
	int n = 0;
	for (int a = 0; a < 360; a++)
	{
		int InR = intRound((m_pu_r_um[a] + 10) * x_px_um);
		int OutR = intRound((m_pu_r_um[a] + 100) * x_px_um);

		for (int r = InR; r <= OutR; r++)
		{
			x = intRound(x0 + r * ::COS[a]);
			y = intRound(y0 + r * ::SIN[a]);

			s.Format(_T("*%i,%i*"), x, y);
			FindPos = O_Str.Find(s, 0);

			if (FindPos == -1)
			{
				int R = GetRAt(x, y);

				aveR[n] += (real_t)R;

				nt[n]++;

				O_Str += s;
			}
		}

		t++;

		if (t == 10)
		{
			t = 0;
			n++;
		}
	}

	for (int i = 0; i < 36; i++)
	{
		aveR[i] = aveR[i] / (real_t)nt[i];
	}

	//------------------------------------
	//Variance
	t = 0;
	n = 0;

	O_Str = "";

	for (int a = 0; a < 360; a++)
	{
		int InR = intRound((m_pu_r_um[a] + 10) * x_px_um);
		int OutR = intRound((m_pu_r_um[a] + 100) * x_px_um);

		for (int r = InR; r <= OutR; r++)
		{
			x = intRound(x0 + r * ::COS[a]);
			y = intRound(y0 + r * ::SIN[a]);

			s.Format(_T("*%i,%i*"), x, y);
			FindPos = O_Str.Find(s, 0);

			if (FindPos == -1)
			{
				varianceR[n] += pow(aveR[n] - GetRAt(x, y), 2);

				O_Str += s;
			}
		}

		t++;

		if (t == 10)
		{
			t = 0;
			n++;
		}
	}

	//res
	real_t varR = 0;

	for (int i = 0; i < 36; i++)
	{
		varianceR[i] = sqrt(varianceR[i] / (real_t)nt[i]);

		varR += fabs(varianceR[i]) / 36.0;
	}

	return varR;
}


//***************************************************************************************

//7.0.0 Ensure the ir dot is not blurry
void CEyeImage::CheckIRDot()
{
	if (m_RGBData.GetMem() == NULL) return;

	m_IRDotReady = FALSE;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	int x0, y0;
	int Inner_R = m_ve_r - 6;
	int Outline_R = m_ve_r + 6;

	int x = intRound(m_ve_x_um * x_px_um + cx);
	int y = intRound(m_ve_y_um * y_px_um + cy);

	//Ensure Inside is not iR_dot blurry
	CString oneStr;
	if (m_ve_r != 0)
	{
		int aveR_O = 0;
		int aveR_I = 0;

		int FindPos;
		int t_O = 0;
		int t_I = 0;

		CString In_Point_O_Str = "";
		CString In_Point_I_Str = "";
		CString s;

		for (int a = 0; a < 360; a++)
		{
			for (int r = Inner_R; r <= Outline_R; r++)
			{
				x0 = intRound(x + r * ::COS[a]);
				y0 = intRound(y + r * ::SIN[a]);

				s.Format(_T("*%i,%i*"), x0, y0);

				FindPos = In_Point_O_Str.Find(s, 0);

				if (FindPos == -1)
				{
					aveR_O += GetRAt(x0, y0);
					t_O++;
					In_Point_O_Str += s;
				}
			}

			for (int r = 1; r < Inner_R; r++)
			{
				x0 = intRound(x + r * ::COS[a]);
				y0 = intRound(y + r * ::SIN[a]);

				s.Format(_T("*%i,%i*"), x0, y0);

				FindPos = In_Point_I_Str.Find(s, 0);

				if (FindPos == -1)
				{
					aveR_I += GetRAt(x0, y0);
					t_I++;
					In_Point_I_Str += s;
				}
			}
		}

		aveR_O = intRound((real_t)aveR_O / (real_t)t_O);
		aveR_I = intRound((real_t)aveR_I / (real_t)t_I);

		real_t Rat = (real_t)aveR_O / (real_t)aveR_I; //1.94 390; 1.55 316

		if (Rat > 1.2)//original is 1.4
		{
			m_IRDotReady = TRUE;
		}
	}
}

//***************************************************************************************
//check the clearance of eye image
void CEyeImage::CheckClear(int timeLeft)
{
	if (m_RGBData.GetMem() == NULL) return;

	int n = 0;
	int all = 0;
	int cx = intRound(0.5 * m_w);
	int cy = intRound(0.5 * m_h);
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	for (int i = (cx - 50); i < (cx + 50); i++)
	{
		for (int j = (cy - 50); j < (cy + 50); j++)
		{
			int R = GetRAt(i, j);
			int G = GetGAt(i, j);
			int B = GetBAt(i, j);

			if (R > 200 && G > 200 && B > 200)
			{
				n++;
			}
		}
	}

	if (n <= 1000) m_pu_clear = TRUE;
	else m_pu_clear = FALSE;
}

//***************************************************************************************

void CEyeImage::FindPupil()
{
	ClearPupil();

	if (m_RGBData.GetMem() == NULL) return;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;
	int icx = intRound(cx);
	int icy = intRound(cy);
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	int dx = intRound(1000.0 * x_px_um);
	int dy = intRound(1000.0 * y_px_um);

	{
		int t[40]; // 40
		int hist[256];

		memset(hist, 0, sizeof(hist));
		int pavg = 0;
		int n = 0;
		int k = 0;

		real_t Dif = 0.0;

		for (int j = -2; j <= 1; j++)
		{
			int y1 = icy + j * dy;
			int y2 = y1 + dy;

			for (int i = -5; i <= 4; i++)
			{
				int x1 = icx + i * dx;
				int x2 = x1 + dx;
				t[k] = 0;

				for (int y = y1; y <= y2; y++)
				{
					for (int x = x1; x <= x2; x++)
					{
						int p = GetRAt(x, y);
						hist[p]++;
						t[k] += p;
						pavg += p;

						n++;
					}
				}

				k++;
			}
		}
		pavg /= n;

		int imin = INT_MAX;

		for (int i = 0; i < 40; i++)
		{
			if (t[i] < imin) imin = t[i];
		}

		imin /= (dy + 1) * (dx + 1);

		m_hist_max = 0.0;

		for (int i = 0; i < 256; i++)
		{
			int s = 0;
			int n = 0;

			for (int j = -5; j <= 5; j++)
			{
				int k = i + j;
				if (k < 0) k = 0; else if (k > 255) k = 255;
				int p = hist[k];
				if (p > 0)
				{
					s += p;
					n++;
				}
			}

			real_t v = n == 0 ? 0.0 : (real_t)s / n;
			m_hist[i] = v;
			if (m_hist_max < v) m_hist_max = v;
		}

		for (int i = imin + 1; i <= pavg; i++)
		{
			if (m_hist[i] < m_hist[imin])
			{
				imin = i;
			}
		}

		m_pu_thr = imin;
	}

	m_pu_d_um = 100;
	const int Y = 101;
	const int X = 101;
	const int Y2 = 50;
	const int X2 = 50;
	m_table.Create(Y, X);
	m_table.Fill(0);
	for (int y = 0; y < Y; y++)
	{
		int y_um = (y - Y2) * m_pu_d_um;

		for (int x = 0; x < X; x++)
		{
			int x_um = (x - X2) * m_pu_d_um;
			if (sqr(x_um) + sqr(y_um) <= sqr(5000))
			{
				int y0 = intRound(cy + y_um * y_px_um);
				int x0 = intRound(cx + x_um * x_px_um);
				int s = 0;

				for (int j = y0 - 2; j <= y0 + 2; j++)
				{
					for (int i = x0 - 2; i <= x0 + 2; i++)
					{
						s += GetRAt(i, j);
					}
				}

				if (s <= 25 * m_pu_thr) m_table(y, x) = 1;
			}
		}
	}

	if (FindLargestGroup(m_table, 1, 2) < 100)
	{
		ClearPupil();
		return;
	}

	FillHollows(m_table, 2);

	List<CNode> hull;
	FindHull(hull, m_table, 2, 3);

	int xmin = 1000000;
	int xmax = -1000000;
	int ymin = 1000000;
	int ymax = -1000000;
	CNode* pNode = hull.MoveFirst();

	while (pNode)
	{
		if (xmin > pNode->m_x) xmin = pNode->m_x;
		if (xmax < pNode->m_x) xmax = pNode->m_x;
		if (ymin > pNode->m_y) ymin = pNode->m_y;
		if (ymax < pNode->m_y) ymax = pNode->m_y;
		pNode = hull.MoveNext();
	}

	int n = hull.GetSize();
	real_t x0 = (xmin + xmax) * 0.5;
	real_t y0 = (ymin + ymax) * 0.5;
	m_pu_x0_um = (x0 - X2) * m_pu_d_um;
	m_pu_y0_um = (y0 - Y2) * m_pu_d_um;

	n = hull.GetSize();
	Matrix<real_t> R_um(n);
	Matrix<real_t> A_rd(n);
	Matrix<real_t> Cos(n);
	Matrix<real_t> Sin(n);
	int n2 = 0;
	int pu_thr = m_pu_thr * 25;

	pNode = hull.MoveFirst();
	while (pNode)
	{
		real_t x0_um = (pNode->m_x - X2) * m_pu_d_um - m_pu_x0_um;
		real_t y0_um = (pNode->m_y - Y2) * m_pu_d_um - m_pu_y0_um;
		real_t r0_um = hyp(x0_um, y0_um) - 20.0;
		real_t a_rd = angle(y0_um, x0_um);
		real_t C = cos(a_rd);
		real_t S = sin(a_rd);
		real_t p[12];

		for (int r = 0; r < 12; r++)
		{
			real_t r_um = r0_um + r * 20.0;
			int x = intRound(cx + (m_pu_x0_um + r_um * C) * x_px_um);
			int y = intRound(cy + (m_pu_y0_um + r_um * S) * y_px_um);
			int s = 0;

			for (int j = y - 2; j <= y + 2; j++)
			{
				for (int i = x - 2; i <= x + 2; i++)
				{
					s += GetRAt(i, j);
				}
			}
			p[r] = s;
		}

		int rmax = -1;
		for (int r = 0; r < 11; r++)
		{
			if (p[r] < pu_thr && p[r + 1] > pu_thr)
			{
				rmax = r;
			}
		}

		if (rmax != -1)
		{
			R_um[n2] = r0_um + rmax * 20.0;
			A_rd[n2] = a_rd;
			Cos[n2] = C;
			Sin[n2] = S;
			n2++;
		}

		pNode = hull.MoveNext();

		while (pNode)
		{
			real_t nxt_x0_um = (pNode->m_x - X2) * m_pu_d_um - m_pu_x0_um;
			real_t nxt_y0_um = (pNode->m_y - Y2) * m_pu_d_um - m_pu_y0_um;
			real_t nxt_a_rd = angle(nxt_y0_um, nxt_x0_um);
			if (a_rd < nxt_a_rd) break;
			if (a_rd > nxt_a_rd && (3.0 * _Pi_2 < a_rd && a_rd <= _2_Pi) && (0.0 <= nxt_a_rd && nxt_a_rd < _Pi_2)) break;
			pNode = hull.MoveNext();
		}
	}

	if (n2 < 16)
	{
		ClearPupil();
		return;
	}

	Matrix<real_t> t(n2);
	t.Fill(0.0);

	for (int i = 0; i < n2; i++)
	{
		for (int j = -3; j <= 3; j++)
		{
			int m = i + j;
			if (m < 0) m += n2; else if (m >= n2) m -= n2;
			t[i] += R_um[m];
		}
		t[i] /= 7.0;
	}

	R_um = t;

	real_t x_um_min = 10000.0;
	real_t x_um_max = -10000.0;
	real_t y_um_min = 10000.0;
	real_t y_um_max = -10000.0;

	for (int i = 0; i < n2; i++)
	{
		real_t x_um = R_um[i] * Cos[i];
		real_t y_um = R_um[i] * Sin[i];
		if (x_um < x_um_min) x_um_min = x_um;
		if (x_um > x_um_max) x_um_max = x_um;
		if (y_um < y_um_min) y_um_min = y_um;
		if (y_um > y_um_max) y_um_max = y_um;
	}

	real_t dx_um = (x_um_min + x_um_max) * 0.5;
	real_t dy_um = (y_um_min + y_um_max) * 0.5;

	for (int i = 0; i < n2; i++)
	{
		real_t old_x_um = R_um[i] * Cos[i];
		real_t old_y_um = R_um[i] * Sin[i];
		real_t new_x_um = old_x_um - dx_um;
		real_t new_y_um = old_y_um - dy_um;
		R_um[i] = hyp(new_y_um, new_x_um);
		A_rd[i] = angle(new_y_um, new_x_um);
	}

	m_pu_x0_um += dx_um;
	m_pu_y0_um += dy_um;

	real_t amin = _2_Pi;
	int imin;

	for (int i = 0; i < n2; i++)
	{
		if (A_rd[i] < amin)
		{
			imin = i;
			amin = A_rd[i];
		}
	}

	Matrix<real_t> A(n2 + 2);
	Matrix<real_t> R(n2 + 2);
	int k = 1;

	for (int i = imin; i < n2; i++)
	{
		A[k] = A_rd[i];
		R[k] = R_um[i];
		k++;
	}

	for (int i = 0; i < imin; i++)
	{
		A[k] = A_rd[i];
		R[k] = R_um[i];
		k++;
	}

	A[0] = A[n2] - _2_Pi;
	R[0] = R[n2];
	A[n2 + 1] = A[1] + _2_Pi;
	R[n2 + 1] = R[1];

	k = 0;

	for (int a = 0; a < 360; a++)
	{
		real_t a_rd = a * _Pi_180;
		while (!(A[k] <= a_rd && a_rd <= A[k + 1])) k++;
		m_pu_r_um[a] = R[k] + (R[k + 1] - R[k]) * (a_rd - A[k]) / (A[k + 1] - A[k]);
	}

	m_pu_r_min_um = 10000.0;
	m_pu_r_max_um = 0.0;
	m_pu_r_mean_um = 0.0;

	for (int a = 0; a < 360; a++)
	{
		if (m_pu_r_min_um > m_pu_r_um[a]) m_pu_r_min_um = m_pu_r_um[a];
		if (m_pu_r_max_um < m_pu_r_um[a]) m_pu_r_max_um = m_pu_r_um[a];
		m_pu_r_mean_um += m_pu_r_um[a];
	}

	m_pu_r_mean_um /= 360.0;

	//7.0.0 Check whether the pupil is circle
	real_t variance = 0;
	for (int a = 0; a < 360; a++)
	{
		variance += pow((m_pu_r_mean_um - m_pu_r_um[a]), 2);
	}

	variance /= 360.0;
	variance = sqrt(variance);

	real_t Ratio = variance / m_pu_r_mean_um;

	if (Ratio > 0.1)
	{
		m_pu_IsCircle = FALSE;
	}
	else
	{
		m_pu_IsCircle = TRUE;
	}
	//7.0.0 Check whether the pupil is circle

	m_pu_ok = TRUE;
}

//***************************************************************************************

void CEyeImage::FindLensDots()
{
	ClearLensDots();

	if (m_RGBData.GetMem() == NULL) return;

	if (!m_pu_ok) return;

	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;
	real_t cx = m_w * 0.5;
	real_t cy = m_h * 0.5;

	real_t le_r_um[360];
	int BestIntensity[360];
	int AverageBestIntensity = 255;
	real_t dr_um = 100.0;
	int k = 0;

	for (int b = 135; b <= 405; b++)
	{
		int a = b > 359 ? b - 360 : b;
		le_r_um[a] = m_pu_r_um[a];
		BestIntensity[a] = 255;

		for (int r = (int)ceil(m_pu_r_um[a] / dr_um) + 3; r <= 65; r++)
		{
			real_t r_um = r * dr_um;
			real_t y_um = m_pu_y0_um + r_um * SIN[a];
			real_t x_um = m_pu_x0_um + r_um * COS[a];
			int y = intRound(cy + y_um * y_px_um);
			int x = intRound(cx + x_um * x_px_um);
			int y1 = y - 2, y2 = y + 2, x1 = x - 2, x2 = x + 2;
			if (y1 < 2 || y2 > m_h - 3 || x1 < 2 || x2 > m_w - 3) break;
			int s = 0;

			for (int j = y1; j <= y2; j++)
			{
				for (int i = x1; i <= x2; i++)
				{
					s += GetRAt(i, j);
				}
			}

			s /= 25;
			if (s < BestIntensity[a])
			{
				le_r_um[a] = r_um;
				BestIntensity[a] = s;
			}
		}
		AverageBestIntensity += BestIntensity[a];
		k++;
	}

	AverageBestIntensity /= k;

	int D = 20;
	int BestThreeIntensities[3] = { AverageBestIntensity - D, AverageBestIntensity - D, AverageBestIntensity - D };
	int best_b[3];

	for (int b = 135; b <= 405; b++)
	{
		int a = b > 359 ? b - 360 : b;
		if (BestIntensity[a] < BestThreeIntensities[0])
		{
			BestThreeIntensities[0] = BestIntensity[a];
			best_b[0] = b;
		}
	}

	for (int b = 135; b <= 405; b++)
	{
		if (abs(b - best_b[0]) < 60) continue;
		int a = b > 359 ? b - 360 : b;

		if (BestIntensity[a] < BestThreeIntensities[1])
		{
			BestThreeIntensities[1] = BestIntensity[a];
			best_b[1] = b;
		}
	}

	for (int b = 135; b <= 405; b++)
	{
		if (abs(b - best_b[0]) < 60 || abs(b - best_b[1]) < 60) continue;
		int a = b > 359 ? b - 360 : b;

		if (BestIntensity[a] < BestThreeIntensities[2])
		{
			BestThreeIntensities[2] = BestIntensity[a];
			best_b[2] = b;
		}
	}

	for (int k = 0; k < 3; k++)
	{
		if (BestThreeIntensities[k] == AverageBestIntensity - D)
		{
			ClearLensDots();
			return;
		}
	}

	for (int k = 0; k < 3; k++)
	{
		int a = best_b[k];
		if (a > 359) a -= 360;
		m_le_y_um[k] = m_pu_y0_um + le_r_um[a] * SIN[a];
		m_le_x_um[k] = m_pu_x0_um + le_r_um[a] * COS[a];
		int y = intRound(cy + m_le_y_um[k] * y_px_um);
		int x = intRound(cx + m_le_x_um[k] * x_px_um);
		int dy = intRound(500.0 * y_px_um);
		int dx = intRound(500.0 * x_px_um);
		int y1 = __max(2, y - dy);
		int y2 = __min(m_h - 3, y + dy);
		int x1 = __max(2, x - dx);
		int x2 = __min(m_w - 3, x + dx);
		int cols = 0;
		int rows = 0;
		int pixs = 0;

		for (int j = y1; j <= y2; j++)
		{
			for (int i = x1; i <= x2; i++)
			{
				if (GetRAt(i, j) < BestThreeIntensities[k] + 5)
				{
					cols += j;
					rows += i;
					pixs++;
				}
			}
		}

		if (pixs > 0)
		{
			m_le_y_um[k] = ((real_t)cols / pixs - cy) / y_px_um;
			m_le_x_um[k] = ((real_t)rows / pixs - cx) / x_px_um;
		}
	}

	ValidateLensDots();
}

//***************************************************************************************

void CEyeImage::ValidateLensDots()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = i + 1; j < 3; j++)
		{
			if (m_le_x_um[i] > m_le_x_um[j])
			{
				Swap(m_le_x_um[i], m_le_x_um[j]);
				Swap(m_le_y_um[i], m_le_y_um[j]);
			}
		}
	}

	real_t x0_um = 0.5 * (m_le_x_um[0] + m_le_x_um[2]);
	real_t y0_um = 0.5 * (m_le_y_um[0] + m_le_y_um[2]);
	if (hyp(x0_um, y0_um) > 2000.0) { ClearLensDots(); return; }

	real_t dx_um = m_le_x_um[2] - m_le_x_um[0];
	real_t dy_um = m_le_y_um[2] - m_le_y_um[0];
  if (dx_um < 5000.0 || dx_um > 10000.0) { ClearLensDots(); return; }

	real_t Angle1 = _180_Pi * angle(dy_um, dx_um);
	if (!((0.0 <= Angle1 && Angle1 <= 45.0) || (315.0 <= Angle1 && Angle1 < 360.0))) { ClearLensDots(); return; }

	dy_um = y0_um - m_le_y_um[1];
	dx_um = x0_um - m_le_x_um[1];
	if (dy_um < 2500.0) { ClearLensDots(); return; }

	real_t Angle2 = _180_Pi * angle(dy_um, dx_um);
	if (!(45.0 <= Angle2 && Angle2 <= 135.0)) { ClearLensDots(); return; }

	m_le_ok = TRUE;
}

//***************************************************************************************

real_t CEyeImage::GetLensX0Um()
{
	if (!m_le_ok) return 0.0;
	return 0.5 * (m_le_x_um[0] + m_le_x_um[2]);
}

real_t CEyeImage::GetLensY0Um()
{
	if (!m_le_ok) return 0.0;
	return 0.5 * (m_le_y_um[0] + m_le_y_um[2]);
}

real_t CEyeImage::GetLensRotationAngle()
{
	if (!m_le_ok) return 0.0;
	return _180_Pi * atan((m_le_y_um[2] - m_le_y_um[0]) / (m_le_x_um[2] - m_le_x_um[0]));
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CEyeImage::FindVertex0(BOOL TriLaserOn, int LaserIntensityThreshold)
{
	if (m_RGBData.GetMem() == NULL) return;

	int icx = intRound(0.5 * m_w);
	int icy = intRound(0.5 * m_h);

	//--------------------------------------------------------------
	{
		int dx = 10;
		int dy = 10;
		int t[64];
		int hist[256];
		memset(hist, 0, sizeof(hist));
		int pavg = 0;
		int n = 0;
		int k = 0;
		for (int j = -4; j < 4; j++) {
			int y1 = icy + dy * j;
			int y2 = y1 + dy - 1;
			for (int i = -4; i < 4; i++) {
				int x1 = icx + dx * i;
				int x2 = x1 + dx - 1;
				int s = 0;
				for (int y = y1; y <= y2; y++) {
					for (int x = x1; x <= x2; x++) {
						int p = GetRAt(x, y);
						s += p;
						hist[p]++;
						pavg += p;
						n++;
					}
				}
				t[k++] = s;
			}
		}
		pavg /= n;

		int pmin = INT_MAX;
		for (int i = 0; i < 64; i++)
		{
			if (t[i] < pmin) pmin = t[i];
		}

		pmin /= (dy * dx);

		m_hist_max = 0.0;
		for (int i = 0; i < 256; i++)
		{
			int s = 0;
			int n = 0;
			for (int j = -5; j <= 5; j++)
			{
				int k = i + j;
				if (k < 0) k = 0; else if (k > 255) k = 255;
				int p = hist[k];
				if (p > 0)
				{
					s += p;
					n++;
				}
			}
			real_t v = n == 0 ? 0.0 : (real_t)s / n;
			m_hist[i] = v;
			if (m_hist_max < v) m_hist_max = v;
		}

		int imin = pmin;
		for (int i = pmin + 1; i <= pavg; i++)
		{
			if (m_hist[i] < m_hist[imin])
			{
				imin = i;
			}
		}
		m_ve0_thr = imin;
	}

	//--------------------------------------------------------------

	m_table.Create(81, 81);
	int Y = m_table.GetNumRows();
	int X = m_table.GetNumCols();
	int Y2 = Y >> 1;
	int X2 = X >> 1;
	for (int y = 0; y < Y; y++)
	{
		int yy = y - Y2;
		for (int x = 0; x < X; x++)
		{
			int xx = x - X2;
			int s = 0;
			for (int j = yy - 2; j <= yy + 2; j++)
			{
				for (int i = xx - 2; i <= xx + 2; i++)
				{
					s += GetRAt(icx + i, icy + j);
				}
			}
			m_table(y, x) = s <= 25 * m_ve0_thr ? 1 : 0;
		}
	}

	//--------------------------------------------------------------
	if (FindLargestGroup(m_table, 1, 2) < 200) return;

	//--------------------------------------------------------------
	int s = 0, sx = 0, sy = 0;
	for (int y = 0; y < Y; y++) {
		for (int x = 0; x < X; x++) {
			if (m_table(y, x) == 2) {
				sx += x;
				sy += y;
				s++;
			}
		}
	}
	m_ve0_x = sx / s - X2;
	m_ve0_y = sy / s - Y2;

	//--------------------------------------------------------------
	FindHull(m_hull, m_table, 2, 3);

	//--------------------------------------------------------------
	m_ve0_ok = TRUE;

	//--------------------------------------------------------------

	//if (::NewSettings.m_Adjust_CT)
	//{
	//	//original code
	//	if (TriLaserOn)
	//	{
	//		int t = 11 * 21 * LaserIntensityThreshold;
	//		int xmax, smax = 0;
	//		for (int x = -40; x <= 40; x++)
	//		{
	//			int s = 0;
	//			for (int j = -5; j <= 5; j++)
	//			{
	//				for (int i = -10; i <= 10; i++)
	//				{
	//					s += GetRAt(icx + x + i, icy + j);
	//				}
	//			}
	//			if (s > smax)
	//			{
	//				smax = s;
	//				xmax = x;
	//			}
	//		}
	//		if (smax >= t)
	//		{
	//			m_la_x = xmax;
	//			m_la_v = smax / (11 * 21);
	//			m_la_ok = TRUE;
	//			m_la_RtoC_OK = TRUE;
	//		}
	//	}
	//}
	//else
	{
		//[cjf***04122012], tracing the first laser spot, do not allow second spot trigger capture
		int row = intRound((m_w - 400) / 10); //[cjf06112012]
		if (TriLaserOn)
		{
			int t = 11 * 11 * LaserIntensityThreshold;
			int xmax, smax = 0;

			for (int i = 2; i < row; i++)
			{
				int s = 0;
				int m = i * 10 + 200;
				if ((m > icx + 35) || (m < icx - 35))
				{
					for (int x = m; x <= m + 10; x++)
					{
						for (int y = icy - 5; y <= icy + 5; y++)
						{
							int R = GetRAt(x, y);
							s += R;
						}
					}

					if (s > smax)
					{
						smax = s;
						xmax = m + 5;
					}
				}
			}

			//m_la_xmax = xmax - icx;

			//[cjf***04122012]
			int xmaxCen, smaxCen = 0;

			for (int x = -30; x <= 30; x++)
			{
				int s = 0;
				for (int j = -5; j <= 5; j++)
				{
					for (int i = -5; i <= 5; i++)
					{
						s += GetRAt(icx + x + i, icy + j);
					}
				}
				if (s > smaxCen)
				{
					smaxCen = s;
					xmaxCen = x + icx;
				}
			}

			if (smaxCen >= t)
			{
				m_la_x = xmaxCen - icx;
				m_la_v = smaxCen / (11 * 11);
				if (xmax >= (icx - 20) && smax >= t)
				{
					m_Target_ok = TRUE;//[5.1.1]
				}
				else
				{
					m_Target_ok = FALSE;//[5.1.1]
				}

				if (xmax >= (icx - 10) && smax >= t)
				{
					//sometimes it is the second light target the capture in the center since it has maxium value
					//however it has a 'bright long tail' in its right side
					//Let check this 'long tail'
					int s = 0;
					for (int j = -5; j <= 5; j++)
					{
						for (int i = -5; i <= 5; i++)
						{
							s += GetRAt(icx + 20 + i, icy + j);
						}
					}
					if (s < t)  m_la_ok = TRUE;
					else m_la_ok = FALSE;
					//
				}

				//if((xmax - icx) == -37 || (xmax - icx) == -87) // for the caliboard sphere
				if ((xmax - icx) == -37 || (xmax - icx) == -87 || (xmax - icx) == -77 || (xmax - icx) == 43) // [5.1.1]
				{
					m_la_ok = TRUE;
					m_Target_ok = TRUE;
				}
				else
				{
					int a = xmax - icx;
					a = a;
				}
				m_la_RtoC_OK = FindDirt(xmaxCen, icx);
			}
			else m_la_x = xmax;
		}
		//[cjf***04112012]
	}
}

//***************************************************************************************
//[cjf***04112012], Judge: whether the laser reflection is from right to the center;
BOOL CEyeImage::FindDirt(int xmax, int icx)
{
	if (m_la_Ok_Times < 30)
	{
		m_la_Prior_x[m_la_Ok_Times] = xmax;
		m_la_Ok_Times++;
		return FALSE;
	}
	else
	{
		int m = 0;
		int n = 0;
		for (int i = 0; i < 30; i++)
		{
			if (m_la_Prior_x[i] > icx - 5) //7.0.0 change from icx to icx - 5
			{
				m++;// if most of  laser reflection is on the right side of center
			}
			else
			{
				n++;
			}
		}

		// add new coming laser position if the laser spot is not in the center
		if (abs(icx - xmax) >= 10)
		{
			for (int i = 0; i< 29; i++)
			{
				m_la_Prior_x[i] = m_la_Prior_x[i + 1];
			}
			m_la_Prior_x[29] = xmax;
		}

		if (m >= n + 10) return TRUE;
		else return FALSE;
	}
}
//[cjf***04122012]
//***************************************************************************************








int CEyeImage::FindLargestGroup(Matrix<int>& table, int index_to_look, int index_to_set)
{
	const int dx[8] = { 1,  1,  0, -1, -1, -1,  0,  1 };
	const int dy[8] = { 0,  1,  1,  1,  0, -1, -1, -1 };
	const int nn = 4; // nn

	int Y = table.GetNumRows();
	int X = table.GetNumCols();

	Matrix<int> table2;
	table2 = table;

	int index_bad = std::min(index_to_look, index_to_set) - 1;

	int rem = 1;
	while (rem) {
		rem = 0;
		for (int y = 0; y < Y; y++) {
			for (int x = 0; x < X; x++) {
				if (table2(y, x) == index_to_look) {
					int n = 0;
					for (int i = 0; i < 16; i++) {
						int yn = y + dy[i % 8];
						int xn = x + dx[i % 8];
						if (yn < 0 || yn >= Y || xn < 0 || xn >= X) continue;
						if (table2(yn, xn) == index_to_look) {
							if (++n == nn) break;
						}
						else {
							n = 0;
						}
					}
					if (n < nn) {
						table2(y, x) = index_bad;
						rem++;
					}
				}
			}
		}
	}

	// index_to_look  (index_bad)
	int new_group_index = index_to_look;
	int num_group_nodes_max = 0;
	int group_index_max = index_bad - 1; // index_bad
	for (int y = 0; y < Y; y++) {
		for (int x = 0; x < X; x++) {
			if (table2(y, x) == index_to_look) {

				//-------------------------------------------------------------------------------------
				int num_new_group_nodes = CreateNewGroup(table2, x, y, index_to_look, ++new_group_index);
				//-------------------------------------------------------------------------------------

				//so far
				if (num_new_group_nodes > num_group_nodes_max)
				{
					num_group_nodes_max = num_new_group_nodes;
					group_index_max = new_group_index;
				}
			}
		}
	}

	for (int y = 0; y < Y; y++) {
		for (int x = 0; x < X; x++) {
			if (table2(y, x) == group_index_max) {
				table(y, x) = index_to_set;
			}
		}
	}

	return num_group_nodes_max;
}

//***************************************************************************************

int CEyeImage::CreateNewGroup(Matrix<int>& table, int x0, int y0, int index_to_look, int index_to_set)
{
	const int dx[8] = { 1,  1,  0, -1, -1, -1,  0,  1 };
	const int dy[8] = { 0,  1,  1,  1,  0, -1, -1, -1 };

	int Y = table.GetNumRows();
	int X = table.GetNumCols();

	List<CNode> L[2];
	List<CNode>* pL1 = &L[0];
	List<CNode>* pL2 = &L[1];

	table(y0, x0) = index_to_set;

	int num_group_nodes = 1;

	pL1->Append(CNode(x0, y0));

	while (TRUE)
	{
		CNode* pNode = pL1->MoveFirst();
		while (pNode)
		{
			for (int i = 0; i < 8; i++)
			{
				int xn = pNode->m_x + dx[i];
				int yn = pNode->m_y + dy[i];
				if (xn < 0 || xn >= X || yn < 0 || yn >= Y) continue;

				if (table(yn, xn) == index_to_look)
				{
					table(yn, xn) = index_to_set;
					num_group_nodes++;
					pL2->Append(CNode(xn, yn));
				}
			}
			pNode = pL1->MoveNext();
		}

		if (pL2->GetSize() == 0) break;

		pL1->Destroy();

		Swap(pL1, pL2);

	}

	return num_group_nodes;
}

//***************************************************************************************

void CEyeImage::FillHollows(Matrix<int>& table, int index)
{
	int Y = table.GetNumRows();
	int X = table.GetNumCols();

	for (int y = 0; y < Y; y++)
	{
		int xmin = X - 1;
		for (int x = 0; x < X; x++)
		{
			if (table(y, x) == index)
			{ xmin = x; break; }
		}
		int xmax = 0;
		for (int x = X - 1; x >= 0; x--)
		{
			if (table(y, x) == index)
			{ xmax = x; break; }
		}
		for (int x = xmin; x <= xmax; x++)
		{
			table(y, x) = index;
		}
	}

	for (int x = 0; x < X; x++)
	{
		int ymin = Y - 1;
		for (int y = 0; y < Y; y++)
		{
			if (table(y, x) == index)
			{ ymin = y; break; }
		}
		int ymax = 0;
		for (int y = Y - 1; y >= 0; y--)
		{
			if (table(y, x) == index)
			{ ymax = y; break; }
		}
		for (int y = ymin; y <= ymax; y++)
		{
			table(y, x) = index;
		}
	}
}

//***************************************************************************************

void CEyeImage::FindHull(List<CNode>& hull, Matrix<int>& table, int index_to_look, int index_to_set)
{
	const int dx[8] = { 1,  1,  0, -1, -1, -1,  0,  1 };
	const int dy[8] = { 0,  1,  1,  1,  0, -1, -1, -1 };

	hull.Destroy();

	int Y = table.GetNumRows();
	int X = table.GetNumCols();

	int x0 = -1;
	int y0 = -1;
	int x, y;
	for (x = X - 1; x >= 0; x--) {
		for (y = 0; y < Y; y++) {
			if (table(y, x) == index_to_look) {
				x0 = x;
				y0 = y;
				goto L;
			}
		}
	}
L:
	if (x0 == -1) return;

	table(y0, x0) = index_to_set;
	hull.Append(CNode(x0, y0));
	x = x0;
	y = y0;
	int f = 2;
	while (TRUE) {
		for (int i = f; i < f + 8; i++) {
			int j = i % 8;
			int xn = x + dx[j];
			int yn = y + dy[j];
			if (yn < 0 || yn >= Y || xn < 0 || xn >= X) continue;
			int p = table(yn, xn);
			if (p == index_to_look || p == index_to_set) {
				x = xn;
				y = yn;
				f = (j + 5) % 8;
				break;
			}
		}
		if (x != x0 || y != y0) {
			table(y, x) = index_to_set;
			hull.Append(CNode(x, y));
		}
		else break;
	}
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

//7.0.0 Used to ensure the ring is already show on the eye image, for blink threshold calculation
BOOL CEyeImage::FindFirstRing()
{
	// Take the center of the image
	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;

	// Get pixel -> micron ratio
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	BOOL ve_ok;
	real_t ve_x_um;
	real_t ve_y_um;
	real_t ri0_um[360];


	//find the 1st ring(inner edge of center ring) and vertex of rings map
	int kl1[5] = { 30, 25, 20, 15, 10 };
	// For each kl1 factor
	for (int l1 = 0; l1 < 5; l1++)
	{
		ve_ok = FALSE;
		ve_x_um = 0.0;
		ve_y_um = 0.0;

		const int nl2 = 3;
		for (int l2 = 0; l2 < nl2; l2++)
		{
			int edge[360];
			memset(edge, 0, sizeof(edge));

			// For each angle 0 to 360 degrees
			for (int a = 0; a < 360; a++)
			{
				// t[] contains 101 R values from image along spoke originating at (cx,cy) + ve factor
				// in same direction at 10um intervals
				real_t t[101];
				for (int r = 0; r <= 100; r++)
				{
					real_t r_um = 200.0 + r * 10.0;
					real_t x_um = ve_x_um + r_um * COS[a];
					real_t y_um = ve_y_um + r_um * SIN[a];
					int x = intRound(cx + x_um * x_px_um);
					int y = intRound(cy + y_um * y_px_um);
					t[r] = GetRAt(x, y);
				}

				// p[] contains an averaged R value for each pixel on spoke
				real_t p[101];
				real_t min = DBL_MAX;
				real_t max = -DBL_MAX;
				int k = kl1[l1];

				for (int r = 10; r <= 90; r++)
				{
					real_t s = 0.0;
					// Compute a weighted R value total of each pixel on spoke
					for (int i = -10; i <= 10; i++)
					{
						int q = k - abs(i);// Weighting q scales on how far from target pixel we're looking at
						s += q * t[r + i];
					}
					p[r] = s;
					if (s < min) min = s;
					if (s > max) max = s;
				}

				// If the highest and lowest R values on spoke are effectively equal, return.
				// Normalising the value is impossible if the range is effectively 0
				if (max - min <= 0.000000001) return FALSE;

				// Normalise the R value to range [0,1]
				for (int r = 10; r <= 90; r++)
				{
					p[r] = (p[r] - min) / (max - min);
				}

				// Find the radius value (r) with normalised value > 0.25 that is strictly decreasing
				// i.e. p[rmax - 1] > p[rmax] > p[rmax + 1]
				int rmax = 0;
				for (int r = 11; r <= 89; r++)
				{
					/*//ori code
					if (p[r] > 0.25 && p[r - 1] < p[r] && p[r] > p[r + 1])
					{
					rmax = r;
					break;
					}*/

					//New code by cjf 12-18-2019
					if (p[r] > 0.35 && p[r - 1] < p[r])//for test
					{
						rmax = r;
						break;
					}
					//New code by cjf
				}

				// Search through pixels [0,rmax) to find the point with the biggest pixel gradient
				real_t dmax = -DBL_MAX;
				for (int r = rmax - 1; r >= 11; r--)
				{
					real_t d = p[r] - p[r - 1];
					if (dmax < d)
					{
						dmax = d;
						edge[a] = r;// Store the edge for this angle
					}
				}
			}// end for(int a = 0; a < 360; a++)

			int z[360];
			memcpy(z, edge, sizeof(z));// Copy out the edge data
			sort_asc(z, 360, 45);// Sort so the lowest 45 are in correct position (nth element, n = 45, ascending order)

			int z1 = z[44];// z1 = 45th smallest value
			memcpy(z, edge, sizeof(z));
			sort_des(z, 360, 45);// Sort so the highest 45 are in correct position (nth element, n = 45, descending order)

			int z2 = z[44];	// z2 = 45th largest value

			for (int a = 0; a < 360; a++)
			{
				if (edge[a] < z1 || edge[a] > z2)
				{
					edge[a] = INVALID_VALUE;// Edges outside of [z1,z2] are labelled invalid
				}
			}

			// For each angle (spoke) get averaged (weighted) edge position
			for (int a = 0; a < 360; a++)
			{
				int s = 0;
				int k = 0;
				for (int i = -30; i <= 30; i++)
				{
					int v = edge[CheckAngle(a + i)];
					if (v != INVALID_VALUE)
					{
						int q = 31 - abs(i);
						s += q * v;
						k += q;
					}
				}

				// Weighted average edge position in microns stored in ri0_um.
				ri0_um[a] = k != 0 ? (200.0 + ((real_t)s / (real_t)k) * 10.0 - 5.0) : INVALID_VALUE;
			}

			// If this is not the last l2 loop.
			// Push ve_x/y values out to get the next ring
			if (l2 < nl2 - 1)
			{
				real_t x_min_um = DBL_MAX;
				real_t x_max_um = -DBL_MAX;
				real_t y_min_um = DBL_MAX;
				real_t y_max_um = -DBL_MAX;
				for (int a = 0; a < 360; a++)
				{
					if (ri0_um[a] != INVALID_VALUE)
					{
						real_t x_um = ve_x_um + ri0_um[a] * COS[a];
						real_t y_um = ve_y_um + ri0_um[a] * SIN[a];
						if (x_min_um > x_um) x_min_um = x_um;
						if (x_max_um < x_um) x_max_um = x_um;
						if (y_min_um > y_um) y_min_um = y_um;
						if (y_max_um < y_um) y_max_um = y_um;
					}
				}
				ve_x_um = (x_min_um + x_max_um) * 0.5;
				ve_y_um = (y_min_um + y_max_um) * 0.5;
			}
		}// End for(int l2 = 0; l2 < nl2; l2++)

		 // Check if the ring data has any breaks in it
		BOOL Broken = FALSE;
		real_t min = DBL_MAX;
		real_t max = -DBL_MAX;
		for (int a = 0; a < 360; a++)
		{
			if (ri0_um[a] == INVALID_VALUE)
			{
				Broken = TRUE;
				break;
			}
			else
			{
				if (ri0_um[a] < min) min = ri0_um[a];
				if (ri0_um[a] > max) max = ri0_um[a];
			}
		}

		// Check for smoothness of the ring data
		// Smoothness => No invalid values && Range < 200 && delta between spokes < 4.0
		if (!Broken && (max - min < 200.0))
		{
			BOOL Smooth = TRUE;
			for (int a = 0; a < 359; a++)
			{
				if (fabs(ri0_um[a + 1] - ri0_um[a]) > 4.0) { // 2009-03-15 changed from 3.0 to 4.0
					Smooth = FALSE;
					break;
				}
			}
			ve_ok = Smooth;
		}

		if (ve_ok) break;// Iterate on k waiting value until smooth ring data is found
	}// end for(int l1 = 0; l1 < 5; l1++)

	if (!ve_ok) return FALSE;// If no smooth ring data could be managed, return.

	return TRUE;
}
//Used to ensure the ring is already show on the image

//***************************************************************************************

//7.0.0 New rings detection code by cjf 12-18-2019
void CEyeImage::FindVertexAndRings()
{
	ClearVertexAndRings();

	if (m_RGBData.GetMem() == NULL) return;

	// Take the center of the image
	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;

	// Get pixel -> micron ratio
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	BOOL ve_ok;
	real_t ve_x_um;
	real_t ve_y_um;
	real_t ri0_um[360];

	int kl1[5] = { 30, 25, 20, 15, 10 };
	// For each kl1 factor
	for (int l1 = 0; l1 < 5; l1++)
	{
		ve_ok = FALSE;
		ve_x_um = 0.0;
		ve_y_um = 0.0;

		const int nl2 = 3;
		for (int l2 = 0; l2 < nl2; l2++)
		{
			int edge[360];
			memset(edge, 0, sizeof(edge));

			// For each angle 0 to 360 degrees
			for (int a = 0; a < 360; a++)
			{
				// t[] contains 101 R values from image along spoke originating at (cx,cy) + ve factor
				// in same direction at 10um intervals
				real_t t[101];
				for (int r = 0; r <= 100; r++)
				{
					real_t r_um = 200.0 + r * 10.0;
					real_t x_um = ve_x_um + r_um * COS[a];
					real_t y_um = ve_y_um + r_um * SIN[a];
					int x = intRound(cx + x_um * x_px_um);
					int y = intRound(cy + y_um * y_px_um);
					t[r] = GetRAt(x, y);
				}

				// p[] contains an averaged R value for each pixel on spoke
				real_t p[101];
				real_t min = DBL_MAX;
				real_t max = -DBL_MAX;
				int k = kl1[l1];

				for (int r = 10; r <= 90; r++)
				{
					real_t s = 0.0;
					// Compute a weighted R value total of each pixel on spoke
					for (int i = -10; i <= 10; i++)
					{
						int q = k - abs(i);// Weighting q scales on how far from target pixel we're looking at
						s += q * t[r + i];
					}
					p[r] = s;
					if (s < min) min = s;
					if (s > max) max = s;
				}

				// If the highest and lowest R values on spoke are effectively equal, return.
				// Normalising the value is impossible if the range is effectively 0
				if (max - min <= 0.000000001) return;

				// Normalise the R value to range [0,1]
				for (int r = 10; r <= 90; r++)
				{
					p[r] = (p[r] - min) / (max - min);
				}

				// Find the radius value (r) with normalised value > 0.25 that is strictly decreasing
				// i.e. p[rmax - 1] > p[rmax] > p[rmax + 1]
				int rmax = 0;
				for (int r = 11; r <= 89; r++)
				{
					//ori code
		/*			if (p[r] > 0.25 && p[r - 1] < p[r] && p[r] > p[r + 1])
					{
						rmax = r;
						break;
					}*/

					//New code by cjf 12-18-2019
					if (p[r] > 0.35 && p[r - 1] < p[r])//for test
					{
						rmax = r;
						break;
					}
					//New code by cjf
				}

				// Search through pixels [0,rmax) to find the point with the biggest pixel gradient
				real_t dmax = -DBL_MAX;
				for (int r = rmax - 1; r >= 11; r--)
				{
					real_t d = p[r] - p[r - 1];
					if (dmax < d)
					{
						dmax = d;
						edge[a] = r;// Store the edge for this angle
					}
				}
			}// end for(int a = 0; a < 360; a++)

			int z[360];
			memcpy(z, edge, sizeof(z));// Copy out the edge data
			sort_asc(z, 360, 45);// Sort so the lowest 45 are in correct position (nth element, n = 45, ascending order)

			int z1 = z[44];// z1 = 45th smallest value
			memcpy(z, edge, sizeof(z));
			sort_des(z, 360, 45);// Sort so the highest 45 are in correct position (nth element, n = 45, descending order)

			int z2 = z[44];	// z2 = 45th largest value

			for (int a = 0; a < 360; a++)
			{
				if (edge[a] < z1 || edge[a] > z2)
				{
					edge[a] = INVALID_VALUE;// Edges outside of [z1,z2] are labelled invalid
				}
			}

			// For each angle (spoke) get averaged (weighted) edge position
			for (int a = 0; a < 360; a++)
			{
				int s = 0;
				int k = 0;
				for (int i = -30; i <= 30; i++)
				{
					int v = edge[CheckAngle(a + i)];
					if (v != INVALID_VALUE)
					{
						int q = 31 - abs(i);
						s += q * v;
						k += q;
					}
				}

				// Weighted average edge position in microns stored in ri0_um.
				ri0_um[a] = k != 0 ? (200.0 + ((real_t)s / (real_t)k) * 10.0 - 5.0) : INVALID_VALUE;
			}

			// If this is not the last l2 loop.
			// Push ve_x/y values out to get the next ring
			if (l2 < nl2 - 1)
			{
				real_t x_min_um = DBL_MAX;
				real_t x_max_um = -DBL_MAX;
				real_t y_min_um = DBL_MAX;
				real_t y_max_um = -DBL_MAX;
				for (int a = 0; a < 360; a++)
				{
					if (ri0_um[a] != INVALID_VALUE)
					{
						real_t x_um = ve_x_um + ri0_um[a] * COS[a];
						real_t y_um = ve_y_um + ri0_um[a] * SIN[a];
						if (x_min_um > x_um) x_min_um = x_um;
						if (x_max_um < x_um) x_max_um = x_um;
						if (y_min_um > y_um) y_min_um = y_um;
						if (y_max_um < y_um) y_max_um = y_um;
					}
				}
				ve_x_um = (x_min_um + x_max_um) * 0.5;
				ve_y_um = (y_min_um + y_max_um) * 0.5;
			}
		}// End for(int l2 = 0; l2 < nl2; l2++)

		 // Check if the ring data has any breaks in it
		BOOL Broken = FALSE;
		real_t min = DBL_MAX;
		real_t max = -DBL_MAX;
		for (int a = 0; a < 360; a++)
		{
			if (ri0_um[a] == INVALID_VALUE)
			{
				Broken = TRUE;
				break;
			}
			else
			{
				if (ri0_um[a] < min) min = ri0_um[a];
				if (ri0_um[a] > max) max = ri0_um[a];
			}
		}

		// Check for smoothness of the ring data
		// Smoothness => No invalid values && Range < 200 && delta between spokes < 4.0
		if (!Broken && (max - min < 200.0))
		{
			BOOL Smooth = TRUE;
			for (int a = 0; a < 359; a++)
			{
				if (fabs(ri0_um[a + 1] - ri0_um[a]) > 4.0) { // 2009-03-15 changed from 3.0 to 4.0
					Smooth = FALSE;
					break;
				}
			}
			ve_ok = Smooth;
		}

		if (ve_ok) break;// Iterate on k waiting value until smooth ring data is found
	}// end for(int l1 = 0; l1 < 5; l1++)

	if (!ve_ok) return;// If no smooth ring data could be managed, return.

					   // ==================================================================

	m_ve_ok = TRUE;
	m_ve_x_um = ve_x_um;
	m_ve_y_um = ve_y_um;

	for (int a = 0; a < 360; a++)
	{
		//m_ri_r_um[0][a] = ri0_um[a];//Ori code
		m_ri_r_um[0][a] = ri0_um[a] + 20;//New code by cjf 12-18-2019  #why
	}

	// ==================================================================

	// Create matrix P filled with R values along 360 spokes

	int NumSteps = 512;
	real_t UmStep = 5200.0 / (NumSteps - 1);
	Matrix<real_t> P(360, NumSteps);
	P.Fill(INVALID_VALUE);
	for (int a = 0; a < 360; a++)
	{
		for (int s = 0; s < NumSteps; s++)
		{
			real_t r_um = s * UmStep;
			real_t x_um = m_ve_x_um + r_um * COS[a];
			real_t y_um = m_ve_y_um + r_um * SIN[a];
			real_t p;
			if (!GetRGBAtUm(x_um, y_um, &p, NULL, NULL)) break;
			P(a, s) = p;
		}
	}

	// ==================================================================

	{
		// Average the R values between spokes

		real_t t[360];
		const int m = 10;
		for (int p = 0; p < NumSteps; p++)
		{
			for (int a = 0; a < 360; a++)
			{
				real_t s = 0.0;
				int k = 0;

				for (int i = -m; i <= m; i++)
				{
					real_t v = P(CheckAngle(a + i), p);
					if (v != INVALID_VALUE)
					{
						int q = m + 1 - abs(i);
						s += q * v;
						k += q;
					}
				}

				t[a] = k != 0 ? s / k : INVALID_VALUE;
			}

			for (int a = 0; a < 360; a++)
			{
				P(a, p) = t[a];
			}
		}
	}

	// ==================================================================

	{
		// Set the values in P equal to the difference between a small and large neighbourhood average
		Matrix<real_t> t(2, NumSteps);
		for (int a = 0; a < 360; a++)
		{
			for (int j = 0; j < 2; j++)
			{
				int m = j == 0 ? 10 : 50;

				for (int p = 0; p < NumSteps; p++)
				{
					real_t s = 0.0;
					int k = 0;

					for (int i = -m; i <= m; i++)
					{
						if (p + i >= 0 && p + i < NumSteps)
						{
							real_t v = P(a, p + i);
							if (v != INVALID_VALUE)
							{
								int q = m + 1 - abs(i);
								s += q * v;
								k += q;
							}
						}
					}

					t(j, p) = k != 0 ? s / k : INVALID_VALUE;
				}
			}

			for (int p = 0; p < NumSteps; p++)
			{
				real_t v0 = t(0, p);
				real_t v1 = t(1, p);

				if (v0 != INVALID_VALUE && v1 != INVALID_VALUE)
				{
					P(a, p) = v0 - v1;
				}
			}
		}
	}

  auto dbgImg1 = dbg::DrawRingsUnwrapped(P, this);

  // ==================================================================

	for (int a = 0; a < 360; a++)
	{
		Matrix<int> Extrema(m_NumRings - 1);

		int f = __min((int)(m_ri_r_um[0][a] / UmStep) + 3, NumSteps - 1);

		// find extrema
		int NumExtrema = 0;
		for (int p = f; p < NumSteps - 1 && NumExtrema < m_NumRings - 1; p++)
		{
			real_t p1 = P(a, p - 1);
			real_t p2 = P(a, p);
			real_t p3 = P(a, p + 1);

			if (p3 == INVALID_VALUE || p2 == INVALID_VALUE || p1 == INVALID_VALUE) break;

			BOOL OddExtremum = NumExtrema & 1;

			if ((!OddExtremum && p1 < p2 && p2 > p3) || (OddExtremum && p1 > p2 && p2 < p3))
			{
				Extrema[NumExtrema++] = p;
			}
		}

		if (NumExtrema < 2) continue;

		// find edges
		Matrix<int> Edges(m_NumRings);
		Edges.Fill(INVALID_VALUE);

		// all edges except for first and last one
		for (int r = 1; r < NumExtrema; r++)
		{
			for (int p = Extrema[r - 1] + 1; p <= Extrema[r] - 1; p++)
			{
				if (fabs(P(a, p) - P(a, p - 1)) > fabs(P(a, p) - P(a, p + 1)))
				{
					Edges[r] = p - 1;
					break;
				}
			}
		}

		// last edge
		for (int p = Extrema[NumExtrema - 1] + 1; p < NumSteps - 1; p++)
		{
			real_t p1 = P(a, p - 1);
			real_t p2 = P(a, p);
			real_t p3 = P(a, p + 1);

			if (p3 == INVALID_VALUE || p2 == INVALID_VALUE || p1 == INVALID_VALUE) break;

			if (fabs(p2 - p1) > fabs(p2 - p3))
			{
				Edges[NumExtrema++] = p - 1;
				break;
			}
		}

		for (int r = 1; r < NumExtrema; r++)
		{
			m_ri_r_um[r][a] = Edges[r] == INVALID_VALUE ? INVALID_VALUE : Edges[r] * UmStep;
		}

	}

  auto dbgImg2 = dbg::DrawRingsUnwrapped(P, this);

  // ==================================================================

	// Specially correct for 2 20 degree wedges around 0 and 180 degree spokes, only for the last 2 rings
	for (int r = m_NumRings - 2; r < m_NumRings; r++)
	{
		int da = 10;

		for (int a = -da; a <= da; a++)
		{
			m_ri_r_um[r][CheckAngle(a)] = INVALID_VALUE;
		}

		for (int a = 180 - da; a <= 180 + da; a++)
		{
			m_ri_r_um[r][a] = INVALID_VALUE;
		}

		int a1 = -da - 1;
		int a2 = da + 1;
		real_t t1 = m_ri_r_um[r][a1 + 360];
		real_t t2 = m_ri_r_um[r][a2];

		if (t1 != INVALID_VALUE && t2 != INVALID_VALUE && fabs(t2 - t1) <= 100.0)
		{
			for (int a = a1 + 1; a <= a2 - 1; a++)
			{
				m_ri_r_um[r][CheckAngle(a)] = t1 + (a - a1) * (t2 - t1) / (a2 - a1);
			}
		}


		a1 = 180 - da - 1;
		a2 = 180 + da + 1;
		t1 = m_ri_r_um[r][a1];
		t2 = m_ri_r_um[r][a2];

		if (t1 != INVALID_VALUE && t2 != INVALID_VALUE && fabs(t2 - t1) <= 100.0)
		{
			for (int a = a1 + 1; a <= a2 - 1; a++) {
				m_ri_r_um[r][a] = t1 + (a - a1) * (t2 - t1) / (a2 - a1);
			}
		}
	}

  auto dbgImg3 = dbg::DrawRingsUnwrapped(P, this);

  // ==================================================================

	// Searches for continuous ring measurement segments
	for (int r = 0; r < m_NumRings; r++)
	{
		struct { int f, l; } Chains[360];// f-first, l-last spoke angles for each chain. 360 reserved as worst case
		int NumChains = 0;
		BOOL LookForBeginning = TRUE;
		int f;

		for (int a = 0; a <= 360; a++)
		{
			if (LookForBeginning)
			{
				if (m_ri_r_um[r][a == 360 ? 0 : a] != INVALID_VALUE)
				{
					f = a;
					LookForBeginning = FALSE;
				}
			}
			else
			{
				if (a == 360)
				{
					Chains[NumChains].f = f;
					Chains[NumChains].l = 359;
					NumChains++;
				}
				else if (m_ri_r_um[r][a] == INVALID_VALUE || intRound(fabs(m_ri_r_um[r][a] - m_ri_r_um[r][a - 1])) > 2.0 * UmStep)
				{
					Chains[NumChains].f = f;
					Chains[NumChains].l = a - 1;
					NumChains++;
					LookForBeginning = TRUE;
					if (m_ri_r_um[r][a] != INVALID_VALUE) a--;
				}
			}
		}

		if (NumChains > 1)
		{
			if (Chains[0].f == 0 && Chains[NumChains - 1].l == 359)
			{
				Chains[0].f = Chains[NumChains - 1].f - 360;
				NumChains--;
			}
		}

		// Invalidate any short chains (<90 continuous degrees)
		for (int c = 0; c < NumChains; c++)
		{
			if (Chains[c].l - Chains[c].f + 1 < 90)
			{
				for (int a = Chains[c].f; a <= Chains[c].l; a++)
				{
					m_ri_r_um[r][CheckAngle(a)] = INVALID_VALUE;
				}
			}
		}
	}

  auto dbgImg4 = dbg::DrawRingsUnwrapped(P, this);

	// ==================================================================

	{
		// An additional smoothing
		real_t t[360];
		int m = 3;
		for (int r = 0; r < m_NumRings; r++)
		{
			for (int a = 0; a < 360; a++)
			{
				if (m_ri_r_um[r][a] != INVALID_VALUE)
				{
					real_t s = 0.0;
					int k = 0;
					for (int i = -m; i <= m; i++)
					{
						real_t v = m_ri_r_um[r][CheckAngle(a + i)];
						if (v != INVALID_VALUE)
						{
							int q = m + 1 - abs(i);
							s += q * v;
							k += q;
						}
					}
					t[a] = s / k;
				}
				else
				{
					t[a] = INVALID_VALUE;
				}
			}

			for (int a = 0; a < 360; a++)
			{
				m_ri_r_um[r][a] = t[a];
			}
		}
	}

  auto dbgImg5 = dbg::DrawRingsUnwrapped(P, this);

	m_ri_ok = TRUE;
}

//void CEyeImage::FindVertexAndRings()
//{
//	ClearVertexAndRings();
//
//	if (m_RGBData.GetMem() == NULL) return;
//
//	real_t cx = 0.5 * m_w;
//	real_t cy = 0.5 * m_h;
//	real_t x_px_um = m_w / m_w_um;
//	real_t y_px_um = m_h / m_h_um;
//
//	BOOL ve_ok;
//	real_t ve_x_um;
//	real_t ve_y_um;
//	real_t ri0_um[360];
//
//	int kl1[5] = { 30, 25, 20, 15, 10 };
//
//	for (int l1 = 0; l1 < 5; l1++)
//	{
//		ve_ok = FALSE;
//		ve_x_um = 0.0;
//		ve_y_um = 0.0;
//
//		const int nl2 = 3;
//		for (int l2 = 0; l2 < nl2; l2++)
//		{
//			int edge[360];
//			memset(edge, 0, sizeof(edge));
//
//			for (int a = 0; a < 360; a++)
//			{
//				// 101  10 ,  200  1000
//				real_t t[101];
//				for (int r = 0; r <= 100; r++)
//				{
//					real_t r_um = 200.0 + r * 10.0;
//					real_t x_um = ve_x_um + r_um * COS[a];
//					real_t y_um = ve_y_um + r_um * SIN[a];
//					int x = intRound(cx + x_um * x_px_um);
//					int y = intRound(cy + y_um * y_px_um);
//					t[r] = GetRAt(x, y);
//				}
//
//				real_t p[101];
//				real_t min = DBL_MAX;
//				real_t max = -DBL_MAX;
//				int k = kl1[l1];
//
//				for (int r = 10; r <= 90; r++)
//				{
//					real_t s = 0.0;
//					for (int i = -10; i <= 10; i++)
//					{
//						int q = k - abs(i);
//						s += q * t[r + i];
//					}
//					p[r] = s;
//					if (s < min) min = s;
//					if (s > max) max = s;
//				}
//
//				if (max - min <= 0.000000001) return;
//
//				for (int r = 10; r <= 90; r++)
//				{
//					p[r] = (p[r] - min) / (max - min);
//				}
//
//				int rmax = 0;
//				for (int r = 11; r <= 89; r++)
//				{
//					if (p[r] > 0.25 && p[r - 1] < p[r] && p[r] > p[r + 1])
//					{
//						rmax = r;
//						break;
//					}
//				}
//
//				real_t dmax = -DBL_MAX;
//				for (int r = rmax - 1; r >= 11; r--)
//				{
//					real_t d = p[r] - p[r - 1];
//					if (dmax < d)
//					{
//						dmax = d;
//						edge[a] = r;
//					}
//				}
//			}
//
//			int z[360];
//			memcpy(z, edge, sizeof(z));
//			sort_asc(z, 360, 45);
//			int z1 = z[44];
//			memcpy(z, edge, sizeof(z));
//			sort_des(z, 360, 45);
//			int z2 = z[44];
//			for (int a = 0; a < 360; a++)
//			{
//				if (edge[a] < z1 || edge[a] > z2) edge[a] = INVALID_VALUE;
//			}
//
//			for (int a = 0; a < 360; a++)
//			{
//				int s = 0;
//				int k = 0;
//				for (int i = -30; i <= 30; i++)
//				{
//					int v = edge[CheckAngle(a + i)];
//					if (v != INVALID_VALUE)
//					{
//						int q = 31 - abs(i);
//						s += q * v;
//						k += q;
//					}
//				}
//				ri0_um[a] = k != 0 ? (200.0 + ((real_t)s / (real_t)k) * 10.0 - 5.0) : INVALID_VALUE;
//			}
//
//			if (l2 < nl2 - 1)
//			{
//				real_t x_min_um = DBL_MAX;
//				real_t x_max_um = -DBL_MAX;
//				real_t y_min_um = DBL_MAX;
//				real_t y_max_um = -DBL_MAX;
//				for (int a = 0; a < 360; a++)
//				{
//					if (ri0_um[a] != INVALID_VALUE)
//					{
//						real_t x_um = ve_x_um + ri0_um[a] * COS[a];
//						real_t y_um = ve_y_um + ri0_um[a] * SIN[a];
//						if (x_min_um > x_um) x_min_um = x_um;
//						if (x_max_um < x_um) x_max_um = x_um;
//						if (y_min_um > y_um) y_min_um = y_um;
//						if (y_max_um < y_um) y_max_um = y_um;
//					}
//				}
//				ve_x_um = (x_min_um + x_max_um) * 0.5;
//				ve_y_um = (y_min_um + y_max_um) * 0.5;
//			}
//		}
//
//		BOOL Broken = FALSE;
//		real_t min = DBL_MAX;
//		real_t max = -DBL_MAX;
//		for (int a = 0; a < 360; a++)
//		{
//			if (ri0_um[a] == INVALID_VALUE)
//			{
//				Broken = TRUE;
//				break;
//			}
//			else
//			{
//				if (ri0_um[a] < min) min = ri0_um[a];
//				if (ri0_um[a] > max) max = ri0_um[a];
//			}
//		}
//
//		if (!Broken && (max - min < 200.0))
//		{
//			BOOL Smooth = TRUE;
//			for (int a = 0; a < 359; a++)
//			{
//				if (fabs(ri0_um[a + 1] - ri0_um[a]) > 4.0) { // 2009-03-15 changed from 3.0 to 4.0
//					Smooth = FALSE;
//					break;
//				}
//			}
//			ve_ok = Smooth;
//		}
//
//		if (ve_ok) break;
//	}
//
//	if (!ve_ok) return;
//
//	// ==================================================================
//
//	m_ve_ok = TRUE;
//	m_ve_x_um = ve_x_um;
//	m_ve_y_um = ve_y_um;
//
//	for (int a = 0; a < 360; a++)
//	{
//		m_ri_r_um[0][a] = ri0_um[a];
//	}
//
//	// ==================================================================
//
//	int NumSteps = 512;
//	real_t UmStep = 5200.0 / (NumSteps - 1);
//	Matrix<real_t> P(360, NumSteps);
//	P.Fill(INVALID_VALUE);
//	for (int a = 0; a < 360; a++)
//	{
//		for (int s = 0; s < NumSteps; s++)
//		{
//			real_t r_um = s * UmStep;
//			real_t x_um = m_ve_x_um + r_um * COS[a];
//			real_t y_um = m_ve_y_um + r_um * SIN[a];
//			real_t p;
//			if (!GetRGBAtUm(x_um, y_um, &p, NULL, NULL)) break;
//			P(a, s) = p;
//		}
//	}
//
//	// ==================================================================
//
//	{
//		real_t t[360];
//		const int m = 10;
//		for (int p = 0; p < NumSteps; p++)
//		{
//			for (int a = 0; a < 360; a++)
//			{
//				real_t s = 0.0;
//				int k = 0;
//
//				for (int i = -m; i <= m; i++)
//				{
//					real_t v = P(CheckAngle(a + i), p);
//					if (v != INVALID_VALUE)
//					{
//						int q = m + 1 - abs(i);
//						s += q * v;
//						k += q;
//					}
//				}
//
//				t[a] = k != 0 ? s / k : INVALID_VALUE;
//			}
//
//			for (int a = 0; a < 360; a++)
//			{
//				P(a, p) = t[a];
//			}
//		}
//	}
//
//	// ==================================================================
//
//	{
//		Matrix<real_t> t(2, NumSteps);
//		for (int a = 0; a < 360; a++)
//		{
//			for (int j = 0; j < 2; j++)
//			{
//				int m = j == 0 ? 10 : 50;
//
//				for (int p = 0; p < NumSteps; p++)
//				{
//					real_t s = 0.0;
//					int k = 0;
//
//					for (int i = -m; i <= m; i++)
//					{
//						if (p + i >= 0 && p + i < NumSteps)
//						{
//							real_t v = P(a, p + i);
//							if (v != INVALID_VALUE)
//							{
//								int q = m + 1 - abs(i);
//								s += q * v;
//								k += q;
//							}
//						}
//					}
//
//					t(j, p) = k != 0 ? s / k : INVALID_VALUE;
//				}
//			}
//
//			for (int p = 0; p < NumSteps; p++)
//			{
//				real_t v0 = t(0, p);
//				real_t v1 = t(1, p);
//
//				if (v0 != INVALID_VALUE && v1 != INVALID_VALUE)
//				{
//					P(a, p) = v0 - v1;
//				}
//			}
//		}
//	}
//
//	// ==================================================================
//
//	for (int a = 0; a < 360; a++)
//	{
//		Matrix<int> Extrema(m_NumRings - 1);
//
//		int f = __min((int)(m_ri_r_um[0][a] / UmStep) + 3, NumSteps - 1);
//
//		// find extrema
//		int NumExtrema = 0;
//		for (int p = f; p < NumSteps - 1 && NumExtrema < m_NumRings - 1; p++)
//		{
//			real_t p1 = P(a, p - 1);
//			real_t p2 = P(a, p);
//			real_t p3 = P(a, p + 1);
//
//			if (p3 == INVALID_VALUE || p2 == INVALID_VALUE || p1 == INVALID_VALUE) break;
//
//			BOOL OddExtremum = NumExtrema & 1;
//
//			if ((!OddExtremum && p1 < p2 && p2 > p3) || (OddExtremum && p1 > p2 && p2 < p3))
//			{
//				Extrema[NumExtrema++] = p;
//			}
//		}
//
//		if (NumExtrema < 2) continue;
//
//		// find edges
//		Matrix<int> Edges(m_NumRings);
//		Edges.Fill(INVALID_VALUE);
//
//		// all edges except for first and last one
//		for (int r = 1; r < NumExtrema; r++)
//		{
//			for (int p = Extrema[r - 1] + 1; p <= Extrema[r] - 1; p++)
//			{
//				if (fabs(P(a, p) - P(a, p - 1)) > fabs(P(a, p) - P(a, p + 1)))
//				{
//					Edges[r] = p - 1;
//					break;
//				}
//			}
//		}
//
//		// last edge
//		for (int p = Extrema[NumExtrema - 1] + 1; p < NumSteps - 1; p++)
//		{
//			real_t p1 = P(a, p - 1);
//			real_t p2 = P(a, p);
//			real_t p3 = P(a, p + 1);
//
//			if (p3 == INVALID_VALUE || p2 == INVALID_VALUE || p1 == INVALID_VALUE) break;
//
//			if (fabs(p2 - p1) > fabs(p2 - p3))
//			{
//				Edges[NumExtrema++] = p - 1;
//				break;
//			}
//		}
//
//		for (int r = 1; r < NumExtrema; r++)
//		{
//			m_ri_r_um[r][a] = Edges[r] == INVALID_VALUE ? INVALID_VALUE : Edges[r] * UmStep;
//		}
//
//	}
//
//	// ==================================================================
//
//	for (int r = m_NumRings - 2; r < m_NumRings; r++)
//	{
//		int da = 10;
//
//		for (int a = -da; a <= da; a++)
//		{
//			m_ri_r_um[r][CheckAngle(a)] = INVALID_VALUE;
//		}
//
//		for (int a = 180 - da; a <= 180 + da; a++)
//		{
//			m_ri_r_um[r][a] = INVALID_VALUE;
//		}
//
//		int a1 = -da - 1;
//		int a2 = da + 1;
//		real_t t1 = m_ri_r_um[r][a1 + 360];
//		real_t t2 = m_ri_r_um[r][a2];
//
//		if (t1 != INVALID_VALUE && t2 != INVALID_VALUE && fabs(t2 - t1) <= 100.0)
//		{
//			for (int a = a1 + 1; a <= a2 - 1; a++)
//			{
//				m_ri_r_um[r][CheckAngle(a)] = t1 + (a - a1) * (t2 - t1) / (a2 - a1);
//			}
//		}
//
//
//		a1 = 180 - da - 1;
//		a2 = 180 + da + 1;
//		t1 = m_ri_r_um[r][a1];
//		t2 = m_ri_r_um[r][a2];
//
//		if (t1 != INVALID_VALUE && t2 != INVALID_VALUE && fabs(t2 - t1) <= 100.0)
//		{
//			for (int a = a1 + 1; a <= a2 - 1; a++) {
//				m_ri_r_um[r][a] = t1 + (a - a1) * (t2 - t1) / (a2 - a1);
//			}
//		}
//	}
//
//	// ==================================================================
//
//	for (int r = 0; r < m_NumRings; r++)
//	{
//		struct { int f, l; } Chains[360];
//		int NumChains = 0;
//		BOOL LookForBeginning = TRUE;
//		int f;
//
//		for (int a = 0; a <= 360; a++)
//		{
//			if (LookForBeginning)
//			{
//				if (m_ri_r_um[r][a == 360 ? 0 : a] != INVALID_VALUE)
//				{
//					f = a;
//					LookForBeginning = FALSE;
//				}
//			}
//			else
//			{
//				if (a == 360)
//				{
//					Chains[NumChains].f = f;
//					Chains[NumChains].l = 359;
//					NumChains++;
//				}
//				else if (m_ri_r_um[r][a] == INVALID_VALUE || intRound(fabs(m_ri_r_um[r][a] - m_ri_r_um[r][a - 1])) > 2.0 * UmStep)
//				{
//					Chains[NumChains].f = f;
//					Chains[NumChains].l = a - 1;
//					NumChains++;
//					LookForBeginning = TRUE;
//					if (m_ri_r_um[r][a] != INVALID_VALUE) a--;
//				}
//			}
//		}
//
//		if (NumChains > 1)
//		{
//			if (Chains[0].f == 0 && Chains[NumChains - 1].l == 359)
//			{
//				Chains[0].f = Chains[NumChains - 1].f - 360;
//				NumChains--;
//			}
//		}
//
//		for (int c = 0; c < NumChains; c++)
//		{
//			if (Chains[c].l - Chains[c].f + 1 < 90)
//			{
//				for (int a = Chains[c].f; a <= Chains[c].l; a++)
//				{
//					m_ri_r_um[r][CheckAngle(a)] = INVALID_VALUE;
//				}
//			}
//		}
//	}
//
//	// ==================================================================
//
//	{
//		real_t t[360];
//		int m = 3;
//		for (int r = 0; r < m_NumRings; r++)
//		{
//			for (int a = 0; a < 360; a++)
//			{
//				if (m_ri_r_um[r][a] != INVALID_VALUE)
//				{
//					real_t s = 0.0;
//					int k = 0;
//					for (int i = -m; i <= m; i++)
//					{
//						real_t v = m_ri_r_um[r][CheckAngle(a + i)];
//						if (v != INVALID_VALUE)
//						{
//							int q = m + 1 - abs(i);
//							s += q * v;
//							k += q;
//						}
//					}
//					t[a] = s / k;
//				}
//				else
//				{
//					t[a] = INVALID_VALUE;
//				}
//			}
//
//			for (int a = 0; a < 360; a++)
//			{
//				m_ri_r_um[r][a] = t[a];
//			}
//		}
//	}
//
//	m_ri_ok = TRUE;
//}

//***************************************************************************************

//7.0.0 Calculation of m_Ring_Ratio;
void CEyeImage::CalRingRatio()
{
	int total = 0;
	int bad = 0;

	real_t Ave[24];
	int  AveCount[24];

	for (int r = 0; r < m_NumRings; r++)
	{
		Ave[r] = 0;
		AveCount[r] = 0;

		for (int a = 0; a < 360; a++)
		{
			if (m_ri_r_um[r][a] == INVALID_VALUE)
			{
				bad++;
			}
			else
			{
				Ave[r] += m_ri_r_um[r][a];
				AveCount[r]++;
			}

			total++;

			//m_Ring_4mRatio
		}

		if (AveCount[r] != 0)
		{
			Ave[r] /= (real_t)AveCount[r];
		}
	}

	real_t BadRatio = (real_t)bad / (real_t)total;

	m_Ring_Ratio = 100 * (real_t)(total - bad) / (real_t)total;

	//ring ratio < 4mm
	bad = 0;
	total = 0;

	for (int r = 0; r < m_NumRings; r++)
	{
		if (Ave[r] <= 2000)
		{
			for (int a = 0; a < 360; a++)
			{
				if (m_ri_r_um[r][a] == INVALID_VALUE)
				{
					bad++;
				}

				total++;

				//m_Ring_4mRatio
			}
		}
	}

	m_Ring_4mRatio = 100 * (real_t)(total - bad) / (real_t)total;

	int  a = 0;
}

//***************************************************************************************

//7.0.0
//good being defined something like 100% of rings detected in central 4mm and 80% of whole eye
BOOL CEyeImage::IsRingsDataGood(BOOL NeeFindRing)
{
	if (NeeFindRing)
	{
		//int S = (int)clock();

		FindVertexAndRings();

		//int E = (int)clock() - S;
	}

	if (!m_ri_ok) return FALSE;

	int total = 0;
	int bad = 0;

	real_t Ave[24];
	int  AveCount[24];

	for (int r = 0; r < m_NumRings; r++)
	{
		Ave[r] = 0;
		AveCount[r] = 0;

		for (int a = 0; a < 360; a++)
		{
			if (m_ri_r_um[r][a] == INVALID_VALUE)
			{
				bad++;
			}
			else
			{
				Ave[r] += m_ri_r_um[r][a];
				AveCount[r]++;
			}

			total++;

			//m_Ring_4mRatio
		}

		if (AveCount[r] != 0)
		{
			Ave[r] /= (real_t)AveCount[r];
		}
	}

	real_t BadRatio = (real_t)bad / (real_t)total;

	m_Ring_Ratio = 100 * (real_t)(total - bad) / (real_t)total;

	//ring ratio < 4mm
	bad = 0;
	total = 0;

	for (int r = 0; r < m_NumRings; r++)
	{
		if (Ave[r] <= 2000)
		{
			for (int a = 0; a < 360; a++)
			{
				if (m_ri_r_um[r][a] == INVALID_VALUE)
				{
					bad++;
				}

				total++;

				//m_Ring_4mRatio
			}
		}
	}

	m_Ring_4mRatio = 100 * (real_t)(total - bad) / (real_t)total;

	if (BadRatio > 0.2 || m_Ring_4mRatio < 100)
	{
		return FALSE;
	}

	return TRUE;
}

//***************************************************************************************

BOOL CEyeImage::IsRingsDataComplete()
{
	if (!m_ri_ok) return FALSE;

	for (int r = 0; r < m_NumRings; r++)
	{
		for (int a = 0; a < 360; a++)
		{
			if (m_ri_r_um[r][a] == INVALID_VALUE) return FALSE;
		}
	}

	return TRUE;
}

//***************************************************************************************

//7.0.0
BOOL CEyeImage::CheckBlink(int threshold)
{
	BOOL res = FALSE;

	//int St = (int)clock();
	real_t pR_ave = 0;
	real_t pG_ave = 0;
	real_t pB_ave = 0;

	int r = (intRound)(0.35*(real_t)m_h);

	int t = 0;

	int x, y;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;

	for (int a = 0; a < 180; a++)
	{
		for (int i = 10; i <= r; i++)
		{
			x = intRound(cx + i * ::COS[a]);
			y = intRound(cy + i * ::SIN[a]);

			int pR = GetRAt(x, y);
			int pG = GetGAt(x, y);
			int pB = GetBAt(x, y);

			pR_ave += pR;
			pG_ave += pR;
			pB_ave += pR;

			t++;
		}
	}

	pR_ave = pR_ave / (real_t)t;
	pG_ave = pG_ave / (real_t)t;
	pB_ave = pB_ave / (real_t)t;

	if (pR_ave > threshold)
	{
		res = TRUE;
	}

	return res;
}

//***************************************************************************************

//7.0.0
real_t CEyeImage::GetAveBright()
{
	real_t pR_ave = 0;

	int r = (intRound)(0.35*(real_t)m_h);

	int t = 0;

	int x, y;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;

	for (int a = 0; a < 180; a++)
	{
		for (int i = 10; i <= r; i++)
		{
			x = intRound(cx + i * ::COS[a]);
			y = intRound(cy + i * ::SIN[a]);

			int pR = GetRAt(x, y);

			pR_ave += pR;

			t++;
		}
	}

	pR_ave = pR_ave / (real_t)t;

	return pR_ave;
}

//***************************************************************************************

//7.0.0
void CEyeImage::FindWFVertex()
{
	ClearVertex();

	if (m_RGBData.GetMem() == NULL) return;

	if (!m_pu_ok) return;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	int r_border = intRound(m_pu_r_max_um* x_px_um);
	int l_border = -1 * r_border;

	int x0 = intRound(cx + m_pu_x0_um * x_px_um);
	int y0 = intRound(cy + m_pu_y0_um * y_px_um);

	FindWFVertex_New(x0, y0, r_border, l_border);

	int vex = intRound(m_ve_x_um * x_px_um);
	int vey = intRound(m_ve_y_um * x_px_um);

	if ((vex <= 50 && vex >= -50) &&
		(vey <= 50 && vey >= -50)
		)
	{
		FindWFVertex_Ori();
	}
}
//***************************************************************************************

//7.0.0 2017-12-06 improvement by cjf,
//1. search inside the pupil
void CEyeImage::FindWFVertex_New(int x0, int y0, int r_border, int l_border)
{
	ClearVertex();

	if (m_RGBData.GetMem() == NULL) return;

	if (!m_pu_ok) return;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	int ST = (int)clock();
	int p_max = 0;
	for (int i = l_border; i < r_border; i++)
	{
		for (int j = l_border; j < r_border; j++)
		{
			if ((x0 + i >= 0 && x0 + i < m_w) &&
				(y0 + j >= 0 && y0 + j < m_h)
				)
			{
				int p = GetRAt(x0 + i, y0 + j);
				if (p > p_max)
				{
					p_max = p;
				}
			}
		}
	}

	int p_thr = 4 * p_max / 5;

	int si = 0;
	int sj = 0;
	int s = 0;
	for (int i = l_border; i < r_border; i++)
	{
		for (int j = l_border; j < r_border; j++)
		{
			if ((x0 + i >= 0 && x0 + i < m_w) &&
				(y0 + j >= 0 && y0 + j < m_h)
				)
			{
				int p = GetRAt(x0 + i, y0 + j);
				if (p >= p_thr)
				{
					si += i;
					sj += j;
					s++;
				}
			}
		}
	}
	x0 += si / s;
	y0 += sj / s;

	int i_best = 0;
	int j_best = 0;
	int r_best = 0;
	int s_best = 0;
	for (int i = -10; i <= 10; i++)
	{
		for (int j = -10; j <= 10; j++)
		{
			int s[26];
			for (int r = 11; r <= 25; r++)
			{
				s[r] = 0;
				for (int a = 0; a < 360; a += 10)
				{
					int x1 = intRound(x0 + i + r * ::COS[a]);
					int y1 = intRound(y0 + j + r * ::SIN[a]);

					if ((x1 >= 0 && x1 < m_w) &&
						(y1 >= 0 && y1 < m_h)
						)
					{
						int p = GetRAt(x1, y1);
						if (p >= p_thr) s[r] += p;
					}
				}
			}

			for (int r = 13; r <= 23; r++)
			{
				int ss = s[r - 2] + s[r - 1] + s[r] + s[r + 1] + s[r + 2];
				if (ss > s_best)
				{
					i_best = i;
					j_best = j;
					r_best = r;
					s_best = ss;
				}
			}
		}
	}

	x0 += i_best;
	y0 += j_best;

	m_ve_x_um = (x0 - cx) / x_px_um;
	m_ve_y_um = (y0 - cy) / y_px_um;
	m_ve_r = r_best;

	m_ve_ok = TRUE;
}

//***************************************************************************************

void CEyeImage::FindWFVertex_Ori()
{
	ClearVertex();

	if (m_RGBData.GetMem() == NULL) return;

	real_t cx = 0.5 * m_w;
	real_t cy = 0.5 * m_h;
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	int x0 = intRound(cx);
	int y0 = intRound(cy);

	int p_max = 0;
	for (int i = -50; i < 50; i++)
	{
		for (int j = -50; j < 50; j++)
		{
			int p = GetRAt(x0 + i, y0 + j);
			if (p > p_max)
			{
				p_max = p;
			}
		}
	}

	int p_thr = 4 * p_max / 5;

	int si = 0;
	int sj = 0;
	int s = 0;
	for (int i = -50; i < 50; i++)
	{
		for (int j = -50; j < 50; j++)
		{
			int p = GetRAt(x0 + i, y0 + j);
			if (p >= p_thr)
			{
				si += i;
				sj += j;
				s++;
			}
		}
	}
	x0 += si / s;
	y0 += sj / s;

	int i_best = 0;
	int j_best = 0;
	int r_best = 0;
	int s_best = 0;
	for (int i = -10; i <= 10; i++)
	{
		for (int j = -10; j <= 10; j++)
		{
			int s[26];
			for (int r = 11; r <= 25; r++)
			{
				s[r] = 0;
				for (int a = 0; a < 360; a += 10)
				{
					int p = GetRAt(intRound(x0 + i + r * ::COS[a]), intRound(y0 + j + r * ::SIN[a]));
					if (p >= p_thr) s[r] += p;
				}
			}

			for (int r = 13; r <= 23; r++)
			{
				int ss = s[r - 2] + s[r - 1] + s[r] + s[r + 1] + s[r + 2];
				if (ss > s_best)
				{
					i_best = i;
					j_best = j;
					r_best = r;
					s_best = ss;
				}
			}
		}
	}

	x0 += i_best;
	y0 += j_best;

	m_ve_x_um = (x0 - cx) / x_px_um;
	m_ve_y_um = (y0 - cy) / y_px_um;
	m_ve_r = r_best;
	m_ve_ok = TRUE;
}

//***************************************************************************************

//improve the comprison of image
void CEyeImage::ImproveComprison()
{
	if (m_RGBData.GetMem() == NULL) return;

	int w = 624;
	int h = 468;
	int num = w*h;
	int AveG = 0;

	//16*16 a block for improve comparison

	int **NewGrayVal = new int*[16];
	for (int i = 0; i < 16; i++) NewGrayVal[i] = new int[16];

	for (int i = 0; i < 38; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			int ave = 0;
			int m = 0;

			//improve the comparison in this block
			for (int k = 0; k < 16; k++)
			{
				int x = i * 16 + k;
				for (int l = 0; l < 16; l++)
				{
					int y = j * 16 + l;

					/*int R0 = GetRAt(x,y);
					int G0 = GetGAt(x,y);
					int B0 = GetBAt(x,y);
					int Gray0 = intRound(0.3*R0 + 0.59*G0 + 0.11*B0);	*/

					NewGrayVal[k][l] = GetRAt(x, y);
					ave += NewGrayVal[k][l];

					m++;
				}
			}
			ave = intRound(ave / m);

			for (int k = 0; k < 16; k++)
			{
				int x = i * 16 + k;
				for (int l = 0; l < 16; l++)
				{
					int y = j * 16 + l;

					int newR = ave + intRound(2.5*(NewGrayVal[k][l] - ave));

					if (newR > 250) newR = 250;
					else if (newR < 0) newR = 0;

					SetRAt(x, y, newR);
					SetGAt(x, y, newR);
					SetBAt(x, y, newR);
				}
			}
			//improve the comparison in this block done

		}
	}

	if (NewGrayVal != NULL)
	{
		for (int i = 0; i<16; i++)
		{
			if (NewGrayVal[i] != NULL)
			{
				delete[](NewGrayVal[i]);
				NewGrayVal[i] = NULL;
			}
		}
		delete[](NewGrayVal);
		NewGrayVal = NULL;
	}
}


//***************************************************************************************

void CEyeImage::FindLimbus()
{
	// Debug
	std::string  path = "C:\\1\\FL\\table.txt";
	std::string  path1 = "C:\\1\\FL\\table1.txt";
	std::fstream strm;

	ClearLimbus();

	if (m_RGBData.GetMem() == NULL) return;

	real_t cx = m_w * 0.5;
	real_t cy = m_h * 0.5;
	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	m_li_x0_um = 0.0;
	m_li_y0_um = 0.0;

	for (int l = 0; l < 2; l++)
	{
		//--------------------------------------------------------------
		Matrix<real_t> li_r_um_1(3600);
		for (int a = 0; a < 3600; a++)
		{
			Matrix<real_t> p1(301);
			Matrix<real_t> p2(301);

			for (int r = 0; r <= 300; r++)
			{
				real_t r_um = 4500.0 + (r - 25) * 10.0;
				real_t a_rd = 0.1 * a * _Pi_180;
				if (!GetRGBAtUm(m_li_x0_um + r_um * cos(a_rd), m_li_y0_um + r_um * sin(a_rd), &p1[r], NULL, NULL)) {
					p1[r] = INVALID_VALUE;
				}
			}

			for (int r = 25; r <= 275; r++)
			{
				p2[r] = 0.0;
				int n = 0;

				for (int k = -25; k <= 25; k++)
				{
					if (p1[r + k] != INVALID_VALUE)
					{
						int m = 26 - abs(k);
						p2[r] += m * p1[r + k];
						n += m;
					}
				}

				if (n != 0)
				{
					p2[r] /= n;
				}
				else
				{
					p2[r] = INVALID_VALUE;
				}
			}

			real_t dmax = -1000000.0;
			int rmax = 0;
			for (int r = 25; r < 275; r++)
			{
				if (p2[r] == INVALID_VALUE || p2[r + 1] == INVALID_VALUE) continue;
				real_t d = p2[r + 1] - p2[r];
				if (dmax < d)
				{
					dmax = d;
					rmax = r;
				}
			}

			li_r_um_1[a] = 4500.0 + (rmax - 25) * 10.0 + 5.0;
		}

		//--------------------------------------------------------------
		Matrix<real_t> li_r_um_2(360);
		for (int a = 0; a < 360; a++)
		{
			real_t q[21];

			for (int b = 0; b <= 20; b++)
			{
				int c = 10 * a + (b - 10);
				if (c < 0) c += 3600; else if (c >= 3600) c -= 3600;
				q[b] = li_r_um_1[c];
			}

			sort_asc(q, 21, 11);
			li_r_um_2[a] = q[10];
		}

		//--------------------------------------------------------------
		for (int a = 0; a < 360; a++)
		{
			real_t q[41];
			for (int b = 0; b <= 40; b++)
			{
				int c = a + (b - 20);
				if (c < 0) c += 360; else if (c >= 360) c -= 360;
				q[b] = li_r_um_2[c];
			}
			sort_asc(q, 41, 21);
			m_li_r_um[a] = q[20];
		}

		//--------------------------------------------------------------
		if (l == 0)
		{
			real_t x_min_um = 1000000.0;

			for (int a = 160; a <= 200; a++)
			{
				real_t x_um = m_li_x0_um + m_li_r_um[a] * COS[a];
				if (x_um < x_min_um) x_min_um = x_um;
			}

			real_t x_max_um = -1000000.0;

			for (int a = -20; a <= 20; a++)
			{
				int b = a < 0 ? a + 360 : a;
				real_t x_um = m_li_x0_um + m_li_r_um[b] * COS[b];
				if (x_um > x_max_um) x_max_um = x_um;
			}
			real_t new_li_x0_um = 0.5 * (x_min_um + x_max_um);

			real_t y0_um[80];
			int k = 0;
			for (int m = 0; m < 2; m++)
			{
				int af = m == 0 ? 160 : -20;
				int al = m == 0 ? 200 : 20;

				for (int a = af; a < al; a++)
				{
					int a1 = a; if (a1 < 0) a1 += 360;
					int a2 = a + 1; if (a2 < 0) a2 += 360;
					real_t y1_um = m_li_y0_um + m_li_r_um[a1] * SIN[a1];
					real_t y2_um = m_li_y0_um + m_li_r_um[a2] * SIN[a2];
					if (y1_um != y2_um)
					{
						real_t x1_um = m_li_x0_um + m_li_r_um[a1] * COS[a1];
						real_t x2_um = m_li_x0_um + m_li_r_um[a2] * COS[a2];
						real_t x3_um = 0.5 * (x1_um + x2_um);
						real_t y3_um = 0.5 * (y1_um + y2_um);
						real_t tana = (x1_um - x2_um) / (y1_um - y2_um);
						y0_um[k++] = y3_um - (new_li_x0_um - x3_um) * tana;
					}
				}
			}

			sort_asc(y0_um, k, k / 2 + 1);
			m_li_y0_um = y0_um[k / 2];

			m_li_x0_um = new_li_x0_um;
		}
	}

	//--------------------------------------------------------------
	{
		real_t r_um[82];
		int k = 0;
		for (int m = 0; m < 2; m++)
		{
			int af = m == 0 ? 160 : -20;
			int al = m == 0 ? 200 : 20;

			for (int a = af; a <= al; a++)
			{
				int b = a; if (b < 0) b += 360;
				r_um[k++] = m_li_r_um[b];
			}
		}

		sort_asc(r_um, 82, 41);
		m_li_r_mean_um = r_um[40];
		m_li_r_min_um = m_li_r_mean_um;
		m_li_r_max_um = m_li_r_mean_um;

		for (int a = 0; a < 360; a++)
		{
			m_li_r_um[a] = INVALID_VALUE;
		}

		for (int m = 0; m < 2; m++)
		{
			int af = m == 0 ? 160 : -20;
			int al = m == 0 ? 200 : 20;
			for (int a = af; a <= al; a++)
			{
				int b = a; if (b < 0) b += 360;
				m_li_r_um[b] = m_li_r_mean_um;
			}
		}
	}

	m_li_ok = TRUE;
}

//***************************************************************************************

// the correct limbus searching for color image
void CEyeImage::FindLimbus2(real_t LastCenX, real_t LastCenY, real_t LastR)
{
	ClearLimbus();

	if (m_RGBData.GetMem() == NULL) return;

	real_t x_px_um = m_w / m_w_um;
	real_t y_px_um = m_h / m_h_um;

	m_li_x0_um = LastCenX;
	m_li_y0_um = LastCenY;

	//--------------------------------------------------------------
	real_t li_r_um_1[3600];
	real_t startR = LastR - 250;
	for (int a = 0; a < 900; a++)
	{

		Matrix<real_t> p1(101);
		Matrix<real_t> p2(101);
		for (int r = 0; r <= 100; r++)
		{
			real_t r_um = startR + (r - 25) * 10.0;
			real_t a_rd = 0.4 * a * _Pi_180;
			if (!GetRGBAtUm(m_li_x0_um + r_um * cos(a_rd), m_li_y0_um + r_um * sin(a_rd), &p1[r], NULL, NULL))
			{
				p1[r] = INVALID_VALUE;
			}
		}

		for (int r = 25; r <= 75; r++)
		{
			p2[r] = 0.0;
			int n = 0;
			for (int k = -25; k <= 25; k++)
			{
				if (p1[r + k] != INVALID_VALUE)
				{
					int m = 26 - abs(k);
					p2[r] += m * p1[r + k];
					n += m;
				}
			}
			if (n != 0) p2[r] /= n;
			else p2[r] = INVALID_VALUE;
		}

		real_t dmax = -1000000.0;
		int rmax = 0;

		for (int r = 25; r < 75; r++)
		{
			if (p2[r] == INVALID_VALUE || p2[r + 1] == INVALID_VALUE) continue;
			real_t d = p2[r + 1] - p2[r];
			if (dmax < d)
			{
				dmax = d;
				rmax = r;
			}
		}

		li_r_um_1[a * 4] = startR + (rmax - 25) * 10.0 + 5.0;
		li_r_um_1[a * 4 + 1] = li_r_um_1[a * 4];
		li_r_um_1[a * 4 + 2] = li_r_um_1[a * 4];
		li_r_um_1[a * 4 + 3] = li_r_um_1[a * 4];
	}

	{
		//--------------------------------------------------------------

		Matrix<real_t> li_r_um_2(360);
		for (int a = 0; a < 360; a++)
		{
			real_t q[21];
			for (int b = 0; b <= 20; b++)
			{
				int c = 10 * a + (b - 10);
				if (c < 0) c += 3600; else if (c >= 3600) c -= 3600;
				q[b] = li_r_um_1[c];
			}
			sort_asc(q, 21, 11);
			li_r_um_2[a] = q[10];
		}

		//--------------------------------------------------------------

		for (int a = 0; a < 360; a++)
		{
			real_t q[41];
			for (int b = 0; b <= 40; b++)
			{
				int c = a + (b - 20);
				if (c < 0) c += 360; else if (c >= 360) c -= 360;
				q[b] = li_r_um_2[c];
			}
			sort_asc(q, 41, 21);
			m_li_r_um[a] = q[20];
		}

		//--------------------------------------------------------------

		real_t x_min_um = 1000000.0;

		for (int a = 160; a <= 200; a++)
		{
			real_t x_um = m_li_x0_um + m_li_r_um[a] * COS[a];
			if (x_um < x_min_um) x_min_um = x_um;
		}

		real_t x_max_um = -1000000.0;

		for (int a = -20; a <= 20; a++)
		{
			int b = a < 0 ? a + 360 : a;
			real_t x_um = m_li_x0_um + m_li_r_um[b] * COS[b];
			if (x_um > x_max_um) x_max_um = x_um;
		}

		real_t new_li_x0_um = 0.5 * (x_min_um + x_max_um);

		real_t y0_um[80];
		int k = 0;

		for (int m = 0; m < 2; m++)
		{
			int af = m == 0 ? 160 : -20;
			int al = m == 0 ? 200 : 20;
			for (int a = af; a < al; a++)
			{
				int a1 = a; if (a1 < 0) a1 += 360;
				int a2 = a + 1; if (a2 < 0) a2 += 360;
				real_t y1_um = m_li_y0_um + m_li_r_um[a1] * SIN[a1];
				real_t y2_um = m_li_y0_um + m_li_r_um[a2] * SIN[a2];

				if (y1_um != y2_um)
				{
					real_t x1_um = m_li_x0_um + m_li_r_um[a1] * COS[a1];
					real_t x2_um = m_li_x0_um + m_li_r_um[a2] * COS[a2];
					real_t x3_um = 0.5 * (x1_um + x2_um);
					real_t y3_um = 0.5 * (y1_um + y2_um);
					real_t tana = (x1_um - x2_um) / (y1_um - y2_um);
					y0_um[k++] = y3_um - (new_li_x0_um - x3_um) * tana;
				}
			}
		}

		sort_asc(y0_um, k, k / 2 + 1);
		m_li_y0_um = y0_um[k / 2];

		m_li_x0_um = new_li_x0_um;
	}


	//--------------------------------------------------------------

	real_t r_um[82];
	int k = 0;
	for (int m = 0; m < 2; m++)
	{
		int af = m == 0 ? 160 : -20;
		int al = m == 0 ? 200 : 20;
		for (int a = af; a <= al; a++)
		{
			int b = a; if (b < 0) b += 360;
			r_um[k++] = m_li_r_um[b];
		}
	}
	sort_asc(r_um, 82, 41);
	m_li_r_mean_um = r_um[40];
	m_li_r_min_um = m_li_r_mean_um;
	m_li_r_max_um = m_li_r_mean_um;


	m_li_ok = TRUE;
}
// the correct limbus searching for color image

//***************************************************************************************

void CEyeImage::SwapImageInfo(CEyeImage* FromImage)
{
	m_h = FromImage->m_h;
	m_h_um = FromImage->m_h_um;
	m_w = FromImage->m_w;
	m_w_um = FromImage->m_w_um;

	m_BlurryValue = FromImage->m_BlurryValue;
	m_Find_BL_Val = FromImage->m_Find_BL_Val;

	for (int i = 0; i < 256; i++)
	{
		m_hist[i] = FromImage->m_hist[i];
	}

	m_hist_max = FromImage->m_hist_max;
	m_hull = FromImage->m_hull;
	m_NumRings = FromImage->m_NumRings;

	m_le_ok = FromImage->m_le_ok;

	for (int i = 0; i < 3; i++)
	{
		m_le_x_um[i] = FromImage->m_le_x_um[i];
	}

	for (int i = 0; i < 3; i++)
	{
		m_le_y_um[i] = FromImage->m_le_y_um[i];
	}

	m_table = FromImage->m_table;
}
