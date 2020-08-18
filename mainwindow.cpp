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

    toolBar =addToolBar("toolbar");
    //toolBar->setGeometry(QRect(0, 0, 704, 200));

    setStyleSheet("QToolButton::menu-indicator{ image: url(none.jpg); }");

//    QLabel* label = new QLabel(this);
//    QPixmap pixmap(":/Images/logo.png");
//    label->setPixmap(pixmap);

//    label->setMask(pixmap.mask());
//    label->show();
//    toolBar->addWidget(label);

    cbBla = new QComboBox(toolBar);

    //cbBla->setStyleSheet("QComboBox { background-color: blue; }");

    cbBla->addItem("Main","Main");
    cbBla->addItem("Politics","Politics");
    cbBla->addItem("Sports","Sports");
    cbBla->addItem("Entertainment","Entertainment");
    cbBla->setGeometry(30,0,200,40);
    cbBla->addItem("Business","Business");
    cbBla->addItem("Technology","Technology");

    QFont fnt;// = new QFont("Tahoma",10,3);
    fnt.setFamily("Tahoma");
    fnt.setPointSize(20);
   // fnt.setBold(true);
    cbBla->setFont(fnt);
   cbBla->setItemData( 0, QColor( Qt::blue ), Qt::BackgroundRole );
   cbBla->setItemData( 0,fnt, Qt::FontRole );
   cbBla->setItemData( 1, QColor( Qt::green ), Qt::BackgroundRole );

    toolBar->addWidget(cbBla);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // toolBar is a pointer to an existing toolbar
    toolBar->addWidget(spacer);

//QToolButton::menu-indicator { image: none; }
    mnuCategory = new QMenu("test");
    test1 = new QAction("test1", this);
    mnuCategory->addAction(test1);
    test2 = new QAction("test2", this);
    mnuCategory->addAction(test2);
    toolButton = new QToolButton(toolBar);

    toolButton->setIcon(QIcon(":/Images/3dot.png"));
    //toolButton->setText("...");
    toolButton->setContextMenuPolicy(Qt::ActionsContextMenu);
    toolButton->setMenu(mnuCategory);
    toolButton->setPopupMode(QToolButton::InstantPopup);
    //toolButton->setGeometry(QRect(200,0,40,40));
    toolBar->addWidget(toolButton);





    newsSwipers = new NewItemsWidget;

    mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(mnuCategory);
    //mainLayout->addWidget(cbBla);
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(newsSwipers);

    centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);





    //setCentralWidget(centralWidget);
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
//    QMessageBox msgBox;
//    msgBox.setText("The document has been modified.");
//    msgBox.exec();

    QQmlContext *context = engine.rootContext();
   //    context->setContextProperty(QStringLiteral("initialUrl"),
   //        nsd->myArticle->URL);
       engine.load(QUrl("qrc:/Browser.qml"));

       QObject *topLevel = engine.rootObjects().value(0);
       QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
       window->show();
       hide();

}

MainWindow::~MainWindow()
{
   // QPushButton *btn =

}
