#ifndef NEWITEMSWIDGET_H
#define NEWITEMSWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "newsswipedisplay.h"
#include "userdata.h"
#include <QQmlApplicationEngine>

class NewItemsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewItemsWidget(QWidget *parent = 0);
    QVBoxLayout *verticalLayout;
    QWidget *centralWidget;

private:
    QQmlApplicationEngine engine;
     bool isBrowserShown;
    bool isInitialLoaded;
    UserData* data;
    std::vector<NewsSwipeDisplay*> newsItems;
    void AddNewArticles(int startIndex, std::vector<Article*> newArticles);

signals:
    void BrowserShown();
    void BrowserHidden();
public slots:
    void DataAvailable();
    void Test2();
    void ArticleSwiped(int slot);
    void ArticleClicked(int slot);
};

#endif // NEWITEMSWIDGET_H
