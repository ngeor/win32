// OpenFileName.cpp: implementation of the COpenFileName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenFileName.h"

namespace WinObj
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenFileName::COpenFileName(const CInstance& app, const CWnd& owner) : _app(app), _filter(NULL, 0)
{
	memset(&_of, 0, sizeof(OPENFILENAME));
	_of.lStructSize = sizeof(OPENFILENAME);
	_of.hwndOwner   = owner.GetHandle();
	_of.hInstance   = app.GetHandle();
	_of.lpstrFile   = (LPTSTR)calloc(MAX_PATH, sizeof(TCHAR));
	_of.nMaxFile    = MAX_PATH;
}

COpenFileName::COpenFileName(const COpenFileName& other) : _app(other._app), _of(other._of)
{
}

COpenFileName::~COpenFileName()
{
	free(_of.lpstrFile);
}

// filter needs to be terminated with double null
LPTSTR ReplaceFilter(LPTSTR filter, int len)
{
	LPTSTR newBuffer = (LPTSTR)calloc(len + 2, sizeof(TCHAR));
#if _MSC_VER > 1200
	_tcsncpy_s(newBuffer, len + 1, filter, len);
#else
	_tcsncpy(newBuffer, filter, len);
#endif

	LPTSTR p = newBuffer;
	while (*p)
	{
		if (*p == '|')
		{
			*p = '\0';
		}
		p++;
	}
	return newBuffer;
}

void COpenFileName::BeforeDialog()
{
	if (_filter.GetLen() > 0)
	{
		_of.lpstrFilter = ReplaceFilter(_filter.GetBuffer(), _filter.GetLen());
	}
}

bool COpenFileName::AfterDialog(bool result)
{
	if (_filter.GetLen() > 0)
	{
		free((void*)_of.lpstrFilter);
		_of.lpstrFilter = NULL;
	}
	return result;
}

bool COpenFileName::GetOpenFileName()
{
	BeforeDialog();
	return AfterDialog(::GetOpenFileName(&_of));
}

bool COpenFileName::GetSaveFileName()
{
	BeforeDialog();
	return AfterDialog(::GetSaveFileName(&_of));
}

COpenFileName& COpenFileName::WithFilter(int stringResource)
{
	_app.LoadString(stringResource, _filter);
	return *this;
}

COpenFileName& COpenFileName::WithFlags(DWORD flags)
{
	_of.Flags = flags;
	return *this;
}

COpenFileName& COpenFileName::WithDefaultExtension(LPCTSTR defaultExtension)
{
	_of.lpstrDefExt = defaultExtension;
	return *this;
}

LPCTSTR COpenFileName::GetFile()
{
	return _of.lpstrFile;
}

} // namespace WinObj
