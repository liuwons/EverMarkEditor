#ifndef CONFIG_H
#define CONFIG_H

#include <QSize>
#include <QString>
#include <QSettings>

#include "awesome/QtAwesome.h"


const QString CONFIG_STRING_WORKBENCH_PATH = "workbench_path";
const QString CONFIG_STRING_EVERNOTE_TOKEN = "evernote_token";

extern QSize screenSize;
extern QString theme;
extern QString appDir;
extern QString cssSubDir;

extern QtAwesome* awesome;

extern QSettings* settings;

extern QString* workbenchPath;
extern QString* evernoteToken;
#endif // CONFIG_H

