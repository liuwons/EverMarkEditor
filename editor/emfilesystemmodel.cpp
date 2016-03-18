#include "emfilesystemmodel.h"

#include "config.h"
	
QVariant EMFileSystemModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DecorationRole)
	{
		QFileInfo info = QFileSystemModel::fileInfo(index);

		if (info.isFile())
		{
			if (info.suffix() == "md")
				return awesome->icon(fa::filetext);
			else
				return awesome->icon(fa::file);
		}
		else if (info.isDir())
		{
			return awesome->icon(fa::folder);
		}
	}
	return QFileSystemModel::data(index, role);
}