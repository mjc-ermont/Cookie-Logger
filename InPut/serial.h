#ifndef SERIAL_H
#define SERIAL_H

#include "defines.h"
#include <QStringList>
#include <QDebug>
#include <QFile>


#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Serial : public QObject
{
    Q_OBJECT

public:
    Serial(QString _port="TTYUSB0",qint32 _baudrate=600);
    ~Serial();
    bool init();
    static QString toString(QByteArray str);

    void setChannel(int id);
    void setSpeakersEnabled(bool enabled);
    void balayageFrequenciel();
    void readDataBalayage();

    void setBaudrate(qint32 bd);
    void setPort(QString port);
    qint32 getBaudrate();
    QString getPort();

    bool isOkay() {
        return okay;
    }


    static QStringList getPortList();

public slots:

    void readData();

signals:
    void dataRead(QList<QByteArray>);
    void balayageDone(QVector<double>);

    void message(QString);

private:

    QByteArray balayage_buffer;
    QByteArray data_read;
    QList<QByteArray> content;
    QVector<double> data;
    QTime time;


    QString port;
    bool speakers_enabled;
    int current_channel;

    QSerialPort* serial_port;

    QByteArray skipped_buf;


    bool doingBalayage;

    int nb_read;
    unsigned char buffer[1024];

    int tty_fd;
    bool okay;
    qint32 baudrate;

};

#endif // SERIAL_H
