
#include "sensormanager.h"

SensorManager::SensorManager(FenPrincipale* _parent) {
    getSensorsFromFile();
    parent = _parent;


    valeur=0;
    bdd = new DatabaseController(m_structure);
    connect(bdd, SIGNAL(rangeStartUpdate(QDateTime)),parent,SLOT(onRangeStartUpdate(QDateTime)));

    bdd->start();
}

int SensorManager::indexOf(int idc, int idv) {
    return sensorValueList.indexOf(sensorList[idc]->getValues()[idv]);
}

void SensorManager::newFrame(QVector<double> frame){
    parent->setIndicatorRx();

    QFile backup("backup.csv");
    backup.open(QFile::Append);
    QTextStream out(&backup);

    for(int i=0; i<frame.size();i++) {
        SensorValue* sv = sensorValueList[i];
        sensorList[sv->getCapteur()->getId()]->getValues()[sv->getID()]->addData(frame[i]);
        out << frame[i] << ",";
    }
    getDB()->writeFrame(frame, QDateTime::currentDateTime());
    getDB()->readFrame(QDateTime::currentDateTime(), QDateTime::currentDateTime(), "bt", true);
    out << (QTime::currentTime().toString().toAscii()) << '\n';

    backup.flush();
    backup.close();
}

void SensorManager::getSensorsFromFile() {
    QXmlStreamReader reader;
    QFile* cptConfig = new QFile("conf/cplist.xml");
    cptConfig->open(QIODevice::ReadOnly);
    reader.setDevice(cptConfig);
    reader.readNext();
    Sensor* curSensor = NULL;

   // parent->log_decoder("Récupération de la liste des capteurs depuis <em>conf/cplist.xml</em>");

    int comp =0;
    while(!reader.atEnd()) {
        if((reader.name() == "sensor")&&(reader.attributes().value("name").toString() != "")) {
            Sensor *s = new Sensor(this,reader.attributes().value("name").toString(),reader.attributes().value("id").toString().toInt());
            curSensor = s;
            sensorList.append(s);
            qDebug() << "New sensor:" << s->getName();

            m_structure.push_back(0);
            comp++;

     //       parent->log_decoder("|| Nouveau capteur '" + s->getName() + "' ("+ QString::number(s->getId())+ ")");
        }
        if((reader.name() == "value")&&(reader.attributes().value("name").toString() != "")) {
            QString coef="x";
            QString params = reader.attributes().value("param").toString();
            SensorValue *sv = new SensorValue(reader.attributes().value("name").toString() ,reader.attributes().value("unit").toString(),reader.attributes().value("id").toString().toInt(),curSensor,coef,params);

            sensorValueList.append(sv);
            if(reader.attributes().value("coef").toString() != "")
                sv->setCoef(reader.attributes().value("coef").toString());

            if(curSensor != NULL)
                curSensor->addSensorValue(sv);

            m_structure[comp-1]++;

            qDebug() << "  New value:" << sv->getName();
            qDebug() << "  For:" << curSensor->getName();
      //      parent->log_decoder("||  || Nouvelle valeur '" + sv->getName() + "' ("+ QString::number(sv->getID())+ ")");
        }
        reader.readNext();
    }

    cptConfig->close();
    delete cptConfig;
  //  parent->log_decoder("Terminé !");
}

Sensor* SensorManager::getSensor(int id) {
    return sensorList[id];
}
