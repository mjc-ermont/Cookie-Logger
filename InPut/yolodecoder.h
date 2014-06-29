#ifndef YOLODECODER_H
#define YOLODECODER_H
#include <QString>
#include <QObject>
#include <QByteArray>

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
    int nTrameRepeat;
    bool trameValidee;

    Q_OBJECT

};

#endif // YOLODECODER_H
