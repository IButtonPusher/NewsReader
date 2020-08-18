#ifndef NEWSSWIPEDISPLAY_H
#define NEWSSWIPEDISPLAY_H

#include <QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QString>
#include <QPoint>
#include <QRectF>
#include <QTouchEvent>
#include <QBrush>
#include <QPixmap>
#include <QDateTime>
#include "filedownloader.h"
#include "article.h"
#include "userdata.h"


class NewsSwipeDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit NewsSwipeDisplay(int slotNumber,
        UserData* data,QWidget *parent = 0);
    int CurrentSlot;
    void SetArticle(Article *article);
    Article *myArticle;
    void SetFadeOffset(int value);

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void TouchBegin(QTouchEvent *touch);
    void TouchUpdate(QTouchEvent *touch);
    void TouchEnd(QTouchEvent *touch);

    void HandleTouchEnd();
    void HandleTouchUpdate();


private:
    //QString toPrint;
    QPixmap contentImage;

   //

    QPoint down1;
    QPoint up1;
    QPoint current1;

    QPoint down2;
    QPoint up2;
    QPoint current2;

    FileDownloader *m_pImgCtrl;

    //we need two rectangles for the multi touch inwards 'ok' command
    QRectF leftRect;
    QRectF rightRect;

    QBrush currentBrush;
    UserData* data;
    QDateTime downTime;
    bool isTouching;
    bool isSet;

    int fadeOffset;

    void HandlePaint();


signals:
    //void Swiped(NewsSwipeDisplay* nsd);
    void Swiped(int slot);
    void Clicked(int slot);

public slots:
    void loadImage();
};

#endif // NEWSSWIPEDISPLAY_H
