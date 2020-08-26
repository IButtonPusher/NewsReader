#include "newsswipedisplay.h"
#include <QPainter>
#include <QtWidgets>
#include <QPoint>
#include <QTouchEvent>
#include <QPalette>
#include <QFont>
#include "filedownloader.h"


NewsSwipeDisplay::NewsSwipeDisplay(int slotNumber,
    UserData* data,QWidget *parent)
{
    this->data = data;
    isSet = false;

    fadeOffset = 0;
    isTouching = false;

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(Pal);

    CurrentSlot = slotNumber;
    setAttribute(Qt::WA_AcceptTouchEvents);
    //SetArticle(article);
    currentBrush = QBrush(Qt::black);

    myArticle = NULL;
}

void NewsSwipeDisplay::SetArticle(Article *article)
{
    myArticle = article;
    up1 = QPoint(0,0);
    up2 = QPoint(0,0);
    down1 = QPoint(0,0);
    down2 = QPoint(0,0);
    current1 = QPoint(0,0);
    current2 = QPoint(0,0);

    leftRect = QRect(0,0,0,0);
    //leftRect = QRect(0,0,100,40);
    rightRect = QRect(0,0,0,0);


    //set image
    try
    {
    QString sUrl = "http://www.assaultware.com/oyn/" +
            QString::number(myArticle->Signature) + ".png";
    contentImage.fill(Qt::transparent);
    QUrl imageUrl(sUrl);
    m_pImgCtrl = new FileDownloader(imageUrl, this);
    connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (loadImage()));
    }
    catch (const char* msg)
    {
    }

    isSet=true;
    repaint();
}

void NewsSwipeDisplay::SetFadeOffset(int value)
{
    fadeOffset = value;
    repaint();
}

void NewsSwipeDisplay::TouchBegin(QTouchEvent *touch)
{
    isTouching =true;

    down1 = mapFromGlobal(touch->touchPoints().first().pos().toPoint());
    downTime = QDateTime::currentDateTime();
    //toPrint = "TouchBegin " + QString::number(down1.x());

    up1 = QPoint(0,0);
    down2 = QPoint(0,0);
    up2 = QPoint(0,0);
    repaint();
}

void NewsSwipeDisplay::TouchUpdate(QTouchEvent *touch)
{
    QList<QTouchEvent::TouchPoint> touchPoints = touch->touchPoints();
    int i = 0;

    foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints)
    {
        switch (touchPoint.state())
        {
        case Qt::TouchPointStationary:
        case Qt::TouchPointMoved:
            if (touchPoint.id() == 0)
                current1 = mapFromGlobal(touchPoint.pos().toPoint());
            else if (touchPoint.id() == 1)
                current2 = mapFromGlobal(touchPoint.pos().toPoint());

            continue;
        case Qt::TouchPointReleased:
        {
            if (touchPoint.id() == 0)
            {
                up1 = mapFromGlobal(touchPoint.pos().toPoint());
                current1 = QPoint(0,0);
            }
            else if (touchPoint.id() == 1)
            {
                up2 = mapFromGlobal(touchPoint.pos().toPoint());
                current2 = QPoint(0,0);
            }

            continue;
        }

        case Qt::TouchPointPressed:
        {

            if (touchPoint.id() == 0)
            {
                down1 = mapFromGlobal(touchPoint.pos().toPoint());
                current1 = mapFromGlobal(touchPoint.pos().toPoint());
                up1 = QPoint(0,0);
            }
            else if (touchPoint.id() == 1)
            {
                down2 = mapFromGlobal(touchPoint.pos().toPoint());
                current2 = mapFromGlobal(touchPoint.pos().toPoint());
                up2 = QPoint(0,0);
            }
        }
            continue;
        }

        i++;
    }

    if (up1.x() != 0 || up1.y() != 0 || up2.x() != 0 || up2.y() != 0)
        HandleTouchEnd();
    else
        HandleTouchUpdate();

}

void NewsSwipeDisplay::TouchEnd(QTouchEvent *touch)
{    
    isTouching = false;

    up1 = mapFromGlobal(touch->touchPoints().first().pos().toPoint());
    up1 = QPoint(0,0);
    HandleTouchEnd();    
}

void NewsSwipeDisplay::HandleTouchEnd()
{



    if (width() >0)
    {

        int yesPct = (leftRect.width() * 100) / width();
        int noPct = (rightRect.width() * 100) / width();
        int delta = 0;

        QString debugStr2 = "touch end " +
                QString::number(leftRect.width()) + " " + QString::number(rightRect.width())
                + " " + QString::number(noPct);
                + " " + QString::number(yesPct);

        qDebug() << debugStr2;




        if (noPct >= 50 && yesPct >= 50)
        {
            //ok

            data->ReportSwipe(myArticle, 0);
            emit Swiped(this->CurrentSlot);
            return;

        }
        else if (noPct >= 50)
        {
            //no

            if (myArticle->IsRead)
                delta = -2;
            else
                delta = -1;

        }
        else if (yesPct >= 50)
        {
            //yes
            if (myArticle->IsRead)
                delta = 2;
            else
                delta = 1;
        }
        else if (noPct <= 5 && yesPct <= 5)
        {
            qint64 elapsed = downTime.msecsTo(
                        QDateTime::currentDateTime());

            qDebug() << "clicked from handletouchend";

            if (elapsed < 1000)
                emit Clicked(CurrentSlot);
        }

        if (delta != 0)
        {
            data->ReportSwipe(myArticle, delta);
            emit Swiped(this->CurrentSlot);
            return;
        }
    }



    up1 = QPoint(0,0);
    up2 = QPoint(0,0);
    down1 = QPoint(0,0);
    down2 = QPoint(0,0);
    current1 = QPoint(0,0);
    current2 = QPoint(0,0);

    leftRect = QRect(0,0,0,0);    
    rightRect = QRect(0,0,0,0);

    repaint();
}

void NewsSwipeDisplay::HandleTouchUpdate()
{
    QString debugStr = "Hello there " +
            QString::number(down1.x()) + " " + QString::number(down2.x());

    qDebug() << debugStr;    

    if (down2.x() == 0 && down2.y() == 0)
    {
        int delta = current1.x() - down1.x();        

        if (delta > 0) //swipe right (no)
        {
            int travelled = current1.x() - down1.x();
            int toTravel = width() - down1.x();
            double pctTowardsEdge = (double)travelled /
                    (double)toTravel;

            QString debugStr2 = "moved " +
                    QString::number(travelled) + " " + QString::number(toTravel);

            qDebug() << debugStr2;

            int rectCover = (int)(pctTowardsEdge * width());
            leftRect = QRect(0,0, rectCover, height());
            currentBrush = QBrush(Qt::green);

        }
        else //swipe left (yes)
        {
            int travelled = down1.x() - current1.x();
            int toTravel = down1.x();
            double pctTowardsEdge = (double)travelled /
                    (double)toTravel;

            int rectCover = (int)(pctTowardsEdge * width());
            leftRect = QRect(0,0,0,0);
            rightRect = QRect(width()-rectCover,0, rectCover, height());
            currentBrush = QBrush(Qt::red);
        }
    }
    else
    {
        int startDistance = abs(down1.x() - down2.x());
        int currentDistance = abs(current1.x() - current2.x());

        if (currentDistance > startDistance) //swiping out (ok)
        {
            int toTravel = width() - startDistance;
            double pct = (double)currentDistance / (double)toTravel;
            int rectCover = (int)(pct * width());
            int left = (width() / 2) - (rectCover / 2);

            leftRect = QRect(0,0,0,0);
            rightRect = QRect(left,0, rectCover, height());
            currentBrush = QBrush(Qt::gray);

        }
        else //swiping in (ok)
        {
            //+60 is so they don't have to exactly meet
            //(fingers can't overlap)

            if (currentDistance != startDistance)
            {
                int endDistance = 100;
                int progress = (startDistance - currentDistance) +
                        endDistance;

                double pct = (double)progress /
                        (double)(startDistance - endDistance);
                

                int rectCover = (int)(pct * width()) / 2;
                leftRect = QRect(0,0,rectCover,height());
                rightRect = QRect(width()-rectCover,0,rectCover,height());
                currentBrush = QBrush(Qt::gray);
            }
        }


    }

    repaint();
}

void NewsSwipeDisplay::HandlePaint()
{
    if (!isSet)
        return;

    QPainter painter(this);

    if (myArticle->Headline == "" || fadeOffset == 100)
    {
        QPen whitePen(Qt::white, 10);
        painter.setPen(whitePen);
        painter.drawRect(0,0, width(), height());
        return;
    }

    int leftOffset = ((fadeOffset * width() / 100));

    if (isTouching)
    {
        QPen lgreyPen(Qt::lightGray, 10);
        painter.setPen(lgreyPen);
        painter.drawRect(0,0, width(), height());
    }

    //swipe rectangles
    {


        if (leftRect.width() > 0)
        {
            painter.fillRect(leftRect, currentBrush);
            leftOffset = leftRect.width();
        }

        if (rightRect.width() > 0)
        {
            painter.fillRect(rightRect, currentBrush);
            leftOffset = 0 - rightRect.width();
        }
    }

    int maxHeight = height()-10;
    QSize imgSize;

    QSize originalSize =contentImage.toImage().size();

    if (originalSize.height() > maxHeight)
    {
        float ratio = maxHeight /(float)originalSize.height();
        int newWidth = originalSize.width() * ratio;
        imgSize = QSize(newWidth, maxHeight);
    }
    else
        imgSize = originalSize;

    int startY = (height() / 2) - (imgSize.height() / 2);
    QRect imgDisplay = QRect(20 + leftOffset,startY, imgSize.width(),
        imgSize.height());
    painter.drawImage(imgDisplay, contentImage.toImage());

    if (leftRect.width() > 0)
        painter.fillRect(leftRect, currentBrush);

    if (rightRect.width() > 0)
        painter.fillRect(rightRect, currentBrush);

    int startx = imgDisplay.width() + imgDisplay.x() + 40;
    int headlineX = startx;
    if (headlineX < 0)
        headlineX = 0;

    int www = width();

    int hrectWidth =this->width()-imgDisplay.width()-80;

    QRectF headlineRect = QRectF(startx,imgDisplay.y(), hrectWidth,
                             (this->height() * 52) / 100);
    QPen blackPen(QColor(40,40,40), 10);
    painter.setPen(blackPen);

    QFont font = painter.font();
    font.setPointSize(14);
    painter.setFont(font);
    QString printable = "";
    bool isTruncated = false;

    //make sure we don't use too much space
    {
        QRegExp rx("(\\ |\\-|\\.|\\:|\\,)");

        QStringList list2 = myArticle->Headline.
                split(rx, QString::SkipEmptyParts);

        QString currentLine = "";
        QString currentWord = "";


        int heightUsed = 0;

        for (int i = 0; i < list2.length(); i++)
        {
            currentWord = list2.at(i) + " ";
            currentLine += currentWord;
            QFontMetrics fm(font);
            int width=fm.width(currentLine);
            if (width > headlineRect.width())
            {
                //this will wrap to the next line

                heightUsed += fm.height();
                currentLine = list2.at(i);

                if (heightUsed + fm.height() > headlineRect.height())
                {
                    //no room for another line => we have to cut it off here
                    //printable += "...";
                    isTruncated = true;
                    break;
                }
                else
                {
                    printable += currentWord;
                }
            }
            else
            {
                printable += currentWord;
            }

        }
    }

    printable = myArticle->Headline.left(printable.length());
    if (isTruncated)
        printable += "...";

    painter.drawText(headlineRect, printable);   



    QPen grayPen(Qt::gray, 10);
    painter.setPen(grayPen);
    font.setPointSize(10);
    painter.setFont(font);

    int spaceFromBottom = 50;
    spaceFromBottom = height() - (headlineRect.y() + headlineRect.height());
    if (spaceFromBottom > 50)
        spaceFromBottom = 50;

    QFontMetrics fm1(font);
    int width1=fm1.width(myArticle->Site);

    QRectF siteRect = QRectF(startx,height()-spaceFromBottom, width1 + 10,spaceFromBottom);
    painter.drawText(siteRect, myArticle->Site);



    int waffle = font.pointSize();
    font.setPointSize(10);//font.pointSize() - 2);
    painter.setFont(font);

    QFontMetrics fm2(font);
    int width2=fm2.width(myArticle->Preview);

    QRectF sigRect = QRectF(www - (width2 + 10), height()-spaceFromBottom,
                            width2, spaceFromBottom);
    bool isDrawSig = sigRect.x() > (siteRect.x() + siteRect.width());

    if (isDrawSig)
        painter.drawText(sigRect, myArticle->Preview);

    font.setPointSize(10);
    painter.setFont(font);


    if (CurrentSlot < 3)
    {
        QPen linePen(QColor(225,225,225), 1);
        painter.setPen(linePen);        
    }

}

void NewsSwipeDisplay::loadImage()
{
     contentImage.loadFromData(m_pImgCtrl->downloadedData());
     repaint();
}


bool NewsSwipeDisplay::event(QEvent *event)
{
    QPoint p = mapFromGlobal(QCursor::pos());

    switch (event->type())
    {
        case QEvent::TouchBegin:
            TouchBegin(static_cast<QTouchEvent *>(event));
        break;
        case QEvent::TouchEnd:
            TouchEnd(static_cast<QTouchEvent *>(event));
        break;
        case QEvent::TouchUpdate:
            TouchUpdate(static_cast<QTouchEvent *>(event));
        break;
    case QEvent::MouseButtonPress:
    {
        QMouseEvent *mEvent = (QMouseEvent*)event;

        if(mEvent->button() == Qt::RightButton)
        {
            data->ReportSwipe(myArticle, -3);
            emit Swiped(CurrentSlot);
        }
        else
        {
            qDebug() << "clicked from mousebuttonpress";
            emit Clicked(CurrentSlot);
        }

        repaint();

    }
        break;
    case QEvent::Paint:
    {
       HandlePaint();

    }

        break;
    case QEvent::MouseButtonRelease:        
        repaint();
        
        break;
    case QEvent::MouseMove:        

        repaint();
        
        break;
    }

    return true;
}
