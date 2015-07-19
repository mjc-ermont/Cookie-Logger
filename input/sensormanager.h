#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "sensor.h"
#include "sensorvalue.h"
#include "FenPrincipale.h"
#include "tablemgr.h"
#include "databasecontroller.h"

#include <QVector>
#include <QHttp>
#include <QUrl>
#include <QtXml/QtXml>

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
    DatabaseController* getDB() { return bdd;}

    int indexOf(int idc, int idv);
    SensorValue* valueAt(int i) {return sensorValueList[i];}

public slots:
    void newValue(int id_capteur, int id_valeur, double valeur);
    void newFrame(QVector<double>);
protected:
    void getSensorsFromFile();
private:
    QVector<Sensor*> sensorList;
    QVector<SensorValue*> sensorValueList;
    QVector<int> m_structure;

    FenPrincipale* parent;
    DatabaseController* bdd;
    double valeur;

    Q_OBJECT
};

#endif // SENSORMANAGER_H
