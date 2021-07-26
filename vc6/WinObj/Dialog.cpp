// Dialog.cpp: implementation of the CDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dialog.h"
namespace WinObj
{
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDialog::CDialog(const CInstance& instance) : CWnd(NULL), _instance(instance)
{
}

CDialog::~CDialog()
{
}

const CInstance& CDialog::GetInstance() const
{
	return _instance;
}

LRESULT CALLBACK __InternalDialogBootstrapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CDialog* dialog;
	switch (message)
	{
	case WM_INITDIALOG:
		if (lParam)
		{
#if _MSC_VER > 1200
			SetWindowLongPtr(hWnd, GWLP_USERDATA, lParam);
#else
			SetWindowLong(hWnd, GWL_USERDATA, lParam);
#endif
			dialog = (CDialog*)lParam;
			dialog->SetHandle(hWnd);
			return dialog->OnMessage(message, wParam, lParam);
		}
		return 1;
	default:
#if _MSC_VER > 1200
		dialog = (CDialog*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
#else
		dialog = (CDialog*)GetWindowLong(hWnd, GWL_USERDATA);
#endif
		if (dialog)
		{
			return dialog->OnMessage(message, wParam, lParam);
		}

		return 0;
	}
}

bool CDialog::Create(int dialogResource)
{
	HWND hWnd = ::CreateDialogParam(GetInstance().GetHandle(), MAKEINTRESOURCE(dialogResource), 0,
	                                (DLGPROC)__InternalDialogBootstrapProc, (LPARAM)this);
	return hWnd != NULL;
}

INT_PTR CDialog::Modal(const CWnd& parent, int dialogResource)
{
	return ::DialogBoxParam(GetInstance().GetHandle(), MAKEINTRESOURCE(dialogResource), parent.GetHandle(),
	                        (DLGPROC)__InternalDialogBootstrapProc, (LPARAM)this);
}

INT_PTR CDialog::Modal(int dialogResource)
{
	return ::DialogBoxParam(GetInstance().GetHandle(), MAKEINTRESOURCE(dialogResource), 0,
	                        (DLGPROC)__InternalDialogBootstrapProc, (LPARAM)this);
}

LRESULT CDialog::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

bool CDialog::EndDialog(INT_PTR result)
{
	return ::EndDialog(GetHandle(), result) != 0;
}

HPROPSHEETPAGE CDialog::CreatePropertyPage(int dialogResource, int iconResource)
{
	PROPSHEETPAGE p;
	memset(&p, 0, sizeof(p));
	p.dwSize      = sizeof(p);
	p.dwFlags     = PSP_USEICONID;
	p.hInstance   = GetInstance().GetHandle();
	p.pszTemplate = MAKEINTRESOURCE(dialogResource);
	p.pfnDlgProc  = (DLGPROC)__InternalDialogBootstrapProc;
	p.lParam      = (LPARAM)this;
	p.pszIcon     = MAKEINTRESOURCE(iconResource);
	return CreatePropertySheetPage(&p);
}

void CDialog::SetDialogMessageResult(LPARAM result)
{
#if _MSC_VER > 1200
	SetWindowLongPtr(GetHandle(), DWLP_MSGRESULT, result);
#else
	SetWindowLong(GetHandle(), DWL_MSGRESULT, result);
#endif
}

void CDialog::NotifyParentChanged()
{
	HWND hWnd = GetHandle();
	::SendMessage(GetParent(hWnd), PSM_CHANGED, (WPARAM)hWnd, 0);
}

} // namespace WinObj
