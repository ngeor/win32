// SimplePage.cpp: implementation of the SimplePage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimplePage.h"

str IncludeTrailingBackSlash(const str& buf)
{
	int len = buf.size();
	if (len > 0 && buf[len - 1] != '\\')
	{
		return buf + _T("\\");
	}
	return buf;
}

class RecurseModeDialog : public WinObj::CDialog
{
public:
	TCHAR filter[MAX_PATH];

	RecurseModeDialog(const WinObj::CInstance& app) : WinObj::CDialog(app)
	{
	}

	virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_INITDIALOG:
			CheckRadioButton(IDC_APPLY_SIMPLE, IDC_APPLY_RECURSE, IDC_APPLY_SIMPLE);
			CheckDlgButton(IDC_FILES, BST_CHECKED);
			CheckDlgButton(IDC_FOLDERS, BST_CHECKED);
			return 1;
			break;

		case WM_CLOSE:
			EndDialog(0);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDOK:
				if (IsDlgButtonChecked(IDC_APPLY_SIMPLE) == BST_CHECKED)
					EndDialog(RECURSE_SIMPLE);
				else
				{
					int ret = 0;
					if (IsDlgButtonChecked(IDC_FILES) == BST_CHECKED)
						ret |= RECURSE_FILES;
					if (IsDlgButtonChecked(IDC_FOLDERS) == BST_CHECKED)
						ret |= RECURSE_FOLDERS;
					if (IsDlgButtonChecked(IDC_NOT_FOLDER) == BST_CHECKED)
						ret |= RECURSE_SKIP_ROOT;
					GetDlgItemText(IDC_FILTER, filter, MAX_PATH);
					EndDialog(ret);
				}
				break;
			case IDCANCEL:
				EndDialog(0);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		return 0;
	}
};

int CSimplePage::RecurseMode()
{
	RecurseModeDialog dlg(GetInstance());
	int ret = dlg.Modal(*this, IDD_RECURSE);
	lstrcpy(recurseFilter, dlg.filter);
	if (lstrlen(recurseFilter) == 0)
	{
		lstrcpy(recurseFilter, _T("*.*"));
	}

	return ret;
}

void CSimplePage::recurseThat(const str& szDir)
{
	HANDLE fh;
	WIN32_FIND_DATA fd;
	str buf = IncludeTrailingBackSlash(szDir) + _T("*.*");

	// MessageBox(0, buf, "", 0);
	if ((fh = FindFirstFile(buf.c_str(), &fd)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (lstrcmp(fd.cFileName, _T(".")) && lstrcmp(fd.cFileName, _T("..")))
			{
				buf = IncludeTrailingBackSlash(szDir) + fd.cFileName;
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					recurseThat(buf);
				}
			}
		} while (FindNextFile(fh, &fd));
		FindClose(fh);
	}

	buf = IncludeTrailingBackSlash(szDir) + recurseFilter;

	if ((fh = FindFirstFile(buf.c_str(), &fd)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (lstrcmp(fd.cFileName, _T(".")) && lstrcmp(fd.cFileName, _T("..")))
			{
				buf = IncludeTrailingBackSlash(szDir) + fd.cFileName;
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (recurseMode & RECURSE_FOLDERS)
						templist.push_back(buf);
				}
				else
				{
					if (recurseMode & RECURSE_FILES)
						templist.push_back(buf);
				}
			}
		} while (FindNextFile(fh, &fd));
		FindClose(fh);
	}
}
