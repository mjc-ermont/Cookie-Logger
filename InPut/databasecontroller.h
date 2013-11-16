#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QDateTime>

#include "sensormanager.h"

class DatabaseController
{
    public:
        DatabaseController();
        void setup();
        bool isSetup();
        void write(SensorValue* sv, double value, QDateTime t=QDateTime::currentDateTime());

    private:
        QSqlDatabase db;
        bool dbsetup;
};

#endif // DATABASECONTROLLER_H
