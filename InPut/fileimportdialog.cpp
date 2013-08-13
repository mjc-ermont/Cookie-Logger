#include "fileimportdialog.h"

FileImportDialog::FileImportDialog(FenPrincipale *parent) : QDialog(parent)
{
    m_parent = parent;
    setupUi(this);
}

FileImportDialog::~FileImportDialog()
{
}

void FileImportDialog::on_choirlefichier_clicked() {
    QString path = QFileDialog::getOpenFileName(this,"Jus de carotte");
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
