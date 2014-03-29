#include "dialog.h"


using namespace std;

AskDialog::AskDialog()
{

    setupUi(this);
    this->setWindowTitle("Bonjour, veuillez choisir le port halal de connexion série.");
    connect(pushButton,SIGNAL(clicked()),this,SLOT(continueClicked()));
    fen=NULL;

    list<QString> l;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        l.push_back(info.portName());
    }


   list<QString>::iterator it = l.begin();
   while (it != l.end()) {
       selectEntry->addItem(*it);
       it++;
       nItems++;
   }


    nItems++;
    selectEntry->addItem("Autre");
    if(nItems > 1)
        textEntry->setVisible(false);
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

    Serial* com;
    com = new Serial(port, baudrate->currentText().toInt());



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
