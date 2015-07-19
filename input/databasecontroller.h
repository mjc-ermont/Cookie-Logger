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
        DatabaseController(QVector<int> structure);
        void run();
        void setup();

        void writeFrame(QVector<double> frame,QDateTime time);

        void readFrame(QDateTime from, QDateTime to,QString reason,bool last);

    private:
        QSqlDatabase db;
        QQueue<QString> work;
        QMutex mutex;
        QWaitCondition condition;

        QVector<int> m_structure;

    signals:
        void dataWritten();
        void dataRead(QVector<QVector<Data>> data, QString reason);
        void rangeStartUpdate(QDateTime range_start);
        void message(QString);
};

#endif // DATABASECONTROLLER_H
