#include "stagesmanager.h"

StagesManager::StagesManager(MyGraphicsView *view)
{
    mView = view;

    mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mScene = new QGraphicsScene(mView);
    mView->setScene(mScene);
    render();



    mView->verticalScrollBar()->installEventFilter(this);
    mView->horizontalScrollBar()->installEventFilter(this);

    stages.append("Initialisation");
    enabled.push_back(true);
    stages.append("Réception données");
    enabled.push_back(false);
#ifdef TROLL
    stages.append("GPS OK Isomewre");
#else
    stages.append("GPS OK");
#endif
    enabled.push_back(false);
    stages.append("Ballon lancé");
    enabled.push_back(false);
    stages.append("Apogée atteinte");
    enabled.push_back(false);

    currentStage = 0;
}

bool StagesManager::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::Wheel) {
        QWheelEvent *ev = static_cast<QWheelEvent*>(event);
        int elementWidth = ELEMENT_WIDTH;

        if(stages.size() != 0) {
            if(mView->width() / stages.size() > elementWidth)
                elementWidth = mView->width() / stages.size();
        }


        int width = elementWidth*stages.size() - mView->width();

        int value = mView->horizontalScrollBar()->value() - ev->delta();
        if(value > width)
            value = width;
        mView->horizontalScrollBar()->setValue(value);
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

        mScene->addLine(QLine(QPoint(xdebut, 0), QPoint(xfin, height/2 - 1)), QPen(QColor(0,0,0,196)));
        mScene->addLine(QLine(QPoint(xdebut, height), QPoint(xfin, height/2)), QPen(QColor(0,0,0,240)));


        QGraphicsTextItem *texte = mScene->addText(stages.at(i));
        texte->setPos((i+0.5)*elementWidth - (texte->document()->size().width()/2) ,height - 50);

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

        bool isEnabled = enabled.at(i);

        if(i<currentStage && isEnabled == true)
            color = QColor(20,100,20,196);
        else if(i<currentStage && isEnabled == false)
            color = QColor(120,120,20,196);
        else if(i==currentStage && isEnabled == true)
            color = QColor(80,200,80,196);
        else if(i==currentStage && isEnabled == false)
            color = QColor(180,180,30,196);
        else
            color = QColor(196,0,0,196);

        mScene->addPolygon(QPolygonF(points),QPen(), QBrush(color));
    }

    mScene->addLine(0, 15, width-arrowWidth , 15, QPen(QColor(0,0,0,196)));
}


void StagesManager::unlockDataReceivedStage() {
    enabled[1] = true;
    unlockStage(1);
}

void StagesManager::unlockGPSFixStage() {
    enabled[2] = true;
    unlockStage(2);
}

void StagesManager::unlockLaunchStage() {
    enabled[3] = true;
    unlockStage(3);
}

void StagesManager::unlockApogeeStage() {
    enabled[4] = true;
    unlockStage(4);
}

void StagesManager::animation(int v) {
    mView->horizontalScrollBar()->setValue(v);
}

void StagesManager::unlockStage(int n) {
    if(currentStage < n) {

        int width = mView->width();
        QTimeLine* tl = new QTimeLine(1000, this);

        int elementWidth = 200;

        if(stages.size() != 0) {
            if(width / stages.size() > elementWidth)
                elementWidth = width / stages.size();
        }

        tl->setFrameRange(mView->horizontalScrollBar()->value(), 100+currentStage*elementWidth);
        connect(tl, SIGNAL(frameChanged(int)), this, SLOT(animation(int)));
        tl->start();

        currentStage = n;
        render();
    }
}

void StagesManager::unlockNextStage() {
    if(currentStage < stages.size() - 1) {

        int width = mView->width();
        QTimeLine* tl = new QTimeLine(1000, this);

        int elementWidth = 200;

        if(stages.size() != 0) {
            if(width / stages.size() > elementWidth)
                elementWidth = width / stages.size();
        }

        tl->setFrameRange(mView->horizontalScrollBar()->value(), 100+currentStage*elementWidth);
        connect(tl, SIGNAL(frameChanged(int)), this, SLOT(animation(int)));
        tl->start();


        currentStage++;
        render();
    }
}

void StagesManager::resetStage() {
    currentStage = 0;
    render();

    for(int i=1;i<enabled.size();i++)
        enabled[i] = false;
}

void StagesManager::goToPreviousStage() {
    if(currentStage > 0) {
        int width = mView->width();
        QTimeLine* tl = new QTimeLine(1000, this);

        int elementWidth = 200;

        if(stages.size() != 0) {
            if(width / stages.size() > elementWidth)
                elementWidth = width / stages.size();
        }

        tl->setFrameRange(mView->horizontalScrollBar()->value(), 100+(currentStage-2)*elementWidth);
        connect(tl, SIGNAL(frameChanged(int)), this, SLOT(animation(int)));
        tl->start();

        currentStage--;
        render();
    }
}
