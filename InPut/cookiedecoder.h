#ifndef COOKIEDECODER_H
#define COOKIEDECODER_H

#include <QString>
#include <QObject>
#include <QtGui>
#include "../defines.h"

class CookieDecoder : public QObject
{
public:
    CookieDecoder();
    QString splitCharacter();
    void decodeString(QString &str);

signals:
    void newValue(int id_capteur, int id_valeur, double valeur);
private:
    Q_OBJECT
    QString get_checksum(QString trame);
    int precIdCapteur;
    int precIdValeur;
};

#endif // COOKIEDECODER_H
