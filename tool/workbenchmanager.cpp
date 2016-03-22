#include "workbenchmanager.h"
#include "editor/config.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>

WorkbenchManager::WorkbenchManager()
{
	root = 0;
}

WorkbenchManager::~WorkbenchManager()
{
	if (root)
		delete root;
}

// Init from a path, load dump file if there is one, or init an empty hierarchy 
bool WorkbenchManager::init(QString workbenchPath)
{
	QDir dir(workbenchPath);
	if (!dir.exists())
		return false;
	QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
	for (int i = 0; i < list.size(); i++)
	{
		QFileInfo f = list.at(i);
		QString fn = f.fileName();
		if (fn == WORKBENCH_CONFIG_FILE_NAME)
		{
			QString fpath = workbenchPath + "/" + WORKBENCH_CONFIG_FILE_NAME;
			QFile file(fpath);
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				qDebug() << "ERROR: read file failed: " << fpath;
				return false;
			}
			QString content = file.readAll();
			file.close();
			if (root)
				delete root;
			root = NoteItem::fromJsonString(content);
			return true;
		}
	}

	root = new NoteItem;
	root->type = TYPE_ROOT;

	path = workbenchPath;
	return true;
}

// Dump workbench information to a file in the workbench directory
bool WorkbenchManager::dump()
{
	if (!root)
		return false;
	QDir dir(path);
	if (!dir.exists())
		return false;

	QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
	for (int i = 0; i < list.size(); i++)
	{
		QFileInfo f = list.at(i);
		QString fn = f.fileName();
		if (fn == WORKBENCH_CONFIG_FILE_NAME)
		{
			QString fpath = path + "/" + WORKBENCH_CONFIG_FILE_NAME;
			QFile file(fpath);
			if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
			{
				qDebug() << "ERROR: open file failed: " << fpath;
				return false;
			}
			QString content = root->toJsonString();
			std::string strContent = content.toStdString();
			file.write(strContent.c_str());
			file.close();
		}
	}
	return true;
}

bool WorkbenchManager::createNote(QString stack, QString notebookGuid, QString guid, QString title, QString content)
{
	return true;
}

bool WorkbenchManager::updateNote(QString noteGuid, QString content)
{
	return true;
}
