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
private:
    QAction *exitAct;
};

#endif // MAINWINDOW_H
