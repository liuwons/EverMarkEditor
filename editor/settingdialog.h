#ifndef SETTING_DIALOG_H
#define SETTING_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class SettingDialog : public QDialog
{
	Q_OBJECT

public:
	SettingDialog();

public slots:
	void setEvernoteToken();

private:
	QVBoxLayout* mainLayout;

	QWidget* wgEvernote;
	QHBoxLayout* evernoteTokenLayout;
	QLabel* lbEvernoteToken;
	QLineEdit* editEvernoteToken;
	QPushButton* btEvernoteToken;
};

#endif