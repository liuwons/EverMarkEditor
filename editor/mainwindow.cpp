#include "mainwindow.h"
#include "config.h"

#include <QDockWidget>
#include <QFileDialog>


#include <QTreeView>
#include "tool/evernotemanager.h"
#include "tool/notemodel.h"

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
	setWindowIcon(awesome->icon(fa::beer));
	//setWindowIcon(QIcon(":/res/write.png"));

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
	dockEditor->setFloating(true);
	this->setCentralWidget(dockEditor);
	
	connect(editor, SIGNAL(contentChanged(QString)), preview, SLOT(updateContent(QString)));
	
	localFileModel = new QFileSystemModel;
	localFileModel->setRootPath(QDir::currentPath());
	localFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::AllEntries);
	localFileTree = new QTreeView();
	localFileTree->setModel(localFileModel);
	localFileTree->hideColumn(1);
	localFileTree->hideColumn(2);
	localFileTree->hideColumn(3);
	
	EvernoteManager* man = new EvernoteManager(
		QString("S=s23:U=4e5fbd:E=15ab8c856fd:C=15361172888:P=1cd:A=en-devtoken:V=2:H=e1e63fcf88e8417bfab0c6aeb8ac2c05"),
		QString("yinxiang"),
		QString("github"),
		QString("D:/work/github/EverMarkEditor/release/evermark"),
		QString("D:/work/github/EverMarkEditor/release/evermark"),
		QString("sync"));
	if (!man->init())
	{
		qDebug() << "Init failed";
		exit(0);
	}
	if (!man->login())
	{
		qDebug() << "Login failed";
		exit(0);
	}
	NoteModel* nm = new NoteModel();
	bool load = nm->loadFromEvernoteManager(man);
	if (!load)
	{
		qDebug() << "Load evernote model failed";
		exit(0);
	}
	QTreeView* evernoteTreeView = new QTreeView;
	evernoteTreeView->setModel(nm);

	QWidget* openedNavigation = new QWidget;

	QVBoxLayout* dockNavigationLayout = new QVBoxLayout;
	dockNavigationLayout->setMargin(0);
	dockNavigationLayout->addWidget(localFileTree, 1);
	dockNavigationLayout->addWidget(evernoteTreeView, 1);
	dockNavigationLayout->addWidget(openedNavigation, 1);

	QWidget* navigationWidget = new QWidget;
	navigationWidget->setLayout(dockNavigationLayout);

	dockNavigation = new QDockWidget;
	dockNavigation->setMinimumWidth(screenSize.width() / 7);
	dockNavigation->setWidget(navigationWidget);

	addDockWidget(Qt::LeftDockWidgetArea, dockNavigation);

	connect(localFileTree, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(openFileFromLocalNavigation(const QModelIndex &)));
}


void MainWindow::createMenu()
{
	menubar = this->menuBar();

	fileMenu = menubar->addMenu(tr("File"));
	openFileAct = new QAction(tr("Open File"), this);
	openFileAct->setIcon(QIcon(":/res/file.png"));
	openFileAct->setShortcut(QKeySequence::Open);
	importAct = new QAction(tr("Import"), this);
	importAct->setIcon(QIcon(":/res/import.png"));
	openDirAct = new QAction(tr("Open Folder"), this);
	openDirAct->setIcon(QIcon(":/res/folder.png"));
	saveAct = new QAction(tr("Save"), this);
	saveAct->setIcon(QIcon(":/res/save_file.png"));
	saveAct->setShortcut(QKeySequence::Save);
	saveAsAct = new QAction(tr("Save As"), this);
	saveAsAct->setIcon(QIcon(":/res/save_as.png"));
	saveAsAct->setShortcut(QKeySequence::SaveAs);
	exitAct = new QAction(tr("Exit"), this);
	exitAct->setIcon(QIcon(":/res/exit.png"));
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
	aboutAct->setIcon(QIcon(":/res/about.png"));
	helpAct = new QAction(tr("Help"), this);
	helpAct->setIcon(QIcon(":/res/help.png"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(helpAct);

	windowMenu = menubar->addMenu(tr("Window"));
	previewNowAct = new QAction(tr("Preview"), this);
	previewNowAct->setIcon(QIcon(":/res/preview.png"));
	editorWindowAct = new QAction(tr("Editor"), this);
	editorWindowAct->setIcon(QIcon(":/res/write.png"));
	windowMenu->addAction(previewNowAct);
	windowMenu->addAction(editorWindowAct);

	connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFileFromMenu()));
	connect(openDirAct, SIGNAL(triggered()), this, SLOT(openDirFromMenu()));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(closeFromMenu()));
	connect(previewNowAct, SIGNAL(triggered()), this, SLOT(previewWindow()));
	connect(editorWindowAct, SIGNAL(triggered()), this, SLOT(editorWindow()));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFromMenu()));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsFromMenu()));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
}

void MainWindow::createToolBar()
{
	toolBar = new QToolBar("top toolbar");
	lbSyncStatus = new QLabel;
	lbSyncStatus->setPixmap(QPixmap(":/res/synced_yes.png"));
	toolBar->addWidget(lbSyncStatus);
	toolBar->addSeparator();

	tbPreviewNow = new QToolButton;
	tbPreviewNow->setIcon(QIcon(":/res/refresh.png"));
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

void MainWindow::openFileFromLocalNavigation(const QModelIndex & index)
{
	QString fpath = localFileModel->filePath(index);
	qDebug() << "Open File From Local Navigation:" << fpath;
	QFileInfo fi(fpath);
	if (fi.isFile())
	{
		loadFile(fpath);
	}
}

void MainWindow::loadFile(QString fpath)
{
	QFile file(fpath);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		qDebug() << "ERROR: read file failed. " << fpath;
		return;
	}

	QString content = file.readAll();
	file.close();

	editor->loadContent(content);
}

void MainWindow::openFileFromMenu()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Markdown File"), "/", tr("Markdown Files (*.md)"));
	if (fileName.length() == 0)
	{
		return;
	}
	QFileInfo info(fileName);
}


void MainWindow::openDirFromMenu()
{

}

void MainWindow::saveFromMenu()
{

}

void MainWindow::saveAsFromMenu()
{

}

void MainWindow::closeFromMenu()
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