#include "cookiedecoder.h"

CookieDecoder::CookieDecoder()
{

}


void CookieDecoder::decodeString(QByteArray(&data)) {
    QString str(data);

    QStringList elements = str.split("$");
    if(elements.size() < 6) {
        if(elements.size()>1)
            qDebug() << "Trame incomplete";
        return;
    }

    QString firstPart = elements[0] + "$" + elements[1] + "$" + elements[2] + "$" + elements[3] + "$";
    QString checkSum = get_checksum(firstPart);

    qDebug() << "Trame:" << str;
    qDebug() << "CS: "<< QString(checkSum).toInt(NULL, 10) << " | "  << elements[4].toInt(NULL, 16) ;

    bool checkLeSum = false;
    int numCapteur = elements[1].toInt();
    double valeur = elements[3].toDouble();
    int numValeur = elements[2].toInt();


    if(((QString(checkSum).toInt(NULL, 10) == elements[4].toInt(NULL, 16))||(!checkLeSum))&&((numCapteur!=precIdCapteur)||(precIdValeur!=numValeur))){

        emit newValue(numCapteur, numValeur, valeur);

        precIdCapteur=numCapteur;
        precIdValeur=numValeur;
    }
}


QString CookieDecoder::get_checksum(QString trame) {

    char XOR = 0;
    for (int i = 0; i < trame.length() ; i++) {
       XOR = XOR ^ trame.toStdString()[i];
    }

    return QString::number(XOR);
}
