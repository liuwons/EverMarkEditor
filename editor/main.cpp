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
		qDebug() << "ERROR: read file failed: " << "theme.css";
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
		qDebug() << "load workbench path: " << *context->workbenchPath;
	}
	else    // Set default workbench path as ~/evermark
	{
		QString homePath = QDir::homePath();
		context->workbenchPath = new QString(homePath + "/evermark");
		context->settings->setValue(CONFIG_STRING_WORKBENCH_PATH, homePath + *context->workbenchPath);
		qDebug() << "set workbench path: " << *context->workbenchPath;
	}

	if (AppContext::getContext()->settings->contains(CONFIG_STRING_EVERNOTE_TOKEN))
	{
		context->evernoteToken = new QString(context->settings->value(CONFIG_STRING_EVERNOTE_TOKEN).toString());
		qDebug() << "load evernote token: " << *context->evernoteToken;
	}

	if (context->settings->contains(CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE))
	{
		context->evernoteAccountType = new QString(context->settings->value(CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE).toString());
		qDebug() << "load evernote account type: " << *context->evernoteAccountType;
	}
	else    // Set default Account Type
	{
		context->evernoteAccountType = new QString("evernote");
		context->settings->setValue(CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE, *context->evernoteAccountType);
		qDebug() << "set evernote account type: " << *context->evernoteAccountType;
	}

	if (context->settings->contains(CONFIG_STRING_MARKDOWN_THEME))
	{
		context->markdownTheme = new QString(context->settings->value(CONFIG_STRING_MARKDOWN_THEME).toString());
		qDebug() << "load markdown theme: " << *context->markdownTheme;
	}
	else    // Set default Theme
	{
		context->markdownTheme = new QString("github");
		context->settings->setValue(CONFIG_STRING_MARKDOWN_THEME, *context->markdownTheme);
		qDebug() << "set markdown theme: " << *context->markdownTheme;
	}

	context->settings->sync();
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

	init();
	setStyle();
	initFontAwesome();

	AppContext* context = AppContext::getContext();

    QDesktopWidget* desk = a.desktop();
    QRect screenRect  = desk->screenGeometry();
	context->screenSize = QSize(screenRect.width(), screenRect.height());

	context->appDir = a.applicationDirPath();

    MainWindow w;
    w.show();

    return a.exec();
}
