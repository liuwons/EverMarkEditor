#include "mainwindow.h"
#include "config.h"

#include <QDockWidget>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	createUI();
	createMenu();
	createToolBar();
	createStatusBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createUI()
{
	setWindowIcon(QIcon("res/write.png"));

	dockPreview = new QDockWidget(tr("Preview"));
	dockPreview->setMinimumWidth(screenSize.width() / 3);
	dockPreview->setMinimumHeight(screenSize.height() / 3*2);
	dockPreview->setObjectName("preview_dock_widget");
	preview = new Preview;
	dockPreview->setWidget(preview);
	dockPreview->setAllowedAreas(Qt::RightDockWidgetArea);
	dockPreview->setFloating(true);
	addDockWidget(Qt::RightDockWidgetArea, dockPreview);

	dockEditor = new QDockWidget(tr("Editor"));
	dockEditor->setMinimumWidth(screenSize.width() / 3);
	dockEditor->setMinimumHeight(screenSize.height() / 3*2);
	dockEditor->setObjectName("editor_dock_widget");
	editor = new Editor;
	dockEditor->setWidget(editor);
	dockEditor->setAllowedAreas(Qt::LeftDockWidgetArea);
	dockEditor->setFloating(true);
	addDockWidget(Qt::LeftDockWidgetArea, dockEditor);
	
	connect(editor, SIGNAL(contentChanged(QString)), preview, SLOT(updateContent(QString)));
}


void MainWindow::createMenu()
{
	menubar = this->menuBar();

	fileMenu = menubar->addMenu(tr("File"));
	openFileAct = new QAction(tr("Open File"), this);
	openFileAct->setIcon(QIcon("res/file.png"));
	openFileAct->setShortcut(QKeySequence::Open);
	importAct = new QAction(tr("Import"), this);
	importAct->setIcon(QIcon("res/import.png"));
	openDirAct = new QAction(tr("Open Folder"), this);
	openDirAct->setIcon(QIcon("res/folder.png"));
	saveAct = new QAction(tr("Save"), this);
	saveAct->setIcon(QIcon("res/save_file.png"));
	saveAct->setShortcut(QKeySequence::Save);
	saveAsAct = new QAction(tr("Save As"), this);
	saveAsAct->setIcon(QIcon("res/save_as.png"));
	saveAsAct->setShortcut(QKeySequence::SaveAs);
	exitAct = new QAction(tr("Exit"), this);
	exitAct->setIcon(QIcon("res/exit.png"));
	exitAct->setShortcut(QKeySequence::Quit);
	fileMenu->addAction(openFileAct);
	fileMenu->addAction(openDirAct);
	fileMenu->addSeparator();
	fileMenu->addAction(importAct);
	fileMenu->addSeparator();
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	helpMenu = menubar->addMenu(tr("Help"));
	aboutAct = new QAction(tr("About"), this);
	aboutAct->setIcon(QIcon("res/about.png"));
	helpAct = new QAction(tr("Help"), this);
	helpAct->setIcon(QIcon("res/help.png"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(helpAct);

	windowMenu = menubar->addMenu(tr("Window"));
	previewNowAct = new QAction(tr("Preview"), this);
	previewNowAct->setIcon(QIcon("res/preview.png"));
	editorWindowAct = new QAction(tr("Editor"), this);
	editorWindowAct->setIcon(QIcon("res/write.png"));
	windowMenu->addAction(previewNowAct);
	windowMenu->addAction(editorWindowAct);

	connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(openDirAct, SIGNAL(triggered()), this, SLOT(openDir()));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	connect(previewNowAct, SIGNAL(triggered()), this, SLOT(previewWindow()));
	connect(editorWindowAct, SIGNAL(triggered()), this, SLOT(editorWindow()));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
}

void MainWindow::createToolBar()
{
	toolBar = new QToolBar("top toolbar");
	lbSyncStatus = new QLabel;
	lbSyncStatus->setPixmap(QPixmap("res/synced_yes.png"));
	toolBar->addWidget(lbSyncStatus);
	toolBar->addSeparator();

	tbPreviewNow = new QToolButton;
	tbPreviewNow->setIcon(QIcon("res/refresh.png"));
	toolBar->addWidget(tbPreviewNow);

	addToolBar(Qt::TopToolBarArea, toolBar);

	connect(tbPreviewNow, SIGNAL(clicked()), this, SLOT(previewNow()));
}


void MainWindow::createStatusBar()
{
	statusbar = statusBar();
	lbFileName = new QLabel;
	lbPosition = new QLabel;
	statusbar->addWidget(lbFileName, 1);
	statusbar->addWidget(lbPosition, 1);
}

void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Markdown File"), "/", tr("Markdown Files (*.md)"));
	if (fileName.length() == 0)
	{
		return;
	}
	QFileInfo info(fileName);
}


void MainWindow::openDir()
{

}

void MainWindow::save()
{

}

void MainWindow::saveAs()
{

}

void MainWindow::close()
{

}

void MainWindow::previewWindow()
{

}

void MainWindow::editorWindow()
{

}

void MainWindow::dockedWindow()
{

}

void MainWindow::about()
{

}

void MainWindow::help()
{

}


void MainWindow::previewNow()
{

}