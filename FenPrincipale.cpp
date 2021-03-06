#include "FenPrincipale.h"
#include "input/fileimportdialog.h"

FenPrincipale::FenPrincipale(Serial* _com) {
    // Initialisation paramètres
    position=0;
    n=0;
    com = _com;

    optimisation_graph = false;
    resetIndicator=NULL;
    // --------------------
    // Compilation de l'UI
    setupUi(this);
    indicator_rx->setEnabled(false);
    stack->setCurrentIndex(0);
    // --------------------
    // Code Konami (shhht)
    konami << Qt::Key_Up << Qt::Key_Up << Qt::Key_Down << Qt::Key_Down << Qt::Key_Left << Qt::Key_Right << Qt::Key_Left << Qt::Key_Right << Qt::Key_B << Qt::Key_A;
    this->installEventFilter(this);
    konamify(false);
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute (QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    this->setFocusPolicy(Qt::StrongFocus);
    kwebview = new QWebView();
    p_konami_layout->addWidget(kwebview,1);
    // --------------------
    if(com->isOkay()) {
        log_serial("Port série activé avec succès");
    } else {
        log_serial("Problème d'ouverture du port série");
    }

    // Initialisation radio buttons de la barre de menu (sélection port/baudrate)
    QActionGroup* group = new QActionGroup( this );
    this->action137050->setCheckable(true);
    this->action137500->setCheckable(true);
    this->action137500->setChecked(true);
    this->action137050->setActionGroup(group);
    this->action137500->setActionGroup(group);
    QActionGroup* group_ports = new QActionGroup( this );
    this->action600->setCheckable(true);
    this->action9600->setCheckable(true);
    this->action57600->setCheckable(true);
    switch(com->getBaudrate()) {
        case 600:
            this->action600->setChecked(true);
            break;
        case 9600:
            this->action9600->setChecked(true);
            break;
        case 57600:
            this->action57600->setChecked(true);
            break;
    }

    this->action137500->setChecked(true);
    this->action600->setActionGroup(group_ports);
    this->action9600->setActionGroup(group_ports);
    this->action57600->setActionGroup(group_ports);
    group_ports_name = new QActionGroup(this);

    connect(this->menuPort, SIGNAL( aboutToShow()), this, SLOT( updatePortListMenu()));
    // -----------------
    // Initialisation des capteurs disponibles
    sensormgr = new SensorManager(this);
    QVector<Sensor*> sensorList = sensormgr->getSensors();
    nbSensors = sensorList.size();
    log_decoder("Lecture de la liste des capteurs depuis 'conf/cplist.xml'");
    foreach(Sensor* s, sensorList) {
        sel_capteur->addItem(s->getName());
        sel_capteur_x->addItem(s->getName());
    }

    sel_capteur_x->setEnabled(false);
    sel_valeur_x->setEnabled(false);

    for(int i=0;i<nbSensors ;i++) {
        QTableView *t = new QTableView;
        tableauxHist.append(t);

        tab_historique->addTab(t,sensormgr->getSensor(i)->getName());

        QStandardItemModel *modele = new QStandardItemModel;
        int v=0;

        log_decoder("||");
        log_decoder("|| Capteur : "+sensormgr->getSensor(i)->getName());

        foreach(SensorValue* s,  sensormgr->getSensor(i)->getValues()) {
            if(s->getUnit().isEmpty())
                log_decoder("|| ↳ "+s->getName());
            else log_decoder("|| ↳ "+s->getName()+ " en "+s->getUnit());

            modele->setHorizontalHeaderItem(v,new QStandardItem(s->getName()));
            v++;
        }
        modele->setHorizontalHeaderItem(v,new QStandardItem("Temps"));
        t->horizontalHeader()->setStretchLastSection(true);
        t->setModel(modele);
    }



    qRegisterMetaType<QVector<Data> > ("QVector<Data>");
    qRegisterMetaType<QVector<QVector<Data> > > ("QVector<QVector<Data> >");
    // Connect entre la bdd et fenprincipale pour les réponses aux requêtes de lecture
    connect(sensormgr->getDB(),SIGNAL(dataRead(QVector<QVector<Data>>,QString)), this,SLOT(data_read(QVector<QVector<Data>>,QString)));
    connect(sensormgr->getDB(),SIGNAL(message(QString)), this,SLOT(log_database(QString)));
    // -------------------
    // Initialisation des différentes pages
    graphic_range_selector = new TimeRangeSelector();
    p_graphics_range_selection_layout->addLayout(graphic_range_selector);

    historique_range_selector = new TimeRangeSelector();
    p_historique_range_selection_layout->addLayout(historique_range_selector);

    tableManager = new TableMgr(&tableauxHist,sensormgr);
    carte = new MapsView(c_maps);
    log_logger("[INFO] Loading boarding table...");
    tableauBord = new BoardingTable(container,sensormgr);
    log_logger("[INFO] Loaded !");

    // -----------------------------
    // Démarrage du décodeur de trames
    myDecoder = new pythondecoder();

    connect(myDecoder,SIGNAL(newFrame(QVector<double>)), sensormgr, SLOT(newFrame(QVector<double>)));
    connect(myDecoder,SIGNAL(message(QString)),this,SLOT(log_decoder(QString)));
    connect(myDecoder,SIGNAL(trame_erreur(int)),this, SLOT(incrementStatTramesEchouees(int)));
    connect(myDecoder,SIGNAL(trame_increment(int)),this,SLOT(incrementStatTramesRecues(int)));
    connect(myDecoder,SIGNAL(trame_corrigee(int)),this,SLOT(incrementStatTramesCorrigees(int)));

    myDecoder->init();
    log_logger("[INFO] All started !");
    // -----------------------------
    // Prise en charge du port série
    connect(com,SIGNAL(received(QByteArray)),this,SLOT(received(QByteArray)));
    connect(com,SIGNAL(message(QString)),this,SLOT(log_serial(QString)));
    connect(com, SIGNAL(nBytesRead(int)),this, SLOT(incrementStatBytesRecus(int)));
    // -------------------
    // Implémentation du chronoreader
   /* h_depart = QDateTime::currentDateTime();
    chronoWidget = new ChronoReaderWidget;
    chronolayout->addWidget(chronoWidget);*/
    // --------------------
    // Lecture des paramètres enregistrés
    QSettings *data_settings = new QSettings();
    loadSettings();

    QDateTime t = data_settings->value("datedepart",QDateTime::currentDateTime()).toDateTime();
    heureLancement->setDateTime(t);

    delete data_settings;

    mWebServicesManager = new WebServicesManager(this);
    connect(mWebServicesManager, SIGNAL(notification(int,QString)), this, SLOT(onWebServicesNotification(int,QString)));
    connect(mWebServicesManager, SIGNAL(message(QString)), this, SLOT(log_webservices(QString)));

    mWebServicesManager->test(DATASERVER);
    mWebServicesManager->test(METEWOWSERVER);
    // -------------------------
    // Couleur custom pour l'indicateur de réception d'informations
    QPalette p = indicator_rx->palette();
    p.setColor(QPalette::Disabled, QPalette::Background, QColor(255,0,0));
    indicator_rx->setPalette(p);
    //---------------------------


    // --------------------------------



    stmgr = new StagesManager(stageGraphicsView);

    nTramesRecues = 0;
    nTramesEchouees = 0;
    nBytesRecus = 0;
    nTramesCorrigees = 0;
    QTimer* teemo = new QTimer();
    connect(teemo, SIGNAL(timeout()), stmgr, SLOT(render()));
    teemo->setSingleShot(true);
    teemo->start(50);



    connect(graphic_range_selector, SIGNAL(startDateChanged(QDateTime)), this, SLOT(updateGraphs()));
    connect(graphic_range_selector, SIGNAL(endDateChanged(QDateTime)),  this, SLOT(updateGraphs()));

    sensormgr->getDB()->readFrame(QDateTime::fromTime_t(0),QDateTime::currentDateTime(),"bt",true);

    zone_graph->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);
}

FenPrincipale::~FenPrincipale(){
    delete com;
}

void FenPrincipale::updateGraphs() {
    sensormgr->getDB()->readFrame(graphic_range_selector->getLowerDate(), graphic_range_selector->getUpperDate(),"graph",false);
}

void FenPrincipale::resizeEvent(QResizeEvent *) {
    if(optimisation_graph)
        optimise_graph();
    if(stmgr != 0)
        stmgr->render();
    if(tableauBord != 0)
        tableauBord->onResize();
}

/*
 * Outil de balayage fréquenciel
 */


void FenPrincipale::on_actionBalayage_frequentiel_triggered()
{
    BalayageDialog* tamer = new BalayageDialog(new BalaiFrequenciel(com,this));
    tamer->show();
}

void FenPrincipale::onRangeStartUpdate(QDateTime range_start) {
    graphic_range_selector->setMinimumDate(range_start);
    historique_range_selector->setMinimumDate(range_start);
}


/*
 * Gestion des données
 */

void FenPrincipale::received(QByteArray d) {
    myDecoder->appendData(d);

    QPair<GraphicView*,QMdiSubWindow*> value;
    foreach(value,graphiques) {
        value.first->majData();
    }
}


QVector<Data> FenPrincipale::getDataColumn(int idc, int idv, QVector<QVector<Data> > &data) {
    int i = sensormgr->indexOf(idc,idv);
    QVector<Data> col;
    foreach(QVector<Data> line, data)
        col.push_back(line[i]);
    return col;
}

void FenPrincipale::data_read(QVector<QVector<Data> > data, QString reason) {
    if(reason == "graph") {
        for(int i=0;i<graphiques.size();i++) {
            GraphicView* g = graphiques.at(i).first;
            if(g->isXY()) {
                g->setDataX(getDataColumn(g->getCapteur_x(),g->getValeur_x(),data));
                g->setDataY(getDataColumn(g->getCapteur(),g->getValeur(),data));
            } else {
                g->setData(getDataColumn(g->getCapteur(),g->getValeur(),data));
            }

        }
    } else if(reason == "bt") {
        if(data.size() > 0) {
            this->getBT()->update(data.last());

            qDebug() << "maximum date:" << data.last()[0].time;

            graphic_range_selector->setMaximumDate(data.last()[0].time);
            historique_range_selector->setMaximumDate(data.last()[0].time);
        }
    } else if(reason == "xmap") {
        //this->getMap()->onStartDataX(data);
    } else if(reason == "ymap") {
        //this->getMap()->onStartDataY(data);
    } else if(reason == "tab") {
        this->getTableMgr()->actualisay(data);
    }
}

/*
 * Quelques fonctions de display
 */
void FenPrincipale::setIndicatorRx() {
    resetIndicator = new QTimer();
    n++;
    connect(resetIndicator,SIGNAL(timeout()),this,SLOT(resetIndicatorRx()));

    resetIndicator->setSingleShot(true);
    resetIndicator->start(500);

    indicator_rx->setChecked(true);
}

void FenPrincipale::resetIndicatorRx() {
    n--;
    if(n==0)
        indicator_rx->setChecked(false);
}

void FenPrincipale::log_logger      (QString message){ log(0, message);}
void FenPrincipale::log_serial      (QString message){ log(1, message);}
void FenPrincipale::log_decoder     (QString message){ log(2, message);}
void FenPrincipale::log_database    (QString message){ log(3, message);}
void FenPrincipale::log_webservices (QString message){ log(4, message);}

void FenPrincipale::log(int section, QString message) {
    message = QDateTime::currentDateTime().toString("[hh:mm:ss] ") + message;

    switch(section) {
    case 0: //logger
        console->appendPlainText(message);
        break;
    case 1: //serial
        console_serie->append(message);
        break;
    case 2: //decoder
        console_decodeur->append(message);
        break;
    case 3: // bdd
        console_bdd->append(message);
        break;
    case 4: //web
        console_webservices->append(message);
        break;
    }
}


void FenPrincipale::incrementStatBytesRecus(int n) {
    nBytesRecus += n;
    stats_octets_recus->setText("Octets reçus: "+QString::number(nBytesRecus));
}

void FenPrincipale::incrementStatTramesRecues(int n) {
    nTramesRecues += n;
    stats_trames_recues->setText("Trames reçues: "+QString::number(nTramesRecues));
}

void FenPrincipale::incrementStatTramesEchouees(int n) {
    nTramesEchouees += n;
    stats_trames_echouees->setText("Trames échouées: "+QString::number(nTramesEchouees));
}
void FenPrincipale::incrementStatTramesCorrigees(int n) {
    nTramesCorrigees += n;
    stats_trames_corrigees->setText("Trames corrigées: "+QString::number(nTramesCorrigees));
}

/*
 * Gestion des actions du menu supérieur (tout comme le jambon)
 */
void FenPrincipale::on_actionQuitter_triggered() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Pourquoi la vie? Pourquoi la mort?","Êtes vous sûr de ce que vous faites?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
      QApplication::quit();
}

void FenPrincipale::on_actionOuvrir_triggered() {
    FileImportDialog *fi_dialog = new FileImportDialog(this);
    fi_dialog->show();
}

void FenPrincipale::portTriggered() {
    QObject* sender = QObject::sender();
    QAction* act = qobject_cast<QAction*>(sender);
    if(act) {
        com->setPort(act->data().toString());
        log(1,"Port de réception: " + act->data().toString());
    }
}


void FenPrincipale::updatePortListMenu() {
    this->menuPort->clear();
    QStringList portList = Serial::getPortList();
    for(int i=0;i<portList.size();i++) {
        QString cur_port = portList.at(i);
        QAction* portAction = new QAction(cur_port,this);
        portAction->setData(QVariant(cur_port));
        portAction->setCheckable(true);
        group_ports_name->addAction(portAction);

        if(com->getPort() == cur_port)
            portAction->setChecked(true);

        connect(portAction,SIGNAL(triggered()),this,SLOT(portTriggered()));
        this->menuPort->addAction(portAction);
    }
}

void FenPrincipale::on_actionHaut_parleurs_toggled(bool arg1) {com->setSpeakersEnabled(arg1);}
void FenPrincipale::on_action600_triggered() { com->setBaudrate(600);}
void FenPrincipale::on_action57600_triggered() {com->setBaudrate(57600);}
void FenPrincipale::on_action9600_triggered() {com->setBaudrate(9600);}
void FenPrincipale::on_action137050_triggered() {com->setChannel(21);}
void FenPrincipale::on_action137500_triggered(){ com->setChannel(30);}

void FenPrincipale::on_actionEntrer_le_canal_manuellement_triggered() {
    QStringList canaux;
    for(int i=0;i<=30;i++) {
        canaux.append(QString::number(136.000 + 0.050*i, 'f', 3) + " MHz");
    }
    QString item = QInputDialog::getItem(this,"Choisir le canal","", canaux, 21, false);

    int index = canaux.indexOf(item);
    com->setChannel(index);

    action137050->setChecked(false);
    action137500->setChecked(false);

    if(index == 21)
        action137050->setChecked(true);
    else if(index == 30)
        action137500->setChecked(true);
}

/*
 * Gestion des boutons du menu de gauche
 */
void FenPrincipale::on_b_tb_clicked(){reinit_b();b_tb->setDefault(true);stack->setCurrentIndex(0);}
void FenPrincipale::on_b_console_clicked(){reinit_b();b_console->setDefault(true);stack->setCurrentIndex(1);}
void FenPrincipale::on_b_table_clicked(){reinit_b();b_table->setDefault(true);stack->setCurrentIndex(2);}
void FenPrincipale::on_b_param_clicked(){reinit_b();b_param->setDefault(true);stack->setCurrentIndex(3);}
void FenPrincipale::on_b_graph_clicked(){reinit_b();b_graph->setDefault(true);stack->setCurrentIndex(4);}
void FenPrincipale::on_b_carte_clicked(){reinit_b();b_carte->setDefault(true);stack->setCurrentIndex(5);}

void FenPrincipale::reinit_b() {
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

/*
 * Actions liées aux graphiques
 */

void FenPrincipale::on_sel_capteur_currentIndexChanged(int index) {
    sel_valeur->clear();
    foreach(SensorValue *v, sensormgr->getSensor(index)->getValues())
        sel_valeur->addItem(v->getName(), QVariant(v->getID()));
}



void FenPrincipale::on_sel_capteur_x_currentIndexChanged(int index) {
    sel_valeur_x->clear();
    foreach(SensorValue *v, sensormgr->getSensor(index)->getValues())
        sel_valeur_x->addItem(v->getName(), QVariant(v->getID()));
}


void FenPrincipale::on_add_graph_clicked() {
    int selected_sensor = sel_capteur->currentIndex();
    int selected_value  = sel_valeur->itemData(sel_valeur->currentIndex()).toInt();
    if(selected_sensor == -1)
        return;

    GraphicView* g;
    if(xyModeCheckbox->isChecked()) {
        int selected_sensor_x = sel_capteur_x->currentIndex();
        int selected_value_x  = sel_valeur_x->itemData(sel_valeur_x->currentIndex()).toInt();
        if(selected_sensor_x == -1)
            return;

        g = new GraphicView(selected_sensor, selected_value, selected_sensor_x, selected_value_x, this);
    } else {
        g = new GraphicView(selected_sensor, selected_value,this);
    }

    connect(graphic_range_selector, SIGNAL(startDateChanged(QDateTime)), g, SLOT(setStartDT(QDateTime)));
    connect(graphic_range_selector, SIGNAL(endDateChanged(QDateTime)), g, SLOT(setEndDT(QDateTime)));


    QPair<GraphicView*,QMdiSubWindow*> group;
    QMdiSubWindow *w = zone_graph->addSubWindow(g);
    group.first = g;
    group.second = w;
    graphiques.append(group);

    w->setGeometry(w->geometry().x(),w->geometry().y(),500,500);
    w->show();

    connect(g,SIGNAL(destroyed()),this,SLOT(graphClosed()));

    if(optimisation_graph)
        optimise_graph();

    g->setWindowTitle(g->windowTitle());
    on_sel_capteur_currentIndexChanged(sel_capteur->currentIndex());
    on_sel_capteur_x_currentIndexChanged(sel_capteur_x->currentIndex());

    g->setRange(graphic_range_selector->getLowerDate(), graphic_range_selector->getUpperDate());
    g->majData();
}

void FenPrincipale::graphClosed() {
    GraphicView* g = (GraphicView *)sender();

    int capteur = g->getCapteur();

    for(int i=0; i < graphiques.size(); i++) {
        if(g == graphiques[i].first)
            graphiques.remove(i);
    }

    if(optimisation_graph)
        optimise_graph();


    on_sel_capteur_currentIndexChanged(capteur);
    on_sel_capteur_x_currentIndexChanged(capteur);

    disconnect(graphic_range_selector, SIGNAL(startDateChanged(QDateTime)), g, SLOT(setStartDT(QDateTime)));
    disconnect(graphic_range_selector, SIGNAL(endDateChanged(QDateTime)), g, SLOT(setEndDT(QDateTime)));
}


/*
 * Actions liée à la table des valeurs
 */
void FenPrincipale::on_actualizeTableButton_clicked() {
    tableManager->requestActualization(historique_range_selector->getLowerDate(), historique_range_selector->getUpperDate());
}

void FenPrincipale::onWebServicesNotification(int type, QString text) {
    if(type == DATASERVER)
        dataServerCheckBox->setText(text);
    else if(type == METEWOWSERVER)
        metewowServerCheckBox->setText(text);
}

/*
 * Paramètres
 */


void FenPrincipale::loadSettings() {
    QSettings *data_settings = new QSettings();
    dataServerLineEdit->setText(data_settings->value("dataserverurl", "").toString());
    dataServerCheckBox->setChecked(data_settings->value("dataserverenabled", false).toBool());
    metewowServerLineEdit->setText(data_settings->value("metewowserverurl", "").toString());
    metewowServerCheckBox->setChecked(data_settings->value("metewowserverenabled", false).toBool());
    metewowMacLineEdit->setText(data_settings->value("metewowid","LOGGER").toString());
    metewowMdpLineEdit->setText(data_settings->value("metewowmdp","").toString());

    delete data_settings;
}


void FenPrincipale::saveSettings() {
    QSettings* settings = new QSettings();
    settings->setValue("datedepart", heureLancement->dateTime());
    settings->setValue("dataserverurl",dataServerLineEdit->text());
    settings->setValue("dataserverenabled", dataServerCheckBox->isChecked());
    QString mtwServer = metewowServerLineEdit->text();
    if(mtwServer.size() > 0 && mtwServer.at(mtwServer.size()-1) != '/')
        metewowServerLineEdit->setText(mtwServer + "/");
    settings->setValue("metewowserverurl",metewowServerLineEdit->text());
    settings->setValue("metewowserverenabled", metewowServerCheckBox->isChecked());
    settings->setValue("metewowid", metewowMacLineEdit->text());
    settings->setValue("metewowmdp", metewowMdpLineEdit->text());

    delete settings;
}



void FenPrincipale::on_dataServerLineEdit_editingFinished() {saveSettings();mWebServicesManager->test(DATASERVER);}
void FenPrincipale::on_metewowServerLineEdit_editingFinished() {saveSettings();mWebServicesManager->test(METEWOWSERVER);}
void FenPrincipale::on_metewowMacLineEdit_editingFinished() {saveSettings();}
void FenPrincipale::on_metewowMdpLineEdit_editingFinished() {saveSettings();}
void FenPrincipale::on_heureLancement_timeChanged(const QTime &time) {saveSettings();}
void FenPrincipale::on_metewowServerCheckBox_clicked(){ saveSettings();mWebServicesManager->test(METEWOWSERVER);}
void FenPrincipale::on_dataServerCheckBox_clicked() {saveSettings();mWebServicesManager->test(DATASERVER);}
void FenPrincipale::on_metewowRegisterPushButton_clicked(){mWebServicesManager->metewowRegister();}
void FenPrincipale::on_metewowDeletePushButton_clicked(){mWebServicesManager->metewowDelete();}

WebServicesManager* FenPrincipale::getWebServicesManager() {return mWebServicesManager;}

/*
 * Gestion du menu Konami
 */
bool FenPrincipale::eventFilter( QObject *o, QEvent *e ) {
    if ( e->type() == QEvent::KeyPress ) {
        QKeyEvent *k = (QKeyEvent *)e;
        if(position >= konami.size())
            return false;

        if(konami.at(position) == k->key())
            ++position;
        else
            position = 0;


        if(position>=konami.size())
            konamify(true);
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

#ifdef TROLL
        this->setWindowTitle("To log, or not to log ? Random compilation.");
#else
        this->setWindowTitle("Logger 3.14");
#endif
    }
}

void FenPrincipale::on_konami_1_clicked() {
    reinit_b();
    konami_1->setDefault(true);
    stack->setCurrentIndex(6);
    if(kwebview->url().toString() != "http://www.nyan.cat/original.php")
        kwebview->load(QUrl("http://www.nyan.cat/original.php"));
}

void FenPrincipale::on_konami_2_clicked(){
    reinit_b();
    konami_2->setDefault(true);
    stack->setCurrentIndex(6);
    if(kwebview->url().toString() != "http://uploads.ungrounded.net/526000/526596_GSwitch.swf")
        kwebview->load(QUrl("http://uploads.ungrounded.net/526000/526596_GSwitch.swf"));
}

void FenPrincipale::on_konami_3_clicked(){
    reinit_b();
    konami_3->setDefault(true);
    stack->setCurrentIndex(6);
    if(kwebview->url().toString() != "http://www.youtube.com/watch?v=oavMtUWDBTM")
        kwebview->load(QUrl("http://www.youtube.com/watch?v=oavMtUWDBTM"));
}

void FenPrincipale::on_konami_4_clicked() {
    reinit_b();
    konami_4->setDefault(true);
    stack->setCurrentIndex(6);
    if(kwebview->url().toString() != "http://www.google.fr")
        kwebview->load(QUrl("http://www.google.fr"));
}

void FenPrincipale::on_konami_close_clicked() {konamify(false);reinit_b();stack->setCurrentIndex(0);}


/*
 * L'OPTIMISATEUR DE PLACEMENT DES GRAPHIQUES OP OF THE DEAD
 */
void FenPrincipale::on_btn_optimiser_clicked(){
    if(btn_optimiser->isChecked())
        optimise_graph();

    optimisation_graph = btn_optimiser->isChecked();
}

void FenPrincipale::optimise_graph() {
    int nbItems = graphiques.size();
    int rowSize = zone_graph->geometry().height();
    int colSize = zone_graph->geometry().width();

    qDebug() << "Nb graphs: " << nbItems;
    qDebug() << "Taille w: " << zone_graph->geometry().width() << "| h: " << zone_graph->geometry().height();

    float rapportColRow = (float)rowSize / (float)colSize; // rowNb ~= rapportColRow * colNb
    int colNb = round( std::sqrt( (float)nbItems / rapportColRow ) ); // on veut n tq rapportColRow*n² >= nbItems
    int rowNb = std::floor( (float)colNb*rapportColRow );
    while( colNb*rowNb < nbItems ) rowNb++; // Ajuste par exès le nombre de lignes

    qDebug() << "colones:" << colNb;
    qDebug() << "lignes:" << rowNb;

    int place = colNb*rowNb - nbItems; // La place disponible en trop
    int iLigne = 0, iColone = 0;
    for(int i=0 ; i < nbItems ; i++) {
        int colStretch = 1;
        if( place > 0 && iColone + 1 < colNb ) { // Si il reste de la place en trop, on étend en largeur
            colStretch += 1;
            place--;
        }

        graphiques[i].second->showNormal();
        graphiques[i].second->setGeometry(iColone * (colSize / colNb), iLigne * (rowSize / rowNb), (colSize / colNb) * colStretch, rowSize / rowNb);

        iColone += colStretch;
        if( iColone >= colNb ) {
            iColone = 0;
            iLigne++;
        }
    }

}


void FenPrincipale::on_actionPasser_l_tape_suivante_triggered() {
    stmgr->unlockNextStage();
}

void FenPrincipale::on_actionRemettre_z_ro_triggered() {
    stmgr->resetStage();
}

void FenPrincipale::on_actionPasser_l_tape_pr_c_dente_triggered() {
    stmgr->goToPreviousStage();
}


void FenPrincipale::on_xyModeCheckbox_toggled(bool checked) {
    sel_capteur_x->setEnabled(checked);
    sel_valeur_x->setEnabled(checked);
}
