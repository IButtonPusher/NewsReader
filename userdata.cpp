#include "userdata.h"
#include "database.h"
#include "article.h"
#include <QDomDocument>
#include <QMessageBox>


UserData::UserData(QObject *parent) : QObject(parent)
{
    IsReady = false;

    database = new Database(parent);
    connect(database, SIGNAL(Ready()), this, SLOT(DatabaseReady()));

    if (database->IsReady)
        DatabaseReady();

}



void UserData::DatabaseReady()
{

    database->DeleteOldArticles();
    std::vector<Article *> fromDB = database->GetFreshArticles(10);

    lastInternetError = QDateTime::currentDateTime().addDays(-1);

    for(std::vector<Article *>::iterator it = fromDB.begin(); it !=
        fromDB.end(); ++it)
    {
        availableArticles.push_back(*it);
    }

    if (availableArticles.size() >= 10)
    {
        IsReady = true;
        emit Ready();
    }
    else
    {
        QString qstr = "http://www.assaultware.com/oyn.aspx?op=iwanttoknowmore&me=" +
                database->UserID;

        database->AddLog("asking web service for new article " +
            qstr);

        /* Need to pull from the web right away*/
        QUrl loadArticles(qstr);
        fdWeb= new FileDownloader(loadArticles, this);
        connect(fdWeb, SIGNAL(downloaded()), this, SLOT(ArticlesDownloaded()));
    }
}

void UserData::ArticlesDownloaded()
{
    QByteArray qba = fdWeb->downloadedData();
    QString xml = QString(qba);

   
        qint64 elapsed = lastInternetError.msecsTo(
                    QDateTime::currentDateTime());

        if (elapsed >= 3600000) //1 hour?
        {
            lastInternetError = QDateTime::currentDateTime();
        }
  

    QDomDocument qdd;
    qdd.setContent(xml);
    QDomElement docElem = qdd.documentElement();

    QDomNodeList articlesList = docElem.childNodes();

    database->AddLog("downloaded " + QString::number(articlesList.count()));

    for (int i = 0; i < articlesList.count(); i++)
    {
        QDomNode elem = articlesList.at(i);
        QDomNamedNodeMap gfy = elem.attributes();

        int cnt = gfy.count();

        Article *na = new Article();

        for (int j = 0; j < cnt; j++)
        {
            QDomAttr attr = gfy.item(j).toAttr();


            QString nom = attr.name();
            QString val = attr.value();

            if (attr.name() == "ID")
                na->ID = attr.value().toInt();
            else if (attr.name() == "Headline")
                na->Headline = XmlDecode(attr.value());
            else if (attr.name() == "Site")
                na->Site = attr.value();
            else if (attr.name() == "URL")
                na->URL = attr.value();
            else if (attr.name() == "Category")
                na->Category = attr.value();
            else if (attr.name() == "Signature")
                na->Signature = attr.value().toLongLong();
            else if (attr.name() == "SignatureDescription")
                na->Preview = XmlDecode(attr.value());
            else if (attr.name() == "Score")
                na->Score = attr.value().toInt();
        }

        if (!database->UpdateArticle(na))
        {
            if (database->InsertArticle(na) &&
                availableArticles.size() < 10)
            {
                //we are going to pull after we're done saving so we
                //can sort and also avoid reloading
                //availableArticles.push_back(na);
                //if ()
            }
        }
    }

    std::vector<Article *> fromDB = database->GetFreshArticles(10);

    for(std::vector<Article *>::iterator it = fromDB.begin(); it !=
        fromDB.end(); ++it)
    {
        availableArticles.push_back(*it);
    }

    IsReady = true;
    emit Ready();
}

Article* UserData::GetNewArticle()
{    
    Article *toReturn;

    if (availableArticles.size() > 0)
    {
        toReturn = availableArticles.front();
        availableArticles.erase(availableArticles.begin());
    }

    if (availableArticles.size() < 10)
    {
        QUrl loadArticles(
            "http://www.assaultware.com/oyn.aspx?op=iwanttoknowmore&me=" +
                    database->UserID);
        fdWeb= new FileDownloader(loadArticles, this);
        connect(fdWeb, SIGNAL(downloaded()), this, SLOT(ArticlesDownloaded()));
    }

    return toReturn;
}

void UserData::ReportSwipe(Article* article, int delta)
{
    if (article == NULL)
        return;

    try
    {
    database->AddLog("report swipe " + article->Headline + " " +
                     QString::number(delta));

    QString url = "http://www.assaultware.com/oyn.aspx?op=swipe&me=" +
            database->UserID + "&article=" +
            QString::number(article->ID) +
            "&delta=" + QString::number(delta);

    QUrl reportSwipe(url);

    fdWeb= new FileDownloader(reportSwipe, this);
    database->SetArticleRead(article);

    }
    catch (std::exception& e)
    {
    }



}

QString UserData::XmlDecode(QString input)
{
    return input.replace("&amp;", "&").replace("&lt;", "<").replace("&gt;", ">")
            .replace("&quot;", "\"").replace("&apos;", "'")
            .replace("&#x27","'").replace("&#8217","'").replace("&#8216","'")
            .replace("&#8212","-");

}

std::vector<Article *> UserData::GetArticles(int count)
{
    database->DeleteOldArticles();

    std::vector<Article *> toReturn;

    database->AddLog("requesting " + QString::number(count) + " new articles from " +
                     QString::number(availableArticles.size()) + " available");

    while (toReturn.size() < count)
    {
        if (availableArticles.size() == 0)
            break;

        toReturn.push_back(availableArticles.front());
        availableArticles.erase(availableArticles.begin());
    }

    database->AddLog("now available " +
                     QString::number(availableArticles.size()));

    if (availableArticles.size() < 10)
    {
        //try to move more from db to memory

        std::vector<Article *> fromDB = database->GetFreshArticles(count);

        for(std::vector<Article *>::iterator it = fromDB.begin(); it !=
            fromDB.end(); ++it)
        {
            availableArticles.push_back(*it);
        }

        if (availableArticles.size() < 10)
        {
            //try to move more from web to db

            QUrl loadArticles(
                "http://www.assaultware.com/oyn.aspx?op=iwanttoknowmore&me=" +
                        database->UserID);
            fdWeb= new FileDownloader(loadArticles, this);
            connect(fdWeb, SIGNAL(downloaded()), this, SLOT(ArticlesDownloaded()));
        }
    }

    database->AddLog("returning " + QString::number(toReturn.size()) +
                     " articles to UI ");

    return toReturn;
}




