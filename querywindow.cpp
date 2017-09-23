#include "querywindow.h"
#include <QLayout>
#include <QLabel>
#include <QTextTable>
#include <QTextTableFormat>


QueryWindow::QueryWindow(WordLocalDB *localDB,QWidget *parent) : QWidget(parent)
{
    this->localDB = localDB;
    queryWord = new WordData;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QWidget *widget = new QWidget;
    //widget->setContentsMargins(0,0,0,0);
    int lineHeight = 35;
    widget->setFixedHeight(lineHeight);
    QHBoxLayout *queryInputLayout = new QHBoxLayout;
    queryInputLayout->setContentsMargins(0,0,0,0);
    queryInputLayout->setSpacing(0);
    QPushButton *previousButton = new QPushButton(QIcon(":/icons/arrow_left_blue.png"),tr(""),this);
    previousButton->setFixedSize(lineHeight,lineHeight);
    QPushButton *nextButton = new QPushButton(QIcon(":/icons/arrow_right_blue.png"),tr(""),this);
    nextButton->setFixedSize(lineHeight,lineHeight);

    queryInput = new QLineEdit;
    queryInput->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    queryInput->setFixedHeight(lineHeight-2);
    queryInput->setFont(QFont("微软雅黑",15));

//    QLabel *lineLabel = new QLabel;
//    QPixmap pix(":/icons/line.png");
//    lineLabel->setPixmap(pix);
    QPushButton *queryButton = new QPushButton(QIcon(":/icons/query.png"),tr(""),this);
    queryButton->setFixedSize(lineHeight,lineHeight);
    queryInputLayout->addWidget(previousButton);
    queryInputLayout->addWidget(nextButton);
    queryInputLayout->addWidget(queryInput);
    //queryInputLayout->addWidget(lineLabel);
    queryInputLayout->addWidget(queryButton);

    widget->setLayout(queryInputLayout);
    mainLayout->addWidget(widget);

    nextButton->setDisabled(true);


    queryResult = new QTextEdit;
    mainLayout->addWidget(queryResult);
    setLayout(mainLayout);
    //setFixedWidth(430);
    setMinimumWidth(400);
    setMinimumSize(400,270);
    //setFixedSize(430,350);


    QFile file(":/qss/queryWindow.qss");
    if(!file.open(QFile::ReadOnly))
        qDebug()<<"cannot open qss";
    QString styleSheet = QString::fromLatin1(file.readAll());
    widget->setStyleSheet(styleSheet);

    connect(queryButton,SIGNAL(clicked(bool)),this,SLOT(querySlot()));
    connect(queryInput,SIGNAL(returnPressed()),this,SLOT(querySlot()));

    connect(queryWord,SIGNAL(finishQuery(bool)),this,SLOT(fillContentSlot(bool)));
    connect(queryWord,SIGNAL(finishQuery(bool)),this,SLOT(addWordAfterQueryNetSlot(bool)));
}

void QueryWindow::fillContentSlot(bool flag)
{

    if(!flag)
    {
        QMessageBox::information(this,tr("Fail to Find!"),tr("Please check the word you inputed!"));
        return;
    }

    queryResult->clear();

    QFont font;
    font.setFamily("微软雅黑");

    QTextDocument *document = queryResult->document();
    QTextCursor cursor(document);

    QTextCharFormat charFormat;
    QTextBlockFormat blockFormat;
    font.setBold(true);
    font.setPointSize(20);
    charFormat.setFont(font);
    //blockFormat.setAlignment(Qt::AlignCenter);
    cursor.setCharFormat(charFormat);
    cursor.setBlockFormat(blockFormat);
    cursor.insertText(queryWord->getWordName());

    //phonetic
    font.setBold(false);
    font.setPointSize(9);
    charFormat.setFont(font);
    //blockFormat.setAlignment(Qt::AlignLeft);
    //cursor.insertImage();
    cursor.insertBlock(blockFormat,charFormat);
    cursor.insertText(queryWord->getPhonetic().join("  "));

    //paraphrase
    font.setPointSize(11);
    charFormat.setFont(font);
    QStringList paraList = queryWord->getParaphrase();
    if(!paraList.isEmpty())
    {
        cursor.insertTable(paraList.count(),2);

        QTextTableFormat tableFormat;
        tableFormat.setBorder(0);
        tableFormat.setCellPadding(2);
        //tableFormat.setAlignment(Qt::AlignCenter);
        cursor.currentTable()->setFormat(tableFormat);

        blockFormat.setBackground(Qt::gray);

        for(int i = 0; i< paraList.count();i++)
        {
            QString currentPara = paraList.at(i);
            if(currentPara.isEmpty())
                continue;
            int index = currentPara.indexOf(QRegularExpression("\\s"));
            QString classifyPara = currentPara.left(index);
            QString meaningPara = currentPara.mid(index);

            QTextCursor classifyCursor = cursor.currentTable()->cellAt(i,0).firstCursorPosition();
            classifyCursor.setCharFormat(charFormat);
            classifyCursor.setBlockFormat(blockFormat);
            classifyCursor.insertText(classifyPara);

            QTextCursor meaningCursor = cursor.currentTable()->cellAt(i,1).firstCursorPosition();
            meaningCursor.setCharFormat(charFormat);
            meaningCursor.insertText(meaningPara);
        }
    }

}

void QueryWindow::addWordAfterQueryNetSlot(bool flag)
{
    if(flag)
        localDB->addWord(queryWord);
}

void QueryWindow::querySlot()
{
    QString string = queryInput->text().toLower().trimmed();
    if(string.isEmpty())
    {
        QMessageBox::information(this,tr("Empty query word"),tr("Please input a word before to query."));
        return;
    }
    queryWord->setWordName(string);

    if(localDB->queryWord(queryWord))
        fillContentSlot(true);
    else
        queryWord->queryNet();
}

