#ifndef DIALOG_H
#define DIALOG_H


#include "FenPrincipale.h"
#include "input/serial.h"

#include "ui_dialog.h"
#include "QtSerialPort/QSerialPort"
#include <QSettings>
#include <QStringList>

class AskDialog : public QDialog, public Ui::Dialog
{
    Q_OBJECT

public:
    AskDialog();
    ~AskDialog();
    Serial* getSerial() {return com;}
private slots:
    void continueClicked();
    void on_selectEntry_currentIndexChanged(const QString &arg1);
private:
    FenPrincipale* fen;
    Serial *com;

    QSettings* settings;
    int nItems;
};

#endif // DIALOG_H
