#include "mainwindow.h"
#include "config.h"

#include <QDebug.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QSize>
#include <QFile>

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
	qDebug() << "Start init FontAwesome";
	awesome = new QtAwesome(qApp);
	awesome->initFontAwesome();
	qDebug() << "End init FontAwesome";
}

int main(int argc, char *argv[])
{

	/*EvernoteManager em(
		QString("S=s23:U=4e5fbd:E=15ab8c856fd:C=15361172888:P=1cd:A=en-devtoken:V=2:H=e1e63fcf88e8417bfab0c6aeb8ac2c05"),
		QString("yinxiang"),
		QString("github"),
		QString("D:/work/github/EverMarkEditor/release/evermark"),
		QString("D:/work/github/EverMarkEditor/release/evermark"),
		QString("sync"));
	if (!em.init())
	{
		qDebug() << "Init failed";
		return 0;
	}
	if (!em.login())
	{
		qDebug() << "Login failed";
		return 0;
	}
	QMap<QString, QString>* noteBookStatus = em.getNotebookStatus();
	if (!noteBookStatus)
	{
		qDebug() << "Get Notebook Status failed.";
	}

	QList<QString> keys = noteBookStatus->keys();
	for (int i = 0; i < keys.size(); i++)
	{
		QString key = keys.at(i);
		QString val = (*noteBookStatus)[key];
		qDebug() << key << ":" << val;
	}

	qDebug() << "succeed";

	return 0;*/

    QApplication a(argc, argv);

	/*qApp->setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	qApp->setPalette(darkPalette);

	qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");*/


	//qApp->setStyleSheet("QTextEdit{font: 12pt helvetica,arial,freesans,clean,sans-serif; color:#ffffff; background-color: #1e1e1e;}");
	
	setStyle();
	initFontAwesome();
	//qApp->setStyleSheet("QTextEdit{font: 12pt concolas; color:#ffffff; background-color: #1e1e1e;}");


    QDesktopWidget* desk = a.desktop();
    QRect screenRect  = desk->screenGeometry();
    screenSize   = QSize(screenRect.width(), screenRect.height());

	appDir = a.applicationDirPath();

    MainWindow w;
    w.show();

    return a.exec();
}
