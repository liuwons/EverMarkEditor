#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "preview.h"
#include "editor.h"
#include "tool/evernotemanager.h"
#include "tool/notemodel.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QToolButton>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPoint>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void loadFile(QString fpath);
	void loadNote(QString guid);
	void evernoteContextMenuRequest(const QPoint& point);
	void evernoteContextUpload();
	void evernoteContextDownload();
	void evernoteContextOpen();
	void evernoteContextAdd();
	void evernoteContextRefresh();
	void evernoteContextDelete();

	void workbenchContextMenuRequest(const QPoint& point);
	void workbenchContextAdd();
	void workbenchContextAddStack();
	void workbenchContextDelete();
	void workbenchContextOpen();

	// From Menu Bar
	void openFileFromMenu();
	void openDirFromMenu();
	void saveFromMenu();
	void saveAsFromMenu();
	void closeFromMenu();
	void previewView();
	void editorView();
	void dockedWindow();
	void about();
	void help();
	void setting();
	void close();

	// From Tool Bar
	void previewNow();
	void openFileFromLocalNavigation(const QModelIndex & index);
	void openNoteFromEvernoteNavigation(const QModelIndex& index);
	void openNoteFromWorkbenchNavigation(const QModelIndex& index);

	void updateEvernoteNavigation();
	void updateWorkbenchNavigation();

protected:
	void closeEvent(QCloseEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

private:
	void createUI();
	void createMenu();
	void createToolBar();
	void createStatusBar();
	void createNavigation();

	NoteModel* evernoteModel;
	QTreeView* evernoteTree;
	QFileSystemModel* localFileModel;
	QTreeView* localFileTree;
	NoteModel* workbenchModel;
	QTreeView* workbenchTree;
	QString currentFilePath;

    QDockWidget* dockPreview;
    QDockWidget* dockEditor;
	Preview* preview;
	Editor* editor;

	QDockWidget* dockNavigation;
	QDockWidget* dockLocalNavigation;
	QDockWidget* dockRemoteNavigation;

	//---------------------Evernote Context Menu---------------------
	QMenu* evernoteContextMenuStack;
	QAction* evernoteStackRefreshAction;
	QAction* evernoteStackAddAction;

	QMenu* evernoteContextMenuNotebook;
	QAction* evernoteNotebookRefreshAction;
	QAction* evernoteNotebookAddAction;

	QMenu* evernoteContextMenuNote;
	QAction* evernoteNoteUploadAction;
	QAction* evernoteNoteDownloadAction;
	QAction* evernoteNoteOpenAction;
	QAction* evernoteNoteDeleteAction;
	//---------------------Evernote Context Menu-----------------------



	//---------------------Workbench Context Menu----------------------
	QMenu* workbenchContextMenuRoot;
	QAction* workbenchRootAddStackAction;
	QAction* workbenchRootAddNotebookAction;

	QMenu* workbenchContextMenuStack;
	QAction* workbenchStackAddAction;
	
	QMenu* workbenchContextMenuNotebook;
	QAction* workbenchNotebookAddAction;
	QAction* workbenchNotebookDeleteAction;

	QMenu* workbenchContextMenuNote;
	QAction* workbenchNoteOpenAction;
	QAction* workbenchNoteDeleteAction;
	//---------------------Workbench Context Menu----------------------



	// ----------------------Menu Bar-------------------------
	QMenuBar*  menubar;

	QMenu* fileMenu;
	QAction* openFileAct;
	QAction* openDirAct;
	QAction* importAct;
	QAction* saveAct;
	QAction* saveAsAct;
	QAction* exitAct;

	QMenu* helpMenu;
	QAction* aboutAct;
	QAction* helpAct;

	QMenu* viewMenu;
	QAction* previewViewAct;
	QAction* editorViewAct;

	QMenu* windowMenu;
	QAction* settingAct;
	//-----------------------Menu Bar-------------------------


    //-----------------------Tool Bar-------------------------
	QToolBar*  toolBar;
	QLabel* lbSyncStatus;
	QToolButton* tbPreviewNow;
	QAction* previewNowAct;
	//-----------------------Tool Bar------------------------


	//-----------------------Status Bar----------------------
	QStatusBar* statusbar;

	QLabel* lbFileName;
	QLabel* lbPosition;
	//-----------------------Status Bar----------------------
};

#endif // MAINWINDOW_H
