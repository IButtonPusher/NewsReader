#include "newitemswidget.h"
#include <QObject>
#include <QQmlContext>
#include <QQuickWindow>
#include "mainwindow.h"
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QDateTime>

NewItemsWidget::NewItemsWidget(QWidget *parent) : QWidget(parent)
{
    verticalLayout = new QVBoxLayout(this);
    setLayout(verticalLayout);

    isInitialLoaded = false;
        isBrowserShown = false;



    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(UpdateAnimation()));

    data = new UserData(parent);
        connect(data, SIGNAL(Ready()), this, SLOT(DataAvailable()));
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

    isBrowserShown = false;

    engine = new QQmlApplicationEngine(this);
   QQmlContext *context = engine->rootContext();

   engine->load(QUrl("qrc:/Browser.qml"));

   topLevel = engine->rootObjects().value(0);
   connect(topLevel, SIGNAL(closeButtonClicked()), this,
                  SLOT(BrowserCloseRequested()));
   window = qobject_cast<QQuickWindow *>(topLevel);
   //window->show();
}



void NewItemsWidget::BrowserCloseRequested()
{
    emit BrowserHidden();
    timeClicked = QDateTime::currentDateTime();
    isFadingOut = false;
    animationTimer->start(10);
}



void NewItemsWidget::DataAvailable()
{

    if (isInitialLoaded)
            return;   

        std::vector<Article*> newArticles = data->GetArticles(5);

        AddNewArticles(newsItems.size(), newArticles);

        isInitialLoaded = true;
}

void NewItemsWidget::UpdateAnimation()
{
    //QDateTime now = QDateTime::currentDateTime();
    qint64 elapsed = timeClicked.msecsTo(QDateTime::currentDateTime());
    int pct = (elapsed * 100) / 500;

    if (pct > 100)
        pct = 100;
    if (!isFadingOut)
        pct = 100 - pct;

    for (int i = 0; i < 5; i++)
    {
        NewsSwipeDisplay* nsd = newsItems.at(i);
        if (i % 2 == 0)
            nsd->SetFadeOffset(pct);
        else
            nsd->SetFadeOffset(0 - pct);
    }

    if (pct >= 100 && isFadingOut)
    {
         emit BrowserShown();

        NewsSwipeDisplay* nsd = newsItems.at(articleClicked);

           ////////////////
//            QQmlApplicationEngine* engine = new QQmlApplicationEngine(this);
          // QQmlContext *context = engine->rootContext();
         //  context->setContextProperty(QStringLiteral("initialUrl"),
          //     nsd->myArticle->URL);
//           //if (!isBrowserShown)
//               engine->load(QUrl("qrc:/Browser.qml"));

//           topLevel = engine->rootObjects().value(0);
//           connect(topLevel, SIGNAL(closeButtonClicked()), this,
//                          SLOT(BrowserCloseRequested()));
//           window = qobject_cast<QQuickWindow *>(topLevel);


        //QObject *webber = topLevel->findChild<QObject*>("webber");
        //if (webber)
        //    webber->setProperty("url", nsd->myArticle->URL);

        QVariant returnedValue;
        QVariant msg = nsd->myArticle->URL;
        QMetaObject::invokeMethod(topLevel, "setURL",
                Q_RETURN_ARG(QVariant, returnedValue),
                Q_ARG(QVariant, msg));


        window->show();

           ////////

            isBrowserShown = true;

        animationTimer->stop();

    }
    else if (pct <= 0 && !isFadingOut)
    {
        animationTimer->stop();
    }
}

void NewItemsWidget::ArticleSwiped(int slot)
{
    //move the articles up if they are beneath the swiped one
    for (int i = slot; i < newsItems.size()-1; i++)
    {
         NewsSwipeDisplay* current = newsItems.at(i);
         NewsSwipeDisplay* next = newsItems.at(i+1);

         try
         {
             if (next->myArticle != NULL)
                current->SetArticle(next->myArticle);
         }
         catch (const char* msg) {}
    }


    //
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

        int waffle = newsItems.size();

        if (last != NULL && last->myArticle != NULL)
            last->SetArticle(last->myArticle);
    }
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

void NewItemsWidget::ArticleClicked(int slot)
{
    timeClicked = QDateTime::currentDateTime();
    isFadingOut = true;
    animationTimer->start(10);

    articleClicked = slot;

    return;



}
