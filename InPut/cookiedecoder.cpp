#include "cookiedecoder.h"

CookieDecoder::CookieDecoder(){}


void CookieDecoder::decodeString(QByteArray(&data)) {

    QString str(data);

    QStringList elements = str.split("$");
    if(elements.size() < 6) {
        if(elements.size()>1)
            qDebug() << "Trame incomplete";
        return;
    }

    QString firstPart = "#" + elements[0] + "$" + elements[1] + "$" + elements[2] + "$" + elements[3] + "$";
    char checkSum = get_checksum(firstPart.toAscii());

    qDebug() << "Trame:" << str;
    qDebug() << "CS: "<< (int)checkSum << " | "  << QString(elements[4]).toInt(0,16);

    bool checkLeSum = true;
    int numCapteur = elements[1].toInt();
    double valeur = elements[3].toDouble();
    int numValeur = elements[2].toInt();


    if((((int)checkSum == QString(elements[4]).toInt(0,16))||(!checkLeSum))&&((numCapteur!=precIdCapteur)||(precIdValeur!=numValeur))){
        emit newValue(numCapteur, numValeur, valeur);

        precIdCapteur=numCapteur;
        precIdValeur=numValeur;
    }

}


char CookieDecoder::get_checksum(QByteArray trame) {
    char XOR = 0;
    for (int i = 0; i < trame.length() ; i++) {
       XOR = XOR ^ trame.at(i);
    }

    return XOR;
}
