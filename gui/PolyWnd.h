//***************************************************************************************

#pragma once

//***************************************************************************************

#include "DispWnd.h"
#include "Surface.h"

//***************************************************************************************

class CPolyWnd : public CDispWnd
{
public:

  CPolyWnd(RECT& Rect, CWnd* pWnd);

  CZernikeSurface m_Surface;

private:

  virtual void RepaintMemDC() override;
  void Angles();

};

//***************************************************************************************
