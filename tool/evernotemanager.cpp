#include "evernotemanager.h"

#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

EvernoteManager::EvernoteManager(QString auth, QString type, QString style, QString root, QString execPath, QString execName)
{
	this->authToken = auth;
	this->accountType = type;
	this->style = style;
	this->rootPath = root;
	this->executablePath = execPath;
	this->executableName = execName;
	this->is_logined = false;
}

bool EvernoteManager::init()
{
	qDebug() << "EvernoteManager type(" << accountType << "), style(" << style << ")";

	Py_Initialize();

	QString chdirCmd = QString("sys.path.append('") + this->executablePath + "')";
	qDebug() << chdirCmd;
	std::string chdir_cmd = chdirCmd.toStdString();
	const char* cstr_cmd = chdir_cmd.c_str();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString(cstr_cmd);

	qDebug() << this->executableName;
	std::string exec_name = this->executableName.toStdString();
	const char* cstr_name = exec_name.c_str();
	PyObject* moduleName = PyString_FromString(cstr_name);
	PyObject* pModule = PyImport_Import(moduleName);
	if (!pModule) 
	{
		qWarning() << "EvernoteManager import python module failed";
		return false;
	}

	handleInit = PyObject_GetAttrString(pModule, "init");
	if (!handleInit || !PyCallable_Check(handleInit)) 
	{
		qWarning() << "EvernoteManager get python function init failed";
		return false;
	}

	handleLogin = PyObject_GetAttrString(pModule, "login");
	if (!handleLogin || !PyCallable_Check(handleLogin))
	{
		qWarning() << "EvernoteManager get python function login failed";
		return false;
	}

	handleGetNotebookStatus = PyObject_GetAttrString(pModule, "get_notebook_status");
	if (!handleGetNotebookStatus || !PyCallable_Check(handleGetNotebookStatus))
	{
		qWarning() << "EvernoteManager get python function get_notebook_status failed";
		return false;
	}

	handleGetNoteStatus = PyObject_GetAttrString(pModule, "get_note_status");
	if (!handleGetNoteStatus || !PyCallable_Check(handleGetNoteStatus))
	{
		qWarning() << "EvernoteManager get python function get_note_status failed";
		return false;
	}

	handleCreateNotebook = PyObject_GetAttrString(pModule, "create_notebook");
	if (!handleCreateNotebook || !PyCallable_Check(handleCreateNotebook))
	{
		qWarning() << "EvernoteManager get python function create_notebook failed";
		return false;
	}

	handleCreateNote = PyObject_GetAttrString(pModule, "create_note");
	if (!handleCreateNote || !PyCallable_Check(handleCreateNote))
	{
		qWarning() << "EvernoteManager get python function create_note failed";
		return false;
	}

	handleUpdateNote = PyObject_GetAttrString(pModule, "update_note");
	if (!handleUpdateNote || !PyCallable_Check(handleUpdateNote))
	{
		qWarning() << "EvernoteManager get python function update_note failed";
		return false;
	}

	handleGetNote = PyObject_GetAttrString(pModule, "get_note");
	if (!handleGetNote || !PyCallable_Check(handleGetNote))
	{
		qWarning() << "EvernoteManager get python function get_note failed";
		return false;
	}

	std::string str_auth = this->authToken.toStdString();
	std::string str_type = this->accountType.toStdString();
	std::string str_style = this->style.toStdString();
	std::string str_root = this->rootPath.toStdString();

	PyObject* args = Py_BuildValue("(ssss)", str_type.c_str(), str_auth.c_str(), str_style.c_str(), str_root.c_str());
	PyObject* pRet = PyObject_CallObject(handleInit, args);
	if (pRet)
	{
		long result = PyInt_AsLong(pRet);
		if (result == 0)
		{
			qInfo() << "EvernoteManager init succeed";
			return true;
		}
	}

	return false;
}

bool EvernoteManager::login()
{
	PyObject* args = Py_BuildValue("()");
	PyObject* pRet = PyObject_CallObject(handleLogin, args);
	if (pRet)
	{
		long result = PyInt_AsLong(pRet);
		if (result == 0)
		{
			is_logined = true;
			return true;
		}
	}
	return false;
}

bool EvernoteManager::logined()
{
	return is_logined;
}

QMap<QString, QMap<QString, QString> >* EvernoteManager::getNotebookStatus()
{

	PyObject* args = Py_BuildValue("()");
	PyObject* pRet = PyObject_CallObject(handleGetNotebookStatus, args);
	if (pRet)
	{
		char* result = PyString_AsString(pRet);
		if (result)
		{
			QMap<QString, QMap<QString, QString> >* map = new QMap<QString, QMap<QString, QString> >();

			QJsonParseError err;
			QJsonDocument doc = QJsonDocument::fromJson(result, &err);
			QJsonObject obj = doc.object();
			QStringList keys = obj.keys();
			for (int i = 0; i < keys.size(); i++)
			{
				QString guid = keys.at(i);
				QJsonObject notebook = obj.value(guid).toObject();
				QString name = notebook.value("name").toString();
				QString stack = notebook.value("stack").toString();
				QMap<QString, QString> nb;
				nb["name"] = name;
				nb["stack"] = stack;
				(*map)[guid] = nb;
			}
			return map;
		}
	}
	return 0;
}

QMap<QString, QMap<QString, NoteStatus> >* EvernoteManager::getNoteStatus()
{
	QMap<QString, QMap<QString, NoteStatus> >* result = 0;

	PyObject* args = Py_BuildValue("()");
	PyObject* pRet = PyObject_CallObject(handleGetNoteStatus, args);
	if (pRet)
	{
		char* ret = PyString_AsString(pRet);
		if (ret)
		{
			result = new QMap<QString, QMap<QString, NoteStatus> >();
			QJsonParseError err;
			QJsonDocument doc = QJsonDocument::fromJson(ret, &err);
			QJsonObject obj = doc.object();
			QStringList keys = obj.keys();
			for (int i = 0; i < keys.size(); i++)
			{
				QMap<QString, NoteStatus> stats;
				QString notebookGuid = keys.at(i);
				QJsonArray arr = obj.value(notebookGuid).toArray();
				for (int k = 0; k < arr.size(); k++)
				{
					NoteStatus stat;
					QJsonObject note = arr.at(k).toObject();
					stat.guid = note.value("guid").toString();
					stat.title = note.value("title").toString();
					stat.modify = note.value("modify_time").toString();
					stats[stat.guid] = stat;
				}
				(*result)[notebookGuid] = stats;
			}
		}
	}
	return result;
}

bool EvernoteManager::createNotebook(QString name)
{
	std::string str_name = name.toStdString();

	PyObject* args = Py_BuildValue("(s)", str_name.c_str());
	PyObject* pRet = PyObject_CallObject(handleCreateNotebook, args);
	if (pRet)
	{
		long ret = PyInt_AsLong(pRet);
		if (ret == 0)
			return true;
	}
	return false;
}

bool EvernoteManager::createNote(QString notebookGuid, QString title, QString content, bool isMarkdown)
{
	std::string str_notebook_guid = notebookGuid.toStdString();
	std::string str_title = title.toStdString();
	std::string str_content = content.toStdString();
	int is_markdown = isMarkdown ? 1 : 0;

	PyObject* args = Py_BuildValue("(sssi)", str_notebook_guid.c_str(), str_title.c_str(), str_content.c_str(), is_markdown);
	PyObject* pRet = PyObject_CallObject(handleCreateNote, args);
	if (pRet)
	{
		long ret = PyInt_AsLong(pRet);
		if (ret == 0)
			return true;
	}
	return false;
}


bool EvernoteManager::updateNote(QString notebookGuid, QString guid, QString title, QString content, bool isMarkdown)
{
	std::string str_notebook_guid = notebookGuid.toStdString();
	std::string str_guid = guid.toStdString();
	std::string str_title = title.toStdString();
	std::string str_content = content.toStdString();
	int is_markdown = isMarkdown ? 1 : 0;

	PyObject* args = Py_BuildValue("(ssssi)", str_notebook_guid.c_str(), str_guid.c_str(), str_title.c_str(), str_content.c_str(), is_markdown);
	PyObject* pRet = PyObject_CallObject(handleUpdateNote, args);
	if (pRet)
	{
		long ret = PyInt_AsLong(pRet);
		if (ret == 0)
			return true;
	}
	return false;
}

QString EvernoteManager::getNote(QString noteGuid)
{
	std::string str_note_guid = noteGuid.toStdString();
	PyObject* args = Py_BuildValue("(s)", str_note_guid.c_str());
	PyObject* pRet = PyObject_CallObject(handleGetNote, args);
	if (pRet)
	{
		char* str = PyString_AsString(pRet);
		qDebug() << "getNote(" << noteGuid << "): " << str;
		return QString(str);
	}
	else
	{
		qWarning() << "getNote(" << noteGuid << ") failed";
	}

	return QString("");
}
