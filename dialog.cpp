#include "dialog.h"

using namespace std;
AskDialog::AskDialog()
{
    setupUi(this);

    settings = new QSettings("Cookie Project","Logger");

    this->setWindowTitle("Bonjour, veuillez choisir le port halal de connexion série.");
    connect(pushButton,SIGNAL(clicked()),this,SLOT(continueClicked()));
    fen=NULL;

    QString prev_port = settings->value("serial/port","").toString();
    int prev_baudrate = settings->value("serial/baud",0).toInt();
    baudrate->setCurrentIndex(prev_baudrate);

    QStringList portList = Serial::getPortList();
    for(int i=0;i<portList.size();i++) {
        selectEntry->addItem(portList.at(i));
        if(prev_port == portList.at(i)) {
            selectEntry->setCurrentIndex(i);
        }
        nItems++;
    }

    nItems++;
    selectEntry->addItem("Autre");
    if(nItems > 1)
        textEntry->setVisible(false);
    else
        textEntry->setVisible(true);
}

AskDialog::~AskDialog()
{
    if(fen!=NULL)
        delete fen;
}

void AskDialog::continueClicked()
{
    QString port = "";
    if(selectEntry->currentText() == "Autre")
        port = textEntry->text();
    else
        port = selectEntry->currentText();

    settings->setValue("serial/port",port);
    settings->setValue("serial/baud",baudrate->currentIndex());

    Serial* com = new Serial(port, baudrate->currentText().toInt());
    com->init();


    fen = new FenPrincipale(com);

    fen->show();
    this->close();
}

void AskDialog::on_selectEntry_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Autre") {
        textEntry->setVisible(true);
    } else {
        textEntry->setVisible(false);
    }
}
