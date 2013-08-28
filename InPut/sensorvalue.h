#ifndef VALUE_H
#define VALUE_H

#include <QString>
#include <QVector>
#include <defines.h>
#include <exprtk.hpp>

#include "sensor.h"
class Sensor;
class SensorValue : public QObject
{
public:
    SensorValue(QString i_name, QString i_unit, int i_id,Sensor* i_parent, QString function="x", QString i_param="");
    Data* addData(double d, bool parse=true);
    QString getName() { return name;}
    QVector<Data*> getData() {return datalist;}
    int getID() {return id;}
    Sensor* getCapteur() {return parent;}
    double getCoef() { return coef;}
    QString getFunction() { return function;}
    void setCoef(QString p_coef) { function=p_coef;}
    QString getParam() { return param;}

    QString getUnit() { return unit;}

private:
    QString name;
    QString unit;
    int id;
    double coef;
    QString function;

    QString param;
    QVector<Data*> datalist;
    Sensor* parent;

    exprtk::symbol_table<double> symbol_table;
    exprtk::expression<double> expression;
    exprtk::parser<double> parser;
    double valeur=0;
};

#endif // VALUE_H
