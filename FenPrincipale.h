#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#ifndef QWT_DLL
#define QWT_DLL
#endif

#include "defines.h"


#include <QMainWindow>
#include <QDateTime>
#include <QVector>
#include <QTimer>
#include <QActionGroup>
#include <QWebView>
#include <QString>
#include <QList>
#include <QPair>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QInputDialog>


#include "input/pythondecoder.h"
#include "ui_FenPrincipale.h"
#include "input/sensormanager.h"
#include "input/serial.h"
#include "UI/boardingtable.h"
#include "UI/graphicview.h"
#include "UI/mapsview.h"
#include "balaifrequenciel.h"
#include "tablemgr.h"
#include "webservicesmanager.h"
#include "UI/timerangeselector.h"
#include "UI/stagesmanager.h"
#include "UI/balayagedialog.h"



class GraphicView;
class MapsView;
class BoardingTable;
class SensorManager;
class TableMgr;
class WebServicesManager;

class FenPrincipale : public QMainWindow, public Ui::FenPrincipale
{
    Q_OBJECT

    public:
        explicit FenPrincipale(Serial *com);
        ~FenPrincipale();

    // Accesseurs
        QDateTime       getDepart()     { return h_depart;}
        BoardingTable*  getBT()         {return tableauBord;}
        SensorManager*  getSensorMgr()  {return sensormgr; }
        MapsView*       getMap()        {return carte;}
        TableMgr*       getTableMgr()   {return tableManager;}
        StagesManager*  getStagesManager() {return stmgr; }

        TimeRangeSelector*  getGraphicTimeRangeSelector(){return graphic_range_selector;}
        WebServicesManager* getWebServicesManager();
    // Euh rien
        void konamify(bool enable);

        void loadSettings();
        void saveSettings();

    protected:
        void reinit_b();
        void resizeEvent(QResizeEvent *);
        bool eventFilter( QObject *o, QEvent *e );
        QVector<Data> getDataColumn(int idc, int idv, QVector<QVector<Data> > &data);

    private:
        // Timers
        QTimer*                 actTemps;
        QTimer*                 resetIndicator;
        QTimer*                 reset_errors_timer;

        TimeRangeSelector*      historique_range_selector;
        TimeRangeSelector*      graphic_range_selector;

        // UI
        BoardingTable*          tableauBord;
        MapsView*               carte;
        TableMgr*               tableManager;

        QActionGroup*           group_ports_name;
        QVector<QTableView*>    tableauxHist;
        QVector<QPair<GraphicView*,QMdiSubWindow*> > graphiques;

        // Données
        Serial*                 com;
        pythondecoder*          myDecoder;
        SensorManager*          sensormgr;

        WebServicesManager*     mWebServicesManager;

        // Trucs en vrac
        QDateTime               h_depart;
        bool                    optimisation_graph;
        int                     nbSensors;
        int                     position;
        int                     n;

        QList<int>              konami;
        QWebView*               kwebview;

        int                     nTramesRecues;
        int                     nTramesEchouees;
        int                     nTramesCorrigees;
        int                     nBytesRecus;
        StagesManager* stmgr;


    public slots:
        void onWebServicesNotification(int, QString);

        void updatePortListMenu();
        void portTriggered();
        void data_read(QVector<QVector<Data>> data, QString reason);


        void updateGraphs();

        void log_logger(QString str);
        void log_decoder(QString str);
        void log_serial(QString str);
        void log_database(QString str);
        void log_webservices(QString str);
        void log(int id, QString str);

        void received(QByteArray);
        void resetIndicatorRx();
        void setIndicatorRx();


        void incrementStatBytesRecus(int n);
        void incrementStatTramesRecues(int n);
        void incrementStatTramesEchouees(int n);
        void incrementStatTramesCorrigees(int n);


        // Slots de type event sur ui
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
        void on_actualizeTableButton_clicked();
        void on_dataServerLineEdit_editingFinished();
        void on_heureLancement_timeChanged(const QTime &time);
        void on_konami_1_clicked();
        void on_konami_2_clicked();
        void on_konami_3_clicked();
        void on_konami_4_clicked();
        void on_konami_close_clicked() ;
        void on_actionHaut_parleurs_toggled(bool arg1);
        void on_action137050_triggered();
        void on_action137500_triggered();
        void on_actionBalayage_frequentiel_triggered();
        void on_action600_triggered();
        void on_action9600_triggered();
        void on_action57600_triggered();
        void onRangeStartUpdate(QDateTime range_start);

        void on_metewowServerLineEdit_editingFinished();
        void on_metewowMacLineEdit_editingFinished();
        void on_metewowMdpLineEdit_editingFinished();
        void on_metewowServerCheckBox_clicked();
        void on_dataServerCheckBox_clicked();
        void on_metewowRegisterPushButton_clicked();
        void on_metewowDeletePushButton_clicked();
        void on_actionPasser_l_tape_suivante_triggered();
        void on_actionRemettre_z_ro_triggered();
        void on_actionPasser_l_tape_pr_c_dente_triggered();
        void on_actionEntrer_le_canal_manuellement_triggered();
        void on_xyModeCheckbox_toggled(bool checked);
        void on_sel_capteur_x_currentIndexChanged(int index);
};

#endif // FENPRINCIPALE_H


