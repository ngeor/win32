// Instance.cpp: implementation of the CInstance class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Instance.h"

namespace WinObj
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInstance::CInstance(HINSTANCE instance) : CHandle<HINSTANCE>(instance)
{
}

CInstance::~CInstance()
{
}

bool CInstance::LoadString(int id, LoadedString& result) const
{
	HINSTANCE hInstance = GetHandle();
#if _MSC_VER > 1200
	LPTSTR readOnlyBuffer;
	int len = ::LoadString(hInstance, id, (LPTSTR)&readOnlyBuffer, 0);
	// because readOnlyBuffer isn't guaranteed to be null terminated
	LPTSTR buf = (LPTSTR)calloc(len + 1, sizeof(TCHAR));
	_tcsncpy_s(buf, len + 1, readOnlyBuffer, len);
#else
	TCHAR buffer[256];
	int len    = ::LoadString(hInstance, id, buffer, 256);
	LPTSTR buf = (LPTSTR)calloc(len + 1, sizeof(TCHAR));
	_tcsncpy(buf, buffer, len);
#endif
	result.CopyFrom(LoadedString(buf, len));
	return true;
}

LoadedString::LoadedString(const CInstance& instance, int resourceId)
{
	instance.LoadString(resourceId, *this);
}

} // namespace WinObj
