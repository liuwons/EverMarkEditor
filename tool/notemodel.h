#ifndef EVERNOTE_MODEL_H
#define EVERNOTE_MODEL_H

#include "evernotemanager.h"
#include "workbenchmanager.h"
#include "noteitem.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

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
	bool loadFromWorkbenchManager(WorkbenchManager* manager);

private:
	NoteItem* root;
	QList<QVariant> header;
};

#endif // TREEMODEL_H