#include "serial.h"
#include "yolodecoder.h"

YoloDecoder::YoloDecoder()
{
}

char YoloDecoder::splitCharacter() {
    return 0xFF;
}

void YoloDecoder::decodeString(QByteArray &str) {
    qDebug() << "Décodage de la trame: " << Serial::toString(str);
    if(str == tramePrecedente) {
        qDebug() << "trame déjà reçue:";
        return;
    }

    if(str.size() < 10) {
        qDebug() << "trame trop courte: " << str.size();
        return;
    }

    unsigned char checkSum = str.at(9);

    unsigned char check = 0x00;
    for(int i=0; i<9; i++) {
        check += str.at(i);
    }

    check >>= 1;

    if(check == checkSum) {
        tramePrecedente = str;

        qDebug() << "checksum success";
        trameValidee=true;
        for(int i=0; i<9; i++) {
            double value = 0;
            value += (unsigned char)str.at(i);
            qDebug() << "Cake: " << value;
            emit newValue(i, 0, value);
        }
    } else {
        qDebug() << "checksum fail: " << check << "|" << checkSum;
        emit error_frame();
    }

}

