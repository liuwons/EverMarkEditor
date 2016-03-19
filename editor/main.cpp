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
	awesome = new QtAwesome(qApp);
	awesome->initFontAwesome();
}

void init()
{
	QCoreApplication::setOrganizationName("WonSoft");
	QCoreApplication::setOrganizationDomain("lwons.com");
	QCoreApplication::setApplicationName("EverMarkEditor");

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "WonSoft", "EverMarkEditor");

	if (settings->contains(CONFIG_STRING_WORKBENCH_PATH))
	{
		workbenchPath = new QString(settings->value(CONFIG_STRING_WORKBENCH_PATH).toString());
		qDebug() << "load workbench path: " << *workbenchPath;
	}
	else
	{
		QString homePath = QDir::homePath();
		workbenchPath = new QString(homePath + "/evermark");
		settings->setValue(CONFIG_STRING_WORKBENCH_PATH, homePath + *workbenchPath);
		qDebug() << "set workbench path: " << *workbenchPath;
	}

	if (settings->contains(CONFIG_STRING_EVERNOTE_TOKEN))
	{
		evernoteToken = new QString(settings->value(CONFIG_STRING_EVERNOTE_TOKEN).toString());
		qDebug() << "load evernote token: " << *evernoteToken;
	}

	settings->sync();
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

	init();
	setStyle();
	initFontAwesome();

    QDesktopWidget* desk = a.desktop();
    QRect screenRect  = desk->screenGeometry();
    screenSize   = QSize(screenRect.width(), screenRect.height());

	appDir = a.applicationDirPath();

    MainWindow w;
    w.show();

    return a.exec();
}
