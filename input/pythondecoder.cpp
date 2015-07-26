#include "pythondecoder.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>


pythondecoder::pythondecoder() {}



void pythondecoder::init() {

    ok = false;
    buffer = "";

    rs = init_rs_char(8, 285, 1, 1, 20);
    if(rs != NULL) {
        ok = true;
        emit message("Init decoder ok");
    } else {
        emit message("Init decoder failed");
    }

    startByte = 255;
    frame_length = calcframelength();
    qDebug() << "Frame length: " << frame_length;
}

pythondecoder::~pythondecoder() {
}

bool pythondecoder::decode(QByteArray frame) {
    if(!ok)
        return false;



    qDebug() << QString(frame.toHex());
    return false;
}

int pythondecoder::getrslength() {
    return 20;
}

int pythondecoder::calcframelength() {

    return 64;
}

void pythondecoder::appendData(QByteArray received) {
    if(!ok)
        return;

    buffer.append(received);
    qDebug() << "Received:" << QString(received.toHex())<< " || buffer:"<<QString(buffer.toHex());


    while(buffer.indexOf(startByte) != -1 && (buffer.length() - buffer.indexOf(startByte)) >= frame_length) {
        int firstStart = buffer.indexOf(startByte);
        QByteArray frame_to_decode = buffer.mid(firstStart, frame_length);
        bool result = decode(frame_to_decode);
        qDebug() << "result: "<< (result? " true":"false");
        if(result)
            buffer = buffer.mid(firstStart + frame_length);
        else if (buffer.indexOf(startByte,firstStart+1) != -1)
            buffer = buffer.mid(buffer.indexOf(startByte,firstStart+1));
        else
            buffer = "";

        qDebug() << "New buffer: " << QString(buffer.toHex());
    }

}
