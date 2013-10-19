#include "FenPrincipale.h"
#include "InPut/fileimportdialog.h"

FenPrincipale::FenPrincipale(Serial* _com) {

    com = _com;
    optimisation_graph = false;
    setupUi(this);

    indicator_rx->setEnabled(false);

    resetIndicator=NULL;

    stack->setCurrentIndex(0);

    konami << Qt::Key_Up << Qt::Key_Up << Qt::Key_Down << Qt::Key_Down << Qt::Key_Left << Qt::Key_Right << Qt::Key_Left << Qt::Key_Right << Qt::Key_B << Qt::Key_A;
    this->installEventFilter(this);
    konamify(false);
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute (QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    this->setFocusPolicy(Qt::StrongFocus);
    kwebview = new QWebView();
    p_konami_layout->addWidget(kwebview);

    sensormgr = new SensorManager(this);
    QVector<Sensor*> sensorList = sensormgr->getSensors();
    nbSensors = sensorList.size();
    foreach(Sensor* s, sensorList)
        sel_capteur->addItem(s->getName());

    for(int i=0;i<nbSensors ;i++) {
        QTableView *t = new QTableView;
        tableauxHist.append(t);

        tab_historique->addTab(t,sensormgr->getSensor(i)->getName());

        QStandardItemModel *modele = new QStandardItemModel;
       // int v=1;
       // modele->setHorizontalHeaderItem(0,new QStandardItem("Temps"));
        int v=0;
        foreach(SensorValue* s,  sensormgr->getSensor(i)->getValues()) {
            modele->setHorizontalHeaderItem(v,new QStandardItem(s->getName()));
            v++;
        }

        t->setModel(modele);
    }

    tableManager = new TableMgr(&tableauxHist);
    carte = new MapsView(c_maps);


    message("[INFO] Loading boarding table...");

    tableauBord = new BoardingTable(container,sensormgr);
    message("[INFO] Loaded !");


    qDebug() << "pizzdrfa";
    message("[INFO] Starting refreshing timers");

    timerAct = new QTimer();
    connect(timerAct,SIGNAL(timeout()),this,SLOT(requestAct()));

    timerAct->start(UPDATE_TIME);

    actTemps = new QTimer();
    connect(actTemps,SIGNAL(timeout()),this,SLOT(syncTime()));

    actTemps->start(1000);

    myDecoder = new CookieDecoder();
    connect(myDecoder, SIGNAL(newValue(int,int,double)), sensormgr, SLOT(newValue(int,int,double)));
    connect(myDecoder,SIGNAL(error_frame()), this, SLOT(error_frame()));
    message("[INFO] All started !");



    connect(com,SIGNAL(dataRead(QList<QByteArray>)),this,SLOT(informationsReceived(QList<QByteArray>)));
    requestAct();

    h_depart = QTime::currentTime();
    chronoWidget = new ChronoReaderWidget;
    chronolayout->addWidget(chronoWidget);


    QFile file("conf/url.ini");
    file.open(QIODevice::ReadOnly);
    QString url = QString(file.readAll());
    file.close();
    dataServerLineEdit->setText(url);

    QFile df("conf/datedepart.ini");
    df.open(QIODevice::ReadOnly);
    QString date = QString(df.readAll());
    df.close();

    if(date.size() == 3) {
        int h=date.split(":").at(0).toInt();
        int m=date.split(":").at(1).toInt();
        int s=date.split(":").at(2).toInt();

        heureLancement->setTime(QTime(h,m,s));
        chronoWidget->laucherCounter(QTime(h,m,s));
    } else {
        heureLancement->setTime(QTime::currentTime());
        chronoWidget->laucherCounter(QTime::currentTime());
    }

    QPalette p = indicator_rx->palette();
    p.setColor(QPalette::Disabled, QPalette::Background, QColor(255,0,0));
    indicator_rx->setPalette(p);

}

FenPrincipale::~FenPrincipale(){

}

void FenPrincipale::resizeEvent(QResizeEvent *) {
    if(optimisation_graph)
        optimise_graph();

}

void FenPrincipale::requestAct() {
    if(get_infos->isChecked())
        com->readData();
}

void FenPrincipale::syncTime() {
    int h,m,s;
    h = QTime::currentTime().hour();
    m = QTime::currentTime().minute();
    s = QTime::currentTime().second();

    if(s==42) {
        QPalette palette = lcd_sec->palette();
        palette.setColor(QPalette::Normal, QPalette::Foreground, Qt::red);
        lcd_sec->setPalette(palette);
    } else if((s>=42)&&(s<=45)) {
        s = 42;
    }else if(s == 46) {
        lcd_sec->setPalette(lcd_hour->palette());
    }


    if(m==42) {
        QPalette palette = lcd_min->palette();
        palette.setColor(QPalette::Normal, QPalette::Foreground, Qt::red);
        lcd_min->setPalette(palette);
    } else if(m == 43) {
        lcd_min->setPalette(lcd_hour->palette());
    }

    lcd_hour->display(QString("%1").arg(h, 10, 10, QChar('0').toUpper()));
    lcd_min->display(QString("%1").arg(m, 10, 10, QChar('0').toUpper()));
    lcd_sec->display(QString("%1").arg(s, 10, 10, QChar('0').toUpper()));
}

void FenPrincipale::informationsReceived(QList<QByteArray> trames) {
    setIndicatorRx();
    if(trames.size() > 0) {
        for(int i=0;i<trames.size();i++) {

         /*   QString hex = "0x";
            for(int j=0;j<trames[i].size();j++) {
                hex += QString("%1").arg(trames[i].at(j)& 0xff, 2,16).toUpper();
            }

            this->message("[DATA] " + hex);*/
            myDecoder->decodeString(trames[i]);
          //  sensormgr->addData(trames[i]);
        }

        QPair<GraphicView*,QMdiSubWindow*> value;
        foreach(value,graphiques) {
            value.first->majData();
        }
    }
}

void FenPrincipale::reset_error() {//TODO: Ne pas hardcoder ça.
    this->sensormgr->getSensor(9)->getValues().at(0)->addData(0);
    this->getBT()->update(this->sensormgr->getSensor(9)->getValues().at(0));
}

void FenPrincipale::error_frame() {
    SensorValue* sv =  this->sensormgr->getSensor(9)->getValues().at(0);
    Data* d = sv->getData().last();
    d->value++;
    this->getBT()->update(sv);
    this->message("[ERROR] Checksum failed.");
}

void FenPrincipale::setIndicatorRx() {


    resetIndicator = new QTimer();
    n++;
    connect(resetIndicator,SIGNAL(timeout()),this,SLOT(resetIndicatorRx()));

    resetIndicator->start(500);

    indicator_rx->setChecked(true);
}

void FenPrincipale::resetIndicatorRx() {
    n--;
    if(n==0)
        indicator_rx->setChecked(false);
}

void FenPrincipale::message(QString message){

    message = QDateTime::currentDateTime().toString("[hh:mm:ss] ") + message;

    barreStatus->showMessage(message);
    console->appendPlainText(message);
}


void FenPrincipale::on_actionQuitter_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Pourquoi la vie? Pourquoi la mort?","Êtes vous sûr de ce que vous faites?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      QApplication::quit();
    }
}

void FenPrincipale::on_actionOuvrir_triggered()
{
    FileImportDialog *fi_dialog = new FileImportDialog(this);
    fi_dialog->show();
}

void FenPrincipale::on_b_console_clicked()
{
    reinit_b();
    b_console->setDefault(true);

    stack->setCurrentIndex(1);
}

void FenPrincipale::on_b_tb_clicked()
{
    reinit_b();
    b_tb->setDefault(true);

    stack->setCurrentIndex(0);
}

void FenPrincipale::on_b_table_clicked()
{
    reinit_b();
    b_table->setDefault(true);

    stack->setCurrentIndex(2);
}

void FenPrincipale::on_b_param_clicked()
{
    reinit_b();
    b_param->setDefault(true);

    stack->setCurrentIndex(3);
}

void FenPrincipale::on_b_graph_clicked()
{
    reinit_b();
    b_graph->setDefault(true);

    stack->setCurrentIndex(4);
}

void FenPrincipale::on_b_carte_clicked()
{
    reinit_b();
    b_carte->setDefault(true);

    stack->setCurrentIndex(5);
}

void FenPrincipale::reinit_b(){
    b_tb->setDefault(false);
    b_table->setDefault(false);
    b_console->setDefault(false);
    b_param->setDefault(false);
    b_graph->setDefault(false);
    b_carte->setDefault(false);
    konami_1->setDefault(false);
    konami_2->setDefault(false);
    konami_3->setDefault(false);
    konami_4->setDefault(false);

}


bool FenPrincipale::already_added(int capteur, int valeur) {
    for(int i=0;i<graphiques.size();i++) {
        if((graphiques.at(i).first->getCapteur() == capteur) && (graphiques.at(i).first->getValeur() == valeur))
            return true;
    }
    return false;
}

void FenPrincipale::on_sel_capteur_currentIndexChanged(int index)
{
    sel_valeur->clear();
    bool added_something=false;

    foreach(SensorValue *v, sensormgr->getSensor(index)->getValues()) {
        if(!already_added(index, v->getID())) {
            sel_valeur->addItem(v->getName(), QVariant(v->getID()));
            added_something=true;
        }
    }

    add_graph->setEnabled(added_something);
}

void FenPrincipale::on_sel_valeur_currentIndexChanged(int index) {
    // Je voulais faire quelque chose ici, mais je ne me rappelle plus de quoi.
    // Alors j'ai fait ça.
    if(index==42)
        qApp->quit();
}

void FenPrincipale::on_add_graph_clicked()
{
    GraphicView* g = new GraphicView(sel_capteur->currentIndex(), sel_valeur->itemData(sel_valeur->currentIndex()).toInt(),this);

    QPair<GraphicView*,QMdiSubWindow*> group;
    QMdiSubWindow *w = zone_graph->addSubWindow(g);
    group.first = g;
    group.second = w;
    graphiques.append(group);

    w->setGeometry(w->geometry().x(),w->geometry().y(),200,200);

    w->show();

    connect(g,SIGNAL(destroyed()),this,SLOT(graphClosed()));

    if(optimisation_graph) {
        optimise_graph();
    }
    g->setWindowTitle(g->windowTitle());
    on_sel_capteur_currentIndexChanged(sel_capteur->currentIndex());
}

void FenPrincipale::graphClosed() {
    GraphicView* g = (GraphicView *)sender();

    int capteur=g->getCapteur();

    for(int i=0;i<graphiques.size();i++) {
        if(g == graphiques[i].first)
            graphiques.remove(i);
    }

    if(optimisation_graph) {
        optimise_graph();
    }

    on_sel_capteur_currentIndexChanged(capteur);
}

void FenPrincipale::on_btn_optimiser_clicked()
{
    if(btn_optimiser->isChecked())
        optimise_graph();

    optimisation_graph = btn_optimiser->isChecked();
}

void FenPrincipale::on_actualizeTableButton_clicked()
{
    if(check_all_values->isChecked()) {
        //TODO: Travailler
    } else {
        QTime start = time_start->time();
        QTime end   =   time_end->time();

        if(TimeCalcs::toMs(start) < TimeCalcs::toMs(end)) {
            start = TimeCalcs::fromMs(TimeCalcs::toMs(start) + TimeCalcs::toMs(h_depart));
            end   = TimeCalcs::fromMs(TimeCalcs::toMs(end) + TimeCalcs::toMs(h_depart));
            tableManager->actualisay(start,end,sensormgr);
        } else {
            start = TimeCalcs::fromMs(TimeCalcs::toMs(QTime::currentTime()) - TimeCalcs::toMs(time_start->time()));
            end   = TimeCalcs::fromMs(TimeCalcs::toMs(QTime::currentTime()) - TimeCalcs::toMs(time_end->time()));
            tableManager->actualisay(start,end,sensormgr);
        }
    }
}


void FenPrincipale::on_dataServerLineEdit_editingFinished()
{
    QFile file("conf/url.ini");
    file.open(QIODevice::ReadWrite);
    file.resize(0);
    file.write(dataServerLineEdit->text().toStdString().c_str());
    file.close();
}

void FenPrincipale::on_heureLancement_timeChanged(const QTime &time)
{
    QFile file("conf/datedepart.ini");
    file.open(QIODevice::ReadWrite);
    file.resize(0);
    file.write((QString::number(heureLancement->time().hour())+":"+QString::number(heureLancement->time().minute())+":"+QString::number(heureLancement->time().second())).toStdString().c_str());
    file.close();

}

void FenPrincipale::on_horizontalSlider_sliderMoved(int position)
{
    int heures = 0;
    int minutes = 0;

    if(position<60) {
        if(position == 1)
            bowltext->setText(QString::number(position) + " minute");
        else
            bowltext->setText(QString::number(position) + " minutes");

        minutes = position;
    } else  {
        heures = (position-position%60)/60;
        QString heureMsg = heures>1 ? " heures ":" heure ";

        minutes = position%60;
        QString minutesMsg = minutes>1 ? " minutes ":" minute ";

        if(minutes == 0)
            bowltext->setText(QString::number(heures) + heureMsg);
        else
            bowltext->setText(QString::number(heures) + heureMsg + QString::number(minutes) + minutesMsg);
    }

    QPair<GraphicView*,QMdiSubWindow*> value;
    foreach(value,graphiques) {
       value.first->majData(QTime(heures,minutes,0));
    }
}

bool FenPrincipale::eventFilter( QObject *o, QEvent *e ) {
    if ( e->type() == QEvent::KeyPress ) {
        QKeyEvent *k = (QKeyEvent *)e;
        if(position >= konami.size())
            return false;

        if(konami.at(position) == k->key()) {
            ++position;
        } else {
            position = 0;
        }
        qDebug() << "yolo=" << position;

        if(position>=konami.size()) {
            qDebug() << "Trop pimp";
            konamify(true);
        }
    } else if(e->type() == QEvent::Close) {
        e->ignore();
        this->on_actionQuitter_triggered();
        return true;
    }
    return false;
}

void FenPrincipale::konamify(bool enable) {
    if(enable) {
        konami_1->setVisible(true);
        konami_2->setVisible(true);
        konami_3->setVisible(true);
        konami_4->setVisible(true);
        konami_close->setVisible(true);

        this->setWindowTitle("OMG OMG OMG OMG OMG OMG OMG OMG OMG OMG TROP PIMP");
    } else {
        position = 0;
        konami_1->setVisible(false);
        konami_2->setVisible(false);
        konami_3->setVisible(false);
        konami_4->setVisible(false);
        konami_close->setVisible(false);

        this->setWindowTitle("To log, or not to log ? Random compilation.");
    }
}

void FenPrincipale::on_konami_1_clicked() // nyan
{
    reinit_b();
    konami_1->setDefault(true);

    stack->setCurrentIndex(6);
    if(kwebview->url().toString() != "http://www.nyan.cat/original.php")
        kwebview->load(QUrl("http://www.nyan.cat/original.php"));
}

void FenPrincipale::on_konami_2_clicked() // gswitch
{
    reinit_b();
    konami_2->setDefault(true);

    stack->setCurrentIndex(6);
    if(kwebview->url().toString() != "http://uploads.ungrounded.net/526000/526596_GSwitch.swf")
        kwebview->load(QUrl("http://uploads.ungrounded.net/526000/526596_GSwitch.swf"));
}

void FenPrincipale::on_konami_3_clicked() // trololo
{
    reinit_b();
    konami_3->setDefault(true);

    stack->setCurrentIndex(6);
    if(kwebview->url().toString() != "http://www.youtube.com/watch?v=oavMtUWDBTM")
        kwebview->load(QUrl("http://www.youtube.com/watch?v=oavMtUWDBTM"));

}

void FenPrincipale::on_konami_4_clicked() // google
{
    reinit_b();
    konami_4->setDefault(true);

    stack->setCurrentIndex(6);
    if(kwebview->url().toString() != "http://www.google.fr")
        kwebview->load(QUrl("http://www.google.fr"));
}

void FenPrincipale::on_konami_close_clicked() {
    konamify(false);
    reinit_b();

    stack->setCurrentIndex(0);
}

void FenPrincipale::optimise_graph() {
    int nbItems = graphiques.size();
    qDebug() << "Nb graphs: " << nbItems;
    qDebug() << "Taille w: " << zone_graph->geometry().width() << "| h: " << zone_graph->geometry().height();


    int rowNb;
    int rowSize = zone_graph->geometry().height();
    int colNb;
    int colSize = zone_graph->geometry().width();

    for(int i=0;i<graphiques.size();i++) {
        int row, rowStretch, col, colStretch;
        bool error = false;
        switch(nbItems) {
            case 1:
                row = 0;
                rowStretch = 1;
                col = 0;
                colStretch = 1;
                rowNb = 1;
                colNb = 1;
                break;
            case 2:
                row = 0;
                rowStretch = 1;
                col = i;
                colStretch = 1;
                rowNb = 1;
                colNb = 2;
                break;
            case 3:
                row = !(i == 0);
                rowStretch = 1;
                col = (i == 2);
                colStretch = (i == 0) ? 2 : 1;
                rowNb = 2;
                colNb = 2;
                break;
            case 4:
                row = !(i<=1);
                rowStretch = 1;
                col = !((i==0) || (i==2));
                colStretch = 1;
                rowNb = 2;
                colNb = 2;
                break;
            case 5:
                row = !(i<3);
                rowStretch = (i==1) +1;
                col = (i<=2) ? i : ((i == 3) ? 0 : 2 );
                colStretch = 1;
                rowNb = 2;
                colNb = 3;
                break;
            case 6:
                row = !(i<3);
                rowStretch = 1;
                col = (i<=2) ? i : i-3;
                colStretch = 1;
                rowNb = 2;
                colNb = 3;
                break;
            case 7:
                row = !(i<3);
                rowStretch = 1;
                col = (i<=2) ? ((i==2) ? 3 : i): i-3;
                colStretch = (i == 1) ? 2 : 1;
                rowNb = 2;
                colNb = 4;
                break;
            case 8:
                row = !(i<=3);
                rowStretch = 1;
                col = (i<=3) ? i : i-4;
                colStretch = 1;
                rowNb = 2;
                colNb = 4;
                break;
            case 9:
                if(i<=4)
                    row = 0;
                else if(i<=7)
                    row = 1;
                else
                    row = 2;

                rowStretch = ((i == 5) || (i ==7)) + 1;

                if(i<=4) {
                    col = i;
                    colStretch = 1;
                } else if(i == 5) {
                    col = 0;
                    colStretch = 1;
                } else if(i == 6) {
                    col = 1;
                    colStretch = 3;
                } else if(i == 7) {
                    col = 4;
                    colStretch = 1;
                } else if(i == 8) {
                    col = 1;
                    colStretch = 3;
                }


                rowNb = 3;
                colNb = 5;
                break;
            case 10:
                if(i<=3)
                    row = 0;
                else if(i<=5)
                    row = 1;
                else
                    row = 2;

                rowStretch = 1;
                colStretch = ((i == 4) || (i == 5)) + 1;

                if(i<=3) {
                    col = i;
                } else if(i >= 6) {
                    col = i-6;
                } else if(i == 4) {
                    col = 0;
                } else if(i == 5) {
                    col = 2;
                }


                rowNb = 3;
                colNb = 4;
                break;
            default:
                error = true;
        }
        if(!error)
            graphiques[i].second->setGeometry(col * (colSize / colNb),row * (rowSize / rowNb), (colSize / colNb) * colStretch, (rowSize / rowNb) * rowStretch);
    }

}
