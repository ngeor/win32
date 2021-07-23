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

bool CWnd::MoveDlgItem(int dlgItem, int x, int y, int width, int height, bool repaint)
{
	HWND hChildWnd = ::GetDlgItem(GetHandle(), dlgItem);
	if (hChildWnd)
	{
		return ::MoveWindow(hChildWnd, x, y, width, height, repaint) != 0;
	}
	else
	{
		return false;
	}
}

bool CWnd::GetClientRect(LPRECT rect)
{
	return ::GetClientRect(GetHandle(), rect);
}

} // namespace WinObj
