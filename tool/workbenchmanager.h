#ifndef WORKBENCH_MANAGER_H
#define WORKBENCH_MANAGER_H

#include "noteitem.h"

#include <QString>

class WorkbenchManager
{
public:
	WorkbenchManager();
	bool init(QString);		// Init from a path, load dump file if there is one, or init an empty hierarchy 
	bool dump();	// Dump workbench information to a file in the workbench directory

private:
	QString path;
	NoteItem* root;		// The note hierarchy in the workbench
};


#endif