#ifndef PYTHONDECODER_H
#define PYTHONDECODER_H

#include "defines.h"
#include "RS/rs.h"
#include "RS/char.h"

#include <stdlib.h>
#include <string.h>

#include <QByteArray>
#include <QFile>
#include <QVector>
#include <QDebug>

#include <QApplication>

#include "struct.h"


class pythondecoder : public QObject
{
    Q_OBJECT
public:
    pythondecoder();
    ~pythondecoder();

    void init();

protected:
    bool decode(QByteArray frame);
    int calcframelength();
    int getrslength();

public slots:
    void appendData(QByteArray data);
signals:
    void newFrame(QVector<double> value);
    void message(QString);
    void trame_erreur(int);
    void trame_increment(int);
    void trame_corrigee(int);

private:
    QString conf;

    void *rs;

    QByteArray buffer;
    Struct unpacker;

    bool ok;

    char startByte;
    int frame_length;
    int content_size;
};

#endif // PYTHONDECODER_H
