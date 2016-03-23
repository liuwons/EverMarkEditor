#ifndef WORKBENCH_MANAGER_H
#define WORKBENCH_MANAGER_H

#include "noteitem.h"

#include <QString>

class WorkbenchManager
{
public:
	WorkbenchManager();
	~WorkbenchManager();
	bool init(QString);		
	bool dump();
	bool createNote(QString stack, QString notebookGuid, QString guid, QString title, QString content);
	bool updateNote(QString noteGuid, QString content);

private:
	bool flushNote(QString guid, QString content);
	QString getNoteFilePath(QString guid);

	bool inited;
	QString path;
	NoteItem* root;		// The note hierarchy in the workbench
};


#endif