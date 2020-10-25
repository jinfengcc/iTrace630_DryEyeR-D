//***************************************************************************************

#pragma once

#include "Surface.h"
#include "ZernikeMask.h"

//***************************************************************************************

#define COMP_NORMAL 0
#define COMP_LIGHT 1
#define COMP_MODERATE 2
#define COMP_SEVERE 3

//***************************************************************************************

class CZernikeSurface : public CSurface
{
  static real_t         m_nrm[ZERNIKE_NUM_MODES];
  static real_t         m_rc[ZERNIKE_NUM_MODES][ZERNIKE_NUM_ORDERS];
  static const char *   m_rms_name[ZERNIKE_NUM_MODES];

public:
  CZernikeSurface();

  static int m_ord[ZERNIKE_NUM_MODES];
  static int m_frq[ZERNIKE_NUM_MODES];
  real_t     m_c[ZERNIKE_NUM_MODES];

  static const char *RmsName(int i)
  {
    return m_rms_name[i];
  }

  virtual void Reset() override;

  BOOL CreateXY(const int max_order, const real_t r_max_um, const int num_points, const real_t *r_um, const real_t *a_rd,
                const real_t *dw_dx, const real_t *dw_dy, real_t *rmse);
  BOOL CreateR(const int max_order, const real_t r_max_um, const int num_points, const real_t *r_um, const real_t *a_rd,
               const real_t *dw_dr, real_t *rmse);

  void ComputePiston(const BOOL balance);
  void ComputeTilt(const BOOL balance);

  void AddSphEq(const real_t SphEq);

  real_t GetCUm(const int mode) const;
  void   SetCUm(const int mode, const real_t c_um);

  void ChangeRMaxUm(const real_t r_max_um);

  virtual BOOL GetAt(const real_t r_um, const real_t a_rd, real_t *w_um, real_t *dw_dx, real_t *dw_dy, real_t *dw_dr, real_t *dw_da,
                     real_t *rc_um) const override;

  void GetTiltAt(const real_t a_rd, real_t *tilt, real_t *tilt_x, real_t *tilt_y) const;

  void SaveTxtFile(std::wstring_view fileName);
  BOOL LoadTxtFile(std::wstring_view fileName);

  void operator+=(const CZernikeSurface &Surface)
  {
    Add(Surface);
  }
  void operator-=(const CZernikeSurface &Surface)
  {
    Subtract(Surface);
  }
  void operator&=(const CZernikeMask &Mask)
  {
    ApplyMask(Mask);
  }

  real_t ComputeRms() const;
  void   GetCombinedRms(const int mode, real_t &value, int &axis) const;

  int EvaluateMode(const int mode1, const int mode2);

  void SetSphEq(const real_t SphEq);

private:
  void Add(const CZernikeSurface &Surface);
  void Subtract(const CZernikeSurface &Surface);
  void ApplyMask(const CZernikeMask &Mask);
};

//***************************************************************************************
