#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class Editor : public QWidget
{
	Q_OBJECT

public:
    Editor(QSize sz = QSize(0, 0));
	~Editor();

	void loadContent(QString content);

public slots:
	void innerTextChanged();

signals:
	void contentChanged(QString content);

private:
	QVBoxLayout* mainLayout;
	QTextEdit* textEdit;
};

#endif // PREVIEW_H

