#include "databasecontroller.h"

DatabaseController::DatabaseController(QVector<QVector<double> > structure)
{
    m_structure = structure;
}

void DatabaseController::run() {
    setup();


    QSqlQuery insertionQuery;
    insertionQuery.prepare("INSERT INTO data (sensor,sensorvalue,value,time) VALUES (:sensor,:sensorvalue,:value,:time)");


    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT value,time FROM data WHERE sensor=:sensor AND sensorvalue=:sensorvalue AND time>:from AND time<:to");
    QSqlQuery selectQueryLast;
    selectQueryLast.prepare("SELECT value,time FROM data WHERE sensor=:sensor AND sensorvalue=:sensorvalue ORDER BY time DESC LIMIT 0,1");

    forever {
        mutex.lock();

        QTime t1 = QTime::currentTime();

        int s = work.size();
        emit message("Queue size: " + QString::number(s) + " ");
        qDebug() << "[DB] Queue size: " << s;
        while(!work.isEmpty()) {

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
                    bool last = req.at(6) == "1";
                    QSqlQuery rep = last ? selectQueryLast : selectQuery;

                    rep.bindValue("sensor",idc);
                    rep.bindValue("sensorvalue",idv);

                    if(!last) {
                        rep.bindValue("from",from);
                        rep.bindValue("to",to);
                    }

                    rep.exec();

                    QVector<Data> dataset;
                    while(rep.next()) {
                        double value = rep.value(0).toDouble();
                        QDateTime time = QDateTime::fromString(rep.value(1).toString(),"yyyy-MM-dd hh:mm:ss");
                        Data d;
                        d.value = value;
                        d.time = time;

                        dataset.push_back(d);
                    }

                    emit dataRead(idc.toInt(),idv.toInt(),dataset,req.at(5));
                } else if(req.at(0) == "set") {


                    QString value = req.at(3);
                    QString time = req.at(4);

                    insertionQuery.bindValue("sensor",idc);
                    insertionQuery.bindValue("sensorvalue",idv);
                    insertionQuery.bindValue("value",value);
                    insertionQuery.bindValue("time",QDateTime::fromTime_t(time.toUInt()).toString("yyyy-MM-dd hh:mm:ss"));
                    insertionQuery.exec();

                    emit dataWritten(idc.toInt(),idv.toInt());
                }
            }
        }

        QTime t2 = QTime::currentTime();
        qDebug() << "temps : " << t1.msecsTo(t2) << "s";
        condition.wait(&mutex);
        mutex.unlock();
    }
}

void DatabaseController::writeFrame(QVector<QVector<double> > frame,QDateTime time) {
    mutex.lock();
    QString req ="set;";
    foreach(QVector<double> c, frame) {
        foreach(double val, c)
            req.append(QString::number(val)+" ");
        req.append(";");
    }
    req.append(QString::number(time.toTime_t()));
    work.enqueue(req);
    mutex.unlock();
}

void DatabaseController::read(int idc, int idv, QDateTime from, QDateTime to,QString reason,bool last) {
    //emit thingToRead(idc, idv, from, to, reason, last);
    mutex.lock();
    work.enqueue("get;"+QString::number(idc)+";"+QString::number(idv)+";"+from.toString("yyyy-MM-dd hh:mm:ss")+";"+to.toString("yyyy-MM-dd hh:mm:ss")+";"+reason+(last==true?";1":";0"));
    condition.wakeOne();
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
    db.exec("CREATE UNIQUE INDEX IF NOT EXISTS ON data (time)");

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
