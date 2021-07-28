// PropSheet.cpp: implementation of the CPropSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <tchar.h>
#include "PropSheet.h"

namespace WinObj
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropSheet::CPropSheet() : CDialog()
{
}

CPropSheet::~CPropSheet()
{
}

void CPropSheet::NotifyParentChanged()
{
	HWND hWnd = GetHandle();
	::SendMessage(GetParent(hWnd), PSM_CHANGED, (WPARAM)hWnd, 0);
}
} // namespace WinObj
