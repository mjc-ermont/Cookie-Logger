#include <QtXml/QtXml>
#include "sensormanager.h"

SensorManager::SensorManager(FenPrincipale* _parent) {
    getSensorsFromFile();
    parent = _parent;
}

void SensorManager::newValue(int id_capteur, int id_valeur, double valeur) {
    if(id_capteur < sensorList.size()) {
        if(id_valeur < sensorList[id_capteur]->getValues().size()) {

            // TODO: Parse equation.
            valeur = valeur * sensorList[id_capteur]->getValues()[id_valeur]->getCoef();

            Data* d = sensorList[id_capteur]->getValues()[id_valeur]->addData(valeur);
            parent->getBT()->update(sensorList[id_capteur]->getValues()[id_valeur]);

            parent->setIndicatorRx();
            getSensor(id_valeur)->getValues().at(id_capteur)->addData(valeur);

            QString url = parent->dataServerLineEdit->text();
            QStringList split = url.split("||");
            if(split.size() == 2) {
                QHttp *serveur_search = new QHttp(split[0]);
                serveur_search->setHost(split[0]);

                serveur_search->get(split[1]+"?t=token&nc="+QString::number(id_capteur)+"&nv="+QString::number(id_valeur)+"&v="+QString::number(valeur, 'f'));
            } else {
                QHttp *serveur_search = new QHttp("home.konfiot.net");
                serveur_search->setHost("home.konfiot.net");

                serveur_search->get("/Cookie-WebUI-Server/bin/add.php?t=token&nc="+QString::number(id_capteur)+"&nv="+QString::number(id_valeur)+"&v="+QString::number(valeur, 'f'));
            }

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
    Sensor* curSensor;
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
