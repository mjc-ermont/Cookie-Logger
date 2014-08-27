#ifndef COOKIEDECODER_H
#define COOKIEDECODER_H

#include <QString>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QStringList>
#include <QChar>

#include "../defines.h"
#include "ReedSalomon/rs.h"

class CookieDecoder : public QObject
{
    Q_OBJECT

public:
            CookieDecoder       ();
    void    decodeString        (QByteArray &str);

signals:
    void    newValue            (int id_capteur, int id_valeur, double valeur);
    void    message(QString);
    void    trame_erreur(int);
    void    trame_increment(int);
    void    trame_corrigee(int);

private:
    char    get_checksum        (QByteArray trame);

    int     precIdCapteur;
    int     precIdValeur;
};

#endif // COOKIEDECODER_H
