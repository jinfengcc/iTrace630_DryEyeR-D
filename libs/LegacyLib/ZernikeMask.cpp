//***************************************************************************************

#include "stdafx.h"
#include "ZernikeMask.h"

CZernikeMask::CZernikeMask()
{
  std::fill(m_m.begin(), m_m.end(), FALSE);
}

BOOL CZernikeMask::SetType(const int type)
{
  m_m[0] = FALSE;
  m_m[1] = FALSE;
  m_m[2] = FALSE;

  switch (m_type = type) {
  case MASK_TOTAL:
    for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = TRUE;
    return TRUE;

  case MASK_LO_TOTAL:
    m_m[3] = TRUE;
    m_m[4] = TRUE;
    m_m[5] = TRUE;
    for (int z = 6; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = FALSE;
    return TRUE;

  case MASK_DEFOCUS:
    m_m[3] = FALSE;
    m_m[4] = TRUE;
    m_m[5] = FALSE;
    for (int z = 6; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = FALSE;
    return TRUE;

  case MASK_ASTIGMATISM:
    m_m[3] = TRUE;
    m_m[4] = FALSE;
    m_m[5] = TRUE;
    for (int z = 6; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = FALSE;
    return TRUE;

  case MASK_HO_TOTAL: {
    m_m[3] = FALSE;
    m_m[4] = FALSE;
    m_m[5] = FALSE;
    for (int z = 6; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = TRUE;
  }
    return TRUE;

  case MASK_COMA: {
    for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = FALSE;
    m_m[7] = TRUE;
    m_m[8] = TRUE;
  }
    return TRUE;

  case MASK_TREFOIL: {
    for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = FALSE;
    m_m[6] = TRUE;
    m_m[9] = TRUE;
  }
    return TRUE;

  case MASK_SPHERICAL: {
    for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = FALSE;
    m_m[12] = TRUE;
  }
    return TRUE;

  case MASK_4ORD_ASTIGMATISM: {
    for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = FALSE;
    m_m[11] = TRUE;
    m_m[13] = TRUE;
  }
    return TRUE;

  case MASK_TOTAL_NO_DEFOCUS: {
    m_m[3] = TRUE;
    m_m[4] = FALSE;
    m_m[5] = TRUE;
    for (int z = 6; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = TRUE;
  }
    return TRUE;

  case MASK_NONE: {
    for (int z = 3; z < ZERNIKE_NUM_MODES; z++)
      m_m[z] = FALSE;
  }
    return TRUE;
  }

#ifndef NEVER
  return TRUE;
#else
  CMaskDlg *pDlg = new CMaskDlg(NULL);

  for (int z = 0; z < ZERNIKE_NUM_MODES; z++) {
    pDlg->m_m[z] = m_m[z];
  }

  BOOL Res = pDlg->DoModal() == IDOK;

  if (Res) {
    m_m[0] = FALSE;
    m_m[1] = FALSE;
    m_m[2] = FALSE;

    for (int z = 3; z < ZERNIKE_NUM_MODES; z++) {
      m_m[z] = pDlg->m_m[z];
    }
  }

  delete pDlg;

  return Res;
#endif
}

//***************************************************************************************

// int CZernikeMask::GetType() const
//{
//  int Type[11] = {MASK_TOTAL,   MASK_LO_TOTAL,  MASK_DEFOCUS,          MASK_ASTIGMATISM,      MASK_HO_TOTAL, MASK_COMA,
//                  MASK_TREFOIL, MASK_SPHERICAL, MASK_4ORD_ASTIGMATISM, MASK_TOTAL_NO_DEFOCUS, MASK_NONE};
//
//  CZernikeMask Mask;
//
//  for (int i = 0; i < 11; i++) {
//    Mask.SetType(Type[i]);
//    if (memcmp(m_m, Mask.m_m, ZERNIKE_NUM_MODES * sizeof(BOOL)) == 0)
//      return Type[i];
//  }
//
//  return MASK_OTHER;
//}

//***************************************************************************************

const char *CZernikeMask::GetName(const int Type)
{
  const char *Name = "Other";
  switch (Type) {
  case MASK_TOTAL:
    Name = "Total";
    break;
  case MASK_LO_TOTAL:
    Name = "LO Total";
    break;
  case MASK_DEFOCUS:
    Name = "Defocus";
    break;
  case MASK_ASTIGMATISM:
    Name = "Astigmatism";
    break;
  case MASK_HO_TOTAL:
    Name = "HO Total";
    break;
  case MASK_COMA:
    Name = "Coma";
    break;
  case MASK_TREFOIL:
    Name = "Trefoil";
    break;
  case MASK_SPHERICAL:
    Name = "Spherical";
    break;
  case MASK_4ORD_ASTIGMATISM:
    Name = "Secondary Astigmatism";
    break;
  case MASK_TOTAL_NO_DEFOCUS:
    Name = "Total, no Defocus";
    break;
  case MASK_NONE:
    Name = "None";
    break;
  }

  return Name;
}

//***************************************************************************************

// void CZernikeMask::GetName(std::string &Name) const
//{
//  CZernikeMask::GetName(GetType(), Name);
//}
