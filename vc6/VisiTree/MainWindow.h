#if !defined MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "..\WinObj\WinObj.h"
#include "AbstractTree.h"
#include "TreeFactory.h"

class MainWindow : public WinObj::CDialog
{
private:
	TreeFactory treeFactory;
	AbstractTree *myTree;
	void OpenTreeFile(LPCTSTR szFile);
	void OnOpenTreeFile();
	void OnSaveMetafile();
	void OnAdd();
	void SetKeyType(KeyType keyType);
	void KeyTypeChanged(KeyType keyType);
	void SetTreeType(TreeType treeType);
	void TreeTypeChanged(TreeType treeType);
	LRESULT OnCommand(UINT message, WPARAM wParam, LPARAM lParam);
	void SizeControls();

public:
	MainWindow(const WinObj::CInstance &instance, HWND hWnd);
	virtual ~MainWindow();
	virtual LRESULT OnMessage(UINT, WPARAM, LPARAM) OVERRIDE;
};

#endif
