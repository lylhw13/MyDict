#include "mainwindow.h"
#include <QSplitter>
#include <QToolBar>
#include <QIcon>
#include <QMenuBar>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Horizontal);

    DisplayWindow *displayWin = new DisplayWindow;
    QueryWindow *queryWin = new QueryWindow;
    splitter->addWidget(displayWin);
    splitter->addWidget(queryWin);
    setCentralWidget(splitter);

    createActions();
    createMenu();

    resize(sizeHint());
}

MainWindow::~MainWindow()
{

}

QSize MainWindow::sizeHint()
{
    return QSize(350,550);
}

void MainWindow::createActions()
{
    exitAct = new QAction(tr("&Exit"),this);
    connect(exitAct,SIGNAL(triggered(bool)),qApp,SLOT(quit()));
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);

    QMenu *settingMenu = menuBar()->addMenu(tr("&Setting"));
}
