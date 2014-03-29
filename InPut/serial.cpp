#include "serialdecoder.h"
#include "serial.h"

Serial::Serial(QString _port,qint32 _baudrate)
{
    port = _port;
    baudrate = _baudrate;

    speakers_enabled = true;
    current_channel = 0;
    doingBalayage = false;
}


Serial::~Serial() {

    serial_port->close();
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


    QObject::connect(serial_port, SIGNAL(readyRead()), this, SLOT(readData()));

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
  //  if(enabled != speakers_enabled) {
        speakers_enabled = enabled;

        QString str = enabled?"$HF":"$HM";
        str = str + QChar(0x0D) + QChar(0x0A);
        serial_port->write(str.toStdString().c_str());

 //  }
}

QVector<double> Serial::balayageFrequenciel() {
    qDebug() << "Balayage en cours";

    doingBalayage = true;
    QVector<double> data;

    int nvalues = 9;

    QString str = "$S";
    str = str+  QChar(0x0D) + QChar(0x0A);
    serial_port->write(str.toStdString().c_str());
    bool finished = false;

    QByteArray buffer;
    QByteArray data_read;

    QList<QByteArray> content;

    QTime time;
    time.start();


    while(!finished) {
        if(serial_port->bytesAvailable() > 0) {
            data_read = serial_port->readAll();
            qDebug() << "read:";
            qDebug() << data_read;
        }

        buffer.append(data_read);
        content = buffer.split('#');

        if(content.last().size() < 2) {
            buffer = content.last();
            content.removeLast();
        }

        for(int i=0;i<content.size();i++)
            data.append((content.at(i).toDouble()));

        if(data.size() >= nvalues) {
            finished = true;
        } else if(time.elapsed() > 15000) {
            finished = true;
            qDebug() << " timeout";
        }
    }

    doingBalayage = false;
    qDebug() << "fin balayage";
    return data;
}

void Serial::readData() {
    if(doingBalayage)
        return;

  //  qDebug("l");
    QList<QByteArray> trames;

    QByteArray dataread = serial_port->readAll();

  //  qDebug() << dataread;


    QByteArray data(skipped_buf);
    data.append(dataread);
    //qDebug() << "read: " << dataread;
    qDebug() << "Buffer: " << skipped_buf;

    trames = data.split('@');

    if(trames.last().size() < 19) {
        skipped_buf = trames.last();
    //    qDebug() << "Buffer: " << skipped_buf;
        trames.removeLast();
    }

  //  qDebug() << "f";
    emit dataRead(trames);
}



