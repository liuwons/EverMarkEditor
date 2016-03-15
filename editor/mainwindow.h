#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "preview.h"
#include "editor.h"

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void loadFile(QString fpath);

	// From Menu Bar
	void openFileFromMenu();
	void openDirFromMenu();
	void saveFromMenu();
	void saveAsFromMenu();
	void closeFromMenu();
	void previewWindow();
	void editorWindow();
	void dockedWindow();
	void about();
	void help();

	// From Tool Bar
	void previewNow();
	void openFileFromLocalNavigation(const QModelIndex & index);

private:
	void createUI();
	void createMenu();
	void createToolBar();
	void createStatusBar();

	QFileSystemModel *localFileModel;
	QTreeView *localFileTree;
	QString currentFilePath;

    QDockWidget* dockPreview;
    QDockWidget* dockEditor;
	Preview* preview;
	Editor* editor;

	QDockWidget* dockNavigation;
	QDockWidget* dockLocalNavigation;
	QDockWidget* dockRemoteNavigation;

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

	QMenu* windowMenu;
	QAction* previewWindowAct;
	QAction* editorWindowAct;
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
