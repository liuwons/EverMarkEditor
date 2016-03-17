#include "notemodel.h"

#include <QStringList>
#include <QDebug>


#include "editor/config.h"

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
	return 2;
	if (type == TYPE_NOTE)
		return 2;  // name, mtime
	else
		return 1;  // name
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

NoteModel::NoteModel(QObject *parent) : QAbstractItemModel(parent)
{
}

NoteModel::~NoteModel()
{
}

int NoteModel::columnCount(const QModelIndex &parent) const
{
	int count = 0;
	if (parent.isValid())
		count = static_cast<NoteItem*>(parent.internalPointer())->columnCount();
	else
		count = root->columnCount();
	return count;
}

QVariant NoteModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	NoteItem *item = static_cast<NoteItem*>(index.internalPointer());

	if (role == Qt::DecorationRole)
	{
		if (index.column() != 0)
			return QVariant();
		if (item->type == TYPE_NOTE)
			return awesome->icon(fa::file);
		else if (item->type == TYPE_NOTEBOOK)
			return awesome->icon(fa::book);
	}

	if (role != Qt::DisplayRole)
		return QVariant();
	
	QVariant var = item->data(index.column());
	return  var;
}

Qt::ItemFlags NoteModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant NoteModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	QVariant var;
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		var = header.at(section);
	}
	return var;
}

QModelIndex NoteModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	NoteItem *parentItem;

	if (!parent.isValid())
		parentItem = root;
	else
		parentItem = static_cast<NoteItem*>(parent.internalPointer());

	NoteItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex NoteModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	NoteItem *childItem = static_cast<NoteItem*>(index.internalPointer());
	NoteItem *parentItem = childItem->parent;

	if (parentItem == root)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int NoteModel::rowCount(const QModelIndex &parent) const
{
	NoteItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = root;
	else
		parentItem = static_cast<NoteItem*>(parent.internalPointer());

	return parentItem->childs.size();
}

bool NoteModel::loadFromEvernoteManager(EvernoteManager* manager)
{
	if (!manager)
		return false;

	QMap<QString, QString>* notebookStatus = manager->getNotebookStatus();
	if (!notebookStatus)
		return false;

	QMap<QString, QMap<QString, NoteStatus> >* noteStatus = manager->getNoteStatus();
	if (!noteStatus)
		return false;

	header.append(QString("Evernote"));
	header.append(QString("Modify Time"));

	root = new NoteItem;
	root->type = TYPE_NOTEBOOK;
	root->name = "My Account";

	QList<QString> notebookGuids = notebookStatus->keys();
	for (int i = 0; i < notebookGuids.size(); i++)
	{
		QString notebookGuid = notebookGuids[i];
		QString notebookName = (*notebookStatus)[notebookGuid];

		NoteItem* notebook = new NoteItem;
		notebook->id = notebookGuid;
		notebook->name = notebookName;
		notebook->type = TYPE_NOTEBOOK;
		notebook->parent = root;
		root->childs.append(notebook);

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
	return true;
}
