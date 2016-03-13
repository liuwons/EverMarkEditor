#include "mainwindow.h"
#include "config.h"

#include <QDebug.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QSize>

#pragma comment(linker, "/subsystem:\"CONSOLE\" /entry:\"WinMainCRTStartup\"")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QDesktopWidget* desk = a.desktop();
    QRect screenRect  = desk->screenGeometry();
    screenSize   = QSize(screenRect.width(), screenRect.height());

	appDir = a.applicationDirPath();

    MainWindow w;
    w.show();

    return a.exec();
}
