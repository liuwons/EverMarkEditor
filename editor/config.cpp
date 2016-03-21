#include <QSize>
#include <QString>
#include <QSettings>

#include "awesome/QtAwesome.h"

QString CONFIG_STRING_WORKBENCH_PATH = "workbench_path";
QString CONFIG_STRING_EVERNOTE_TOKEN = "evernote_token";
QString CONFIG_STRING_EVERNOTE_ACCOUNT_TYPE = "evernote_account_type";
QString CONFIG_STRING_MARKDOWN_THEME = "markdown_theme";
QString WORKBENCH_CONFIG_FILE_NAME = ".emworkbench";

QSize screenSize;
QString theme = "github";
QString appDir;
QString cssSubDir = "css";

QtAwesome* awesome = 0;

QSettings* settings = 0;

QString* workbenchPath = 0;
QString* evernoteToken = 0;
QString* evernoteAccountType = 0;

QString* markdownTheme = 0;
