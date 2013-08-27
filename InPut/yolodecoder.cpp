#include "yolodecoder.h"

YoloDecoder::YoloDecoder()
{
}

QString YoloDecoder::splitCharacter() {
    char c[0];
    c[0] = 0xFF;
    return QString::fromLocal8Bit(c);
}

void YoloDecoder::decodeString(QString &str) {
    // #c'estpaspeucher.
    QByteArray data = str.toLocal8Bit();
    if(data.size() < 10)
        return;

    char checkSum = data.at(9);

    char check = 0x00;
    for(int i=0; i<9; i++) {
        check += data.at(i);
    }
    check >> 1;

    if(check == checkSum) {
        for(int i=0; i<9; i++) {
            emit newValue(i, 0, data.at(i));
        }
    }

}
