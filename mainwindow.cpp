#include "mainwindow.h"
#include <QSplitter>
#include <QToolBar>
#include <QIcon>
#include <QMenuBar>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initVariable();

    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Horizontal);

    displayWin = new DisplayWindow(localDB,this);
    queryWin = new QueryWindow(localDB,this);
    splitter->addWidget(displayWin);
    splitter->addWidget(queryWin);
    splitter->setHandleWidth(5);
    setCentralWidget(splitter);

    createActions();
    createMenu();
    synchronizeMenu();

    //resize(sizeHint());
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

    displayAct = new QAction(tr("Dispaly Window"),this);
    displayAct->setCheckable(true);
    connect(displayAct,SIGNAL(triggered(bool)),this,SLOT(displayMenuClicked()));

    queryAct = new QAction(tr("Search Window"),this);
    queryAct->setCheckable(true);
    connect(queryAct,SIGNAL(triggered(bool)),this,SLOT(queryMenuClicked()));
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);

    QMenu *settingMenu = menuBar()->addMenu(tr("&Setting"));
    settingMenu->addAction(displayAct);
    settingMenu->addAction(queryAct);
}

void MainWindow::initVariable()
{
    displayShow = true;
    queryShow = true;
    localDB = new WordLocalDB;
}

void MainWindow::synchronizeMenu()
{
    displayAct->setChecked(displayShow);
    queryAct->setChecked(queryShow);

    displayWin->hide();
    queryWin->hide();
    resize(400,200);
    updateGeometry();

    displayWin->setVisible(displayShow);
    queryWin->setVisible(queryShow);

    displayWin->setTimer(displayShow);
    //setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    //resize(400,200);
    //updateGeometry();
}

void MainWindow::displayMenuClicked()
{
    displayShow = !displayShow;
    if(!displayShow && !queryShow)//make sure at least one window is visiable
        queryShow = !queryShow;
    synchronizeMenu();
}

void MainWindow::queryMenuClicked()
{
    queryShow = !queryShow;
    if(!queryShow && !displayShow)//make sure at least one window is visiable
        displayShow = !displayShow;
    synchronizeMenu();
}
