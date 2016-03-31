#include "noteitem.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

NoteItem::NoteItem()
{
	parent = 0;
}

NoteItem::~NoteItem()
{
	for (int i = 0; i < childs.size(); i++)
	{
		delete childs.at(i);
	}
}


QJsonObject NoteItem::toJsonObject()
{
	QJsonObject root;
	root[NOTE_KEY_NAME_NAME] = name;
	root[NOTE_KEY_NAME_ID] = id;
	root[NOTE_KEY_NAME_MTIME] = mtime;
	root[NOTE_KEY_NAME_TYPE] = type;

	if (childs.length() != 0)
	{
		QJsonArray arr;
		for (int i = 0; i < childs.length(); i++)
		{
			arr.append(childs.at(i)->toJsonObject());
		}
		root[NOTE_KEY_NAME_CHILD] = arr;
	}
	return root;
}

NoteItem* NoteItem::fromJsonObject(QJsonObject obj)
{
	qDebug() << "[DEBUG] NoteItem::fromJsonObject start";
	NoteItem* item = new NoteItem;
	item->id = obj.value(NOTE_KEY_NAME_ID).toString();
	item->type = static_cast<NoteType>(obj.value(NOTE_KEY_NAME_TYPE).toInt());
	item->name = obj.value(NOTE_KEY_NAME_NAME).toString();
	item->mtime = obj.value(NOTE_KEY_NAME_MTIME).toString();

	QJsonArray arr = obj.value(NOTE_KEY_NAME_CHILD).toArray();
	for (int i = 0; i < arr.size(); i++)
	{
		QJsonObject objChild = arr.at(i).toObject();
		NoteItem* itemChild = fromJsonObject(objChild);
		item->childs.append(itemChild);
		itemChild->parent = item;
	}

	qDebug() << "[DEBUG] NoteItem::fromJsonObject end";
	return item;
}

QString NoteItem::toJsonString()
{
	QJsonObject obj = toJsonObject();
	QJsonDocument doc(obj);
	return doc.toJson();
}


NoteItem* NoteItem::fromJsonString(QString json)
{
	std::string strJson = json.toStdString();
	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(strJson.c_str(), &err);

	QJsonObject obj = doc.object();
	return fromJsonObject(obj);
}

NoteItem* NoteItem::fromEvernoteStatus(QMap<QString, QMap<QString, QString> >* notebookStatus, QMap<QString, QMap<QString, NoteStatus> >* noteStatus)
{
	NoteItem* root = new NoteItem;
	root->type = TYPE_ROOT;
	root->name = QObject::tr("Evernote");

	QList<QString> notebookGuids = notebookStatus->keys();
	for (int i = 0; i < notebookGuids.size(); i++)
	{
		QString notebookGuid = notebookGuids[i];
		QMap<QString, QString> nb = (*notebookStatus)[notebookGuid];

		NoteItem* notebook = new NoteItem;
		notebook->id = notebookGuid;
		notebook->name = nb[NOTE_KEY_NAME_NAME];
		notebook->type = TYPE_NOTEBOOK;

		QString strStack = nb[NOTE_TYPE_NAMES[TYPE_STACK]];
		if (strStack.length() == 0)
		{
			root->childs.append(notebook);
			notebook->parent = root;
		}
		else
		{
			int idx = -1;
			for (int i = 0; i < root->childs.size(); i++)
			{
				if (strStack.compare(root->childs.at(i)->name) == 0)
				{
					idx = i;
					break;
				}
			}

			if (idx >= 0 && root->childs.at(idx)->type == TYPE_STACK)
			{
				root->childs.at(idx)->childs.append(notebook);
				notebook->parent = root->childs.at(idx);
			}
			else
			{
				NoteItem* stack = new NoteItem;
				stack->name = strStack;
				stack->type = TYPE_STACK;
				stack->childs.append(notebook);
				stack->parent = root;
				root->childs.append(stack);
				notebook->parent = stack;
			}
		}

		QMap<QString, NoteStatus> notes = (*noteStatus)[notebookGuid];
		QList<QString> guids = notes.keys();
		for (int k = 0; k < guids.size(); k++)
		{
			QString guid = guids.at(k);
			NoteStatus stat = notes[guid];
			NoteItem* note = new NoteItem;
			note->id = guid;
			note->name = stat.title;
			note->mtime = stat.modify;
			note->type = TYPE_NOTE;
			note->parent = notebook;

			notebook->childs.append(note);
		}
	}

	return root;
}
