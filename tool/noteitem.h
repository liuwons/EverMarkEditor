#ifndef NOTE_ITEM_H
#define NOTE_ITEM_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QJsonObject>

#include "evernotemanager.h"

enum NoteType
{
	TYPE_ROOT = 0,
	TYPE_STACK = 1,
	TYPE_NOTEBOOK = 2,
	TYPE_NOTE = 3
};

const QString NOTE_KEY_NAME_ID = "id";
const QString NOTE_KEY_NAME_NAME = "name";
const QString NOTE_KEY_NAME_TYPE = "type";
const QString NOTE_KEY_NAME_MTIME = "modify_time";
const QString NOTE_KEY_NAME_CHILD = "child";

const QString NOTE_TYPE_NAMES[] = {"root", "stack", "notebook", "note"};

class NoteItem
{
public:
	QString id;
	NoteType type;
	QString name;
	QString mtime;
	NoteItem* parent;
	QList<NoteItem*> childs;

	NoteItem();
	~NoteItem();

	QString toJsonString();
	QJsonObject toJsonObject();
	static NoteItem* fromJsonString(QString);
	static NoteItem* fromJsonObject(QJsonObject);
	static NoteItem* fromEvernoteStatus(QMap<QString, QMap<QString, QString> >* notebookStatus, QMap<QString, QMap<QString, NoteStatus> >* noteStatus);
};

#endif