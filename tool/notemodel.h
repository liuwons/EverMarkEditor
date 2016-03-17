#ifndef EVERNOTE_MODEL_H
#define EVERNOTE_MODEL_H

#include "evernotemanager.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>


enum NoteType
{
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
};

class NoteModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit NoteModel(QObject *parent = 0);
	~NoteModel();

	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

	bool loadFromEvernoteManager(EvernoteManager* manager);

private:
	NoteItem* root;
	QList<QVariant> header;
};

#endif // TREEMODEL_H