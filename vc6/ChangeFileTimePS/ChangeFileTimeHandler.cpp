// ChangeFileTimeHandler.cpp : Implementation of CChangeFileTimeHandler
#include "stdafx.h"
#if _MSC_VER > 1200
#include "ChangeFileTimePS_h.h"
#else
#include "ChangeFileTimePS.h"
#endif
#include "ChangeFileTimeHandler.h"
#include "SimplePage.h"

/////////////////////////////////////////////////////////////////////////////
// CChangeFileTimeHandler

HRESULT CChangeFileTimeHandler::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID)
{
	FORMATETC etc;
	STGMEDIUM stg;
	TCHAR temp[MAX_PATH];

	//	filelist.clear();

	etc.cfFormat = CF_HDROP;
	etc.ptd      = NULL;
	etc.dwAspect = DVASPECT_CONTENT;
	etc.lindex   = -1;            // all of the data
	etc.tymed    = TYMED_HGLOBAL; // HGLOBAL
	hasfolders   = false;
	if (SUCCEEDED(lpdobj->GetData(&etc, &stg)))
	{
		HDROP hDrop   = (HDROP)stg.hGlobal;
		int fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

		for (int i = 0; i < fileCount; i++)
		{
			DragQueryFile(hDrop, i, temp, MAX_PATH);
			if (!hasfolders && (GetFileAttributes(temp) & FILE_ATTRIBUTE_DIRECTORY))
				hasfolders = true;
			filelist.push_back(temp);
		}
		ReleaseStgMedium(&stg);
		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT CChangeFileTimeHandler::AddPages(LPFNADDPROPSHEETPAGE lpfnAddPage, LPARAM lParam)
{
	// TODO move to Initialize?
	InitCommonControls();

	INITCOMMONCONTROLSEX ic;
	ic.dwICC  = ICC_DATE_CLASSES;
	ic.dwSize = sizeof(ic);
	InitCommonControlsEx(&ic);

	CSimplePage* simplePage       = new CSimplePage(filelist, hasfolders);
	HPROPSHEETPAGE hPropSheetPage = simplePage->CreatePropertyPage(*_Instance, IDD_SIMPLE_PAGE, IDI_ICON1);
	lpfnAddPage(hPropSheetPage, lParam);

	return NOERROR;
}
