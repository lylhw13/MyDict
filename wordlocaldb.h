#ifndef WORDLOCALDB_H
#define WORDLOCALDB_H

#include <QtSql>
#include "wordData.h"

class WordLocalDB : public QObject
{
    Q_OBJECT

public:
    WordLocalDB();

public:
   void addWord(WordData *);
   void update(WordData *);
   bool isExist(const QString&);
   bool queryWord(WordData *);
   bool queryWord(QString name, WordData *);

   bool randowQueryWord(WordData *);

   void deleteWord(QString);


private:
    //QSqlDatabase db;
    //QSqlQuery sql_query(db);

};

#endif // WORDLOCALDB_H
