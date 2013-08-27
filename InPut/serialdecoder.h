#ifndef SERIALDECODER_H
#define SERIALDECODER_H

#include <QString>
#include "sensormanager.h"

class SerialDecoder
{
    Q_OBJECT

public:
    SerialDecoder(SensorManager* s_);
    virtual QString splitCharacter();

public slots:
    virtual decodeString(QString &str) = 0;

signals:
    void newValue(int id_capteur, int id_valeur, double valeur);

protected:
    SensorManager* sensorManager;
};

#endif // SERIALDECODER_H
