// Instance.h: interface for the CInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTANCE_H__1939BA82_0CA2_47BF_B17C_8CF39E80A47B__INCLUDED_)
#define AFX_INSTANCE_H__1939BA82_0CA2_47BF_B17C_8CF39E80A47B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Handle.h"

namespace WinObj
{

class CInstance;
class LoadedString
{
	LPTSTR buffer;
	int len;

	LoadedString(const LoadedString& other) : LoadedString()
	{
	}

public:
	LoadedString() : LoadedString(NULL, 0)
	{
	}

	LoadedString(LPTSTR buffer, int len) : buffer(buffer), len(len)
	{
	}

	LoadedString(const CInstance& instance, int resourceId);

	~LoadedString()
	{
		if (buffer)
		{
			free(buffer);
		}
	}

	LPTSTR GetBuffer()
	{
		return buffer;
	}

	int GetLen()
	{
		return len;
	}

	void CopyFrom(const LoadedString& other)
	{
		if (buffer)
		{
			free(buffer);
		}
		buffer = other.buffer;
		len    = other.len;
		if (buffer)
		{
			buffer = (LPTSTR)calloc(len + 1, sizeof(TCHAR));
#if _MSC_VER > 1200
			_tcsncpy_s(buffer, len + 1, other.buffer, len);
#else
			_tcsncpy(buffer, other.buffer, len);
#endif
		}
	}
};

/// Encapsulates the HINSTANCE handle.
class CInstance : public CHandle<HINSTANCE>
{
public:
	CInstance(HINSTANCE instance);
	virtual ~CInstance();

	/// Loads the specified string from resources.
	/// The memory is allocated with `calloc`, so the
	/// caller must use `free` when the string is no longer needed.
	bool LoadString(int id, LoadedString& result) const;
};
} // namespace WinObj

#endif // !defined(AFX_INSTANCE_H__1939BA82_0CA2_47BF_B17C_8CF39E80A47B__INCLUDED_)
