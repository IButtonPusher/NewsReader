#ifndef USERDATA_H
#define USERDATA_H
#include "database.h"
#include <QObject>
#include <QString>
#include "article.h"
#include <QDateTime>

class UserData : public QObject
{
    Q_OBJECT
public:
     Database *database;
    explicit UserData(QObject *parent);
    std::vector< Article*> GetArticles(int count);
    Article* GetNewArticle();
    bool IsReady;
    void ReportSwipe(Article* article, int delta);


private:

    std::vector<Article*> availableArticles;
    QString XmlDecode(QString input);
    QDateTime lastInternetError;

signals:
    void Ready();

private slots:
    void DatabaseReady();
    void ArticlesDownloaded();

protected:
    FileDownloader *fdWeb;


};

#endif // USERDATA_H
