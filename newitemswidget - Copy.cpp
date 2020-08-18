#include "newitemswidget.h"
#include "newsswipedisplay.h"
#include "article.h"
//#include "articleviewerwidget.h"
//#include "utils.h"
#include <qapplication.h>
#include <QUrl>
#include <QQuickView>
#include <QQuickWindow>
#include <QQmlContext>

NewItemsWidget::NewItemsWidget(QWidget *parent) : QWidget(parent)
{
    isInitialLoaded = false;
    isBrowserShown = false;

    data = new UserData(parent);
    connect(data, SIGNAL(Ready()), this, SLOT(DataAvailable()));

    verticalLayout = new QVBoxLayout(this);
    setLayout(verticalLayout);

    for (int i = 0; i < 5; i++)
    {
        NewsSwipeDisplay* nsd = new NewsSwipeDisplay(i,
            data, centralWidget);
        newsItems.push_back(nsd);
        verticalLayout->addWidget(nsd);
        connect(nsd, SIGNAL(Swiped(int)),this,
            SLOT(ArticleSwiped(int)));
        connect(nsd, SIGNAL(Clicked(int)),this,
            SLOT(ArticleClicked(int)));
    }



    if (data->IsReady)
        DataAvailable();
}

void NewItemsWidget::AddNewArticles(int startIndex,
        std::vector<Article*> newArticles)
{

    data->database->AddLog("adding " + QString::number(newArticles.size()) +
        " to UI");

    for (int i = 0; i < newArticles.size(); i++)
    {
        //articlesAdded++;
        Article* na = newArticles.at(i);
        NewsSwipeDisplay* nsd = newsItems.at(i);
        nsd->SetArticle(na);

    }
}

void NewItemsWidget::Test2()
{
    //emit BrowserShown();
   // browserWindow->close();
    //delete browserWindow;
    emit BrowserHidden();
}

void NewItemsWidget::DataAvailable()
{
    if (isInitialLoaded)
        return;

//    int needed = 4 - articlesAdded;// newsItems.size();

//    if (needed <= 0)
//        return;

    std::vector<Article*> newArticles = data->GetArticles(5);

    AddNewArticles(newsItems.size(), newArticles);

    isInitialLoaded = true;
}

void NewItemsWidget::ArticleClicked(int slot)
{
    NewsSwipeDisplay* nsd = newsItems.at(slot);

    ////////////////

    QQmlContext *context = engine.rootContext();
    context->setContextProperty(QStringLiteral("initialUrl"),
        nsd->myArticle->URL);
    if (!isBrowserShown)
        engine.load(QUrl("qrc:/Browser.qml"));

    QObject *topLevel = engine.rootObjects().value(0);
    connect(topLevel, SIGNAL(bitchFucker()), this,
                   SLOT(Test2()));
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    window->show();

     isBrowserShown = true;
emit BrowserShown();
    ////////////////

    //ArticleViewerWidget* avw = new ArticleViewerWidget(
     //           (MainWindow*)QWidget::window (), nsd,centralWidget);
    //verticalLayout->addWidget(avw);
}

void NewItemsWidget::ArticleSwiped(int slot)//NewsSwipeDisplay* nsd)
{
    int test = newsItems.size();

    //NewsSwipeDisplay* nsd = newsItems.at(slot);

    for (int i = slot; i < newsItems.size()-1; i++)
    {
         NewsSwipeDisplay* current = newsItems.at(i);
         NewsSwipeDisplay* next = newsItems.at(i+1);

         current->SetArticle(next->myArticle);
    }

    std::vector<Article*> newArticles = data->GetArticles(1);

    if (newArticles.size() > 0)
    {
        NewsSwipeDisplay* last = newsItems.back();
        last->SetArticle(newArticles.front());
    }
    else
    {
        Article* emAr;
        NewsSwipeDisplay* last = newsItems.back();
        last->SetArticle(emAr);
    }
}

