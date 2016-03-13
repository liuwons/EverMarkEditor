#ifndef PREVIEW_H
#define PREVIEW_H

#include "heodown/html.h"

#include <QTimer>
#include <QWidget>
#include <QWebView>
#include <QVBoxLayout>

class Preview : public QWidget
{
	Q_OBJECT

public:
    Preview(QSize sz = QSize(0, 0));
	~Preview();

public slots:
	void updateContent(QString md);
	void refresh();
	void loadCSS();

private:
	QString themelize(QString html);

	hoedown_renderer *render;
	hoedown_document *document;
	hoedown_buffer *buf;

	QTimer* timer;
	QString cssString;

	QString markdown;
	bool needRefresh;

	QWebView* webView;
	QVBoxLayout* mainLayout;
};

#endif // PREVIEW_H

