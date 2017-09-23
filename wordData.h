#ifndef WORDDATA_H
#define WORDDATA_H

#include <QString>
#include <QtNetwork>
#include <QObject>
#include <QMessageBox>
#include <QtSql>

class WordData : public QObject
{
    Q_OBJECT
public:
    WordData();
private:
    QString wordName;
    int memoryNumber;
    QStringList phonetic;
    QStringList paraphrase;

public:
    void setPhoneticFromLocal(const QString &);
    void setParaphraseFromLocal(const QString &);
    void setRemFromLocal(int);
    QStringList & getPhonetic();
    QStringList & getParaphrase();
    QString &getWordName();

    int getMemoryNumber();
    void addMemoryNumber(int);

    bool flagQuery();
    void setWordName(const QString &);
    //bool queryLocal();
    void queryNet();

signals:
    void finishQuery(bool);
private slots:
    void handleNetworkData(QNetworkReply *);
};

#endif // WORDDATA_H
