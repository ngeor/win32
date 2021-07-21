// Wnd.cpp: implementation of the CWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wnd.h"

namespace WinObj
{
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWnd::CWnd(HWND hWnd) : CHandle<HWND>(hWnd)
{
}

CWnd::~CWnd()
{
}

void CWnd::Show()
{
	::ShowWindow(GetHandle(), SW_SHOWNORMAL);
}

bool CWnd::IsDialogMessage(LPMSG msg)
{
	return ::IsDialogMessage(GetHandle(), msg) != 0;
}

} // namespace WinObj
