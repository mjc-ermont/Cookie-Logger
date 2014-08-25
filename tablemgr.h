#ifndef TABLEMGR_H
#define TABLEMGR_H

#include <QVector>
#include <QTableView>
#include <QDateTime>
#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>

#include "defines.h"
#include "InPut/sensormanager.h"
#include "InPut/sensorvalue.h"


class TableMgr
{
public:
                TableMgr                (QVector<QTableView*>* tab_historique, SensorManager* sensormgr_i);
    void        addData                 (SensorValue *valeur,int index,QTime start, QTime end);
    void        actualisay              (int idc, int idv, QVector<Data> data);
    void        requestActualization    (QDateTime start, QDateTime end);
protected:
    void        update                  (int capteur);
    bool        lineFull                (int capteur);
private:
    QVector<QTableView*>*               m_tab_historique;
    QVector<QVector<QString> >          line;
    QVector<QVector<QString> >          bef_line;
    SensorManager*                      sensormgr;
};

#endif // TABLEMGR_H
