#include <QtXml/QtXml>
#include "sensormanager.h"

SensorManager::SensorManager(FenPrincipale* _parent) {
    getSensorsFromFile();
    parent = _parent;


    valeur=0;
    bdd = new DatabaseController();
    connect(bdd, SIGNAL(rangeStartUpdate(QDateTime)),parent,SLOT(onRangeStartUpdate(QDateTime)));

    bdd->start();
    bdd->read(1,0,QDateTime::currentDateTime().addDays(-1),QDateTime::currentDateTime());
}

void SensorManager::newValue(int id_capteur, int id_valeur, double valeur) {
    qDebug() << "Valeur: " << valeur;

    if(id_capteur < sensorList.size()) {
        if(id_valeur < sensorList[id_capteur]->getValues().size()) {

            sensorList[id_capteur]->getValues()[id_valeur]->addData(valeur);
            parent->getBT()->requestUpdate(sensorList[id_capteur]->getValues()[id_valeur]);

            parent->setIndicatorRx();



            QFile log("log.dan"); // DAN = Data ANalysis
            log.open(QFile::Append);
            log.write((QString::number(id_capteur) + ";" + QString::number(id_valeur) + ";" + QString::number(valeur) + ";" + QTime::currentTime().toString()).toStdString().c_str());
            log.write("!!"); // Separator
            log.flush();
            log.close();
        }
    }
}

void SensorManager::getSensorsFromFile() {
    QXmlStreamReader reader;
    QFile* cptConfig = new QFile("conf/cplist.xml");
    cptConfig->open(QIODevice::ReadOnly);
    reader.setDevice(cptConfig);
    reader.readNext();
    Sensor* curSensor = NULL;
    while(!reader.atEnd()) {
        if((reader.name() == "sensor")&&(reader.attributes().value("name").toString() != "")) {
            Sensor *s = new Sensor(this,reader.attributes().value("name").toString(),reader.attributes().value("id").toString().toInt());
            curSensor = s;
            sensorList.append(s);
            qDebug() << "New sensor:" << s->getName();
        }
        if((reader.name() == "value")&&(reader.attributes().value("name").toString() != "")) {
            QString coef="x";
            QString params = reader.attributes().value("param").toString();
            SensorValue *sv = new SensorValue(reader.attributes().value("name").toString() ,reader.attributes().value("unit").toString(),reader.attributes().value("id").toString().toInt(),curSensor,coef,params);

            if(reader.attributes().value("coef").toString() != "")
                sv->setCoef(reader.attributes().value("coef").toString());

            if(curSensor != NULL)
                curSensor->addSensorValue(sv);


            qDebug() << "  New value:" << sv->getName();
            qDebug() << "  For:" << curSensor->getName();
        }
        reader.readNext();
    }
}

Sensor* SensorManager::getSensor(int id) {
    return sensorList[id];
}
