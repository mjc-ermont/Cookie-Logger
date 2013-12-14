#include "sensorvalue.h"


SensorValue::SensorValue(QString i_name, QString i_unit, int i_id, Sensor *i_parent, QString i_function, QString i_param) {
    name = i_name;
    unit = i_unit;
    id = i_id;
    parent = i_parent;
    function = i_function;
    param = i_param;

    valeur = 0;

  //  parser.SetExpr(function.toStdString());
    symbol_table.add_constants();
    symbol_table.add_variable("x",valeur);

    expression.register_symbol_table(symbol_table);
}

void SensorValue::addData(double d, bool parse) {
 //   parser.DefineVar("x", &d);
    valeur = d;

    Data newData;
    newData.time = QDateTime::currentDateTime();

    parser.compile(function.toStdString(),expression);
    if(parse)
        newData.value =  expression.value();
    else
        newData.value=d;
    qDebug() << "v: " << d << " parser: " << newData.value;

    parent->getParent()->getDB()->write(parent->getId(),id,valeur);

    if(param == "xmap") {
        parent->getParent()->getParent()->getMap()->updateX(d);
    } else if(param == "ymap") {
        parent->getParent()->getParent()->getMap()->updateY(d);
    }
}

void SensorValue::getData(QString reason, bool last, QDateTime from, QDateTime to) {
    parent->getParent()->getDB()->read(parent->getId(),id,from,to,reason, last);
}

