#include "wordData.h"
#include "wordlocaldb.h"
#include <QtSql>

WordData::WordData()
{
    memoryNumber = 0;
    wordName = "";

}

void WordData::setPhoneticFromLocal(const QString &phoneticStr)
{
    phonetic = phoneticStr.split("$");
}

void WordData::setParaphraseFromLocal(const QString &paraphraseStr)
{
    paraphrase = paraphraseStr.split("$");
}

void WordData::setRemFromLocal(int remInt)
{
    memoryNumber = remInt;
}

QStringList &WordData::getPhonetic()
{
    return phonetic;
}

QStringList &WordData::getParaphrase()
{
    return paraphrase;
}

QString &WordData::getWordName()
{
    return wordName;
}

int WordData::getMemoryNumber()
{
    return memoryNumber;
}

void WordData::addMemoryNumber(int temp)
{
    memoryNumber +=temp;
}

void WordData::queryNet()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;

    request.setUrl(QUrl("https://cn.bing.com/dict/search?mkt=zh-cn&q="+wordName));

    QNetworkReply *reply = manager->get(request);

    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(handleNetworkData(QNetworkReply*)));

}

bool WordData::flagQuery()
{
    return memoryNumber!=0;
}

void WordData::setWordName(const QString &name)
{
    wordName = name;
}

void WordData::handleNetworkData(QNetworkReply *networkReply)
{
    if(networkReply->error()==QNetworkReply::NoError)
    {
        QByteArray bytes = networkReply->readAll();
        QString result(bytes);

        QFile xmlFile("xmloutput.txt");
        xmlFile.open(QIODevice::ReadWrite);

        //QRegularExpression re("(?<=\<meta\\s)name=\"description\" content=\"([^\"]+?)\"");
        QRegularExpression re("释义");
        QRegularExpressionMatch matchResult = re.match(&result);
        if(matchResult.hasMatch())
        {
            re.setPattern("(?<=\<meta\\s)name=\"description\" content=\"([^\"]+?)\"");
            matchResult = re.match(&result);
            if(matchResult.hasMatch())
            {
                phonetic.clear();
                paraphrase.clear();

                QString matchTotal = matchResult.captured(0);
                xmlFile.write(matchTotal.toUtf8().constData());
                xmlFile.close();
                //xmlFile.write(bytes);
                //phonetic
                //re.setPattern("，[^\"]+?\\[[^\"]+?\\]");
                re.setPattern("([^\"]{1})(\\[[^\"]+?\\])");
                matchResult = re.match(&matchTotal);
                if(matchResult.hasMatch())
                    phonetic.append(matchResult.captured(1)+" : "+matchResult.captured(2));

                int index = matchResult.capturedEnd();
                matchResult = re.match(&matchTotal,index);
                if(matchResult.hasMatch())
                    phonetic.append(matchResult.captured(1)+" : "+matchResult.captured(2));

                //paraphrase
                re.setPattern("([a-z]+?\\.\\s)([\\S]+?\\s)");
                re.setPattern("([a-z]+?\\.\\s)([^\"]+?；\\s)");
                matchResult = re.match(&matchTotal);
                while(matchResult.hasMatch())
                {
                    index = matchResult.capturedEnd();
                    //paraphrase.append(matchResult.captured(1)+temp.fill(' ',(10-matchResult.captured(1).length()))+matchResult.captured(2));
                    paraphrase.append(matchResult.captured(0));
                    matchResult = re.match(&matchTotal,index);
                }

                re.setPattern("(网络)(释义：\\s)(.+?)(?=\")");
                matchResult = re.match(&matchTotal);
                if(matchResult.hasMatch())
                    paraphrase.append(matchResult.captured(1)+" "+matchResult.captured(3)+matchResult.captured(4));

                emit finishQuery(true);
            }
        }
        else
        {
            finishQuery(false);
        }

    }
}



















