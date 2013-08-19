#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QtGui>
#include "ui_FenPrincipale.h"
#include "ChronoReader/chronoreaderwidget.h"
#include "InPut/sensormanager.h"
#include "InPut/serial.h"
#include "UI/boardingtable.h"
#include "UI/graphicview.h"
#include "UI/mapsview.h"
#include "defines.h"
#include "tablemgr.h"



class GraphicView;
class MapsView;
class BoardingTable;
class SensorManager;
class TableMgr;

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
        TableMgr* getTableMgr() {return tableManager;}
        void konamify(bool enable);

    protected:
        void reinit_b();
        void resizeEvent(QResizeEvent *);
        bool eventFilter( QObject *o, QEvent *e );

    private:
        Serial* com;
        QTimer *timerAct;
        QTimer *actTemps;
        QTimer *resetIndicator;

        BoardingTable* tableauBord;
        MapsView* carte;
        TableMgr* tableManager;

        SensorManager *sensormgr;
        ChronoReaderWidget* chronoWidget;

        QVector<QTableView*> tableauxHist;
        QVector<QPair<GraphicView*,QMdiSubWindow*> > graphiques;

        QTime h_depart;
        bool optimisation_graph;
        int nbSensors;

        QList<int> konami;
        int position=0;
        QWebView *kwebview;


    public slots:
        void message(QString message);
        void requestAct();
        void informationsReceived(QStringList);
        void syncTime();
        void resetIndicatorRx();
        void setIndicatorRx();

        void on_b_param_clicked();
        void on_b_table_clicked();
        void on_b_tb_clicked();
        void on_b_console_clicked();
        void on_actionQuitter_triggered();
        void on_actionOuvrir_triggered();

        void on_b_carte_clicked();
        void on_b_graph_clicked();
        void on_sel_capteur_currentIndexChanged(int index);

        void on_add_graph_clicked();

        void graphClosed();
        void optimise_graph();
        void on_btn_optimiser_clicked();
private slots:
        void on_actualizeTableButton_clicked();
        void on_horizontalSlider_sliderMoved(int position);
        void on_dataServerLineEdit_editingFinished();
        void on_heureLancement_timeChanged(const QTime &time);
        void on_konami_1_clicked();
        void on_konami_2_clicked();
        void on_konami_3_clicked();
        void on_konami_4_clicked();
        void on_konami_close_clicked() ;
};

#endif // FENPRINCIPALE_H


