#include "sensorvalue.h"


SensorValue::SensorValue(QString i_name, QString i_unit, int i_id, Sensor *i_parent, QString i_function, QString i_param) {
    name = i_name;
    unit = i_unit;
    id = i_id;
    parent = i_parent;
    function = i_function;
    param = i_param;

    valeur = 0;

    symbol_table.add_constants();
    symbol_table.add_variable("x",valeur);

    expression.register_symbol_table(symbol_table);

    upstreak=0;
    downstreak=0;

    last_moy=0;
    first_moy_up=-1;
    first_moy_down=-1;


}

void SensorValue::addData(double d, bool parse) {
    valeur = d;

    Data newData;
    newData.time = QDateTime::currentDateTime();

    parser.compile(function.toStdString(),expression);
    if(parse)
        newData.value =  expression.value();
    else
        newData.value=d;
    qDebug() << "v: " << d << " parser: " << newData.value;
    parent->getParent()->getParent()->log_decoder("Nouvelle valeur: ("+QString::number(parent->getId())+";"+QString::number(id)+") => "+QString::number(newData.value));
    parent->getParent()->getDB()->write(parent->getId(),id,newData.value);

    parent->getParent()->getParent()->getStagesManager()->unlockDataReceivedStage();

    parent->getParent()->getParent()->getWebServicesManager()->update(parent->getId(), id, newData.value);

    if(param == "xmap") {
        parent->getParent()->getParent()->getStagesManager()->unlockGPSFixStage();
        parent->getParent()->getParent()->getMap()->updateX(newData.value);
    } else if(param == "ymap") {
        parent->getParent()->getParent()->getStagesManager()->unlockGPSFixStage();
        parent->getParent()->getParent()->getMap()->updateY(newData.value);
    } else if(param == "pres") {
        last_values.append(newData.value);

        if(last_values.size() > 10)
            last_values.removeFirst();


        if(last_values.size() < 5)
            return;

        QVector<double> derivees;
        for(int i=1;i<last_values.size();i++) {
            derivees.push_back(last_values[i] - last_values[i-1]);
        }

        double moyenne = 0;
        for(int i=0;i<derivees.size();i++) {
            moyenne += derivees[i];
        }
        if(derivees.size() < 2)
            return;

        moyenne /= derivees.size();

        double ecart_type = 0;

        for(int i=0;i<derivees.size();i++) {
            ecart_type += (derivees[i] - moyenne)*(derivees[i] - moyenne);
        }
        ecart_type /= (double)(derivees.size()-1);


        ecart_type = sqrt(ecart_type);


        double low = moyenne - ecart_type;
        double up = moyenne + ecart_type;

        qDebug() << low << moyenne << up;

        if(0 < low && 0 < up && (abs(last_moy - last_values.first()) > 10)) {
            parent->getParent()->getParent()->getStagesManager()->unlockApogeeStage();
        }

        if(0 > low && 0 > up && (abs(last_moy - last_values.first()) > 10)) {
            parent->getParent()->getParent()->getStagesManager()->unlockLaunchStage();
        }
        /*

        if(first_moy_down == -1)
            first_moy_down = moyenne;
        if(first_moy_up == -1)
            first_moy_up = moyenne;

        if(moyenne > last_moy) {
            upstreak++;
            downstreak = 0;
            first_moy_down = moyenne;
        } else {
            downstreak++;
            upstreak=0;
            first_moy_up = moyenne;
        }

        qDebug() << "moyenne: " << moyenne << " downstreak: " << downstreak << " upstreak: " << upstreak;

        last_moy = moyenne;

        qDebug() << first_moy_down - last_moy;

        if(upstreak >= 5 && (last_moy - first_moy_up > 10)) {
            parent->getParent()->getParent()->getStagesManager()->unlockApogeeStage();
        } else if(downstreak >= 5 && (first_moy_down - last_moy > 10)) {
            parent->getParent()->getParent()->getStagesManager()->unlockLaunchStage();
        }*/
    }


}

void SensorValue::getData(QString reason, bool last, QDateTime from, QDateTime to) {
    parent->getParent()->getDB()->read(parent->getId(),id,from,to,reason, last);
}

