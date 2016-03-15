#include "notemodel.h"

#include <QStringList>
#include <QDebug>

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

	if (role != Qt::DisplayRole)
		return QVariant();

	NoteItem *item = static_cast<NoteItem*>(index.internalPointer());
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
	/*header.append(QString("Evernote"));
	header.append(QString("Modify Time"));
	root = new NoteItem;
	root->type = TYPE_NOTEBOOK;
	root->name = "My Account";
	for (int i = 0; i < 5; i++)
	{
		QString notebookGuid = "123";
		QString notebookName = "test";

		NoteItem* notebook = new NoteItem;
		notebook->id = notebookGuid;
		notebook->name = notebookName;
		notebook->type = TYPE_NOTEBOOK;
		notebook->parent = root;
		root->childs.append(notebook);

		for (int k = 0; k < 3; k++)
		{
			QString guid = "12345678";
			NoteItem* note = new NoteItem;
			note->id = guid;
			note->name = "test title";
			note->mtime = "2012-12-21 12:00:00";
			note->type = TYPE_NOTE;
			note->parent = notebook;

			notebook->childs.append(note);
		}
	}
	return true;*/

	//---------------test---------------s
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
