// Dialog.h: interface for the CDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_H__A4E773BE_896D_4FA3_AA36_C1EB26575BCA__INCLUDED_)
#define AFX_DIALOG_H__A4E773BE_896D_4FA3_AA36_C1EB26575BCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Instance.h"
#include "Wnd.h"

namespace WinObj
{
class CDialog : public CWnd
{
	const CInstance &_instance;

public:
	CDialog(const CInstance &instance, HWND hWnd);
	virtual ~CDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	const CInstance &GetInstance() const;
};

LRESULT CALLBACK __InternalDialogBootstrapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

template <typename T> T *BuildDialog(const CInstance &instance, int dialog)
{
	HWND hWnd = CreateDialogParam(instance.GetHandle(), (LPCTSTR)dialog, 0, (DLGPROC)__InternalDialogBootstrapProc, 0);
	if (!hWnd)
	{
		return NULL;
	}
	T *wnd = new T(instance, hWnd);
#if _MSC_VER > 1200
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LPARAM)wnd);
#else
	SetWindowLong(hWnd, GWL_USERDATA, (LPARAM)wnd);
#endif
	// need to call this because WM_INITDIALOG was already called by Windows and the CDialog missed it
	wnd->OnMessage(WM_INITDIALOG, 0, 0);
	return wnd;
}

} // namespace WinObj
#endif // !defined(AFX_DIALOG_H__A4E773BE_896D_4FA3_AA36_C1EB26575BCA__INCLUDED_)
