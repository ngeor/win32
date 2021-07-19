// VisiTree.cpp : Defines the entry point for the application.

#include "StdAfx.h"
#include "App.h"
#include "MainWindow.h"
#include "..\WinObj\WinObj.h"

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	// new style app
	WinObj::CInstance app2(hInstance);

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
