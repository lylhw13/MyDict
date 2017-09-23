#ifndef QUERYWINDOW_H
#define QUERYWINDOW_H

#include <QWidget>
#include <QSize>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>

#include "wordlocaldb.h"

class QueryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QueryWindow(WordLocalDB *localDB, QWidget *parent = 0);
    //QSize sizeHint();

private:
    WordLocalDB *localDB;
    WordData *queryWord;

    QLineEdit *queryInput;
    QTextEdit *queryResult;

    QWidget *createQueryInputWidget();

signals:

public slots:
    void querySlot();

    void fillContentSlot(bool flag);
    void addWordAfterQueryNetSlot(bool flag);
};

#endif // QUERYWINDOW_H
