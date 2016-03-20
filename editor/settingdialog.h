#ifndef SETTING_DIALOG_H
#define SETTING_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

class SettingDialog : public QDialog
{
	Q_OBJECT

public:
	SettingDialog();
	~SettingDialog();

public slots:
	void setEvernoteToken();

private:
	QVBoxLayout* mainLayout;

	QWidget* wgEvernote;
	QVBoxLayout* evernoteLayout;
	QHBoxLayout* evernoteTokenLayout;
	QLabel* lbEvernoteToken;
	QLineEdit* editEvernoteToken;
	QComboBox* cbEvernoteAccountType;
	QComboBox* cbMarkdownTheme;
	QPushButton* btEvernoteToken;
};

#endif