#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QtGui>
#include "ui_FenPrincipale.h"
#include "InPut/sensormanager.h"
#include "InPut/serial.h"
#include "UI/boardingtable.h"
#include "UI/graphicview.h"
#include "UI/mapsview.h"
#include "defines.h"



class GraphicView;
class MapsView;
class BoardingTable;
class SensorManager;

class FenPrincipale : public QMainWindow, public Ui::FenPrincipale
{
    Q_OBJECT

    public:
        explicit FenPrincipale(Serial *com);
        ~FenPrincipale();
        QTime getDepart() { return h_depart;}

        BoardingTable* getBT() {return tableauBord;}
        SensorManager* getSensorMgr(){return sensormgr; }
        MapsView* getMap() {return carte;}

    protected:
        void reinit_b();
        void resizeEvent(QResizeEvent *);

    private:
        Serial* com;
        QTimer *timerAct;
        QTimer *actTemps;
        BoardingTable* tableauBord;
        MapsView* carte;

        SensorManager *sensormgr;

        QVector<QTableView*> tableauxHist;
        QVector<QPair<GraphicView*,QMdiSubWindow*> > graphiques;

        QTime h_depart;
        bool optimisation_graph;
        int nbSensors;


    public slots:
        void message(QString message);
        void requestAct();
        void informationsReceived(QStringList);
        void syncTime();

        void on_b_param_clicked();
        void on_b_table_clicked();
        void on_b_tb_clicked();
        void on_b_console_clicked();
        void on_actionQuitter_triggered();

        void on_b_carte_clicked();
        void on_b_graph_clicked();
        void on_sel_capteur_currentIndexChanged(int index);

        void on_add_graph_clicked();

        void graphClosed();
        void optimise_graph();
        void on_btn_optimiser_clicked();
};

#endif // FENPRINCIPALE_H


