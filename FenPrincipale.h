#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QtGui>
#include "ui_FenPrincipale.h"
#include "ChronoReader/chronoreaderwidget.h"
#include "InPut/sensormanager.h"
#include "InPut/cookiedecoder.h"
#include "InPut/serial.h"
#include "UI/boardingtable.h"
#include "UI/graphicview.h"
#include "UI/mapsview.h"
#include "balaifrequenciel.h"
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
        QDateTime getDepart() { return h_depart;}

        BoardingTable* getBT() {return tableauBord;}
        SensorManager* getSensorMgr(){return sensormgr; }
        MapsView* getMap() {return carte;}
        TableMgr* getTableMgr() {return tableManager;}
        void konamify(bool enable);

    protected:
        void reinit_b();
        void resizeEvent(QResizeEvent *);
        bool eventFilter( QObject *o, QEvent *e );
        bool already_added(int capteur, int valeur);

    private:
        Serial* com;
        QTimer *timerAct;
        QTimer *actTemps;
        QTimer *resetIndicator;
        QTimer *reset_errors_timer;

        CookieDecoder* myDecoder;
        BoardingTable* tableauBord;
        MapsView* carte;
        TableMgr* tableManager;

        SensorManager *sensormgr;
        ChronoReaderWidget* chronoWidget;

        QVector<QTableView*> tableauxHist;
        QVector<QPair<GraphicView*,QMdiSubWindow*> > graphiques;

        QDateTime h_depart;
        bool optimisation_graph;
        int nbSensors;

        QList<int> konami;
        int position;
        QWebView *kwebview;
        int n;

    public slots:
        void data_read(int idc, int idv, QVector<Data> data, QString reason);

        void error_frame();
        void reset_error();

        void message(QString message);
        void requestAct();
        void informationsReceived(QList<QByteArray>);
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
        void on_sel_valeur_currentIndexChanged(int index);

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
        void on_horizontalSlider_valueChanged(int value);
        void on_actionHaut_parleurs_toggled(bool arg1);
        void on_action137050_triggered();
        void on_action137500_triggered();
        void on_actionBalayage_frequentiel_triggered();
};

#endif // FENPRINCIPALE_H


