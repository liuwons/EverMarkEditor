#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include "editor/mainwindow.h"
#include "editor/editor.h"
#include "editor/preview.h"
#include "tool/evernotemanager.h"
#include "tool/workbenchmanager.h"
#include "awesome/QtAwesome.h"

#include <QSettings>

class AppContext
{
public:
	static AppContext* getContext();

	MainWindow* mainWindow;
	Editor* editor;
	Preview* preview;

	EvernoteManager* evernoteManager;
	WorkbenchManager* workbenchManager;

	QSize screenSize;
	QString theme;
	QString appDir;
	QString cssSubDir;

	QtAwesome* awesome;

	QSettings* settings;

	QString* workbenchPath;
	QString* evernoteToken;
	QString* evernoteAccountType;

	QString* markdownTheme;

private:
	AppContext();
	
	static AppContext* instance;
};

#endif