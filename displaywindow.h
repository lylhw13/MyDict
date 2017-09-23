#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>

#include "wordData.h"
#include "wordlocaldb.h"
#include "clickablelabel.h"

class DisplayWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWindow(WordLocalDB *localDB ,QWidget *parent = 0);

    QPushButton *generateButton(QString &name, QColor &color, QWidget *parent = 0);


    QWidget *genContentWidget();
    void fillContent();
    void setTimer(bool);

    WordData *displayWord;

private:
    QTextEdit *displayContent;
    ClickableLabel *displayLabel;
    WordLocalDB *localDB;

    QPushButton *notKnownButton;
    QPushButton *unfamiliarButton;
    QPushButton *familiarButton;

    QStack<QString> previousWordStack;
    QStack<QString> nextWordStack;
    QString lastWord;

    bool isJudged;

    QTimer *timer;

private:

signals:

public slots:
    void nextButtonSlot();
    void previousButtonSlot();
    void labelPressedSlot();
    void labelReleaseSlot();

    void notKnownButtonSlot();//remember number minus 1
    void unfamiliarButtonSlot();//show the meaning
    void familiarButtonSlot();//remember number plus 1

};

#endif // DISPLAYWINDOW_H
