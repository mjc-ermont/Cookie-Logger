#include "serialdecoder.h"
#include "serial.h"

Serial::Serial(QString _port,qint32 _baudrate)
{
    port = _port;
    baudrate = _baudrate;

    speakers_enabled = true;
    current_channel = 0;
    doingBalayage = false;
    okay = false;
}


Serial::~Serial() {
    qDebug() << "fermeture port série";
    serial_port->close();
}

bool Serial::init() {

    serial_port = new QSerialPort(this);
    serial_port->setPortName(port);
    qDebug() << port;
    qDebug() << baudrate;

    serial_port->setBaudRate(baudrate);
    qDebug() << "test";
    if(!serial_port->open(QIODevice::ReadWrite)) {
        qDebug() << "yolo";
        emit message("Echec de l'ouverture du port série: "+serial_port->errorString());
        okay = false;
    } else {
        emit message("Port série ouvert avec succès");
        okay = true;
    }


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

        emit message("Changement vers le canal "+id);
    }
}

void Serial::setSpeakersEnabled(bool enabled) {
  //  if(enabled != speakers_enabled) {
        speakers_enabled = enabled;

        QString str = enabled?"$HF":"$HM";
        str = str + QChar(0x0D) + QChar(0x0A);
        serial_port->write(str.toStdString().c_str());

        if(enabled)
            emit message("Activation du haut parleur");
        else
            emit message("Coupure du haut parleur");
 //  }
}


QStringList Serial::getPortList() {
    QStringList portlist;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        portlist.push_back(info.portName());
    }
    return portlist;
}

qint32 Serial::getBaudrate() {
    return serial_port->baudRate();
}



void Serial::setBaudrate(qint32 bd) {
    serial_port->setBaudRate(bd);
}

void Serial::setPort(QString port) {
    serial_port->close();
    serial_port->setPortName(port);
    serial_port->open(QIODevice::ReadWrite);

}

QString Serial::getPort() {
    return serial_port->portName();
}


void Serial::balayageFrequenciel() { // 05#42#43#12#19#15#23#53#35#17
    qDebug() << "Balayage en cours";
    emit message("Balayage en cours");


    doingBalayage = true;


    QString str = "$S";
    str = str+  QChar(0x0D) + QChar(0x0A);
    serial_port->write(str.toStdString().c_str());
    serial_port->waitForBytesWritten(2000);
    data.clear();
    balayage_buffer.clear();

    time.start();
}

void Serial::readDataBalayage() {
    int nvalues = 9;


    data_read = serial_port->readAll();
    qDebug() << "read:";
    qDebug() << data_read;

    balayage_buffer.append(data_read);
    content = balayage_buffer.split('#');

    if(content.last().size() < 2) {
        balayage_buffer = content.last();
        content.removeLast();
    }

    for(int i=0;i<content.size();i++)
        data.append((content.at(i).toDouble()));

    if(data.size() >= nvalues) {
        doingBalayage = false;
        emit message("Balayage terminé");
        emit balayageDone(data);
    } else if(time.elapsed() > 5000) {
        doingBalayage = false;
        emit message("Balayage terminé (timeout)");
        emit balayageDone(data);
        qDebug() << " timeout";
    }
}



void Serial::readData() {
    if(doingBalayage) {
        readDataBalayage();
        return;
    }

  //  qDebug("l");
    QList<QByteArray> trames;

    QByteArray dataread = serial_port->readAll();

    emit nBytesRead(dataread.size());

    if(dataread.size() != 0)
        emit message(QString(dataread));
  //  qDebug() << dataread;


    QByteArray data(skipped_buf);
    data.append(dataread);

    skipped_buf.clear();

    trames = data.split('#');
    if(trames.last().size() < 19) {
        skipped_buf = trames.last();
        trames.removeLast();
    }

    for(int i=0;i<trames.size();i++) {
        if(trames.at(i).isEmpty())
            trames.removeAt(i);
    }
    emit dataRead(trames);
}



