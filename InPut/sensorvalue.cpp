#include "sensorvalue.h"


SensorValue::SensorValue(QString i_name, QString i_unit, int i_id, Sensor *i_parent, QString i_function, QString i_param) {
    name = i_name;
    unit = i_unit;
    id = i_id;
    parent = i_parent;
    function = i_function;
    param = i_param;

    parser.SetExpr(function.toStdString());
}

Data* SensorValue::addData(double d) {
    parser.DefineVar("x", &d);

    Data *newData = new Data;
    newData->time = QTime::currentTime();
    try {
        newData->value = parser.Eval();
    }
    catch (mu::Parser::exception_type &e)
    {
      std::cout << e.GetMsg() << std::endl;
    }
    datalist.append(newData);
    qDebug()<<"Param: " << param;
    if(param == "xmap") {
        parent->getParent()->getParent()->getMap()->updateX(d);
    } else if(param == "ymap") {
        parent->getParent()->getParent()->getMap()->updateY(d);
    }
    return newData;
}
