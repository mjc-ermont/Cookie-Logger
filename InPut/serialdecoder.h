#ifndef SERIALDECODER_H
#define SERIALDECODER_H

#include <QString>
#include <QObject>
#include "sensormanager.h"

class SerialDecoder : public QObject
{

    public:
        SerialDecoder();
        QString splitCharacter();

    public slots:
        void decodeString(QString &str);
};

#endif // SERIALDECODER_H
