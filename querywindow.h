#ifndef QUERYWINDOW_H
#define QUERYWINDOW_H

#include <QWidget>
#include <QSize>

class QueryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QueryWindow(QWidget *parent = 0);
    //QSize sizeHint();

signals:

public slots:
};

#endif // QUERYWINDOW_H
