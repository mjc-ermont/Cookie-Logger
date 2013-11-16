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
#include "databasecontroller.h"

class FenPrincipale;
class Sensor;
class DatabaseController;

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
    DatabaseController* bdd;
    double valeur=0;

    Q_OBJECT
};

#endif // SENSORMANAGER_H
