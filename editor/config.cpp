#include <QSize>
#include <QString>
#include <QSettings>

#include "awesome/QtAwesome.h"

QSize screenSize;
QString theme = "github";
QString appDir;
QString cssSubDir = "css";

QtAwesome* awesome = 0;

QSettings* settings = 0;

QString* workbenchPath = 0;
QString* evernoteToken = 0;
QString* evernoteAccountType;

QString* markdownTheme;
