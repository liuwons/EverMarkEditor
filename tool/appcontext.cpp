#include "appcontext.h"

AppContext* AppContext::instance = 0;

AppContext::AppContext()
{
	mainWindow = 0;
	editor = 0;
	preview = 0;

	evernoteManager = 0;
	workbenchManager = 0;

	awesome = 0;

	settings = 0;

	workbenchPath = 0;
	evernoteToken = 0;
	evernoteAccountType = 0;

	markdownTheme = 0;
}

AppContext* AppContext::getContext()
{
	if (!instance)
		instance = new AppContext();
	return instance;
}