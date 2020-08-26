#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <ctime>
#include <QStandardPaths>
#include <qdesktopservices.h>

Database::Database(QObject *parent) : QObject(parent)
{
    QString basePath = QStandardPaths::standardLocations(
                QStandardPaths::HomeLocation)[0];    

    const QStringList __picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);

    basePath = __picturesLocations.back();    

    QString path = basePath + "/livelocal.ror";

    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and not a directory?
    bool isExists = check_file.exists() && check_file.isFile();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();

    if (!isExists)
    {
        IsReady = false;

        QUrl newUser("http://www.assaultware.com/oyn.aspx?op=hello");

        fdNewUser = new FileDownloader(newUser, this);
        connect(fdNewUser, SIGNAL (downloaded()), this,
                SLOT (NewUserReceived()));


        QSqlQuery query;
        query.exec("create table Hello "
                  "(id integer primary key, "
                  "userid TEXT)");

        query.exec("create table LateBreaking "
          "(id integer primary key, "
          "Site Text, Category Text, Score integer,"
           "URL Text, Preview Text, Headline Text,"
           "Voted integer, Added DateTime, Signature bigint, "
                   "IsSwiped bit)");

        query.exec("create table LogInfo(Info Text)");

        query.exec("create index Articles_vote on LateBreaking(IsSwiped)");
        query.exec("create index Articles_score on LateBreaking(Score)");
    }
    else
    {
        QSqlQuery query("SELECT userid FROM Hello");
        query.next();
        QString lol = query.value(0).toString();
        UserID = query.value(0).toString();
        IsReady = true;
        emit Ready();
    }
}

void Database::AddLog(QString info)
{
    QString str = "insert into LogInfo VALUES ('" + info + "')";
    qDebug() << info;
    QSqlQuery query;
    query.exec(str);
}

QString Database::Sanitize(QString input)
{
    return input.replace("'","''").replace("\\","\\\\")
            .replace(";","");
}


Database::~Database()
{

}

std::vector<Article *> Database::GetFreshArticles(int limit)
{    

    std::vector<Article *> toReturn;
    QString str = "SELECT * FROM LateBreaking WHERE IsSwiped=0 ORDER BY Score DESC";

    QSqlQuery query(str);

    AddLog("Asking database for new articles");

    while (query.next())
    {
        Article *current = new Article();
        current->ID = query.value(0).toInt();
        current->Site = query.value(1).toString();
        current->Category = query.value(2).toString();
        current->Score = query.value(3).toInt();
        current->URL = query.value(4).toString();
        current->Preview = query.value(5).toString();
        current->Headline = query.value(6).toString();
        current->voted = query.value(7).toInt();
        current->Signature = query.value(9).toLongLong();

        if (dbReturned.contains(current->ID))
            continue;

        dbReturned.insert(current->ID);        

        AddLog("loaded " + current->Headline + " from db");

        toReturn.push_back(current);
    }

    AddLog("Returning " + QString::number(toReturn.size()));

    return toReturn;

}

void Database::DeleteOldArticles()
{
     QDateTime yesterday = QDateTime::currentDateTime().addDays(-1);
QSqlQuery query;
QString qstr = "DELETE FROM LateBreaking WHERE Added < '" +
        yesterday.toString("yyyy-MM-dd HH:mm:ss") + "';";

    query.exec(qstr);
}

bool Database::UpdateArticle(Article *toUpdate)
{
    QDateTime now = QDateTime::currentDateTime();
    QString dt = now.currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    //now

    QSqlQuery query;



    //t->

    QString str =
"update LateBreaking SET Score = " +
            QString::number(toUpdate->Score) +
            " WHERE ID = " + toUpdate->ID;

    bool res = query.exec(str);

    AddLog(res + "updating article " + toUpdate->Headline +
           " " + QString::number(res));

    return res;
}

bool Database::InsertArticle(Article *toInsert)
{
    QDateTime now = QDateTime::currentDateTime();
    QString dt = now.currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    //now

    QSqlQuery query;



    //t->

    QString str =
"insert into LateBreaking(id, Site,Category, URL, Preview, Headline, Added, Signature, Score, voted, IsSwiped) VALUES (" +

    QString::number(toInsert->ID) + ",'" +
            toInsert->Site +"','" +
            Sanitize(toInsert->Category) + "','" +
            Sanitize(toInsert->URL) + "','" +
            Sanitize(toInsert->Preview) +"','" +
            Sanitize(toInsert->Headline) + "','" +
            dt + "'," + QString::number(toInsert->Signature) + "," +
            QString::number(toInsert->Score) +
            ",0,0);";

    bool res = query.exec(str);

    AddLog(res + "Adding article " + toInsert->Headline);

    return res;
}

void Database::SetArticleRead(Article *toChange)
{
    QSqlQuery query;

    //t->

    QString str = "update LateBreaking SET IsSwiped=1 WHERE ID = "+
            QString::number(toChange->ID);

    query.exec(str);
}



void Database::NewUserReceived()
{
    QSqlQuery query;
    QByteArray qba = fdNewUser->downloadedData();
    QString xml = QString(qba);

    int id = 0;

    QDomDocument qdd;
    qdd.setContent(xml);

    QDomElement docElem = qdd.documentElement();
    int cnt = docElem.attributes().count();

    if (cnt > 0)
    {
        for (int i = 0; i < cnt; i++)
        {
            QDomAttr attr = docElem.attributes().item(i).toAttr();
            if (attr.name() == "id")
                id = attr.value().toInt();
            else
                UserID = attr.value();
        }
    }
    else
    {

        QDomNode child = docElem.firstChild();

        QDomNamedNodeMap gfy = child.attributes();

        int cnt = gfy.count();


        for (int i = 0; i < cnt; i++)
        {
            QDomAttr attr = gfy.item(i).toAttr();
            if (attr.name() == "id")
                id = attr.value().toInt();
            else
                UserID = attr.value();
        }
    }

    QString str = "insert into Hello(id, userid) VALUES (" +
            QString::number(id) + ",'" + UserID + "');";

    query.exec(str);

    emit Ready();
}
