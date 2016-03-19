#include "workbenchmanager.h"

WorkbenchManager::WorkbenchManager()
{

}

bool WorkbenchManager::init(QString workbenchPath)
{

	path = workbenchPath;
	return true;
}


bool WorkbenchManager::dump()
{
	return true;
}