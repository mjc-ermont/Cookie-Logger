#include "defines.h"

#include <QApplication>
#include <QTextCodec>
#include <ctime>


#include "FenPrincipale.h"
#include "input/serial.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    srand(time(NULL)); // Le truc qui rassure grave.

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    AskDialog* d = new AskDialog();
    d->show();

    int rc = a.exec();
    if(d->getSerial() != 0)
        delete d->getSerial();
    return rc;
}
