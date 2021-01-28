//***************************************************************************************

#include "StdAfx.h"
#include "Pref.h"
#include "Compression.h"

//***************************************************************************************
/*
class CDispPref2
{
public:
  int          type;
  BOOL         method_3d;
  BOOL         size_small;
  int          zone_r_um;
  CZernikeMask Mask;
  BOOL         piston;
  BOOL         tilt;
  BOOL         map_eye;
  BOOL         map_pupil;
  BOOL         map_vertex;
  BOOL         map_limbus;
  BOOL         map_lens;
  BOOL         map_points;
  BOOL         map_rings;
  BOOL         map_surface;
  BOOL         map_mesh;
  BOOL         psf_eef;
  BOOL         mtf_avg;
  int          mtf_axis;
  int          ltr_line;
  BOOL         rms_ind;
  int          ref_surf;
  real         ref_ax0_um;
  real         ref_q;
  CScale       op_scale;
  CScale       um_scale;
  CScale       dp_scale;
  void Copy(const CDispPref* pDispPref);
  void operator=(const CDispPref& pDispPref) {Copy(&pDispPref);}
};

void CDispPref2::Copy(const CDispPref* pDispPref)
{
  type = pDispPref->type;
  method_3d = pDispPref->method_3d;
  size_small = pDispPref->size_small;
  zone_r_um = pDispPref->zone_r_um;
  Mask = pDispPref->Mask;
  piston = pDispPref->piston;
  tilt = pDispPref->tilt;
  map_eye = pDispPref->map_eye;
  map_pupil = pDispPref->map_pupil;
  map_vertex = pDispPref->map_vertex;
  map_limbus = pDispPref->map_limbus;
  map_lens = pDispPref->map_lens;
  map_points = pDispPref->map_points;
  map_rings = pDispPref->map_rings;
  map_surface = pDispPref->map_surface;
  map_mesh = pDispPref->map_mesh;
  psf_eef = pDispPref->psf_eef;
  mtf_avg = pDispPref->mtf_avg;
  mtf_axis = pDispPref->mtf_axis;
  ltr_line = pDispPref->ltr_line;
  rms_ind = pDispPref->rms_ind;
  ref_surf = pDispPref->ref_surf;
  ref_ax0_um = pDispPref->ref_ax0_um;
  ref_q = pDispPref->ref_q;
  op_scale = pDispPref->op_scale;
  um_scale = pDispPref->um_scale;
  dp_scale = pDispPref->dp_scale;
}

class CPref2
{
public:
  CDispPref2 wfvrf_pref[4];
  CDispPref2 wfsum_pref[4][4];
  CDispPref2 wfcmp_pref;
  CDispPref2 wfdsm_pref[2];
  CDispPref2 wfmsm_pref;
  CDispPref2 ctvrf_pref;
  CDispPref2 ctsum_pref[4][4];
  CDispPref2 ctcmp_pref;
  CDispPref2 ctdsm_pref[2];
  CDispPref2 ctmsm_pref;
  CDispPref2 insum_pref[4];
  BOOL      pos_cyl;
  int       vertex_um;
  int       zone_um[3];
  BOOL      probing_enabled;
  BOOL      fogging_enabled;
  int       manual_scan_d_um;
  BOOL      scan_after_new_exam;
  int       pat_sort_col;
  BOOL      pat_sort_asc;
  void Copy(CPref* pPref);
};

void CPref2::Copy(CPref*pPref)
{
  for (int i = 0; i < 4; i++) {
    wfvrf_pref[i] = pPref->wfvrf_pref[i];
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      wfsum_pref[i][j] = pPref->wfsum_pref[i][j];
    }
  }
  wfcmp_pref = pPref->wfcmp_pref;
  for (int i = 0; i < 2; i++) {
    wfdsm_pref[i] = pPref->wfdsm_pref[i];
  }
  wfmsm_pref = pPref->wfmsm_pref;

  ctvrf_pref = pPref->ctvrf_pref;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ctsum_pref[i][j] = pPref->ctsum_pref[i][j];
    }
  }
  ctcmp_pref = pPref->ctcmp_pref;
  for (int i = 0; i < 2; i++) {
    ctdsm_pref[i] = pPref->ctdsm_pref[i];
  }

  for (int i = 0; i < 4; i++) {
    insum_pref[i] = pPref->insum_pref[i];
  }
}

void CPref::SaveIntoFile(const CString& FileName)
{
  CPref2* pPref2 = new CPref2;

  pPref2->Copy(this);

  HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

  if (hFile != INVALID_HANDLE_VALUE) {

    void* pZipData = malloc(sizeof(CPref2));
    int ZipSize;

    if (::Compress(pPref2, sizeof(CPref2), pZipData, &ZipSize)) {
      ulong bt;
      ::WriteFile(hFile, pZipData, ZipSize, &bt, NULL);
    }

    free(pZipData);

    ::CloseHandle(hFile);
  }

  delete pPref2;
}
*/
//***************************************************************************************
