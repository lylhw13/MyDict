#include "wordlocaldb.h"
#include <QMessageBox>

#define connectname "Local"

WordLocalDB::WordLocalDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",connectname);
    db.setDatabaseName("wordLocal.db");
    if(!db.open())
        QMessageBox::critical(0,tr("Error"),tr("Unable open the wordLocal.db"));

    QString create_sql = "create table dict (id integer primary key, word varchar, remember integer, phonetic nvarchar, paraphrase nvarchar )";
    QSqlQuery sql_query(db);
    if(!sql_query.exec(create_sql))
    {
        //QMessageBox::critical(0,tr("Error"),tr("Unable build the wordLocal.db"));
        qDebug()<<sql_query.lastError();
    }
}

void WordLocalDB::addWord(WordData * wordData)
{
    QSqlDatabase db = QSqlDatabase::database(connectname);
    QSqlQuery q(db);
    q.prepare("insert into dict (word, remember, phonetic, paraphrase) values(?,?,?,?)");

    q.addBindValue(wordData->getWordName());
    q.addBindValue(QString::number(wordData->getMemoryNumber()));
    q.addBindValue(wordData->getPhonetic().join("$"));
    q.addBindValue(wordData->getParaphrase().join("$"));
    if(!q.exec())
        qDebug()<<q.lastError();
}

void WordLocalDB::update(WordData *wordData)
{
    QSqlDatabase db = QSqlDatabase::database(connectname);
    db.open();
    db.exec("PRAGMA locking_mode = EXCLUSIVE");
    QSqlQuery q(db);
//    QString update_sql = "update dict set remember = :remember where word = :word";

//    q.prepare(update_sql);
//    q.bindValue(":remember",wordData->getRem());
//    q.bindValue(":word",wordData->getWordName());
//    if(!q.exec())
//        qDebug()<<q.lastError();
    QString temp = QString("update dict set remember = %1 where word = \'%2\'").arg(wordData->getMemoryNumber()).arg(wordData->getWordName());//注意要加单引号
    if(!q.exec(temp))
        qDebug()<<q.lastError();
}

bool WordLocalDB::isExist(const QString &word)
{
    QSqlDatabase db = QSqlDatabase::database(connectname);
    QSqlQuery q(db);
    q.prepare("selcet word form dict");
    while(q.next())
    {
        QString wordName = q.value(0).toString();
        if(wordName == word)
            return true;
    }
    return false;
}

bool WordLocalDB::queryWord(WordData *wordData)
{
    QString wordId = wordData->getWordName();
    if(wordId.isEmpty())
        return false;
    QSqlDatabase db = QSqlDatabase::database(connectname);
    QSqlQuery q(db);
    QString select_sql = "select * from dict where word = ?";
    q.prepare(select_sql);
    q.addBindValue(wordId);
    if(q.exec())
    {
        if(q.first())
        {
            wordData->setRemFromLocal(q.value("remember").toInt());
            wordData->setPhoneticFromLocal(q.value("phonetic").toString());
            wordData->setParaphraseFromLocal(q.value("paraphrase").toString());
            return true;
        }
    }
    return false;
}

bool WordLocalDB::queryWord(QString name, WordData *wordData)
{
    QString wordId = name;
    if(wordId.isEmpty())
        return false;
    QSqlDatabase db = QSqlDatabase::database(connectname);
    QSqlQuery q(db);
    QString select_sql = "select * from dict where word = ?";
    q.prepare(select_sql);
    q.addBindValue(wordId);
    if(q.exec())
    {
        if(q.first())
        {
            wordData->setRemFromLocal(q.value("remember").toInt());
            wordData->setPhoneticFromLocal(q.value("phonetic").toString());
            wordData->setParaphraseFromLocal(q.value("paraphrase").toString());

            wordData->setWordName(name);
            return true;
        }
    }
    return false;
}


bool WordLocalDB::randowQueryWord(WordData *wordData)
{
    QSqlDatabase db = QSqlDatabase::database(connectname);
    QSqlQuery q(db);
    QString select_max_sql = "select max(id) from dict";
    q.prepare(select_max_sql);
    if(q.exec())
    {
        if(q.first())
        {
            int maxId = q.value(0).toInt();
            if(maxId == 0)
                return false;
            do
            {
                int id = qrand()%maxId+1;
                qDebug()<<q.value(0).toInt()<<"===="<<id;
                q.finish();
                q.prepare("select * from dict where id = ?");
                q.addBindValue(id);
                if(q.exec())
                {
                    if(q.first())
                    {
                        wordData->setWordName(q.value("word").toString());
                        wordData->setRemFromLocal(q.value("remember").toInt());
                        wordData->setPhoneticFromLocal(q.value("phonetic").toString());
                        wordData->setParaphraseFromLocal(q.value("paraphrase").toString());
                    }
                }
            }while(wordData->getWordName().isEmpty());
            return true;
        }
    }
    return false;
}


void WordLocalDB::deleteWord(QString wordTemp)
{
    QSqlDatabase db = QSqlDatabase::database(connectname);
    QSqlQuery q(db);

    q.prepare("delete from dict where word = ?");
    q.addBindValue(wordTemp);
    if(!q.exec())
        qDebug()<<q.lastError();
}










