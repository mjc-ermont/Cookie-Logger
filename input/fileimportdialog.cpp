#include "fileimportdialog.h"

FileImportDialog::FileImportDialog(FenPrincipale *parent) : QDialog(parent)
{
    m_parent = parent;
    setupUi(this);
    this->setWindowTitle("Il faut plus de données !");
}

FileImportDialog::~FileImportDialog()
{
}

void FileImportDialog::on_choirlefichier_clicked() {
    QString path = QFileDialog::getOpenFileName(this,"Auriez-vous l'extrême amabilité de m'indiquer où se trouve mon fichier log.dan ?");
    if(path != "")
        fileLineEdit->setText(path);
}

void FileImportDialog::on_buttonBox_accepted() {
    QFile f(fileLineEdit->text());
    qDebug() << "Lecture du fichier: " << fileLineEdit->text();
    f.open(QFile::ReadOnly);
    QString content = f.readAll();
    QStringList database = content.split("!!");
    foreach(QString data, database) {
        qDebug() << "data";
        QStringList spl = data.split(";");
        if(spl.size() == 4) {
            int numCapteur = spl.at(0).toInt();
            int numValeur = spl.at(1).toInt();
            double valeur = spl.at(2).toDouble();
            QDateTime t = QDateTime::fromString(spl.at(3),"yyyy-MM-dd hh:mm:ss");
            qDebug() << "c: " << numCapteur << " v:" << numValeur << " vv:"<<valeur;
            if(m_parent->getSensorMgr()->getSensors().size() > numCapteur)
            {
                if(m_parent->getSensorMgr()->getSensor(numCapteur)->getValues().size() > numValeur) {

                   // Data* d = m_parent->getSensorMgr()->getSensor(numCapteur)->getValues().at(numValeur)->addData(valeur);
                  //  d->time = t;
                } else {
                    qDebug() << "bad id value";
                }
            } else {
                qDebug() << "bad id sensor";
            }
        } else {
            qDebug() << "err";
        }
    }

    for(int i=0; i<m_parent->getSensorMgr()->getSensors().size();i++) {
        m_parent->getBT()->requestUpdate(m_parent->getSensorMgr()->getSensors().at(i)->getValues().at(0));
    }

}
