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

    int posgauche = 0;
    int posdroite = 0;


    labels.clear();

    for(int i=0;i<names.size();i++) {

        QFrame* g = new QFrame();
        QGridLayout* l = new QGridLayout;

        g->setLayout(l);
        g->setStyleSheet("QFrame {  border: 1px solid gray; border-radius: 5px;} QLabel { border: none; }");

        capteurs_layouts.append(l);

        if(posgauche <= posdroite) {
            m_container->addWidget(g, posgauche , 0, mgr->getSensor(i)->getValues().size()+1, 1);
            posgauche += mgr->getSensor(i)->getValues().size()+1;
        } else {
            m_container->addWidget(g, posdroite , 1, mgr->getSensor(i)->getValues().size()+1, 1);
            posdroite += mgr->getSensor(i)->getValues().size()+1;
        }

        foreach(SensorValue* v, mgr->getSensor(i)->getValues())
            values << v->getName();

        QLabel* ltitle = new QLabel(mgr->getSensor(i)->getName());

        ltitle->setStyleSheet("QLabel {font-weight: 800; font-size: 20px;}");

        ltitle->setAlignment(Qt::AlignCenter);
        labels.append(ltitle);
        capteurs_layouts[i]->addWidget(ltitle, 0, 0, 1, 11);

        for(int v = 0; v < mgr->getSensor(i)->getValues().size(); v++) {
            QLabel *l = new QLabel(values[v]);
            l->setStyleSheet("QLabel {font-size: 20px; }");
           // l->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            capteurs_layouts[i]->addWidget(l, v+1, 0, 1, 1);
            labels.append(l);
            QLCDNumber *lcd = new QLCDNumber(8);
            lcd->setStyleSheet("border: none;");
            lcd->setSegmentStyle(QLCDNumber::Flat);
            lcd->display(0);
            valeurs.append(lcd);
            capteurs_layouts[i]->addWidget(lcd, v+1, 1, 1, 9);
            QLabel *unitText=new QLabel(mgr->getSensor(i)->getValues()[v]->getUnit());
            unitText->setStyleSheet("QLabel {font-size: 20px; }");
            capteurs_layouts[i]->addWidget(unitText, v+1, 10, 1, 1);
            labels.append(unitText);
        }

        values.clear();
    }
}

void BoardingTable::requestUpdate(SensorValue* value) {
    value->getData("bt",1);
}

void BoardingTable::update(int idc, int idv, double value) {
    QGridLayout* layout = capteurs_layouts[idc];
    QLCDNumber *lcd = (QLCDNumber*)layout->itemAtPosition(idv+1,1)->widget();
    lcd->display(QString::number(value));
}

void BoardingTable::onResize() {
}
