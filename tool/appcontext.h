#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include "editor/mainwindow.h"
#include "editor/editor.h"
#include "editor/preview.h"
#include "tool/evernotemanager.h"
#include "tool/workbenchmanager.h"

class AppContext
{
public:
	static AppContext* getContext();

	MainWindow* mainWindow;
	Editor* editor;
	Preview* preview;

	EvernoteManager* evernoteManager;
	WorkbenchManager* workbenchManager;

private:
	AppContext();
	
	static AppContext* instance;
};

#endif