// PropSheet.cpp: implementation of the CPropSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PropSheet.h"

namespace WinObj
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropSheet::CPropSheet(const CInstance& app) : CDialog(app)
{
}

CPropSheet::~CPropSheet()
{
}

LRESULT CALLBACK __InternalPropSheetBootstrapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CDialog* dialog;
	switch (message)
	{
	case WM_INITDIALOG:
		if (lParam)
		{
			LPPROPSHEETPAGE lp = (LPPROPSHEETPAGE)lParam;

#if _MSC_VER > 1200
			SetWindowLongPtr(hWnd, GWLP_USERDATA, lp->lParam);
#else
			SetWindowLong(hWnd, GWL_USERDATA, lp->lParam);
#endif
			dialog = (CDialog*)lp->lParam;
			dialog->SetHandle(hWnd);
			return dialog->OnMessage(message, wParam, lParam);
		}
		break;
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

		break;
	}

	return 0;
}

HPROPSHEETPAGE CPropSheet::CreatePropertyPage(int dialogResource, int iconResource)
{
	PROPSHEETPAGE p;
	memset(&p, 0, sizeof(p));
	p.dwSize      = sizeof(p);
	p.dwFlags     = PSP_USEICONID;
	p.hInstance   = GetInstance().GetHandle();
	p.pszTemplate = MAKEINTRESOURCE(dialogResource);
	p.pfnDlgProc  = (DLGPROC)__InternalPropSheetBootstrapProc;
	p.lParam      = (LPARAM)this;
	p.pszIcon     = MAKEINTRESOURCE(iconResource);
	return CreatePropertySheetPage(&p);
}

void CPropSheet::NotifyParentChanged()
{
	HWND hWnd = GetHandle();
	::SendMessage(GetParent(hWnd), PSM_CHANGED, (WPARAM)hWnd, 0);
}
} // namespace WinObj