//***************************************************************************************

#pragma once

//***************************************************************************************

class CXPMenu;

class CXPMenuItem
{
public:

	CXPMenuItem();
	~CXPMenuItem();

	uint     m_id;
	BOOL     m_radio;
	BOOL     m_check;
	CString  m_ltext;
	CString  m_rtext;

	CXPMenu* m_pSubmenu;
};

//***************************************************************************************

class CXPMenu : public CMenu
{
public:

	CXPMenu();
	~CXPMenu();

	void AppendSeparator();
	void AppendItem(const uint id, const BOOL radio, const BOOL check, const CString& ltext, const CString& rtext);
	CXPMenu* AppendSubmenu(const CString& ltext, const CString& rtext);

	List<CXPMenuItem> m_Items;

private:

	int m_item_w;
	int m_item_h;

	virtual void DrawItem(DRAWITEMSTRUCT* pDrawItemStruct) override;
	virtual void MeasureItem(MEASUREITEMSTRUCT* pMeasureItemStruct) override;
};

//***************************************************************************************
