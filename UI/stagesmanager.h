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


class StagesManager : public QObject
{
    Q_OBJECT
public:
    StagesManager(QGraphicsView *view);
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
protected:

  bool eventFilter(QObject * obj, QEvent * event);

private :
    QGraphicsView* mView;
    QGraphicsScene* mScene;
    QStringList stages;
    int currentStage;

    QVector<bool> enabled;
};

#endif // STAGESMANAGER_H
