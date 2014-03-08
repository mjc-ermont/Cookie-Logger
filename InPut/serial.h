#ifndef SERIAL_H
#define SERIAL_H

#include "defines.h"
#include <QThread>
#include <QStringList>
#include <string>
//#include <windows.h>
#include <cstdio>
#include <cstdlib>
//#include <conio.h>
#include <QDebug>
#include <QFile>

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <pthread.h>


#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Serial : public QThread
{
    Q_OBJECT

public:
    Serial(QString _port="TTYUSB0",qint32 _baudrate=600,QThread * parent = 0);
    ~Serial();
    void run();
    bool init();
    static QString toString(QByteArray str);

    void setChannel(int id);
    void setSpeakersEnabled(bool enabled);


public slots:

    void readData();

signals:
    void dataRead(QList<QByteArray>);

private:
    QString port;
    bool speakers_enabled;
    int current_channel;

    QSerialPort* serial_port;

    QByteArray skipped_buf;


    int nb_read;
    unsigned char buffer[1024];
    struct termios tio;
    int tty_fd;
    qint32 baudrate;

};

#endif // SERIAL_H
