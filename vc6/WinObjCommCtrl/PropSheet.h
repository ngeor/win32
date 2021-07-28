// PropSheet.h: interface for the CPropSheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPSHEET_H__B2BA2B5B_5C47_4CD5_B26A_C60D42954223__INCLUDED_)
#define AFX_PROPSHEET_H__B2BA2B5B_5C47_4CD5_B26A_C60D42954223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\WinObj\Dialog.h"
#include "..\WinObj\Instance.h"

namespace WinObj
{
class CPropSheet : public CDialog
{
public:
	CPropSheet();
	virtual ~CPropSheet();
	void NotifyParentChanged();
};

template <class T> class Helper
{
public:
	static LRESULT CALLBACK InternalPropSheetBootstrapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		CPropSheet* dialog;
		switch (message)
		{
		case WM_INITDIALOG:
			dialog = new T();

#if _MSC_VER > 1200
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LPARAM)dialog);
#else
			SetWindowLong(hWnd, GWL_USERDATA, (LPARAM)dialog);
#endif
			dialog->SetHandle(hWnd);
			// reminder, the lParam is actually LPPROPSHEETPAGE
			return dialog->OnMessage(message, wParam, lParam);
			break;
		case WM_DESTROY:
#if _MSC_VER > 1200
			dialog = (CPropSheet*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
#else
			dialog = (CPropSheet*)GetWindowLong(hWnd, GWL_USERDATA);
			SetWindowLong(hWnd, GWL_USERDATA, 0);
#endif
			if (dialog)
			{
				dialog->OnMessage(message, wParam, lParam);
				delete dialog;
			}
			break;
		default:
#if _MSC_VER > 1200
			dialog = (CPropSheet*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
#else
			dialog = (CPropSheet*)GetWindowLong(hWnd, GWL_USERDATA);
#endif
			if (dialog)
			{
				return dialog->OnMessage(message, wParam, lParam);
			}

			break;
		}

		return 0;
	}

	HPROPSHEETPAGE CreatePropertyPage(const CInstance& instance, int dialogResource, int iconResource, LPARAM userData)
	{
		PROPSHEETPAGE p;
		memset(&p, 0, sizeof(p));
		p.dwSize      = sizeof(p);
		p.dwFlags     = PSP_USEICONID;
		p.hInstance   = instance.GetHandle();
		p.pszTemplate = MAKEINTRESOURCE(dialogResource);
		p.pfnDlgProc  = (DLGPROC)(InternalPropSheetBootstrapProc);
		p.lParam      = userData;
		p.pszIcon     = MAKEINTRESOURCE(iconResource);
		return CreatePropertySheetPage(&p);
	}
};

} // namespace WinObj

#endif // !defined(AFX_PROPSHEET_H__B2BA2B5B_5C47_4CD5_B26A_C60D42954223__INCLUDED_)
