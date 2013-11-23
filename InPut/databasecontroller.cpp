#include "databasecontroller.h"

DatabaseController::DatabaseController()
{
    qDebug() << ".";
}

void DatabaseController::run() {
    setup();
    forever {
        if(!work.isEmpty()) {
            QStringList req = work.dequeue().split(";");
            if(req.size() < 4)
                break;

            QString idc = req.at(1);
            QString idv = req.at(2);

            if(req.at(0) == "get") {
                if(req.size() < 5)
                    break;
                QString from = req.at(3);
                QString to = req.at(4);

                QSqlQuery rep = db.exec("SELECT value,time FROM data WHERE sensor=\""+idc+"\" AND sensorvalue=\""+idv+"\" AND time>\""+from+"\" AND time<\""+to+"\"");
                QVector<Data> dataset;
                qDebug() << "READ";
                while(rep.next()) {
                    double value = rep.value(0).toDouble();
                    QDateTime time = QDateTime::fromString(rep.value(1).toString(),"yyyy-MM-dd hh:mm:ss");
                    Data d;
                    d.value = value;
                    d.time = time;

                    dataset.push_back(d);
                    qDebug() << "[" << time.toString("yyyy-MM-dd hh:mm:ss") << "|" << value << "]";
                }
                qDebug() << "FINREAD";

                emit dataRead(idc.toInt(),idv.toInt(),dataset);
            } else if(req.at(0) == "set") {
                QString value = req.at(3);

                db.exec("INSERT INTO data (sensor,sensorvalue,value,time) VALUES (\""+idc+"\",\""+idv+"\",\""+value+"\",\""+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"\")");
                qDebug() << "written";
                emit dataWritten(idc.toInt(),idv.toInt());
            }
        }
    }
}

void DatabaseController::write(int idc, int idv, double v) {
    work.push_back("set;"+QString::number(idc)+";"+QString::number(idv)+";"+QString::number(v));
}
void DatabaseController::read(int idc, int idv, QDateTime from, QDateTime to) {
    work.push_back("get;"+QString::number(idc)+";"+QString::number(idv)+";"+from.toString("yyyy-MM-dd hh:mm:ss")+";"+to.toString("yyyy-MM-dd hh:mm:ss"));
}

void DatabaseController::setup() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("database.db");
    db.setUserName("root");
    db.setPassword("");

    if(!db.open()) {
        qDebug() << "la bdd ne s'est pas ouverte";
        return;
    } else {
        qDebug() << "la bdd s'est correctement initialisée";
    }

    db.exec("CREATE TABLE IF NOT EXISTS data (id integer primary key, sensor INT, sensorvalue INT, value DOUBLE, time DATETIME)");

}
