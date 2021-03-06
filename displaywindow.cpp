#include "displaywindow.h"
#include <QLabel>
#include <QLayout>
#include <QTextEdit>

#include <QDebug>
#include <QIcon>
#include <QTreeWidget>
#include <QTextTable>
#include <QTextTableFormat>


DisplayWindow::DisplayWindow(WordLocalDB *localDB, QWidget *parent) : QWidget(parent)
{
    this->localDB = localDB;

    QVBoxLayout *mainLayout = new QVBoxLayout;

//    QHBoxLayout *settingLayout = new QHBoxLayout;
//    QPushButton *setButton = new QPushButton(QIcon(":/icons/settings.png"),tr(""),this);
//    settingLayout->addStretch(1);
//    settingLayout->addWidget(setButton);
//    mainLayout->addLayout(settingLayout);
    QHBoxLayout *showLayout = new QHBoxLayout;
    //display label
    displayLabel = new ClickableLabel(tr("display"));
    QFont font;
    font.setFamily("微软雅黑");
    font.setBold(true);
    font.setPointSize(25);
    displayLabel->setFont(font);

    displayLabel->setFrameStyle(QFrame::Panel);
    displayLabel->setMinimumSize(QSize(250,200));
    displayLabel->setFixedSize(350,200);
    displayLabel->setAlignment(Qt::AlignCenter);
    displayLabel->setStyleSheet("QLable {background-color: white}");


    displayContent = new QTextEdit;
    displayContent->setReadOnly(true);
    //displayContent->setMinimumSize(QSize(250,200));
    displayContent->setFixedSize(350,200);
    displayContent->hide();

    //button
    QPushButton *previousButton = new QPushButton(QIcon(":/icons/previous.png"),tr(""),this);
    QPushButton *nextButton = new QPushButton(QIcon(":/icons/next.png"),tr(""),this);
    previousButton->setFixedWidth(25);
    previousButton->setMinimumHeight(displayLabel->minimumHeight());
    nextButton->setFixedWidth(25);
    nextButton->setMinimumHeight(displayLabel->minimumHeight());

    showLayout->addWidget(previousButton);
    showLayout->addWidget(displayLabel);
    showLayout->addWidget(displayContent);
    showLayout->addWidget(nextButton);
    mainLayout->addLayout(showLayout);

    //markLayout
    QHBoxLayout *markLayout = new QHBoxLayout;
    notKnownButton = generateButton(tr("Not Known"),QColor(219,76,79),this);
    unfamiliarButton = generateButton(tr("Unfamiliar"),QColor(236,180,33),this);
    familiarButton = generateButton(tr("Familiar"),QColor(83,167,92),this);
    markLayout->addWidget(notKnownButton);
    markLayout->addWidget(unfamiliarButton);
    markLayout->addWidget(familiarButton);
    mainLayout->addLayout(markLayout);

    mainLayout->addStretch(1);

    setLayout(mainLayout);

    //connect
    connect(displayLabel,SIGNAL(pressed()),this,SLOT(labelPressedSlot()));
    connect(displayLabel,SIGNAL(released()),this,SLOT(labelReleaseSlot()));

    connect(previousButton,SIGNAL(clicked(bool)),this,SLOT(previousButtonSlot()));
    connect(nextButton,SIGNAL(clicked(bool)),this,SLOT(nextButtonSlot()));

    connect(notKnownButton,SIGNAL(clicked(bool)),this,SLOT(notKnownButtonSlot()));
    connect(unfamiliarButton,SIGNAL(clicked(bool)),this,SLOT(unfamiliarButtonSlot()));
    connect(familiarButton,SIGNAL(clicked(bool)),this,SLOT(familiarButtonSlot()));

    lastWord = "";
    isJudged = false;
    displayWord = new WordData;
//    localDB = new WordLocalDB();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(nextButtonSlot()));
    timer->start(400000);//10s
    nextButtonSlot();
    //setFixedSize(430,350);
    setFixedWidth(430);
}

QPushButton *DisplayWindow::generateButton(QString &name, QColor &color, QWidget *parent)
{
    QPushButton *button = new QPushButton(name,parent);

//    QPalette  pal = palette();
//    pal.setColor(QPalette::Button,color);
//    button->setAutoFillBackground(true);
//    button->setPalette(pal);
//    button->setFlat(true);
//    button->update();
    QSize originalSize = button->sizeHint();
    button->setMinimumHeight( 2 * originalSize.height());


    //if want to set the backgoround-color to transparent, write as follows:
//    color.setAlpha(150);
//    QString style = tr("QPushButton {background-color: ") + color.name(QColor::HexArgb)+tr("; font-family: consolas; font-size: 18pt; font-weight: bold;color: #ffffff;}");

    QString style = tr("QPushButton {background-color: ") + color.name()+tr("; font-family: consolas; font-size: 18pt; font-weight: bold;color: #ffffff;}");
    button->setStyleSheet(style);

    return button;
}

void DisplayWindow::fillContent()
{
    displayContent->clear();

    QFont font;
    font.setFamily("微软雅黑");

    QTextDocument *document = displayContent->document();
    QTextCursor cursor(document);

    QTextCharFormat charFormat;
    QTextBlockFormat blockFormat;
    font.setBold(true);
    font.setPointSize(20);
    charFormat.setFont(font);
    blockFormat.setAlignment(Qt::AlignCenter);
    cursor.setCharFormat(charFormat);
    cursor.setBlockFormat(blockFormat);
    cursor.insertText(displayWord->getWordName());

    //phonetic
    font.setBold(false);
    font.setPointSize(9);
    charFormat.setFont(font);
    //blockFormat.setAlignment(Qt::AlignLeft);
    //cursor.insertImage();
    cursor.insertBlock(blockFormat,charFormat);
    cursor.insertText(displayWord->getPhonetic().join("  "));

    //paraphrase
    font.setPointSize(11);
    charFormat.setFont(font);
    QStringList paraList = displayWord->getParaphrase();
    if(!paraList.isEmpty())
    {
        cursor.insertTable(paraList.count(),2);

        QTextTableFormat tableFormat;
        tableFormat.setBorder(0);
        tableFormat.setCellPadding(2);
        tableFormat.setAlignment(Qt::AlignCenter);
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

void DisplayWindow::setTimer(bool flag)
{
    if(flag)
        timer->start(10000);
    else
        timer->stop();
}

void DisplayWindow::nextButtonSlot()
{
    if(nextWordStack.isEmpty())
    {
        if(!localDB->randowQueryWord(displayWord))
            return;
    }
    else
    {
        QString nextWord = nextWordStack.pop();

        if(!localDB->queryWord(nextWord,displayWord))
            return;
    }

    if(displayWord->getWordName().isEmpty())
        return;

    if(!lastWord.isEmpty())
        previousWordStack.push(lastWord);
    lastWord = displayWord->getWordName();

    if(previousWordStack.count()>5)
        previousWordStack.pop_front();

    displayContent->hide();
    displayLabel->setText(displayWord->getWordName());
    displayLabel->show();
    isJudged = false;
}

void DisplayWindow::previousButtonSlot()
{
    if(previousWordStack.isEmpty())
        return;

    QString previousWord = previousWordStack.pop();
//    nextWordStack.push_back(displayWord->getWordName());

    if(!localDB->queryWord(previousWord,displayWord))
        return;

    if(displayWord->getWordName().isEmpty())
        return;

    if(!lastWord.isEmpty())
        nextWordStack.push(lastWord);
    lastWord = displayWord->getWordName();

    displayContent->hide();
    displayLabel->setText(displayWord->getWordName());
    displayLabel->show();
    isJudged = false;
}

void DisplayWindow::labelPressedSlot()
{
    //displayLabel->setStyleSheet("QLabel { background-color : rgba(219,76,79,50);}");
    displayLabel->setStyleSheet("QLabel {background-color: qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 white, stop:1 gray);}");
}

void DisplayWindow::labelReleaseSlot()
{
    displayLabel->setStyleSheet("");
    displayLabel->hide();
    fillContent();
    displayContent->show();
}

void DisplayWindow::notKnownButtonSlot()
{
    if(isJudged)
        return;
    displayWord->addMemoryNumber(-1);
    localDB->update(displayWord);
    labelReleaseSlot();//show content
}

void DisplayWindow::unfamiliarButtonSlot()
{
    if(isJudged)
        return;
    labelReleaseSlot();
}

void DisplayWindow::familiarButtonSlot()
{
    if(isJudged)
        return;
    displayWord->addMemoryNumber(1);
    localDB->update(displayWord);
    nextButtonSlot();
}

