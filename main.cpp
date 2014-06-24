#include <QtGui>
#include <QInputDialog>
#include <QDebug>
#include <ctime>


#include "defines.h"
#include "FenPrincipale.h"
#include "InPut/serial.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    srand(time(NULL)); // Le truc qui rassure grave.
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    //------------------------------------------------------------------------------
    AskDialog* d = new AskDialog();
    d->show();
    //------------------------------------------------------------------------------

      return a.exec();
}
