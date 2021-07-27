// SimplePage.h: interface for the SimplePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEPAGE_H__DDC5DD14_F203_440C_8BF0_3B6998093395__INCLUDED_)
#define AFX_SIMPLEPAGE_H__DDC5DD14_F203_440C_8BF0_3B6998093395__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\WinObjCommCtrl\DateTimePicker.h"
#include "..\WinObjCommCtrl\PropSheet.h"
#include "..\WinObj\Dialog.h"
#include "..\WinObj\Instance.h"
#include "..\WinObj\str.h"
#include "resource.h"

#define RECURSE_SIMPLE    1
#define RECURSE_FILES     2
#define RECURSE_FOLDERS   4
#define RECURSE_SKIP_ROOT 8

//#define MULTIPLE_FILES list.getFileCount() > 1

#define BUTTON_HANDLER(id, proc)                                                                                       \
	if (LOWORD(wParam) == id && HIWORD(wParam) == BN_CLICKED)                                                          \
	{                                                                                                                  \
		proc();                                                                                                        \
		return 0;                                                                                                      \
	}
#define BUTTON_HANDLER_RANGE(id_min, id_max, proc)                                                                     \
	if (LOWORD(wParam) >= id_min && LOWORD(wParam) <= id_max && HIWORD(wParam) == BN_CLICKED)                          \
	{                                                                                                                  \
		proc(LOWORD(wParam));                                                                                          \
		return 0;                                                                                                      \
	}

class CSimplePage : public WinObj::CPropSheet
{
private:
	void recurseThat(const str& szDir);
	int RecurseMode();
	string_list mylist;
	string_list templist;
	bool hasfolders;
	int recurseMode;
	TCHAR recurseFilter[MAX_PATH];
	UINT bInitArchive, bInitReadOnly, bInitHidden, bInitSystem;

public:
	CSimplePage(const WinObj::CInstance& app, string_list& otherList, bool hasfolders) : WinObj::CPropSheet(app)
	{
		int it;

		for (it = 0; it < otherList.size(); it++)
		{
			// 'it' points at the next filename.  Allocate a new copy of the string
			// that the page will own.
			mylist.push_back(otherList[it]);
		}

		this->hasfolders = hasfolders;
		// MessageBox(0, "SimplePage", "", 0);
	}

	void initTempList()
	{
		templist.clear();
		for (int it = 0; it < mylist.size(); it++)
		{
			templist.push_back(mylist[it]);
		}
	}

	virtual ~CSimplePage()
	{
	}

	void OnAttributesClick()
	{
		bool f = IsDlgButtonChecked(IDC_ATTRIBUTES) == BST_CHECKED;
		EnableDlgItem(IDC_ARCHIVE, f);
		EnableDlgItem(IDC_READONLY, f);
		EnableDlgItem(IDC_HIDDEN, f);
		EnableDlgItem(IDC_SYSTEM, f);
	}

	void OnTimesClick()
	{
		bool f = IsDlgButtonChecked(IDC_TIMES) == BST_CHECKED;
		EnableDlgItem(IDC_ONE_TIME, f);
		EnableDlgItem(IDC_CREATE, f);
		EnableDlgItem(IDC_LAST_ACCESS, f);
		EnableDlgItem(IDC_LAST_WRITE, f);
		EnableDlgItem(IDC_DATEPICKER1, f);
		EnableDlgItem(IDC_DATEPICKER2, f);
		EnableDlgItem(IDC_DATEPICKER3, f);
		EnableDlgItem(IDC_TIMEPICKER1, f);
		EnableDlgItem(IDC_TIMEPICKER2, f);
		EnableDlgItem(IDC_TIMEPICKER3, f);
		EnableDlgItem(IDC_NOW1, f);
		EnableDlgItem(IDC_NOW2, f);
		EnableDlgItem(IDC_NOW3, f);
		EnableDlgItem(IDC_READONLY_TOO, f);
	}

	void OnOneTimeClick()
	{
		int flag = (IsDlgButtonChecked(IDC_ONE_TIME) == BST_CHECKED) ? SW_HIDE : SW_SHOW;
		ShowDlgItem(IDC_DATEPICKER2, flag);
		ShowDlgItem(IDC_DATEPICKER3, flag);
		ShowDlgItem(IDC_TIMEPICKER2, flag);
		ShowDlgItem(IDC_TIMEPICKER3, flag);
		ShowDlgItem(IDC_NOW2, flag);
		ShowDlgItem(IDC_NOW3, flag);
	}

	void applyAttribute(LPDWORD attrs, UINT checkboxState, DWORD fileAttribute)
	{
		if (checkboxState == BST_CHECKED)
			*attrs |= fileAttribute;
		else if (checkboxState == BST_UNCHECKED)
			*attrs &= ~fileAttribute;
	}

	void prepareAttribute(LPDWORD orMask, LPDWORD andMask, UINT bFlag, UINT bInitFlag, DWORD fileAttribute)
	{
		if (bFlag != bInitFlag)
		{
			if (bFlag == BST_CHECKED)
				*orMask |= fileAttribute;
			else if (bFlag == BST_UNCHECKED)
				*andMask &= ~fileAttribute;
		}
	}

	void prepareMasks(LPDWORD orMask, LPDWORD andMask, UINT bArchive, UINT bReadOnly, UINT bHidden, UINT bSystem)
	{
		*andMask = 0xFFFFFFFF;
		*orMask  = 0;
		prepareAttribute(orMask, andMask, bArchive, bInitArchive, FILE_ATTRIBUTE_ARCHIVE);
		prepareAttribute(orMask, andMask, bReadOnly, bInitReadOnly, FILE_ATTRIBUTE_READONLY);
		prepareAttribute(orMask, andMask, bHidden, bInitHidden, FILE_ATTRIBUTE_HIDDEN);
		prepareAttribute(orMask, andMask, bSystem, bInitSystem, FILE_ATTRIBUTE_SYSTEM);
	}

	void MySetFileAttrs(const str& lpFileName, DWORD orMask, DWORD andMask)
	{
		DWORD attrs = GetFileAttributes(lpFileName.c_str());
		if (attrs == 0xFFFFFFFF)
		{
			// add a failure message to the log
		}
		else
		{
			attrs |= orMask;
			attrs &= andMask;

			if (!SetFileAttributes(lpFileName.c_str(), attrs))
			{
				// add a failure message to the log
			}
		}
	}

	BOOL GetSomeFileTime(int ctlIndex, LPFILETIME ft)
	{
		WinObj::CDateTimePicker dateCtl(*this, IDC_DATEPICKER1 + ctlIndex);
		WinObj::CDateTimePicker timeCtl(*this, IDC_TIMEPICKER1 + ctlIndex);
		SYSTEMTIME p1;
		SYSTEMTIME p2;
		bool validDate = dateCtl.GetSystemTime(&p1) == GDT_VALID;
		bool validTime = timeCtl.GetSystemTime(&p2) == GDT_VALID;
		FILETIME f1;
		if (validDate && validTime)
		{
			p1.wHour         = p2.wHour;
			p1.wMilliseconds = p2.wMilliseconds;
			p1.wMinute       = p2.wMinute;
			p1.wSecond       = p2.wSecond;
			if (!SystemTimeToFileTime(&p1, &f1))
				return FALSE;
			return LocalFileTimeToFileTime(&f1, ft);
		}
		else
		{
			return FALSE;
		}
	}

	void MySetFileTime(const str& lpFileName, LPFILETIME f1, LPFILETIME f2, LPFILETIME f3)
	{
		HANDLE hFile = CreateFile(lpFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		                          FILE_FLAG_BACKUP_SEMANTICS, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			// add something to the log
		}
		else
		{
			if (!SetFileTime(hFile, f1, f2, f3))
			{
				// add something to the log
			}
			CloseHandle(hFile);
		}
	}

	void MyGetFileTime(const str& lpFileName, SYSTEMTIME(s)[3])
	{
		HANDLE hFile = CreateFile(lpFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		                          FILE_FLAG_BACKUP_SEMANTICS, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			// add something to the log
		}
		else
		{
			FILETIME f[3], lf[3];
			if (!GetFileTime(hFile, &f[0], &f[1], &f[2]))
			{
				// add something to the log
			}
			for (int i = 0; i < 3; i++)
			{
				FileTimeToLocalFileTime(&f[i], &lf[i]);
				FileTimeToSystemTime(&lf[i], &s[i]);
			}
			CloseHandle(hFile);
		}
	}

	void SetSystemTime(int index, LPSYSTEMTIME systemTime)
	{
		WinObj::CDateTimePicker dateCtl(*this, IDC_DATEPICKER1 + index);
		dateCtl.SetSystemTime(GDT_VALID, systemTime);
		WinObj::CDateTimePicker timeCtl(*this, IDC_TIMEPICKER1 + index);
		timeCtl.SetSystemTime(GDT_VALID, systemTime);
	}

	void InitDateTimeCtls(const str& lpFileName)
	{
		SYSTEMTIME s[3];
		MyGetFileTime(lpFileName, s);
		for (int i = 0; i < 3; i++)
		{
			SetSystemTime(i, &s[i]);
		}
	}

	void OnNowClick(UINT id)
	{
		SYSTEMTIME s;
		GetLocalTime(&s);
		SetSystemTime(id - IDC_NOW1, &s);
		Changed();
	}

	bool OnOK()
	{

		LPCTSTR lpFileName = NULL;
		int it;

		if (IsDlgButtonChecked(IDC_ATTRIBUTES) != BST_CHECKED && IsDlgButtonChecked(IDC_TIMES) != BST_CHECKED)
			return true;

		if (hasfolders)
		{
			recurseMode = RecurseMode();
			if (!recurseMode)
				return false;
		}

		SetCursor(LoadCursor(0, IDC_WAIT));

		if (recurseMode & RECURSE_SKIP_ROOT)
		{
			templist.clear();
			for (it = 0; it < mylist.size(); it++)
			{
				if (!(GetFileAttributes(mylist[it].c_str()) & FILE_ATTRIBUTE_DIRECTORY))
					templist.push_back(mylist[it]);
			}
		}
		else
			initTempList();

		if (recurseMode != RECURSE_SIMPLE)
		{
			for (it = 0; it < mylist.size(); it++)
			{
				if (GetFileAttributes(mylist[it].c_str()) & FILE_ATTRIBUTE_DIRECTORY)
					recurseThat(mylist[it]);
			}
		}

		if (IsDlgButtonChecked(IDC_ATTRIBUTES) == BST_CHECKED)
		{
			// set file attributes

			UINT bArchive  = IsDlgButtonChecked(IDC_ARCHIVE);
			UINT bReadOnly = IsDlgButtonChecked(IDC_READONLY);
			UINT bHidden   = IsDlgButtonChecked(IDC_HIDDEN);
			UINT bSystem   = IsDlgButtonChecked(IDC_SYSTEM);

			DWORD orMask, andMask;
			prepareMasks(&orMask, &andMask, bArchive, bReadOnly, bHidden, bSystem);

			// for each file:
			for (it = 0; it < templist.size(); it++)
			{
				// MessageBox(0, it->c_str(), "", 0);

				MySetFileAttrs(templist[it], orMask, andMask);
			}

			bInitArchive  = IsDlgButtonChecked(IDC_ARCHIVE);
			bInitReadOnly = IsDlgButtonChecked(IDC_READONLY);
			bInitHidden   = IsDlgButtonChecked(IDC_HIDDEN);
			bInitSystem   = IsDlgButtonChecked(IDC_SYSTEM);
		}

		if (IsDlgButtonChecked(IDC_TIMES) == BST_CHECKED)
		{
			bool failure = false;
			FILETIME ftCreate, ftLastAccess, ftLastWrite;
			LPFILETIME lpftCreate = NULL, lpftLastAccess = NULL, lpftLastWrite = NULL;

			if (IsDlgButtonChecked(IDC_ONE_TIME) == BST_CHECKED)
			{
				if (!GetSomeFileTime(0, &ftCreate))
				{
					// generic failure
					failure = true;
				}
				else
				{
					lpftCreate = lpftLastAccess = lpftLastWrite = &ftCreate;
				}
			}
			else
			{
				if (IsDlgButtonChecked(IDC_CREATE) == BST_CHECKED)
				{
					if (!GetSomeFileTime(0, &ftCreate))
					{
						failure = true;
					}
					else
					{
						lpftCreate = &ftCreate;
					}
				}

				if (!failure && IsDlgButtonChecked(IDC_LAST_ACCESS) == BST_CHECKED)
				{
					if (!GetSomeFileTime(1, &ftLastAccess))
					{
						failure = true;
					}
					else
					{
						lpftLastAccess = &ftLastAccess;
					}
				}

				if (!failure && IsDlgButtonChecked(IDC_LAST_WRITE) == BST_CHECKED)
				{
					if (!GetSomeFileTime(2, &ftLastWrite))
					{
						failure = true;
					}
					else
					{
						lpftLastWrite = &ftLastWrite;
					}
				}
			}

			if (!failure)
			{
				bool includeReadOnly = IsDlgButtonChecked(IDC_READONLY_TOO) == BST_CHECKED;
				for (it = 0; it < templist.size(); it++)
				{
					bool restoreReadOnly = false;
					DWORD attrs;

					if (includeReadOnly)
					{
						attrs = GetFileAttributes(templist[it].c_str());
						if (restoreReadOnly = (attrs & FILE_ATTRIBUTE_READONLY))
						{
							SetFileAttributes(templist[it].c_str(), attrs & ~FILE_ATTRIBUTE_READONLY);
							// MessageBox(0, it->c_str(), "", 0);
						}
					}

					MySetFileTime(templist[it], lpftCreate, lpftLastAccess, lpftLastWrite);

					if (restoreReadOnly)
						SetFileAttributes(templist[it].c_str(), attrs);
				}
			}
		}

		templist.clear();
		SetCursor(LoadCursor(0, IDC_ARROW));

		return true;
	}

	void Changed()
	{
		NotifyParentChanged();
	}

	void Nag()
	{
		DWORD test = GetTickCount();
		do
		{
			MsgBox(_T("ChangeFileTimePS is shareware. Visit http://nikosgeorgiou.tripod.com to find out how to ")
			       _T("become a registered user and get rid of this message."),
			       _T("Shareware reminder"), MB_ICONINFORMATION);
			test = GetTickCount() - test;
		} while (test < 2000);
	}

	void checkFileAttribute(DWORD attrs, DWORD attrBit, UINT* value, bool firstTime)
	{
		UINT test;

		test = (attrs & attrBit) ? BST_CHECKED : BST_UNCHECKED;
		if (firstTime)
			*value = test;
		else if (*value != test)
			*value = BST_INDETERMINATE;
	}

	void initCheckBox(UINT ctlID, UINT value)
	{
		if (value == BST_INDETERMINATE)
			SendDlgItemMessage(ctlID, BM_SETSTYLE, BS_AUTO3STATE, 0);

		CheckDlgButton(ctlID, value);
	}

	virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		int it;

		switch (msg)
		{
		case WM_INITDIALOG:
			UINT bArchive, bReadOnly, bHidden, bSystem;

			for (it = 0; it < mylist.size(); it++)
			{

				SendDlgItemMessage(IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)mylist[it].c_str());
				DWORD attrs = GetFileAttributes(mylist[it].c_str());

				checkFileAttribute(attrs, FILE_ATTRIBUTE_ARCHIVE, &bArchive, it == 0);
				checkFileAttribute(attrs, FILE_ATTRIBUTE_READONLY, &bReadOnly, it == 0);
				checkFileAttribute(attrs, FILE_ATTRIBUTE_HIDDEN, &bHidden, it == 0);
				checkFileAttribute(attrs, FILE_ATTRIBUTE_SYSTEM, &bSystem, it == 0);
			}

			initCheckBox(IDC_ARCHIVE, bInitArchive = bArchive);
			initCheckBox(IDC_READONLY, bInitReadOnly = bReadOnly);
			initCheckBox(IDC_HIDDEN, bInitHidden = bHidden);
			initCheckBox(IDC_SYSTEM, bInitSystem = bSystem);

			initCheckBox(IDC_ATTRIBUTES, BST_CHECKED);
			initCheckBox(IDC_TIMES, BST_CHECKED);
			initCheckBox(IDC_CREATE, BST_CHECKED);
			initCheckBox(IDC_LAST_ACCESS, BST_CHECKED);
			initCheckBox(IDC_LAST_WRITE, BST_CHECKED);
			initCheckBox(IDC_ONE_TIME, BST_CHECKED);
			OnOneTimeClick();

			InitDateTimeCtls(mylist[0]);
			return 1;
			break;
		case WM_COMMAND:
			BUTTON_HANDLER(IDC_ATTRIBUTES, OnAttributesClick);
			BUTTON_HANDLER(IDC_TIMES, OnTimesClick);
			BUTTON_HANDLER(IDC_ONE_TIME, OnOneTimeClick);
			BUTTON_HANDLER(IDC_ARCHIVE, Changed);
			BUTTON_HANDLER(IDC_READONLY, Changed);
			BUTTON_HANDLER(IDC_HIDDEN, Changed);
			BUTTON_HANDLER(IDC_SYSTEM, Changed);
			BUTTON_HANDLER_RANGE(IDC_NOW1, IDC_NOW3, OnNowClick);
			break;
		case WM_NOTIFY: {
			NMHDR* phdr = (NMHDR*)lParam;

			switch (phdr->code)
			{
			case PSN_APPLY:
				bool result;
				result = OnOK();
				if (result)
				{
					Nag();
				}
				SetDialogMessageResult(result ? PSNRET_NOERROR : PSNRET_INVALID);
				return 1;
				break;

			case DTN_DATETIMECHANGE:
				// If the user changes any of the DTP controls, enable
				// the Apply button.

				Changed();
				break;
			}
		}
		}
		return 0;
	}
};

#endif // !defined(AFX_SIMPLEPAGE_H__DDC5DD14_F203_440C_8BF0_3B6998093395__INCLUDED_)
