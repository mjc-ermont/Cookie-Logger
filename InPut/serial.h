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


class Serial : public QThread
{
    Q_OBJECT

public:
    Serial(QString _port="/dev/TTYUSB0",speed_t _baudrate=B600,QThread * parent = 0);
    ~Serial();
    void run();
    bool init();
    static QString toString(QByteArray str);

protected:
    bool OpenCOM (int nId);
    bool CloseCOM ();
    bool ReadCOM (void* buffer, int nBytesToRead, int* pBytesRead);
    bool WriteCOM (void* buffer, int nBytesToWrite, int* pBytesWritten);


public slots:

    void readData();

signals:
    void dataRead(QList<QByteArray>);

private:
    QString port;
/*
    HANDLE g_hCOM;
    COMMTIMEOUTS g_cto;

    DCB g_dcb;
*/

    QByteArray skipped_buf;

    void setspeed(speed_t vitesse)
    {
      cfsetospeed(&tio, vitesse);
      cfsetispeed(&tio, vitesse);
      tcsetattr(tty_fd,TCSANOW,&tio);
    }

    int nb_read;
    unsigned char buffer[1024];
    struct termios tio;
    int tty_fd;
    speed_t baudrate;

};

#endif // SERIAL_H
