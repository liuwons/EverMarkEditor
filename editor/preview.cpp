
#include "preview.h"
#include "config.h"

#include <QSize>
#include <QFile.h>
#include <QDebug>

Preview::Preview(QSize sz)
{
    if (sz.width() && sz.height())
    {
        this->setGeometry(QRect(0, 0, sz.width(), sz.height()));
    }
    else
    {
		this->setGeometry(QRect(0, 0, screenSize.width()/3, screenSize.height()/3*2));
    }

	mainLayout = new QVBoxLayout;

	webView = new QWebView;
	mainLayout->addWidget(webView);
	this->setLayout(mainLayout);

	webView->setHtml(QString(""));

	render = hoedown_html_renderer_new(HOEDOWN_HTML_USE_XHTML, 0);
	document = hoedown_document_new(render, HOEDOWN_EXT_TABLES | HOEDOWN_EXT_FENCED_CODE | HOEDOWN_EXT_HIGHLIGHT, 16);
	buf = hoedown_buffer_new(10 * 1024 * 1024);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
	timer->start(5000);

	needRefresh = false;

	loadCSS();
}

Preview::~Preview()
{

}

void Preview::loadCSS()
{
	QString cssFilePath = appDir + "/" + cssSubDir + "/" + theme + ".css";
	QFile f(cssFilePath);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "ERROR: read css file failed.";
		return;
	}

	cssString = f.readAll();
	f.close();
}

void Preview::updateContent(QString md)
{
	markdown = md;
	needRefresh = true;
}

QString Preview::themelize(QString html)
{
	QString result = "<style>";
	result += cssString;
	result += "</style>";
	result += "<article>";
	result += html;
	result += "</article>";

	return result;
}

void Preview::refresh()
{
	if (!needRefresh)
		return;
	
	qDebug() << "Render Markdown and refresh preview";

	std::string std_str = markdown.toStdString();
	const char* cstr = std_str.c_str();

	hoedown_buffer_set(buf, 0, 0);

	hoedown_document_render(document, buf, (const uint8_t*)cstr, strlen(cstr));

	QString html((const char*)buf->data);
	html = themelize(html);

	webView->setHtml(html);

	needRefresh = false;
}