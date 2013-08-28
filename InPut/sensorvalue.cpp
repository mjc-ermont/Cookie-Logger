#include "sensorvalue.h"


SensorValue::SensorValue(QString i_name, QString i_unit, int i_id, Sensor *i_parent, QString i_function, QString i_param) {
    name = i_name;
    unit = i_unit;
    id = i_id;
    parent = i_parent;
    function = i_function;
    param = i_param;

  //  parser.SetExpr(function.toStdString());
    symbol_table.add_constants();
    symbol_table.add_variable("x",valeur);

    expression.register_symbol_table(symbol_table);
}

Data* SensorValue::addData(double d) {
 //   parser.DefineVar("x", &d);
    valeur = d;

    Data *newData = new Data;
    newData->time = QTime::currentTime();

    parser.compile(function.toStdString(),expression);
    newData->value =  expression.value();

    qDebug() << "v: " << d << " parser: " << newData->value;
   /* try {
        newData->value = parser.Eval();
    }
    catch (mu::Parser::exception_type &e)
    {
        string_type& s = e.GetMsg();
        str.
        qDebug() << "uluru:" << ;
    }*/
    datalist.append(newData);

    if(param == "xmap") {
        parent->getParent()->getParent()->getMap()->updateX(d);
    } else if(param == "ymap") {
        parent->getParent()->getParent()->getMap()->updateY(d);
    }
    return newData;
}
