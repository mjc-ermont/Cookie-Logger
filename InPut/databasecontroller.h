#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlResult>
#include <QDebug>
#include <QDateTime>
#include <QThread>

#include "sensormanager.h"

class DatabaseController : public QThread
{
    Q_OBJECT

    public:
        DatabaseController();
        void run();
        void setup();

        void write(int idc, int idv, double v);
        void read(int idc, int idv, QDateTime from, QDateTime to);

    private:
        QSqlDatabase db;
        QQueue<QString> work;

    signals:
        void dataWritten(int idc, int idv);
        void dataRead(int idc, int idv, QVector<Data>);
};

#endif // DATABASECONTROLLER_H
