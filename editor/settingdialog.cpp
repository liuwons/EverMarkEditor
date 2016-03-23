#include "settingdialog.h"
#include "config.h"

#include "tool/appcontext.h"

#include <QMessageBox>
#include <QDebug>

SettingDialog::SettingDialog()
{
	AppContext* context = AppContext::getContext();

	mainLayout = new QVBoxLayout;

	wgEvernote = new QWidget;
	evernoteLayout = new QVBoxLayout;

	evernoteTokenLayout = new QHBoxLayout;
	lbEvernoteToken = new QLabel(tr("Evernote Token"));
	editEvernoteToken = new QLineEdit;
	if (context->evernoteToken)
		editEvernoteToken->setText(*context->evernoteToken);
	evernoteTokenLayout->addWidget(lbEvernoteToken);
	evernoteTokenLayout->addWidget(editEvernoteToken);
	
	cbEvernoteAccountType = new QComboBox;
	cbEvernoteAccountType->addItem("evernote");
	cbEvernoteAccountType->addItem("yinxiang");
	cbMarkdownTheme = new QComboBox;
	cbMarkdownTheme->addItem(tr("github"));
	cbMarkdownTheme->addItem(tr("github2"));

	btConfirm = new QPushButton(tr("OK"));

	evernoteLayout->addLayout(evernoteTokenLayout);
	evernoteLayout->addWidget(cbEvernoteAccountType);
	evernoteLayout->addWidget(cbMarkdownTheme);
	wgEvernote->setLayout(evernoteLayout);

	mainLayout->addWidget(wgEvernote);
	mainLayout->addWidget(btConfirm);
	this->setLayout(mainLayout);

	connect(btConfirm, SIGNAL(clicked()), this, SLOT(setEvernoteToken()));
}

SettingDialog::~SettingDialog()
{
	delete lbEvernoteToken;
	delete editEvernoteToken;
	delete cbEvernoteAccountType;
	delete cbMarkdownTheme;
	delete evernoteTokenLayout;
	delete evernoteLayout;
	delete btConfirm;
	delete mainLayout;
	delete wgEvernote;
}

void SettingDialog::setEvernoteToken()
{
	AppContext* context = AppContext::getContext();

	QString token = editEvernoteToken->text();
	if (!token.length())
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Evernote Token can not be empty!"));
		msgBox.exec();
	}

	if (!context->evernoteToken)
		context->evernoteToken = new QString;
	*context->evernoteToken = token;

	QString evernoteAccountType = cbEvernoteAccountType->currentText();
	QString markdownTheme = cbMarkdownTheme->currentText();

	context->settings->setValue(CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE, evernoteAccountType);
	context->settings->setValue(CONFIG_STRING_MARKDOWN_THEME, markdownTheme);
	context->settings->setValue(CONFIG_STRING_EVERNOTE_TOKEN, token);
	context->settings->sync();
}
