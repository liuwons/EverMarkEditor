#ifndef NOTE_ITEM_H
#define NOTE_ITEM_H

#include <QString>
#include <QList>
#include <QVariant>

enum NoteType
{
	TYPE_STACK,
	TYPE_NOTEBOOK,
	TYPE_NOTE
};

class NoteItem
{
public:
	QString id;
	NoteType type;
	QString name;
	QString mtime;
	NoteItem* parent;
	QList<NoteItem*> childs;

	int row();
	int indexOf(NoteItem*);
	int columnCount();
	QVariant  NoteItem::data(int) const;
	NoteItem* child(int);
	int getChildNamed(QString) const;
};

#endif