#include "notemodel.h"

#include <QStringList>
#include <QDebug>


#include "editor/config.h"
#include "tool/appcontext.h"


NoteModel::NoteModel(QObject *parent) : QAbstractItemModel(parent)
{
}

NoteModel::~NoteModel()
{
}

int NoteModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant NoteModel::data(const QModelIndex &index, int role) const
{
	AppContext* context = AppContext::getContext();

	if (!index.isValid())
		return QVariant();

	NoteItem *item = static_cast<NoteItem*>(index.internalPointer());

	if (role == Qt::DecorationRole)
	{
		if (index.column() != 0)
			return QVariant();
		if (item->type == TYPE_NOTE)
			return context->awesome->icon(fa::filecodeo);
		else if (item->type == TYPE_NOTEBOOK)
			return context->awesome->icon(fa::book);
		else if (item->type == TYPE_STACK)
			return context->awesome->icon(fa::folder);
		else if (item->type == TYPE_ROOT)
			return context->awesome->icon(fa::home);
	}
	else if (role == Qt::DisplayRole)
	{
		int col = index.column();
		if (col == 0)
			return item->name;
		else if (col == 1 && item->type == TYPE_NOTE)
			return item->mtime;
		else
			return QVariant();
	}

	return QVariant();
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
	if (row < 0 || column < 0)
		return QModelIndex();

	if (!parent.isValid())
	{
		return createIndex(row, column, root);
	}
	else
	{
		NoteItem *parentItem;
		parentItem = static_cast<NoteItem*>(parent.internalPointer());
		NoteItem *childItem = parentItem->childs.at(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}
}

QModelIndex NoteModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	NoteItem *childItem = static_cast<NoteItem*>(index.internalPointer());
	NoteItem *parentItem = childItem->parent;

	if (parentItem == 0)
		return QModelIndex();

	int row = -1;
	for (int i = 0; i < parentItem->childs.size(); i++)
	{
		if (childItem == parentItem->childs.at(i))
		{
			row = i;
			break;
		}
	}
	if (row >= 0)
		return createIndex(row, 0, parentItem);
	else
		return QModelIndex();
}

int NoteModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return 1;
	else
	{
		NoteItem *parentItem;
		parentItem = static_cast<NoteItem*>(parent.internalPointer());
		return parentItem->childs.size();
	}
}

bool NoteModel::loadFromEvernoteManager(EvernoteManager* manager)
{
	if (!manager)
		return false;

	QMap<QString, QMap<QString, QString> >* notebookStatus = manager->getNotebookStatus();
	if (!notebookStatus)
		return false;

	QMap<QString, QMap<QString, NoteStatus> >* noteStatus = manager->getNoteStatus();
	if (!noteStatus)
		return false;

	header.append(tr("Evernote"));

	root = NoteItem::fromEvernoteStatus(notebookStatus, noteStatus);
	
	return true;
}

bool NoteModel::loadFromWorkbenchManager(WorkbenchManager* manager)
{
	if (!manager)
		return false;

	QString strJson = manager->toString();
	qDebug() << "loadFromWorkbenchManager json: " << strJson;
	header.append(tr("Workbench"));
	root = NoteItem::fromJsonString(strJson);
	return true;
}