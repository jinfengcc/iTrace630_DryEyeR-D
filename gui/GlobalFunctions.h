/*******************************************************************
Autor:         Jinfeng Chen
E-mail:        jinfengcc@gmail.com
Date:          04-27-2016
Version:       
Description:   Generated this class for global functions
*****************************************************************************************/

//***************************************************************************************

#pragma once

//***************************************************************************************

#include "Image.h"
#include "Data.h"

void LimbusAdjust(CWFExam* pWFExam, CCTExam* pCTExam);

int G_GetChineseId(CString str);//6.2.0 Chinese

BOOL    G_IsChinese(CString str);

CString G_GetChinese(int ID);//6.2.0 Chinese

CString  G_ConvertChineseStr(CString Str);//6.2.0 Chinese

CString  G_ReadChinese(CString Str);//6.2.0 Chinese

BOOL     G_Is64BitOS();//6.2.0
 
CString  G_TransformEurope(CString Input);

//void  G_MatirxTest(int col, int row, int COLVAL, int ROWVAL, CString InfoS);//Matrix error test
//***************************************************************************************

