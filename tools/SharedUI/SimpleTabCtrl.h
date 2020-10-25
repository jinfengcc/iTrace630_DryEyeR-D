#pragma once

class CSimpleTabCtrl
{
public:
   virtual ~CSimpleTabCtrl() { }

   static std::unique_ptr<CSimpleTabCtrl> CreateIntance(HWND hParentWnd, int idc, std::function<bool(unsigned ndx)> func);

   virtual void  AddButton   (const TCHAR *psz) = 0;
   virtual void  AddSeparator() = 0;
   virtual void  EnableView  (bool b) = 0;
   virtual void  SelectItem  (size_t ndx) = 0;
};
