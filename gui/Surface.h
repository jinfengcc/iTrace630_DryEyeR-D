//***************************************************************************************

#pragma once

//***************************************************************************************

class CSurface
{
public:

	real_t m_r_max_um;

	virtual void Reset() { m_r_max_um = 0.0; }

	virtual BOOL GetAt(const real_t r_um, const real_t a_rd,
		real_t* z_um, real_t* dz_dx, real_t* dz_dy, real_t* dz_dr, real_t* dz_da, real_t* rc_um) const
	{
		return FALSE;
	}

	virtual BOOL GetAxUmAt(const real_t r_um, const real_t a_rd, real_t& ax_um) const;
	virtual BOOL GetTnUmAt(const real_t r_um, const real_t a_rd, real_t& tn_um) const;
	virtual BOOL GetRfUmAt(const real_t r_um, const real_t a_rd, real_t& rf_um) const;
};

//***************************************************************************************

class CConicSurface : public CSurface
{
public:

	CConicSurface();

	virtual void Reset() override;

	void Create(const real_t r_max_um, const real_t ax0_um, const real_t q);

	real_t GetEccentricity();

	virtual BOOL GetAt(const real_t r_um, const real_t a_rd,
		real_t* z_um, real_t* dz_dx, real_t* dz_dy, real_t* dz_dr, real_t* dz_da, real_t* rc_um) const override;

	virtual BOOL GetAxUmAt(const real_t r_um, const real_t a_rd, real_t& ax_um) const override;

	real_t m_ax0_um;
	real_t m_q;

private:

	real_t m_a_um;
	real_t m_b_um;
	real_t m_p_um;
};

//***************************************************************************************

#define ZERNIKE_ORDER_MAX        8
#define ZERNIKE_NUM_ORDERS       9
#define ZERNIKE_NUM_FREQUENCIES  9
#define ZERNIKE_NUM_MODES       45

//***************************************************************************************

#define MASK_TOTAL             0
#define MASK_LO_TOTAL          1
#define MASK_DEFOCUS           2
#define MASK_ASTIGMATISM       3
#define MASK_HO_TOTAL          4
#define MASK_COMA              5
#define MASK_TREFOIL           6
#define MASK_SPHERICAL         7
#define MASK_4ORD_ASTIGMATISM  8
#define MASK_TOTAL_NO_DEFOCUS  9
#define MASK_NONE             20
#define MASK_OTHER            21

//***************************************************************************************

class CZernikeMask
{
public:

	CZernikeMask();

	BOOL SetType(const int type);
	int GetType() const;

	void Set(const int i, const BOOL v) { m_m[i] = v; }
	BOOL Get(const int i) const { return m_m[i]; }

	static void GetName(const int Type, CString& Name);

	void GetName(CString& Name) const;

private:

	BOOL m_m[ZERNIKE_NUM_MODES];

};

//***************************************************************************************

#define COMP_NORMAL   0
#define COMP_LIGHT    1
#define COMP_MODERATE 2
#define COMP_SEVERE   3

//***************************************************************************************

class CZernikeSurface : public CSurface
{
public:

	CZernikeSurface();

	static int m_ord[ZERNIKE_NUM_MODES];
	static int m_frq[ZERNIKE_NUM_MODES];
	static real_t m_nrm[ZERNIKE_NUM_MODES];
	static real_t m_rc[ZERNIKE_NUM_MODES][ZERNIKE_NUM_ORDERS];
	static Matrix<real_t> m_cs;
	static Matrix<real_t> m_sc;
	static CString m_rms_name[ZERNIKE_NUM_MODES];

	real_t m_c[ZERNIKE_NUM_MODES];

	virtual void Reset() override;

	BOOL CreateXY(const int max_order, const real_t r_max_um, const int num_points, real_t* r_um, real_t* a_rd, real_t* dw_dx, real_t* dw_dy, real_t* rmse);
	BOOL CreateR(const int max_order, const real_t r_max_um, const int num_points, real_t* r_um, real_t* a_rd, real_t* dw_dr, real_t* rmse);

	void ComputePiston(const BOOL balance);
	void ComputeTilt(const BOOL balance);

	void AddSphEq(const real_t SphEq);

	real_t GetCUm(const int mode) const;
	void SetCUm(const int mode, const real_t c_um);

	void ChangeRMaxUm(const real_t r_max_um);

	virtual BOOL GetAt(const real_t r_um, const real_t a_rd,
		real_t* w_um, real_t* dw_dx, real_t* dw_dy, real_t* dw_dr, real_t* dw_da, real_t* rc_um) const override;

	void GetTiltAt(const real_t a_rd, real_t* tilt, real_t* tilt_x, real_t* tilt_y) const;

	void SaveTxtFile(const CString& FileName);
	BOOL LoadTxtFile(const CString& FileName);

	void operator+=(const CZernikeSurface& Surface) { Add(Surface); }
	void operator-=(const CZernikeSurface& Surface) { Subtract(Surface); }
	void operator&=(const CZernikeMask& Mask) { ApplyMask(Mask); }

	real_t ComputeRms() const;
	void GetCombinedRms(const int mode, real_t& value, int& axis) const;

	int EvaluateMode(const int mode1, const int mode2);

	void SetSphEq(const real_t SphEq);

private:

	void Add(const CZernikeSurface& Surface);
	void Subtract(const CZernikeSurface& Surface);
	void ApplyMask(const CZernikeMask& Mask);
};

//***************************************************************************************
