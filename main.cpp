#include "mainwindow.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtWebView>


int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    QtWebView::initialize();
    MainWindow w;
    w.show();

    return a.exec();

//        QGuiApplication app(argc, argv);



//        QQmlApplicationEngine engine;
//        //engine.rootContext()->setContextProperty("oslo2Berlin", &oslo2berlin);
//        //engine.rootContext()->setContextProperty("berlin2London", &berlin2london);
//        engine.load(QUrl(QStringLiteral("qrc:/Browser.qml")));

//        return app.exec();

}
