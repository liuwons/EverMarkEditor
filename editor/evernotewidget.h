#ifndef EVERNOTE_WIDGET_H
#define PEVERNOTE_WIDGET_H

#include "tool/evernotemanager.h"

#include <QWidget>
#include <QVBoxLayout>

class EvernoteWidget : public QWidget
{
	Q_OBJECT

public:
	EvernoteWidget(QString auth, QString type, QString style, QString root, QString execPath, QString execName, QSize sz=QSize(0, 0));
	~EvernoteWidget();

	void setSource(QString auth, QString type, QString style, QString root, QString execPath, QString execName);

public slots:
	void refresh();

private:
	QVBoxLayout* mainLayout;
	EvernoteManager* manager;
};

#endif // PREVIEW_H

