#include "mainwindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QObject>
#include <QQmlContext>
#include <QQuickWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include "newitemswidget.h"
#include <QtWidgets/QAction>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLabel>
#include <QBitmap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    toolBar = addToolBar("toolbar");    

    setStyleSheet("QToolButton::menu-indicator{ image: url(none.jpg); }");


    cbBla = new QComboBox(toolBar);    

    cbBla->addItem("Main","Main");
    cbBla->addItem("Politics","Politics");
    cbBla->addItem("Sports","Sports");
    cbBla->addItem("Entertainment","Entertainment");
    cbBla->setGeometry(30,0,200,40);
    cbBla->addItem("Business","Business");
    cbBla->addItem("Technology","Technology");

    QFont fnt;
    fnt.setFamily("Tahoma");
    fnt.setPointSize(20);
   
    cbBla->setFont(fnt);
   cbBla->setItemData( 0, QColor( Qt::blue ), Qt::BackgroundRole );
   cbBla->setItemData( 0,fnt, Qt::FontRole );
   cbBla->setItemData( 1, QColor( Qt::green ), Qt::BackgroundRole );

    toolBar->addWidget(cbBla);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // toolBar is a pointer to an existing toolbar
    toolBar->addWidget(spacer);


    mnuCategory = new QMenu("test");
    test1 = new QAction("test1", this);
    mnuCategory->addAction(test1);
    test2 = new QAction("test2", this);
    mnuCategory->addAction(test2);
    toolButton = new QToolButton(toolBar);

    toolButton->setIcon(QIcon(":/Images/3dot.png"));

    toolButton->setContextMenuPolicy(Qt::ActionsContextMenu);
    toolButton->setMenu(mnuCategory);
    toolButton->setPopupMode(QToolButton::InstantPopup);

    toolBar->addWidget(toolButton);





    newsSwipers = new NewItemsWidget;

    mainLayout = new QVBoxLayout;
    
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(newsSwipers);

    centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(newsSwipers, SIGNAL(BrowserShown()), this,
        SLOT(HideMe()));
    connect(newsSwipers, SIGNAL(BrowserHidden()), this,
        SLOT(ShowMe()));
    mainLayout->addWidget(newsSwipers);
    resize(400,600);

}

void MainWindow::HideMe()
{
    hide();
}

void MainWindow::ShowMe()
{
    show();
}

void MainWindow::Test()
{

    QQmlContext *context = engine.rootContext();
   
       engine.load(QUrl("qrc:/Browser.qml"));

       QObject *topLevel = engine.rootObjects().value(0);
       QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
       window->show();
       hide();

}

MainWindow::~MainWindow()
{
   

}
