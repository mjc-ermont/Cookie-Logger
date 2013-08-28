#include "yolodecoder.h"

YoloDecoder::YoloDecoder()
{
}

char YoloDecoder::splitCharacter() {
    return 0xFF;
}

void YoloDecoder::decodeString(QByteArray &str) {
    for(int i=0; i<str.size();i++) {
        qDebug() << "[" << i << "] " << (u_int8_t) str.at(i);
    }

    if(str.size() < 10) {
        qDebug() << "c'est pas peu cher ici";
        return;
    }

    unsigned char checkSum = str.at(9);

    unsigned char check = 0x00;
    for(int i=0; i<9; i++) {


        check += str.at(i);
    }

    check >>= 1;

    if(check == checkSum) {
        qDebug() << "checksum success";
        for(int i=0; i<9; i++) {
            double value = 0;
            value += (unsigned char)str.at(i);
            qDebug() << "Cake: " << value;
            emit newValue(i, 0, value);
        }
    } else {
        qDebug() << "checksum fail: " << check << "|" << checkSum;
    }

}
