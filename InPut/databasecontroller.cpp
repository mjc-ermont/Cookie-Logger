#include "databasecontroller.h"

DatabaseController::DatabaseController()
{
    qDebug() << ".";
}

void DatabaseController::run() {
    setup();
    forever {
        mutex.lock();
        if(!work.isEmpty()) {
            int s = work.size();
            emit message("Queue size: " + QString::number(s) + " ");
            qDebug() << "[DB] Queue size: " << s;
            QStringList req = work.dequeue().split(";");
            mutex.unlock();
            if(req.size() >= 4) {
                QString idc = req.at(1);
                QString idv = req.at(2);

                if(req.at(0) == "get") {
                    if(req.size() < 7)
                        break;
                    QString from = req.at(3);
                    QString to = req.at(4);
                    qDebug() << req.at(6);
                    bool last = req.at(6) == "1";
                    QSqlQuery rep;
                    if(!last)
                        rep = db.exec("SELECT value,time FROM data WHERE sensor=\""+idc+"\" AND sensorvalue=\""+idv+"\" AND time>\""+from+"\" AND time<\""+to+"\"");
                    else
                        rep = db.exec("SELECT value,time FROM data WHERE sensor=\""+idc+"\" AND sensorvalue=\""+idv+"\" ORDER BY time DESC LIMIT 0,1");

                    QVector<Data> dataset;
                    //qDebug() << "READ";
                    while(rep.next()) {
                        double value = rep.value(0).toDouble();
                        QDateTime time = QDateTime::fromString(rep.value(1).toString(),"yyyy-MM-dd hh:mm:ss");
                        Data d;
                        d.value = value;
                        d.time = time;

                        dataset.push_back(d);
                        //qDebug() << "[" << time.toString("yyyy-MM-dd hh:mm:ss") << "|" << value << "]";
                    }
                    //qDebug() << "FINREAD";
                    //qDebug() << "req=" << req.at(5) << ";" << last;

                    emit dataRead(idc.toInt(),idv.toInt(),dataset,req.at(5));
                } else if(req.at(0) == "set") {
                    QString value = req.at(3);

                    db.exec("INSERT INTO data (sensor,sensorvalue,value,time) VALUES (\""+idc+"\",\""+idv+"\",\""+value+"\",\""+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"\")");
                    //qDebug() << "INSERT (" << idc << ";" << idv<<") " << value << " [" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "]";
                    emit dataWritten(idc.toInt(),idv.toInt());
                }
            }
        } else {
            mutex.unlock();
        }


    }
}

void DatabaseController::write(int idc, int idv, double v) {
    mutex.lock();
    work.enqueue("set;"+QString::number(idc)+";"+QString::number(idv)+";"+QString::number(v));
    mutex.unlock();
}
void DatabaseController::read(int idc, int idv, QDateTime from, QDateTime to,QString reason,bool last) {
    mutex.lock();
    work.enqueue("get;"+QString::number(idc)+";"+QString::number(idv)+";"+from.toString("yyyy-MM-dd hh:mm:ss")+";"+to.toString("yyyy-MM-dd hh:mm:ss")+";"+reason+(last==true?";1":";0"));
    mutex.unlock();
}

void DatabaseController::setup() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("database.db");
    db.setUserName("root");
    db.setPassword("");

    if(!db.open()) {
        emit message("La base de données ne s'est pas ouverte. Chiasse.");
        qDebug() << "la bdd ne s'est pas ouverte";
        return;
    } else {
        emit message("La base de données s'est correctement initialisée");
        qDebug() << "la bdd s'est correctement initialisée";
    }

    db.exec("CREATE TABLE IF NOT EXISTS data (id integer primary key, sensor INT, sensorvalue INT, value DOUBLE, time DATETIME)");


    QSqlQuery rep;
    rep = db.exec("SELECT time FROM data ORDER BY time ASC LIMIT 0,1");
    QDateTime range_start;
    if(rep.next()) {
        range_start = QDateTime::fromString(rep.value(0).toString(),"yyyy-MM-dd hh:mm:ss");
    } else {
        range_start = QDateTime::currentDateTime();
    }

    emit rangeStartUpdate(range_start);
}
