#ifndef YOLODECODER_H
#define YOLODECODER_H
#include <QString>
#include <QtGui>

class YoloDecoder : public QObject
{
public:
    YoloDecoder();
    static char splitCharacter();
    void decodeString(QByteArray &str);

signals:
    void newValue(int id_capteur, int id_valeur, double valeur);
    void error_frame();
private:
    QByteArray tramePrecedente;
    int nTrameRepeat=1;
    bool trameValidee=false;

    Q_OBJECT

};

#endif // YOLODECODER_H
