#include "noteitem.h"

int NoteItem::row()
{
	if (parent)
		return parent->indexOf(const_cast<NoteItem*>(this));

	return 0;
}

int NoteItem::indexOf(NoteItem* item)
{
	for (int i = 0; i < childs.size(); i++)
	{
		if (item == childs.at(i))
			return i;
	}
	return -1;
}

int NoteItem::columnCount()
{
	return 1;
}


int NoteItem::getChildNamed(QString strName) const
{
	for (int i = 0; i < childs.size(); i++)
	{
		if (strName.compare(childs.at(i)->name) == 0)
			return i;
	}
	return -1;
}

NoteItem* NoteItem::child(int idx)
{
	return childs.at(idx);
}

QVariant  NoteItem::data(int col) const
{
	if (col == 0)
		return name;
	else if (col == 1 && type == TYPE_NOTE)
		return mtime;
	else
		return QVariant();
}