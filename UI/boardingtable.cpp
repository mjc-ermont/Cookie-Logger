#include "boardingtable.h"

BoardingTable::BoardingTable(QGridLayout *container,SensorManager *mgr)
{
    m_container = container;

    init(mgr);
}

BoardingTable::~BoardingTable() {

}

void BoardingTable::init(SensorManager* mgr) {
    QStringList names, values;
    foreach(Sensor* s, mgr->getSensors())
        names << s->getName();

    for(int i=0;i<names.size();i++) {

        QGroupBox* g = new QGroupBox(names[i]);
        QGridLayout* l = new QGridLayout;

        g->setLayout(l);

        capteurs_layouts.append(l);
        m_container->addWidget(g, (int)(i/2) , i%2);

        foreach(SensorValue* v, mgr->getSensor(i)->getValues())
            values << v->getName();

        for(int v = 0; v < mgr->getSensor(i)->getValues().size(); v++) {
            QLabel *l = new QLabel(values[v]);
            capteurs_layouts[i]->addWidget(l, v, 0, 1, 1);

            QLCDNumber *lcd = new QLCDNumber(8);
            lcd->display(0);
            valeurs.append(lcd);
            capteurs_layouts[i]->addWidget(lcd, v, 1, 1, 9);
            QLabel *unitText=new QLabel(mgr->getSensor(i)->getValues()[v]->getUnit());
            capteurs_layouts[i]->addWidget(unitText, v, 10, 1, 1);
        }

        values.clear();
    }
}

void BoardingTable::requestUpdate(SensorValue* value) {
    value->getData("bt",1);
}

void BoardingTable::update(int idc, int idv, double value) {
    QGridLayout* layout = capteurs_layouts[idc];
    QLCDNumber *lcd = (QLCDNumber*)layout->itemAtPosition(idv,1)->widget();
    lcd->display(QString::number(value));
}
