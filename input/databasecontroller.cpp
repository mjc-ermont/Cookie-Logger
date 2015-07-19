#include "databasecontroller.h"

DatabaseController::DatabaseController(QVector<int> structure)
{
    m_structure = structure;
}

void DatabaseController::run() {
    setup();


    QSqlQuery insertionQuery;
    QString q = "INSERT INTO data (";

    for(int c=0;c<m_structure.size();c++)
        for(int cv=0;cv<m_structure[c];cv++)
            q.push_back("c"+QString::number(c)+"v"+QString::number(cv)+",");

    q.push_back("time) VALUES (");
    for(int c=0;c<m_structure.size();c++)
        for(int cv=0;cv<m_structure[c];cv++)
            q.push_back(":c"+QString::number(c)+"v"+QString::number(cv)+",");
    q.push_back(":time)");
    insertionQuery.prepare(q);


    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM data WHERE time>:from AND time<:to");
    QSqlQuery selectQueryLast;
    selectQueryLast.prepare("SELECT * FROM data ORDER BY time DESC LIMIT 0,1");

    forever {
        mutex.lock();

        int s = work.size();
        emit message("Queue size: " + QString::number(s) + " ");
        qDebug() << "[DB] Queue size: " << s;
        while(!work.isEmpty()) {

            QStringList req = work.dequeue().split(";");
            mutex.unlock();
            if(req.at(0) == "get") {
                QString from = req.at(1);
                QString to = req.at(2);

                bool last = req.at(4) == "1";
                QSqlQuery rep = last ? selectQueryLast : selectQuery;

                if(!last) {
                    rep.bindValue("from",from);
                    rep.bindValue("to",to);
                }

                qDebug() << "requete: " << req.join(";");

                rep.exec();

                QVector<QVector<Data>> dataset;

                while(rep.next()) {
                    QVector<Data> res;

                    int comp = 1;
                    for(int c=0;c<m_structure.size();c++) {
                        for(int cv=0;cv<m_structure[c];cv++) {
                            Data d;
                            d.value = rep.value(comp).toDouble();
                            d.time = QDateTime::currentDateTime();
                            comp++;
                            res.push_back(d);
                        }
                    }

                    QDateTime t = QDateTime::fromString(rep.value(comp).toString(),"yyyy-MM-dd hh:mm:ss");

                    for(int i=0;i<res.size();i++)
                        res[i].time = t;

                    dataset.push_back(res);
                }
                qDebug() << "je suis ici";
                emit dataRead(dataset,req.at(3));
            } else if(req.at(0) == "set") {
                int comp = 1;
                for(int c=0;c<m_structure.size();c++) {
                    for(int cv=0;cv<m_structure[c];cv++) {
                        insertionQuery.bindValue("c"+QString::number(c)+"v"+QString::number(cv),req.at(comp));
                        comp++;
                    }
                }

                insertionQuery.bindValue("time",QDateTime::fromTime_t(req.last().toUInt()).toString("yyyy-MM-dd hh:mm:ss"));
                insertionQuery.exec();

                emit dataWritten();
            }

        }

        condition.wait(&mutex);
        mutex.unlock();
    }
}

void DatabaseController::writeFrame(QVector<double> frame,QDateTime time) {
    mutex.lock();
    QString req ="set;";
    foreach(double val, frame) {
        req.append(QString::number(val)+";");
    }
    req.append(QString::number(time.toTime_t()));
    work.enqueue(req);
    mutex.unlock();
}

void DatabaseController::readFrame(QDateTime from, QDateTime to,QString reason,bool last) {
    mutex.lock();
    work.enqueue("get;"+from.toString("yyyy-MM-dd hh:mm:ss")+";"+to.toString("yyyy-MM-dd hh:mm:ss")+";"+reason+(last==true?";1":";0"));
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

    QString req = "CREATE TABLE IF NOT EXISTS data (id integer primary key,";
    for(int c=0;c<m_structure.size();c++) {
        for(int cv=0;cv<m_structure[c];cv++) {
            req.push_back(" c"+QString::number(c)+"v"+QString::number(cv)+" DOUBLE,");
        }
    }
    req.push_back(" time DATETIME)");

    QSqlQuery result = db.exec(req);
    qDebug() << "Requete: " << req;
    qDebug() << result.lastError().text();
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
