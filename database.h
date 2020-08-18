#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include "filedownloader.h"
#include "database.h"
#include <QString>
#include "article.h"
#include <QSet>

class Database: public QObject
{
     Q_OBJECT

public:
    explicit Database(QObject *parent);
     virtual ~Database();
    QString UserID;
    bool IsReady;
    std::vector< Article*> GetFreshArticles(int limit);
    void DeleteOldArticles();
    bool InsertArticle(Article *toInsert);
    bool UpdateArticle(Article *toUpdate);
    void SetArticleRead(Article *toChange);
    void AddLog(QString info);
    //int maxArticleIDPulled;

protected:
    QSqlDatabase database;
    FileDownloader *fdNewUser;
    QString Sanitize(QString input);
    QSet<int> dbReturned;

protected slots:
    void NewUserReceived();

signals:
    void Ready();

};

#endif // DATABASE_H
