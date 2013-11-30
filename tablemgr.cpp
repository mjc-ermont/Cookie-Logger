#include "tablemgr.h"

TableMgr::TableMgr(QVector<QTableView*> *tab_historique, SensorManager *sensormgr_)
{
      m_tab_historique = tab_historique;
      sensormgr = sensormgr_;
}

void TableMgr::addData(SensorValue *valeur) {
   // SensorManager* sensormgr = valeur->getCapteur()->getParent();
  /*  int idcapteur = valeur->getCapteur()->getId();
    int idvaleur = valeur->getID();

    while(line.size() <= idcapteur) {
        QVector<QString> v;
        v.push_back("null");
        line.push_back(v);
        bef_line.push_back(v);
    }

    while(line[idcapteur].size() <= idvaleur+1) {
        line[idcapteur].push_back("null");
        bef_line[idcapteur].push_back("null");
    }

    line[idcapteur][idvaleur+1] = QString::number(valeur->getData().last()->value);
    line[idcapteur][0] = valeur->getData().last()->time.toString();



    if(lineFull(idcapteur)) {
        update(idcapteur);*/
        qDebug() << "test";
   // }
}

void TableMgr::requestActualization(QDateTime start,QDateTime end) {
    for(int i_capteur=0;i_capteur<sensormgr->getSensors().size();i_capteur++) {
        for(int i_valeur=0;i_valeur<sensormgr->getSensor(i_capteur)->getValues().size();i_valeur++) {
            sensormgr->getSensor(i_capteur)->getValues()[i_valeur]->getData("tab",false);
        }
    }
}

void TableMgr::actualisay(int idc, int idv, QVector<Data> data) {

    for(int i_data=0;i_data<data.size();i_data++) {
       if(idv==0) {
           QList<QStandardItem*> items;
           for(int i=0;i<sensormgr->getSensor(idc)->getValues().size();i++) {
               QStandardItem* curElement  = new QStandardItem;

               curElement->setText("nul");
               items << curElement;
           }
           ((QStandardItemModel*)m_tab_historique->at(idc)->model())->appendRow(items);

       }

       QString dataValue = QString::number(data[i_data].value);
       QStandardItemModel* curModel = ((QStandardItemModel*)m_tab_historique->at(idc)->model());
       QStandardItem* item = curModel->item(i_data,idv);

       if(item != NULL)
            item->setText(dataValue);
    }

    if(idv == sensormgr->getSensor(idc)->getValues().size()-1) {
        bool beat = true;
        while(beat) {
            beat = false;
            for(int idc=0;idc<sensormgr->getSensors().size();idc++) {
                QStandardItemModel* curModel = ((QStandardItemModel*)m_tab_historique->at(idc)->model());
                for(int i=0;i<curModel->rowCount();i++) {
                    bool noProblem = false;

                    for(int v=0;v<sensormgr->getSensor(idc )->getValues().size();v++) {
                        if(curModel->item(i,v)->text() != "nul")
                            noProblem = true;
                    }

                    if(!noProblem) {
                        beat = true;
                        curModel->removeRow(i);
                    }
                }
            }
        }
    }
}

bool TableMgr::lineFull(int capteur) {
    bool okay=true;
    foreach(QString value, line[capteur]) {
        if(value == "")
            okay = false;
    }
    return okay;
}

void TableMgr::update(int capteur) {
    QList<QStandardItem*> items;
    foreach(QString value, line[capteur]) {
        QStandardItem* element  = new QStandardItem;
        element->setText(value);
        items << element;
    }
    ((QStandardItemModel*)m_tab_historique->at(capteur)->model())->appendRow(items);
}
