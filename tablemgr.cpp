#include "tablemgr.h"

TableMgr::TableMgr(QVector<QTableView*> *tab_historique, SensorManager *sensormgr_) {
    m_tab_historique  =   tab_historique;
    sensormgr         =   sensormgr_;
}

// Fait appel à la base de données pour récupérer les informations
void TableMgr::requestActualization(QDateTime start,QDateTime end) {

    sensormgr->getDB()->readFrame(start,end,"tab",false);
}

// Met à jour le tableau avec les données reçues
void TableMgr::actualisay(QVector<QVector<Data>> data) {
    foreach(QVector<Data> frame, data) {
        QList<QStandardItem*> items;
        int curCapteur = 0;
        for(int i=0;i<frame.size();i++) {
            qDebug() << "ici:"<<i;
            SensorValue *v = sensormgr->valueAt(i);
            int idc = v->getCapteur()->getId();

            if(idc != curCapteur) {
                QStandardItem* timeElement  = new QStandardItem;
                timeElement->setText(frame[0].time.toString("dd/MM/yy hh:mm:ss"));
                items << timeElement;

                ((QStandardItemModel*)m_tab_historique->at(curCapteur)->model())->appendRow(items);
                items.clear();

                curCapteur = idc;

            }


            QStandardItem* curElement  = new QStandardItem;
            curElement->setText(QString::number(frame[i].value));
            items << curElement;
        }

        QStandardItem* timeElement  = new QStandardItem;
        timeElement->setText(frame[0].time.toString("dd/MM/yy hh:mm:ss"));
        items << timeElement;

        ((QStandardItemModel*)m_tab_historique->at(curCapteur)->model())->appendRow(items);
        items.clear();
    }



    /*

    for(int i_data=0; i_data<data.size(); i_data++) {
       if(idv==0) {
           QList<QStandardItem*> items;
           for(int i=0;i<=sensormgr->getSensor(idc)->getValues().size();i++) {
               QStandardItem* curElement  = new QStandardItem;

               curElement->setText("nul");
               items << curElement;
           }
       }

       QString dataValue            =   QString::number(data[i_data].value);
       QString dataTime             =   data[i_data].time.toString("dd/MM/yy hh:mm:ss");
       QStandardItemModel* curModel =   ((QStandardItemModel*)m_tab_historique->at(idc)->model());
       QStandardItem* itemValue     =   curModel->item(i_data,idv);
       QStandardItem* itemTime      =   curModel->item(i_data,sensormgr->getSensor(idc)->getValues().size());

       if(itemValue != NULL)
           itemValue->setText(dataValue);

       if(itemTime != NULL)
           itemTime->setText(dataTime);

    }

    if(idv == sensormgr->getSensor(idc)->getValues().size()-1) {
        bool beat = true;
        while(beat) {
            beat = false;
            for(int idc=0;idc<sensormgr->getSensors().size();idc++) {
                QStandardItemModel* curModel = ((QStandardItemModel*)m_tab_historique->at(idc)->model());
                for(int i=0;i<curModel->rowCount();i++) {
                    bool noProblem = false;

                    for(int v=0;v<sensormgr->getSensor(idc )->getValues().size();v++)
                        if(curModel->item(i,v)->text() != "nul")
                            noProblem = true;


                    if(!noProblem) {
                        beat = true;
                        curModel->removeRow(i);
                    }
                }
            }
        }
    }*/
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
