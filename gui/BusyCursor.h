//***************************************************************************************

#pragma once

//***************************************************************************************

class CBusyCursor
{
public:
	CBusyCursor() { ::SetCursor(::LoadCursor(NULL, IDC_WAIT)); }
	~CBusyCursor() { ::SetCursor(::LoadCursor(NULL, IDC_ARROW)); }
};

//***************************************************************************************
