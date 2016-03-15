#include "evernotewidget.h"

#include <QSize>
#include <QDebug>

EvernoteWidget::EvernoteWidget(QSize sz)
{
    if (sz.width() && sz.height())
    {
        this->setGeometry(QRect(0, 0, sz.width(), sz.height()));
    }


	mainLayout = new QVBoxLayout;

	this->setLayout(mainLayout);

	this->manager = new EvernoteManager();
}

EvernoteWidget::~EvernoteWidget()
{

}

void EvernoteWidget::refresh()
{
}