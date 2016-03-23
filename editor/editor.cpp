
#include "editor.h"
#include "config.h"

#include "tool/appcontext.h"

#include <QSize>
#include <QDebug>

Editor::Editor(QSize sz)
{
	AppContext* context = AppContext::getContext();

    if (sz.width() && sz.height())
    {
        this->setGeometry(QRect(0, 0, sz.width(), sz.height()));
    }
    else
    {
		this->setGeometry(QRect(0, 0, context->screenSize.width() / 3, context->screenSize.height() / 3 * 2));
    }

	this->mainLayout = new QVBoxLayout;
	this->textEdit = new QTextEdit;

	this->mainLayout->addWidget(this->textEdit);
	this->setLayout(this->mainLayout);

	connect(textEdit, SIGNAL(textChanged()), this, SLOT(innerTextChanged()));
}

Editor::~Editor()
{

}


void Editor::innerTextChanged()
{
	QString text = textEdit->toPlainText();
	emit contentChanged(text);
}


void Editor::loadContent(QString content)
{
	textEdit->setPlainText(content);
}