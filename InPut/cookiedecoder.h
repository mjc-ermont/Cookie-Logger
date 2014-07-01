#ifndef COOKIEDECODER_H
#define COOKIEDECODER_H

#include <QString>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QStringList>

#include "../defines.h"

class CookieDecoder : public QObject
{
    Q_OBJECT

public:
            CookieDecoder       ();
    void    decodeString        (QByteArray &str);

signals:
    void    newValue            (int id_capteur, int id_valeur, double valeur);

private:
    char    get_checksum        (QByteArray trame);

    int     precIdCapteur;
    int     precIdValeur;
};

#endif // COOKIEDECODER_H
