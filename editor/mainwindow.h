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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	// From Menu Bar
	void openFile();
	void openDir();
	void save();
	void saveAs();
	void close();
	void previewWindow();
	void editorWindow();
	void dockedWindow();
	void about();
	void help();

	// From Tool Bar
	void previewNow();


private:
	void createUI();
	void createMenu();
	void createToolBar();
	void createStatusBar();

	QString currentWorkFilePath;

    QDockWidget* dockPreview;
    QDockWidget* dockEditor;
	Preview* preview;
	Editor* editor;

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
