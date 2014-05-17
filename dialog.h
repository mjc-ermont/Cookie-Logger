#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui>
#include "ui_dialog.h"
#include "InPut/serial.h"
#include "FenPrincipale.h"
#include "QtSerialPort/QSerialPort"
#include <QSettings>

class AskDialog : public QDialog, public Ui::Dialog
{
    Q_OBJECT

public:
    AskDialog();
    ~AskDialog();

private slots:
    void continueClicked();
    void on_selectEntry_currentIndexChanged(const QString &arg1);
private:
    FenPrincipale* fen;


    QSettings* settings;
    int nItems;
};

#endif // DIALOG_H
