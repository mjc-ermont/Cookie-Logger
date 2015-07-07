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
    }

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
    // Connect entre la bdd et fenprincipale pour les réponses aux requêtes de lecture
    connect(sensormgr->getDB(),SIGNAL(dataRead(int,int,QVector<Data>,QString)), this,SLOT(data_read(int,int,QVector<Data>,QString)));
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
    // ------------------------
    // Démarrage du timer d'actualisation
    log_logger("[INFO] Starting refreshing timer");
    actTemps = new QTimer();
    connect(actTemps,SIGNAL(timeout()),this,SLOT(syncTime()));
    actTemps->start(1000);
    // -----------------------------
    // Démarrage du décodeur de trames
    myDecoder = new pythondecoder();

    connect(myDecoder,SIGNAL(newValue(int,int,double)), sensormgr, SLOT(newValue(int,int,double)));
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



    stmgr = new StagesManager(stageGraphicsView);

    nTramesRecues = 0;
    nTramesEchouees = 0;
    nBytesRecus = 0;
    nTramesCorrigees = 0;
    QTimer* teemo = new QTimer();
    connect(teemo, SIGNAL(timeout()), stmgr, SLOT(render()));
    teemo->setSingleShot(true);
    teemo->start(50);
}

FenPrincipale::~FenPrincipale(){
    qDebug() << "tamer";
    delete com;
}


void FenPrincipale::resizeEvent(QResizeEvent *) {
    if(optimisation_graph)
        optimise_graph();


    if(stmgr != 0) {
        stmgr->render();
    }

    if(tableauBord != 0) {
        tableauBord->onResize();
    }
}

/*
 * Outil de balayage fréquenciel
 */
class BalayageDialog : QDialog {
public:
    BalayageDialog(BalaiFrequenciel* b);
    void show();
};

BalayageDialog::BalayageDialog(BalaiFrequenciel* b) {
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(b);
    setLayout(layout);
}

void BalayageDialog::show() {
    QDialog::show();
}

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
 * Gestion de l'horloge
 */
void FenPrincipale::syncTime() {
    graphic_range_selector->setMaximumDate(QDateTime::currentDateTime());
    historique_range_selector->setMaximumDate(QDateTime::currentDateTime());
}

/*
 * Gestion des données
 */

void FenPrincipale::received(QByteArray d) {
    myDecoder->appendData(d);

    QPair<GraphicView*,QMdiSubWindow*> value;
    foreach(value,graphiques) {
        value.first->majData(); //TODO: Truc de porc?
    }
}


void FenPrincipale::data_read(int idc, int idv, QVector<Data> data, QString reason) {

    qDebug() << "data read "<< idc << ";" << idv  << ";" << reason;
    if(reason == "graph") {
        for(int i=0;i<graphiques.size();i++) {
            GraphicView* g = graphiques.at(i).first;
            if(g->getCapteur() == idc && g->getValeur() == idv) {
                g->setData(data);
            }
        }
    } else if(reason == "bt") {
        this->getBT()->update(idc,idv,data.last().value);
    } else if(reason == "map") {

    } else if(reason == "tab") {
        this->getTableMgr()->actualisay(idc,idv,data);
    }
}

/*
 * Quelques fonctions de display
 */
void FenPrincipale::setIndicatorRx() {

    resetIndicator = new QTimer();
    n++;
    qDebug() << "Increment indicator" << n;
    connect(resetIndicator,SIGNAL(timeout()),this,SLOT(resetIndicatorRx()));

    resetIndicator->setSingleShot(true);
    resetIndicator->start(500);

    indicator_rx->setChecked(true);
}

void FenPrincipale::resetIndicatorRx() {
    n--;
    qDebug() << "Decrement indicator" << n;
    if(n==0)
        indicator_rx->setChecked(false);
}

void FenPrincipale::log_logger(QString message){
    log(0, message);
}

void FenPrincipale::log_serial(QString message){
    log(1, message);
}

void FenPrincipale::log_decoder(QString message){
    log(2, message);
}

void FenPrincipale::log_database(QString message){
    log(3, message);
}

void FenPrincipale::log_webservices(QString message){
    log(4, message);
}

void FenPrincipale::log(int section, QString message) {
    message = QDateTime::currentDateTime().toString("[hh:mm:ss] ") + message;
    //barreStatus->showMessage(message);

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

void FenPrincipale::portTriggered() {
    QObject* sender = QObject::sender();
    QAction* act = qobject_cast<QAction*>(sender);
    if(act) {
        com->setPort(act->data().toString());
        qDebug() << "Set port:" << act->data().toString();
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

        if(com->getPort() == cur_port) {
            portAction->setChecked(true);
        }
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

void FenPrincipale::on_actionEntrer_le_canal_manuellement_triggered()
{
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
void FenPrincipale::on_b_console_clicked(){reinit_b();b_console->setDefault(true);stack->setCurrentIndex(1);}
void FenPrincipale::on_b_tb_clicked(){reinit_b();b_tb->setDefault(true);stack->setCurrentIndex(0);}
void FenPrincipale::on_b_table_clicked(){reinit_b();b_table->setDefault(true);stack->setCurrentIndex(2);}
void FenPrincipale::on_b_param_clicked(){reinit_b();b_param->setDefault(true);stack->setCurrentIndex(3);}
void FenPrincipale::on_b_graph_clicked(){reinit_b();b_graph->setDefault(true);stack->setCurrentIndex(4);}
void FenPrincipale::on_b_carte_clicked(){reinit_b();b_carte->setDefault(true);stack->setCurrentIndex(5);}

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

/*
 * Actions liées aux graphiques
 */
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

void FenPrincipale::on_add_graph_clicked()
{
    int selected_sensor = sel_capteur->currentIndex();
    int selected_value  = sel_valeur->itemData(sel_valeur->currentIndex()).toInt();
    if(selected_sensor == -1)
        return;

    GraphicView* g = new GraphicView(selected_sensor, selected_value,this);


    connect(graphic_range_selector, SIGNAL(startDateChanged(QDateTime)), g, SLOT(setStartDT(QDateTime)));
    connect(graphic_range_selector, SIGNAL(endDateChanged(QDateTime)), g, SLOT(setEndDT(QDateTime)));
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

    disconnect(graphic_range_selector, SIGNAL(startDateChanged(QDateTime)), g, SLOT(setStartDT(QDateTime)));
    disconnect(graphic_range_selector, SIGNAL(endDateChanged(QDateTime)), g, SLOT(setEndDT(QDateTime)));
}


/*
 * Actions liée à la table des valeurs
 */
void FenPrincipale::on_actualizeTableButton_clicked()
{
    tableManager->requestActualization(historique_range_selector->getLowerDate(), historique_range_selector->getUpperDate());
}

void FenPrincipale::onWebServicesNotification(int type, QString text) {
    if(type == DATASERVER) {
        dataServerCheckBox->setText(text);
    } else if(type == METEWOWSERVER) {
        metewowServerCheckBox->setText(text);
    }
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



void FenPrincipale::on_dataServerLineEdit_editingFinished() {
    saveSettings();
    mWebServicesManager->test(DATASERVER);
}

void FenPrincipale::on_metewowServerLineEdit_editingFinished() {
    saveSettings();
    mWebServicesManager->test(METEWOWSERVER);
}

void FenPrincipale::on_metewowMacLineEdit_editingFinished() {
    saveSettings();
}

void FenPrincipale::on_metewowMdpLineEdit_editingFinished() {
    saveSettings();
}

void FenPrincipale::on_heureLancement_timeChanged(const QTime &time) {
    saveSettings();
}

void FenPrincipale::on_metewowServerCheckBox_clicked()
{
    saveSettings();
    mWebServicesManager->test(METEWOWSERVER);
}

void FenPrincipale::on_dataServerCheckBox_clicked()
{
    saveSettings();
    mWebServicesManager->test(DATASERVER);
}

void FenPrincipale::on_metewowRegisterPushButton_clicked()
{
    mWebServicesManager->metewowRegister();
}

void FenPrincipale::on_metewowDeletePushButton_clicked()
{
    mWebServicesManager->metewowDelete();
}

WebServicesManager* FenPrincipale::getWebServicesManager() {
    return mWebServicesManager;
}

/*
 * Gestion du menu Konami
 */
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
void FenPrincipale::on_btn_optimiser_clicked()
{
    if(btn_optimiser->isChecked())
        optimise_graph();

    optimisation_graph = btn_optimiser->isChecked();
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
        int row, rowStretch, col=1, colStretch=1;
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



void FenPrincipale::on_actionPasser_l_tape_suivante_triggered()
{
    stmgr->unlockNextStage();
}

void FenPrincipale::on_actionRemettre_z_ro_triggered()
{
    stmgr->resetStage();
}

void FenPrincipale::on_actionPasser_l_tape_pr_c_dente_triggered()
{
    stmgr->goToPreviousStage();
}

