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
private:
    Q_OBJECT

};

#endif // YOLODECODER_H
