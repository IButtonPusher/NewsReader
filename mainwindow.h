#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQmlEngine>
#include <QStackedWidget>
#include <QPushButton>
#include <QQmlApplicationEngine>
#include "newitemswidget.h"
#include <QVBoxLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    ~MainWindow();

    QPushButton *btn;
    NewItemsWidget *newsSwipers;

private:
    QQmlApplicationEngine engine;
    QVBoxLayout *mainLayout;
    QWidget *centralWidget;

    QMenuBar *menuBar;
    QMenu *mnuCategory;
    QMenu *mnuCategory2;
    QAction *test1;
    QAction *test2;
    QComboBox *cbBla;
    //QWidgetAction *qwa;
    QToolBar *toolBar;
    QToolButton *toolButton;

private slots:
     void Test();
     void HideMe();
     void ShowMe();

};



#endif // MAINWINDOW_H
