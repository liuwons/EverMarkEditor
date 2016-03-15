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
}

bool EvernoteManager::init()
{
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
		qDebug() << "[ERROR] Python get module failed.";
		return false;
	}
	qDebug() << "[INFO] Python get module succeed.";

	handleInit = PyObject_GetAttrString(pModule, "init");
	if (!handleInit || !PyCallable_Check(handleInit)) 
	{
		qDebug() << "[ERROR] Can't find funftion (init)";
		return false;
	}
	qDebug() << "[INFO] Get function (init) succeed.";

	handleLogin = PyObject_GetAttrString(pModule, "login");
	if (!handleLogin || !PyCallable_Check(handleLogin))
	{
		qDebug() << "[ERROR] Can't find funftion (login)";
		return false;
	}
	qDebug() << "[INFO] Get function (login) succeed.";

	handleGetNotebookStatus = PyObject_GetAttrString(pModule, "get_notebook_status");
	if (!handleGetNotebookStatus || !PyCallable_Check(handleGetNotebookStatus))
	{
		qDebug() << "[ERROR] Can't find funftion (get_notebook_status)";
		return false;
	}
	qDebug() << "[INFO] Get function (get_notebook_status) succeed.";

	handleGetNoteStatus = PyObject_GetAttrString(pModule, "get_note_status");
	if (!handleGetNoteStatus || !PyCallable_Check(handleGetNoteStatus))
	{
		qDebug() << "[ERROR] Can't find funftion (get_note_status)";
		return false;
	}
	qDebug() << "[INFO] Get function (get_note_status) succeed.";

	handleCreateNotebook = PyObject_GetAttrString(pModule, "create_notebook");
	if (!handleCreateNotebook || !PyCallable_Check(handleCreateNotebook))
	{
		qDebug() << "[ERROR] Can't find funftion (create_notebook)";
		return false;
	}
	qDebug() << "[INFO] Get function (create_notebook) succeed.";

	handleCreateNote = PyObject_GetAttrString(pModule, "create_note");
	if (!handleCreateNote || !PyCallable_Check(handleCreateNote))
	{
		qDebug() << "[ERROR] Can't find funftion (create_note)";
		return false;
	}
	qDebug() << "[INFO] Get function (create_note) succeed.";

	handleUpdateNote = PyObject_GetAttrString(pModule, "update_note");
	if (!handleUpdateNote || !PyCallable_Check(handleUpdateNote))
	{
		qDebug() << "[ERROR] Can't find funftion (update_note)";
		return false;
	}
	qDebug() << "[INFO] Get function (update_note) succeed.";

	std::string str_auth = this->authToken.toStdString();
	std::string str_type = this->accountType.toStdString();
	std::string str_style = this->style.toStdString();
	std::string str_root = this->rootPath.toStdString();

	PyObject* args = Py_BuildValue("ssss", str_type.c_str(), str_auth.c_str(), str_style.c_str(), str_root.c_str());
	PyObject* pRet = PyObject_CallObject(handleInit, args);
	if (pRet)
	{
		long result = PyInt_AsLong(pRet);
		if (result == 0)
			return true;
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
			return true;
	}
	return false;
}

QMap<QString, QString>* EvernoteManager::getNotebookStatus()
{

	PyObject* args = Py_BuildValue("()");
	PyObject* pRet = PyObject_CallObject(handleGetNotebookStatus, args);
	if (pRet)
	{
		char* result = PyString_AsString(pRet);
		if (result)
		{
			QMap<QString, QString>* map = new QMap<QString, QString>();

			QJsonParseError err;
			QJsonDocument doc = QJsonDocument::fromJson(result, &err);
			QJsonObject obj = doc.object();
			QStringList keys = obj.keys();
			for (int i = 0; i < keys.size(); i++)
			{
				QString guid = keys.at(i);
				QString name = obj.value(guid).toString();
				(*map)[guid] = name;
			}
			qDebug() << "[INFO] Get notebook status succeed, notebook count: " << map->size();
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
	qDebug() << "[INFO] Get all notes in " << result->size() << " notebooks succeed.";
	return result;
}

bool EvernoteManager::createNotebook(QString name)
{
	std::string str_name = name.toStdString();

	PyObject* args = Py_BuildValue("s", str_name.c_str());
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

	PyObject* args = Py_BuildValue("sssi", str_notebook_guid.c_str(), str_title.c_str(), str_content.c_str(), is_markdown);
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

	PyObject* args = Py_BuildValue("ssssi", str_notebook_guid.c_str(), str_guid.c_str(), str_title.c_str(), str_content.c_str(), is_markdown);
	PyObject* pRet = PyObject_CallObject(handleUpdateNote, args);
	if (pRet)
	{
		long ret = PyInt_AsLong(pRet);
		if (ret == 0)
			return true;
	}
	return false;
}
