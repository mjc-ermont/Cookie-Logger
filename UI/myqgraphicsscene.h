#ifndef MYQGRAPHICSSCENE_H
#define MYQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QDebug>

class MyQGraphicsScene : public  QGraphicsScene
{
    Q_OBJECT

    public:
        MyQGraphicsScene(QObject *parent = 0) :  QGraphicsScene(parent){

        }

    protected:
         void wheelEvent ( QGraphicsSceneWheelEvent * wheelEvent );
};


#endif // MYQGRAPHICSSCENE_H
