#include "evernotewidget.h"

#include <QSize>
#include <QDebug>

EvernoteWidget::EvernoteWidget(QString auth, QString type, QString style, QString root, QString execPath, QString execName, QSize sz)
{
    if (sz.width() && sz.height())
    {
        this->setGeometry(QRect(0, 0, sz.width(), sz.height()));
    }


	mainLayout = new QVBoxLayout;
	this->manager = new EvernoteManager(auth, type, style, root, execPath, execName);
	this->setLayout(mainLayout);
}


EvernoteWidget::~EvernoteWidget()
{

}

void EvernoteWidget::refresh()
{
}