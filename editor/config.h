#ifndef CONFIG_H
#define CONFIG_H

#include <QSize>
#include <QString>
#include <QSettings>

#include "awesome/QtAwesome.h"

extern QString CONFIG_STRING_WORKBENCH_PATH;
extern QString CONFIG_STRING_EVERNOTE_TOKEN;
extern QString CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE;
extern QString CONFIG_STRING_MARKDOWN_THEME;
extern QString WORKBENCH_CONFIG_FILE_NAME;

extern QSize screenSize;
extern QString theme;
extern QString appDir;
extern QString cssSubDir;

extern QtAwesome* awesome;

extern QSettings* settings;

extern QString* workbenchPath;
extern QString* evernoteToken;
extern QString* evernoteAccountType;

extern QString* markdownTheme;
#endif // CONFIG_H

