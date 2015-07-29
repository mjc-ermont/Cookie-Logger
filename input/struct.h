#ifndef STRUCT_H
#define STRUCT_H

#include <QString>
#include <QVector>
#include <QMap>
#include <algorithm>
#include <QDebug>

class Struct {
public:
    Struct();
    int calcsize(QString str);
    QVector<double> unpack(QString str, QByteArray data);

protected:
    template<typename _II, typename _OI>
      void copy(_II __first, _II __last, _OI __result);

private:
    QMap<char, uint8_t> typesize;

    bool isLittleEndian;
};

#endif // STRUCT_H
