//***************************************************************************************

#pragma once

//***************************************************************************************

// #include "GlobalFunctions.h"

class CBasicImage
{
public:

	CBasicImage();
	virtual ~CBasicImage() {}

	void Destroy();

	int m_w;
	int m_h;

	Matrix<uchar> m_RGBData;//Matrix error test
	Matrix<uchar> m_AData;

	BOOL LoadFromRes(int ResourceID);
	BOOL LoadFromMem(int Size, uchar* pData);
	BOOL SaveIntoMem(int* pSize, uchar** ppData, int CompressionType);
	BOOL LoadFromFile(const CString& FileName);
	BOOL SaveIntoFile(const CString& FileName);

	int GetRAt(int x, int y) const { ASSERT(x >= 0 && x < m_w && y >= 0 && y < m_h); return m_RGBData(y, 3 * x + 2); }
	int GetGAt(int x, int y) const { ASSERT(x >= 0 && x < m_w && y >= 0 && y < m_h); return m_RGBData(y, 3 * x + 1); }
	int GetBAt(int x, int y) const { ASSERT(x >= 0 && x < m_w && y >= 0 && y < m_h); return m_RGBData(y, 3 * x); }
	int GetAAt(int x, int y) const { ASSERT(x >= 0 && x < m_w && y >= 0 && y < m_h); return m_AData(y, x); }

	void SetRAt(int x, int y, int r)
	{
		//MatirxTest(x, y, m_w, m_h, "0");
		ASSERT(x >= 0 && x < m_w && y >= 0 && y < m_h);
		m_RGBData(y, 3 * x + 2) = r;
	}

	void SetGAt(int x, int y, int g)
	{
		//MatirxTest(x, y, m_w, m_h, "1");
		ASSERT(x >= 0 && x < m_w && y >= 0 && y < m_h);
		m_RGBData(y, 3 * x + 1) = g;
	}
	void SetBAt(int x, int y, int b)
	{
		//MatirxTest(x, y, m_w, m_h, "2");
		ASSERT(x >= 0 && x < m_w && y >= 0 && y < m_h);
		m_RGBData(y, 3 * x) = b;
	}
	void SetAAt(int x, int y, int a)
	{
		//MatirxTest(x, y, m_w, m_h, "3");
		ASSERT(x >= 0 && x < m_w && y >= 0 && y < m_h);
		m_AData(y, x) = a;
	}
};

//***************************************************************************************

class CTraceyLogo : public CBasicImage
{
public:

	CTraceyLogo(int i);
};

//***************************************************************************************

class CProductLogo : public CBasicImage
{
public:

	CProductLogo(int i);
};

//***************************************************************************************

class CiTraceLogo : public CBasicImage
{
public:

	CiTraceLogo(int i);
};

//***************************************************************************************

class CWavetouchLogo : public CBasicImage
{
public:

	CWavetouchLogo(int i);
};

//***************************************************************************************

class CHoyaLogo : public CBasicImage
{
public:

	CHoyaLogo();
};

//***************************************************************************************

#define MAX_NUM_RINGS 26

//***************************************************************************************

#define COMPRESSION_JPG 1
#define COMPRESSION_PNG 2

//***************************************************************************************

class CNode
{
public:

	CNode(int x = 0, int y = 0) { m_x = x; m_y = y; }

	int m_x;
	int m_y;
};

BOOL operator==(const CNode& a, const CNode& b);
BOOL operator!=(const CNode& a, const CNode& b);

//***************************************************************************************

class CEyeImage : public CBasicImage
{
public:

	CEyeImage();

	void Destroy();

	real_t m_w_um;
	real_t m_h_um;

	real_t m_BlurryValue; //[cjf***03302012], the boundary rms to judge wether the pupil image is blurry
	BOOL m_Find_BL_Val; //[cjf***03302012], Find the Blurry value;
	int m_Loop_Time;    //[cjf***03302012], The loop times used to find the BlurryValue;
	void CheckClear(int timeLeft);//[cjf***04042012],
	BOOL m_pu_clear; //[cjf***04042012],

	BOOL GetRGBAtUm(real_t x_um, real_t y_um, real_t* pR, real_t* pG, real_t* pB);

	BOOL m_pu_ok;
	real_t m_pu_x0_um;
	real_t m_pu_y0_um;
	real_t m_pu_r_um[360];
	real_t m_pu_r_min_um;
	real_t m_pu_r_max_um;
	real_t m_pu_r_mean_um;
	void ClearPupil();
	void FindPupil();
	int  m_pu_thr; // ��������������� ����������
	int  m_pu_d_um; // ��������������� ����������

	BOOL m_li_ok;
	real_t m_li_x0_um;
	real_t m_li_y0_um;
	real_t m_li_r_um[360];
	real_t m_li_r_min_um;
	real_t m_li_r_max_um;
	real_t m_li_r_mean_um;
	void ClearLimbus();
	void FindLimbus();
	void FindLimbus2(real_t LastCenX, real_t LastCenY, real_t LastR);
	void ImproveComprison();

	BOOL m_ve0_ok;
	int  m_ve0_x;
	int  m_ve0_y;
	BOOL m_la_ok;
	int  m_la_x;
	int  m_la_Prior_x[30];//[record the last 50 laser x places of first laser reflection
	int  m_la_Ok_Times;//How many times the first laser spot been found
	BOOL m_la_RtoC_OK;//Is the laser reflection are from right to the center;
	BOOL m_Target_ok;

	int  m_la_v;
	void ClearVertex0();
	void FindVertex0(BOOL TriLaserOn, int LaserIntensityThreshold);
	int  m_ve0_thr;

	BOOL m_ve_ok;
	real_t m_ve_x_um;
	real_t m_ve_y_um;
	int  m_ve_r;
	void ClearVertex();
	void FindWFVertex();

	int  m_NumRings;
	BOOL m_ri_ok;
	real_t m_ri_r_um[MAX_NUM_RINGS][360];
	void ClearVertexAndRings();
	void FindVertexAndRings();
	BOOL IsRingsDataComplete();

	BOOL m_le_ok; //black marks lens
	real_t m_le_x_um[3];
	real_t m_le_y_um[3];
	real_t GetLensX0Um();
	real_t GetLensY0Um();
	real_t GetLensRotationAngle();
	void ClearLensDots();
	void FindLensDots();
	void ValidateLensDots();

	//The new attributes also nned to be added into the SaveWFExam and LoadWFExam, and TDB_WF_EXAM_DATA
	//Add the info of the Radial Ruler
	BOOL m_Ra_ok;
	real_t m_Ra_x_um;
	real_t m_Ra_y_um;
	real_t m_Ra_r_um;
	void ClearRuler();

	//Linear Ruler
	BOOL m_LRa_ok;
	real_t m_LRa_x0_um;
	real_t m_LRa_y0_um;
	real_t m_LRa_x1_um;
	real_t m_LRa_y1_um;
	void ClearLRuler();

	//Add the info of the  Inlay
	BOOL m_In_ok;
	real_t m_In_x_um;
	real_t m_In_y_um;
	real_t m_In_r_um;
	real_t m_In_In_r_um;//inner inlay radius

	real_t m_hist[256];
	real_t m_hist_max;
	Matrix<int> m_table;
	List<CNode> m_hull;

	void SwapImageInfo(CEyeImage* FromImage);

private:

	BOOL FindDirt(int xmax, int icx);//Is the laser reflection are from right to the center;
	int FindLargestGroup(Matrix<int>& table, int index_to_look, int index_to_set);
	void FillHollows(Matrix<int>& table, int index);
	int CreateNewGroup(Matrix<int>& table, int x0, int y0, int index_to_look, int index_to_set);

	void FindHull(List<CNode>& hull, Matrix<int>& table, int index_to_look, int index_to_set);
};

//***************************************************************************************
