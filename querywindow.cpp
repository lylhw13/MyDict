#include "querywindow.h"
#include <QLayout>
#include <QLabel>
#include <QTextEdit>

QueryWindow::QueryWindow(QWidget *parent) : QWidget(parent)
{

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QTextEdit *test = new QTextEdit;
    test->append("Text Query");
    mainLayout->addWidget(test);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

//QSize QueryWindow::sizeHint()
//{
//    return QSize(550,600);
//}
