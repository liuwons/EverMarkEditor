#include "emfilesystemmodel.h"

#include "config.h"

#include "tool/appcontext.h"
	
QVariant EMFileSystemModel::data(const QModelIndex& index, int role) const
{
	AppContext* context = AppContext::getContext();

	if (role == Qt::DecorationRole)
	{
		QFileInfo info = QFileSystemModel::fileInfo(index);

		if (info.isFile())
		{
			if (info.suffix() == "md")
				return context->awesome->icon(fa::filetext);
			else
				return context->awesome->icon(fa::file);
		}
		else if (info.isDir())
		{
			return context->awesome->icon(fa::folder);
		}
	}
	return QFileSystemModel::data(index, role);
}