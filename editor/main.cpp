#include "mainwindow.h"
#include "config.h"

#include <QDebug.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QSize>
#include <QFile>
#include <QDir>

#include "tool/evernotemanager.h"
#include "tool/appcontext.h"

#pragma comment(linker, "/subsystem:\"CONSOLE\" /entry:\"WinMainCRTStartup\"")

void setStyle()
{
	QFile file("theme.css");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qWarning() << "read file failed: " << "theme.css";
		return;
	}

	QString content = file.readAll();
	file.close();

	qApp->setStyleSheet(content);
}

void initFontAwesome()
{
	AppContext::getContext()->awesome = new QtAwesome(qApp);
	AppContext::getContext()->awesome->initFontAwesome();
}

void init()
{
	AppContext* context = AppContext::getContext();

	context->cssSubDir = "css";

	QCoreApplication::setOrganizationName("WonSoft");
	QCoreApplication::setOrganizationDomain("lwons.com");
	QCoreApplication::setApplicationName("EverMarkEditor");

	context->settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "WonSoft", "EverMarkEditor");

	if (context->settings->contains(CONFIG_STRING_WORKBENCH_PATH))
	{
		context->workbenchPath = new QString(context->settings->value(CONFIG_STRING_WORKBENCH_PATH).toString());
		qInfo() << "Load workbench path: " << *context->workbenchPath;
	}
	else    // Set default workbench path as ~/evermark
	{
		QString homePath = QDir::homePath();
		context->workbenchPath = new QString(homePath + "/evermark");
		context->settings->setValue(CONFIG_STRING_WORKBENCH_PATH, *context->workbenchPath);
		qInfo() << "Set workbench path: " << *context->workbenchPath;
	}

	if (AppContext::getContext()->settings->contains(CONFIG_STRING_EVERNOTE_TOKEN))
	{
		context->evernoteToken = new QString(context->settings->value(CONFIG_STRING_EVERNOTE_TOKEN).toString());
		qInfo() << "Load evernote token: " << *context->evernoteToken;
	}

	if (context->settings->contains(CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE))
	{
		context->evernoteAccountType = new QString(context->settings->value(CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE).toString());
		qInfo() << "Load evernote account type: " << *context->evernoteAccountType;
	}
	else    // Set default Account Type
	{
		context->evernoteAccountType = new QString("yinxiang");
		context->settings->setValue(CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE, *context->evernoteAccountType);
		qInfo() << "Set evernote account type: " << *context->evernoteAccountType;
	}

	if (context->settings->contains(CONFIG_STRING_MARKDOWN_THEME))
	{
		context->markdownTheme = new QString(context->settings->value(CONFIG_STRING_MARKDOWN_THEME).toString());
		qInfo() << "Load markdown theme: " << *context->markdownTheme;
	}
	else    // Set default Theme
	{
		context->markdownTheme = new QString("github");
		context->settings->setValue(CONFIG_STRING_MARKDOWN_THEME, *context->markdownTheme);
		qInfo() << "Set markdown theme: " << *context->markdownTheme;
	}

	context->settings->sync();

	context->workbenchManager = new WorkbenchManager;
	context->workbenchManager->init(*context->workbenchPath);


	if (!context->evernoteToken)
	{
		return;
	}

	if (context->evernoteManager)
		delete context->evernoteManager;
	context->evernoteManager = new EvernoteManager(
		*context->evernoteToken,
		*context->evernoteAccountType,
		*context->markdownTheme,
		context->appDir + "/evermark",
		context->appDir + "/evermark",
		QString("sync"));
	if (!context->evernoteManager->init())
	{
		qWarning() << "EvernoteManager init failed";
		delete context->evernoteManager;
		context->evernoteManager = 0;
		return;
	}
	if (!context->evernoteManager->login())
	{
		qWarning() << "EvernoteManager login failed";
		delete context->evernoteManager;
		context->evernoteManager = 0;
		return;
	}
	qInfo() << "EvernoteManager login succeed";
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

	qSetMessagePattern("[%{type}] %{time [yyyy-MM-dd HH:mm:ss]} (%{function}, %{file}:%{line}) - %{message}");

	AppContext* context = AppContext::getContext();
	context->appDir = a.applicationDirPath();
	
    QDesktopWidget* desk = a.desktop();
    QRect screenRect  = desk->screenGeometry();
	context->screenSize = QSize(screenRect.width(), screenRect.height());
	
	init();
	setStyle();
	initFontAwesome();

    MainWindow w;
    w.show();

    return a.exec();
}
