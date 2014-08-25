#include "myqgraphicsscene.h"


void MyQGraphicsScene::wheelEvent ( QGraphicsSceneWheelEvent * wheelEvent )
{
    qDebug() << "ping";
    wheelEvent->setDelta(0);
}
