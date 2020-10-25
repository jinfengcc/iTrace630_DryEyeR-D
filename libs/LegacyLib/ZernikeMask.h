//***************************************************************************************

#pragma once

#include <array>

#define ZERNIKE_ORDER_MAX 8
#define ZERNIKE_NUM_ORDERS 9
#define ZERNIKE_NUM_FREQUENCIES 9
#define ZERNIKE_NUM_MODES 45

//***************************************************************************************

#define MASK_TOTAL 0
#define MASK_LO_TOTAL 1
#define MASK_DEFOCUS 2
#define MASK_ASTIGMATISM 3
#define MASK_HO_TOTAL 4
#define MASK_COMA 5
#define MASK_TREFOIL 6
#define MASK_SPHERICAL 7
#define MASK_4ORD_ASTIGMATISM 8
#define MASK_TOTAL_NO_DEFOCUS 9
#define MASK_NONE 20
#define MASK_OTHER 21

//***************************************************************************************

class CZernikeMask
{
public:
  CZernikeMask();

  BOOL SetType(const int type);
  int  GetType() const
  {
    return m_type;
  }

  BOOL &operator[](int i)
  {
    return m_m[i];
  }
  BOOL operator[](int i) const
  {
    return m_m[i];
  }

  void Set(const int i, const BOOL v)
  {
    m_m[i] = v;
  }
  BOOL Get(const int i) const
  {
    return m_m[i];
  }

  static const char *GetName(int Type);
  const char *       GetName() const
  {
    return GetName(GetType());
  }

  template <class STR>
  static void GetName(const int Type, STR &str)
  {
    str = GetName(Type);
  }

  template <class STR>
  void GetName(STR &str) const
  {
    str = GetName();
  }

private:
  int                                 m_type{MASK_OTHER};
  std::array<BOOL, ZERNIKE_NUM_MODES> m_m;
};
