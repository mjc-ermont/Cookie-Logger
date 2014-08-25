#include "stagesmanager.h"

StagesManager::StagesManager(QGraphicsView* view)
{
    mView = view;

    mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mScene = new QGraphicsScene(mView);
    mView->setScene(mScene);
    render();

    connect(mScene, SIGNAL(changed(QList<QRectF>)), this, SLOT(render()));

    mView->verticalScrollBar()->installEventFilter(this);

    stages.append("Initialisation");
    stages.append("Réception données");
#ifdef TROLL
    stages.append("GPS OK Isomewre");
#else
    stages.append("GPS OK");
#endif
    stages.append("Ballon lancé");
    stages.append("Apogée");

    currentStage = 1;
}

bool StagesManager::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::Wheel) {
         return true;
    }
    return false;
}

void StagesManager::render() {


    mScene->clear();
    int width = mView->width();
    int height = mView->height();

    const int arrowWidth = 20;
    int elementWidth = 200;

    if(stages.size() != 0) {
        if(width / stages.size() > elementWidth)
            elementWidth = width / stages.size();
    }

    for(int i=0;i<stages.size();i++) {
        int xdebut = (i+1)*(elementWidth) - arrowWidth;
        int xfin   = (i+1)*(elementWidth);

        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(20);
        effect->setYOffset(0);
        effect->setXOffset(5);

        QGraphicsDropShadowEffect* effect2 = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(20);
        effect2->setYOffset(0);
        effect2->setXOffset(5);

        QGraphicsLineItem* l = mScene->addLine(QLine(QPoint(xdebut, 0), QPoint(xfin, height/2 - 1)), QPen(QColor(0,0,0,196)));
        l->setGraphicsEffect(effect);
        QGraphicsLineItem* l2 = mScene->addLine(QLine(QPoint(xdebut, height), QPoint(xfin, height/2)), QPen(QColor(0,0,0,240)));
        l2->setGraphicsEffect(effect2);


        mScene->addText(stages.at(i))->setPos(i*elementWidth,height - 40);

        int decalage = arrowWidth * 15 / (height/2);

        QVector<QPoint> points;
        if(i == 0) {
            points.append(QPoint(i*elementWidth,0));
            points.append(QPoint(i*elementWidth,15));
        } else {
            points.append(QPoint(i*elementWidth - arrowWidth,0));
            points.append(QPoint(i*elementWidth - arrowWidth + decalage,15));
        }
        points.append(QPoint((i+1)*elementWidth - arrowWidth + decalage,15));
        points.append(QPoint((i+1)*elementWidth - arrowWidth,0));

        QColor color;
        if(i<currentStage)
            color = QColor(128,128,128,196);
        else if(i==currentStage)
            color = QColor(0,100,100,196);
        else
            color = QColor(255,100,100,196);
        mScene->addPolygon(QPolygonF(points),QPen(), QBrush(color));
    }

    mScene->addLine(0, 15, width, 15, QPen(QColor(0,0,0,196)));
}
