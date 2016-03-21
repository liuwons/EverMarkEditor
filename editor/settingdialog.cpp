#include "settingdialog.h"
#include "config.h"

#include <QMessageBox>
#include <QDebug>

SettingDialog::SettingDialog()
{
	mainLayout = new QVBoxLayout;

	wgEvernote = new QWidget;
	evernoteLayout = new QVBoxLayout;

	evernoteTokenLayout = new QHBoxLayout;
	lbEvernoteToken = new QLabel(tr("Evernote Token"));
	editEvernoteToken = new QLineEdit;
	if (evernoteToken)
		editEvernoteToken->setText(*evernoteToken);
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
	QString token = editEvernoteToken->text();
	if (!token.length())
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Evernote Token can not be empty!"));
		msgBox.exec();
	}

	if (!evernoteToken)
		evernoteToken = new QString;
	*evernoteToken = token;

	QString evernoteAccountType = cbEvernoteAccountType->currentText();
	QString markdownTheme = cbMarkdownTheme->currentText();

	settings->setValue(CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE, evernoteAccountType);
	settings->setValue(CONFIG_STRING_MARKDOWN_THEME, markdownTheme);
	settings->setValue(CONFIG_STRING_EVERNOTE_TOKEN, token);
	settings->sync();
}
