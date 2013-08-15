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
    f.open(QFile::ReadOnly);
    QString content = f.readAll();
    QStringList database = content.split("!!");
    foreach(QString data, database) {
        QStringList spl = data.split(";");
        if(spl.size() == 4) {
            int numCapteur = spl.at(0).toInt();
            int numValeur = spl.at(1).toInt();
            double valeur = spl.at(2).toDouble();
            QTime t = QTime::fromString(spl.at(3));

            Data* d = m_parent->getSensorMgr()->getSensor(numCapteur)->getValues().at(numValeur)->addData(valeur);
            d->time = t;
        }
    }

}
