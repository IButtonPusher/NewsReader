#ifndef ARTICLE_H
#define ARTICLE_H

#include <QObject>
#include <QString>

class Article : public QObject
{
    Q_OBJECT
public:
    explicit Article(QObject *parent = 0);

    int ID;
    QString Site;
    QString Category;
    QString URL;
    QString Preview;
    QString Headline;
    long long Signature;
    int voted;
    int Score;
    bool IsRead; //impacts vote delta magnitude
    int IsSwiped; //can be swiped but not read etc


signals:

public slots:
};

#endif // ARTICLE_H
