// VBoxHelper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <tchar.h>

void printProcess(DWORD processId);

int main()
{
	DWORD aProcesses[1024];
	DWORD cbNeeded;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		std::cerr << "Could not EnumProcesses\n";
		return 1;
	}
	DWORD cProcesses = cbNeeded / sizeof(DWORD);
	for (unsigned int i = 0; i < cProcesses; i++)
	{
		DWORD processId = aProcesses[i];
		if (processId != 0)
		{
			printProcess(processId);
		}
	}
	return 0;
}

bool startsWith(LPCTSTR s, LPCTSTR prefix)
{
	return _tcsnccmp(s, prefix, _tcslen(prefix)) == 0;
}

bool isVirtualBoxProcess(LPCTSTR s)
{
	return (startsWith(s, TEXT("VBox")) && _tcscmp(s, TEXT("VBoxHelper.exe")) != 0) ||
	       startsWith(s, TEXT("VirtualBox"));
}

void printProcess(DWORD processId)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, processId);
	if (hProcess != NULL)
	{
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			TCHAR szProcessName[MAX_PATH];
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
			if (isVirtualBoxProcess(szProcessName))
			{
				TerminateProcess(hProcess, 0);
			}
		}
		CloseHandle(hProcess);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files
//   to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
