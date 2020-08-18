#ifndef NEWITEMSWIDGET_H
#define NEWITEMSWIDGET_H
#include <QtWidgets/QVBoxLayout>
#include <QPushButton>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "newsswipedisplay.h"
#include "userdata.h"

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QObject>

class NewItemsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewItemsWidget(QWidget *parent = 0);
    QVBoxLayout *verticalLayout;
    QPushButton* btnTest;
    QWidget *centralWidget;
    //QQmlApplicationEngine engine;
    //QQuickWindow *browserWindow;
    //MainWindow frmMain;

private:
    bool isBrowserShown;
    UserData* data;
    bool isInitialLoaded;
    bool isFadingOut;

    QTimer* animationTimer;
    QDateTime timeClicked;
    int articleClicked;
    QObject *topLevel;
    QQuickWindow *window;
    QQmlApplicationEngine* engine;

    std::vector<NewsSwipeDisplay*> newsItems;
    void AddNewArticles(int startIndex, std::vector<Article*> newArticles);

signals:
    void BrowserShown();
    void BrowserHidden();
private slots:
     //void Test();
     void BrowserCloseRequested();
    // void BrowserClosed(QQuickCloseEvent *close);

     void DataAvailable();
     void UpdateAnimation();

     void ArticleSwiped(int slot);
     void ArticleClicked(int slot);

public slots:
};

#endif // NEWITEMSWIDGET_H
