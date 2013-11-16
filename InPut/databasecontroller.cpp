#include "databasecontroller.h"

DatabaseController::DatabaseController()
{
    dbsetup=false;
}

void DatabaseController::setup() {
    dbsetup=true;
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

bool DatabaseController::isSetup() {
    return dbsetup;
}

void DatabaseController::write(SensorValue* sv, double value, QDateTime t) {
    db.exec("INSERT INTO data (sensor,sensorvalue,value,time) VALUES (\""+QString::number(sv->getCapteur()->getId())+"\",\""+QString::number(sv->getID())+"\",\""+QString::number(value)+"\",\""+t.toString("yyyy-MM-dd hh:mm:ss")+"\")");
    qDebug() << "INSERT INTO data (sensor,sensorvalue,value,time) VALUES (\""+QString::number(sv->getCapteur()->getId())+"\",\""+QString::number(sv->getID())+"\",\""+QString::number(value)+"\",\""+t.toString("yyyy-MM-dd hh:mm:ss")+"\")";

}
