#include "serialdecoder.h"
#include "serial.h"

Serial::Serial(QString _port,qint32 _baudrate, QThread * parent) : QThread(parent)
{
    port = _port;
    baudrate = _baudrate;

    speakers_enabled = true;
    current_channel = 0;
}


Serial::~Serial() {

    serial_port->close();
}

void Serial::run() {
    init();
}


bool Serial::init() {

    serial_port = new QSerialPort(this);
    serial_port->setPortName(port);
    qDebug() << port;
    qDebug() << baudrate;

    serial_port->setBaudRate(baudrate);
    qDebug() << "test";
    if(!serial_port->open(QIODevice::ReadWrite))
        qDebug() << "yolo";

    return true;
}

QString Serial::toString(QByteArray str) {
    QString hex = "0x";
    for(int j=0;j<str.size();j++) {
        hex += QString("%1").arg(str.at(j)& 0xff, 2,16).toUpper();
    }
    return hex;
}

void Serial::setChannel(int id) {
    if(current_channel != id) {
        current_channel = id;
        QString str = "$C"+QString::number(id);
        str = str + QChar(0x0D) + QChar(0x0A);
        qDebug() << "ok ta mer" << str;
        serial_port->write(str.toStdString().c_str());
    }
}

void Serial::setSpeakersEnabled(bool enabled) {
 //   if(enabled != speakers_enabled) {
        speakers_enabled = enabled;

        QString str = enabled?"$HF":"$HM";
        str = str + QChar(0x0D) + QChar(0x0A);
        serial_port->write(str.toStdString().c_str());

 //   }
}

void Serial::readData() {

    qDebug("===== lecture série ======");
    QList<QByteArray> trames;
/*    for(int i=0;i<1024;++i)
        buffer[i] = 0x00;

    nb_read = read(tty_fd, buffer, 1024);*/
    //int eol=0;

    /*for(int i=0;(i<1024)&&(eol==0);++i)
        if(buffer[i] == 0x00)
            eol = i;*/

    QByteArray dataread = serial_port->readAll();

    QByteArray data(skipped_buf);
    data.append(dataread);


    trames = data.split('@');

    if(trames.last().size() < 10) {
        skipped_buf = trames.last();
        trames.removeLast();
    }

    qDebug() << "===== fin lecture série =====";
    emit dataRead(trames);
}



