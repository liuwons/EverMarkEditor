#ifndef EM_FILE_SYSTEM_MODEL_H
#define EM_FILE_SYSTEM_MODEL_H

#include <QFileSystemModel>

class EMFileSystemModel : public QFileSystemModel
{
	QVariant data(const QModelIndex& index, int role) const;
};


#endif