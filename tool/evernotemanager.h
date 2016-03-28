#ifndef EVERNOTE_MANAGER_H
#define EVERNOTE_MANAGER_H

#include "python/Python.h"

#include <QString>
#include <QMap>

class NoteStatus
{
public:
	QString guid;
	QString title;
	QString modify;
};


class EvernoteManager
{
public:
	EvernoteManager(QString auth, QString type, QString style, QString root, QString execPath, QString execName);
	bool init();
	bool login();
	QMap<QString, QMap<QString, QString> >* getNotebookStatus();
	QMap<QString, QMap<QString, NoteStatus> >* getNoteStatus();
	bool createNotebook(QString name);
	bool createNote(QString notebookGuid, QString title, QString content, bool isMarkdown);
	bool updateNote(QString notebookGuid, QString guid, QString title, QString content, bool isMarkdown);
	QString getNote(QString noteGuid);

private:
	QString authToken;
	QString accountType;
	QString style;
	QString rootPath;
	QString executablePath;
	QString executableName;

	PyObject* handleInit;
	PyObject* handleLogin;
	PyObject* handleGetNotebookStatus;
	PyObject* handleGetNoteStatus;
	PyObject* handleCreateNotebook;
	PyObject* handleCreateNote;
	PyObject* handleUpdateNote;
	PyObject* handleGetNote;
};


#endif