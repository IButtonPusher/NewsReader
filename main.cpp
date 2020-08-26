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
}
