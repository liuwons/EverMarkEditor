#include "mainwindow.h"
#include "config.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QTabWidget>
#include <QApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QKeyEvent>
#include <QTreeView>
#include <QDesktopServices>

#include "tool/evernotemanager.h"
#include "tool/notemodel.h"
#include "tool/appcontext.h"
#include "emfilesystemmodel.h"
#include "settingdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	createUI();
	createToolBar();
	createMenu();
	createStatusBar();
	createNavigation();
	updateEvernoteNavigation();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createUI()
{
	AppContext* context = AppContext::getContext();

	dockPreview = new QDockWidget(tr("Preview"));
	dockPreview->setMinimumWidth(context->screenSize.width() / 3);
	dockPreview->setMinimumHeight(context->screenSize.height() / 3 * 2);
	dockPreview->setObjectName("preview_dock_widget");
	preview = new Preview;
	AppContext::getContext()->preview = preview;
	dockPreview->setWidget(preview);
	dockPreview->setAllowedAreas(Qt::RightDockWidgetArea);
	dockPreview->setFloating(true);
	addDockWidget(Qt::RightDockWidgetArea, dockPreview);

	dockEditor = new QDockWidget(tr("Editor"));
	dockEditor->setMinimumWidth(context->screenSize.width() / 3);
	dockEditor->setMinimumHeight(context->screenSize.height() / 3 * 2);
	dockEditor->setObjectName("editor_dock_widget");
	editor = new Editor;
	AppContext::getContext()->editor = editor;
	dockEditor->setWidget(editor);
	dockEditor->setFloating(true);
	this->setCentralWidget(dockEditor);
	
	connect(editor, SIGNAL(contentChanged(QString)), preview, SLOT(updateMarkdownContent(QString)));
}

void MainWindow::updateEvernoteNavigation()
{

	AppContext* context = AppContext::getContext();
	if (!context->evernoteManager || !context->evernoteManager->logined())
	{
		qDebug() << "[ERROR] MainWindow updateEvernoteNavigation evernoteManager not logined";
		return;
	}
	
	bool load = evernoteModel->loadFromEvernoteManager(context->evernoteManager);
	if (!load)
	{
		qDebug() << "[ERROR] MainWindow updateEvernoteNavigation loadFromEvernoteManager failed";
		return;
	}
	evernoteTree->setModel(evernoteModel);
}

void MainWindow::createNavigation()
{
	localFileModel = new EMFileSystemModel;
	localFileModel->setRootPath(QDir::currentPath());
	localFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::AllEntries);
	localFileTree = new QTreeView();
	localFileTree->setModel(localFileModel);
	localFileTree->hideColumn(1);
	localFileTree->hideColumn(2);
	localFileTree->hideColumn(3);

	evernoteTree = new QTreeView;
	evernoteModel = new NoteModel;
	workbenchTree = new QTreeView;

	QTabWidget* tab = new QTabWidget;
	tab->setTabPosition(QTabWidget::South);
	tab->addTab(localFileTree, tr("File System"));
	tab->addTab(evernoteTree, tr("Evernote"));
	tab->addTab(workbenchTree, tr("Workbench"));

	QVBoxLayout* dockNavigationLayout = new QVBoxLayout;
	dockNavigationLayout->setMargin(0);
	dockNavigationLayout->addWidget(tab);

	QWidget* navigationWidget = new QWidget;
	navigationWidget->setLayout(dockNavigationLayout);

	dockNavigation = new QDockWidget;
	dockNavigation->setMinimumWidth(AppContext::getContext()->screenSize.width() / 5);
	dockNavigation->setWidget(navigationWidget);

	addDockWidget(Qt::LeftDockWidgetArea, dockNavigation);

	connect(localFileTree, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(openFileFromLocalNavigation(const QModelIndex &)));
	connect(evernoteTree, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(openNoteFromEvernoteNavigation(const QModelIndex &)));
}


void MainWindow::createMenu()
{
	AppContext* context = AppContext::getContext();
	menubar = this->menuBar();

	fileMenu = menubar->addMenu(tr("File"));
	openFileAct = new QAction(tr("Open File"), this);
	openFileAct->setIcon(context->awesome->icon(fa::file));
	openFileAct->setShortcut(QKeySequence::Open);
	importAct = new QAction(tr("Import"), this);
	importAct->setIcon(context->awesome->icon(fa::arrowdown));
	openDirAct = new QAction(tr("Open Folder"), this);
	openDirAct->setIcon(context->awesome->icon(fa::folderopen));
	saveAct = new QAction(tr("Save"), this);
	saveAct->setIcon(context->awesome->icon(fa::save));
	saveAct->setShortcut(QKeySequence::Save);
	saveAsAct = new QAction(tr("Save As"), this);
	saveAsAct->setIcon(context->awesome->icon(fa::edit));
	saveAsAct->setShortcut(QKeySequence::SaveAs);
	exitAct = new QAction(tr("Exit"), this);
	exitAct->setIcon(context->awesome->icon(fa::close));
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
	aboutAct->setIcon(context->awesome->icon(fa::info));
	helpAct = new QAction(tr("Help"), this);
	helpAct->setIcon(context->awesome->icon(fa::mortarboard));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(helpAct);

	viewMenu = menubar->addMenu(tr("View"));
	previewNowAct = new QAction(tr("Preview"), this);
	previewNowAct->setIcon(context->awesome->icon(fa::eye));
	editorViewAct = new QAction(tr("Editor"), this);
	editorViewAct->setIcon(context->awesome->icon(fa::code));
	viewMenu->addAction(previewNowAct);
	viewMenu->addAction(editorViewAct);

	windowMenu = menubar->addMenu(tr("Window"));
	settingAct = new QAction(tr("Setting"), this);
	settingAct->setIcon(context->awesome->icon(fa::gears));
	windowMenu->addAction(settingAct);

	connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFileFromMenu()));
	connect(openDirAct, SIGNAL(triggered()), this, SLOT(openDirFromMenu()));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(closeFromMenu()));
	connect(previewNowAct, SIGNAL(triggered()), this, SLOT(previewView()));
	connect(editorViewAct, SIGNAL(triggered()), this, SLOT(editorView()));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFromMenu()));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsFromMenu()));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
	connect(settingAct, SIGNAL(triggered()), this, SLOT(setting()));
}

void MainWindow::createToolBar()
{
	AppContext* context = AppContext::getContext();

	toolBar = new QToolBar("top toolbar");
	lbSyncStatus = new QLabel(QChar(fa::check));
	lbSyncStatus->setFont(context->awesome->font(16));
	toolBar->addWidget(lbSyncStatus);
	toolBar->addSeparator();

	tbPreviewNow = new QToolButton();
	tbPreviewNow->setIcon(context->awesome->icon(fa::refresh));
	tbPreviewNow->setIconSize(QSize(16, 16));
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
	QFileInfo fi(fpath);
	if (fi.isFile())
	{
		loadFile(fpath);
	}
}

void MainWindow::openNoteFromEvernoteNavigation(const QModelIndex & index)
{
	if (!index.isValid())
		return;

	NoteItem *item = static_cast<NoteItem*>(index.internalPointer());
	if (!item || item->type != TYPE_NOTE)
		return;

	if (item->name.length() > 0)
		qDebug() << "[DEBUG] Open note from evernote navigation: " << item->name;

	QString guid = item->id;
	loadNote(guid);
}

void MainWindow::loadNote(QString guid)
{
	AppContext* context = AppContext::getContext();
	if (!context->evernoteManager || !context->evernoteManager->logined())
	{
		qDebug() << "[ERROR] MainWindow loadNote failed: EvernoteManager not logined";
		return;
	}
	QString content = context->evernoteManager->getNote(guid);
	preview->updateHtmlContent(content);

	qDebug() << "[DEBUG] MainWindow loadNote content length: " << content.length();
}

void MainWindow::loadFile(QString fpath)
{
	QFile file(fpath);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		qDebug() << "[ERROR] read file failed. " << fpath;
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
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	setWindowTitle(dirName);
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

void MainWindow::previewView()
{

}

void MainWindow::editorView()
{

}

void MainWindow::dockedWindow()
{

}

void MainWindow::about()
{
	QUrl url = QUrl::fromLocalFile("about.txt");
	QDesktopServices::openUrl(url);
}

void MainWindow::help()
{
	QUrl url = QUrl::fromLocalFile("help.txt");
	QDesktopServices::openUrl(url);
}


void MainWindow::previewNow()
{
}

void MainWindow::setting()
{
	SettingDialog* sd = new SettingDialog;
	sd->exec();
	delete sd;
}



void MainWindow::keyPressEvent(QKeyEvent *e)
{
	QWidget::keyPressEvent(e);
	switch (e->key())
	{
	case Qt::Key_Control:
		break;
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
	QWidget::keyReleaseEvent(e);
	switch (e->key())
	{
	case Qt::Key_Control:
		break;
	case Qt::Key_D:
		break;
	case Qt::Key_Space:
		break;
	}
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	QMessageBox msgBox;
	msgBox.setWindowIcon(AppContext::getContext()->awesome->icon(fa::warning));
	msgBox.setText("The document has been modified.");
	msgBox.setInformativeText("Do you want to save your changes?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
	switch (ret) {
	case QMessageBox::Save:
		break;
	case QMessageBox::Discard:
		break;
	case QMessageBox::Cancel:
		break;
	default:
		// should never be reached
		break;
	}
}