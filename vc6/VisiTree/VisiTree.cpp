// VisiTree.cpp : Defines the entry point for the application.

#include "StdAfx.h"
#include "App.h"
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

	// Create app instance
	App app(hInstance);

	// Create Window
	MainWindow mainWindow(app);
	mainWindow.Create();

	// Register the message handler
	app.Add(mainWindow.CreateDialogMessageHandler());

	// Show the Window
	mainWindow.Show();

	// App loop
	return app.Run();
}
