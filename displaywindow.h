#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

#include "wordData.h"
#include "wordlocaldb.h"
#include "clickablelabel.h"

class DisplayWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWindow(QWidget *parent = 0);

    QPushButton *generateButton(QString &name, QColor &color, QWidget *parent = 0);


    QWidget *genContentWidget();
    void fillContent();

    WordData *displayWord;

private:
    QTextEdit *displayContent;
    ClickableLabel *displayLabel;
    WordLocalDB *localDB;

    QStack<QString> previousWordStack;
    QStack<QString> nextWordStack;
    QString lastWord;

private:

signals:

public slots:
    void nextButtonSlot();
    void previousButtonSlot();
    void labelPressedSlot();
    void labelReleaseSlot();

};

#endif // DISPLAYWINDOW_H
