#ifndef BOARDINGTABLE_H
#define BOARDINGTABLE_H

#include "defines.h"
#include "../input/sensormanager.h"
#include <QGridLayout>
#include <QLCDNumber>
#include <QGroupBox>
#include <QLabel>

class SensorManager;
class SensorValue;

class BoardingTable
{
public:
    BoardingTable(QGridLayout *container, SensorManager *mgr);
    ~BoardingTable();

    void init(SensorManager* );
    void update(QVector<Data> values);

    void onResize();

private:
    QGridLayout *m_container;
    QVector<QGridLayout *> capteurs_layouts;
    QVector<QLCDNumber *> valeurs;
    QVector<QLabel*> labels;

    SensorManager* mMgr;
};

#endif // BOARDINGTABLE_H
