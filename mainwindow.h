#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "displaywindow.h"
#include "querywindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSize sizeHint();

    void createActions();
    void createMenu();

    void initVariable();
    void synchronizeMenu();
private:
    DisplayWindow *displayWin;
    QueryWindow *queryWin;
    bool displayShow;
    bool queryShow;

    WordLocalDB *localDB;

    QAction *exitAct;
    QAction *displayAct;
    QAction *queryAct;



private slots:
    void displayMenuClicked();
    void queryMenuClicked();
};

#endif // MAINWINDOW_H
