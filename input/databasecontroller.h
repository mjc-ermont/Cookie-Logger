#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H


#include "defines.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlResult>
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QStringList>
#include <QSqlError>


class DatabaseController : public QThread
{
    Q_OBJECT

    public:
        DatabaseController(QVector<QVector<double> > structure);
        void run();
        void setup();

        void write(int idc, int idv, double v, QDateTime time);
        void read(int idc, int idv, QDateTime from, QDateTime to, QString reason="", bool last=false);


    private:
        QSqlDatabase db;
        QQueue<QString> work;
        QMutex mutex;
        QWaitCondition condition;

        QVector<QVector<double> > m_structure;

    signals:
        void dataWritten(int idc, int idv);
        void dataRead(int idc, int idv, QVector<Data> data, QString reason);
        void rangeStartUpdate(QDateTime range_start);
        void message(QString);
};

#endif // DATABASECONTROLLER_H
