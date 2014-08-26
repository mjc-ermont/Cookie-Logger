#ifndef STAGESMANAGER_H
#define STAGESMANAGER_H

#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsDropShadowEffect>
#include <QStringList>
#include <QDebug>
#include <QScrollBar>
#include <QEvent>
#include <QWheelEvent>
#include <QTextDocument>

#include <QGraphicsItemAnimation>
#include <QTimeLine>

#include "mygraphicsview.h"


class StagesManager : public QObject
{
    Q_OBJECT
public:
    StagesManager(MyGraphicsView *view);
    void unlockDataReceivedStage();
    void unlockGPSFixStage();
    void unlockLaunchStage();
    void unlockApogeeStage();
    void unlockStage(int n);
    void unlockNextStage();
    void resetStage();
    void goToPreviousStage();


public slots:
    void render();
    void animation(int);
protected:

  bool eventFilter(QObject * obj, QEvent * event);

private :
    MyGraphicsView* mView;
    QGraphicsScene* mScene;
    QStringList stages;
    int currentStage;

    QVector<bool> enabled;
};

#endif // STAGESMANAGER_H
