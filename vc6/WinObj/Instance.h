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
class CInstance : public CHandle<HINSTANCE>
{
public:
	CInstance(HINSTANCE instance);
	virtual ~CInstance();
	LPTSTR LoadString(int id) const;
};
} // namespace WinObj

#endif // !defined(AFX_INSTANCE_H__1939BA82_0CA2_47BF_B17C_8CF39E80A47B__INCLUDED_)
