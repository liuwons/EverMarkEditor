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
	inited = false;
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

			inited = true;
			return true;
		}
	}

	root = new NoteItem;
	root->type = TYPE_ROOT;

	path = workbenchPath;

	inited = true;
	return true;
}

// Dump workbench information to a file in the workbench directory
bool WorkbenchManager::dump()
{
	if (!root || !inited)
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
	if (!inited)
		return false;

	NoteItem* notebook = 0;
	if (stack.length() > 0)
	{
		for (int i = 0; i < root->childs.size(); i++)
		{
			if (root->childs.at(i)->type == TYPE_STACK && root->childs.at(i)->name == stack)
			{
				NoteItem* stack = root->childs.at(i);
				for (int k = 0; k < stack->childs.size(); k++)
				{
					if (stack->childs.at(k)->id == notebookGuid)
					{
						notebook = stack->childs.at(k);
						break;
					}
				}
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < root->childs.size(); i++)
		{
			if (root->childs.at(i)->type == TYPE_NOTEBOOK && root->childs.at(i)->id == notebookGuid)
			{
				notebook = root->childs.at(i);
				break;
			}
		}
	}

	if (!notebook)
		return false;

	NoteItem* note = new NoteItem;
	note->name = title;
	note->id = guid;
	note->type = TYPE_NOTE;
	note->parent = notebook;
	note->mtime;
	notebook->childs.append(note);

	return flushNote(guid, content);
}

bool WorkbenchManager::updateNote(QString noteGuid, QString content)
{
	return flushNote(noteGuid, content);
}

bool WorkbenchManager::flushNote(QString guid, QString content)
{
	if (!inited)
		return false;
	QString fpath = getNoteFilePath(guid);
	QFile file(fpath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "ERROR: open file failed: " << "theme.css";
		return false;
	}

	std::string strContent = content.toStdString();
	file.write(strContent.c_str());
	file.close();

	return true;
}


QString WorkbenchManager::getNoteFilePath(QString guid)
{
	if (!inited)
		return "";

	return path + "/" + guid + ".md";
}
