#include "settingdialog.h"
#include "config.h"

#include <QMessageBox>

SettingDialog::SettingDialog()
{
	mainLayout = new QVBoxLayout;

	wgEvernote = new QWidget;
	evernoteTokenLayout = new QHBoxLayout;
	lbEvernoteToken = new QLabel(tr("Evernote Token"));
	editEvernoteToken = new QLineEdit;
	btEvernoteToken = new QPushButton(tr("OK"));
	evernoteTokenLayout->addWidget(lbEvernoteToken);
	evernoteTokenLayout->addWidget(editEvernoteToken);
	evernoteTokenLayout->addWidget(btEvernoteToken);
	wgEvernote->setLayout(evernoteTokenLayout);

	mainLayout->addWidget(wgEvernote);
	this->setLayout(mainLayout);

	connect(btEvernoteToken, SIGNAL(clicked()), this, SLOT(setEvernoteToken()));
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
