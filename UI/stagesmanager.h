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


class StagesManager : QObject
{
    Q_OBJECT
public:
    StagesManager(QGraphicsView *view);
public slots:
    void render();
protected:

  bool eventFilter(QObject * obj, QEvent * event);

private :
    QGraphicsView* mView;
    QGraphicsScene* mScene;
    QStringList stages;
    int currentStage;
};

#endif // STAGESMANAGER_H
