#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <QVector>
#include "sensor.h"
#include "sensorvalue.h"
#include "FenPrincipale.h"
#include "tablemgr.h"
#include "serialdecoder.h"
#include <QHttp>
#include <QUrl>

class FenPrincipale;
class Sensor;


class SensorManager : public QObject
{
public:
    SensorManager(FenPrincipale *_parent);
    Sensor* getSensor(int id);
    QVector<Sensor*> getSensors() {return sensorList;}
    FenPrincipale* getParent(){return parent;}

public slots:
    void newValue(int id_capteur, int id_valeur, double valeur);
protected:
    void getSensorsFromFile();
private:
    QVector<Sensor*> sensorList;
    FenPrincipale* parent;

    Q_OBJECT
};

#endif // SENSORMANAGER_H
