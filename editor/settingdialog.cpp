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
	evernoteTokenLayout->addWidget(lbEvernoteToken);
	evernoteTokenLayout->addWidget(editEvernoteToken);
	
	cbEvernoteAccountType = new QComboBox;
	cbEvernoteAccountType->addItem(tr("Evernote"));
	cbEvernoteAccountType->addItem(tr("Yinxiang"));
	cbMarkdownTheme = new QComboBox;
	cbMarkdownTheme->addItem(tr("github"));
	cbMarkdownTheme->addItem(tr("github2"));

	btEvernoteToken = new QPushButton(tr("OK"));

	evernoteLayout->addLayout(evernoteTokenLayout);
	evernoteLayout->addWidget(cbEvernoteAccountType);
	evernoteLayout->addWidget(cbMarkdownTheme);
	wgEvernote->setLayout(evernoteLayout);

	mainLayout->addWidget(wgEvernote);
	this->setLayout(mainLayout);

	connect(btEvernoteToken, SIGNAL(clicked()), this, SLOT(setEvernoteToken()));
}

SettingDialog::~SettingDialog()
{
	delete lbEvernoteToken;
	delete editEvernoteToken;
	delete cbEvernoteAccountType;
	delete cbMarkdownTheme;
	delete btEvernoteToken;
	delete evernoteTokenLayout;
	delete evernoteLayout;
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

	settings->setValue(CONFIG_STRING_EVERNOTE_TOKEN, token);
	settings->sync();
}
