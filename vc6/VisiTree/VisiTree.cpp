// VisiTree.cpp : Defines the entry point for the application.

#include "StdAfx.h"
#include "MainWindow.h"
#include "..\WinObj\WinObj.h"
#include "resource.h"

#define MYCLASSNAME _T("VISITREECLASS")

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	// new style app
	WinObj::CInstance app2(hInstance);

	// new style window class registration
	WinObj::CWindowClass()
		.Dialog()
		.WithClassName(MYCLASSNAME)
		.WithIcon(IDI_VISITREE)
		.WithSmallIcon(IDI_SMALL)
		.WithMenu(IDC_VISITREE)
		.Register(app2);

	// new style window creation
	MainWindow *mainWindow2 = WinObj::BuildDialog<MainWindow>(app2, IDD_MAIN);
	mainWindow2->Show();

	// new style app loop
	WinObj::CDialogMessageLoop messageLoop(*mainWindow2);
	WPARAM result = messageLoop.Run();

	// TODO instance management
	delete mainWindow2;
	return result;
}
