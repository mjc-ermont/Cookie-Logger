#include "struct.h"

Struct::Struct() {

    // Taille en octet des types supportés par struct.pack
    typesize.insert('c',1);
    typesize.insert('b',1);
    typesize.insert('B',1);
    typesize.insert('h',2);
    typesize.insert('H',2);
    typesize.insert('i',4);
    typesize.insert('I',4);
    typesize.insert('l',4);
    typesize.insert('L',4);
    typesize.insert('f',4);
    typesize.insert('q',8);
    typesize.insert('Q',8);
    typesize.insert('d',8);

    // Détermination de l'endianess
    const short int number = 0x1;
    const char *numPtr = (char*)&number;
    isLittleEndian = (numPtr[0] == 1);
}

int Struct::calcsize(QString str) {
    int s=0;
    for(char c : str.toStdString())
        s += typesize[c];

    return s;
}

template<typename _II, typename _OI>
 void Struct::copy(_II __first, _II __last, _OI __result) {
    if(isLittleEndian) {
        std::reverse_copy(__first, __last, __result);
    } else {
        std::copy(__first, __last, __result);
    }
 }


QVector<double> Struct::unpack(QString str, QByteArray data) {
    QVector<double> res;

    int index = 0;
    for(char c : str.toStdString()) {
        double val;
        u_int8_t s = typesize[c];

        char* it = &(data.data()[index]);
        switch(c) {
        case 'd':
            copy(it, &it[s], &val);
            break;
        case 'c':
            val = (double)(char) it[0];
            break;
        case 'b':
            val = (double)(signed char) it[0];
            break;
        case 'B':
            val = (double)(unsigned char) it[0];
            break;
        case 'h':
            short _h;
            copy(it, &it[s], &_h);
            val = (double)_h;
            break;
        case 'H':
            unsigned short _H;
            copy(it, &it[s], &_H);
            val = (double)_H;
            break;
        case 'i':
            int _i;
            copy(it, &it[s], &_i);
            val = (double)_i;
            break;
        case 'I':
            unsigned int _I;
            copy(it, &it[s], &_I);
            val = (double)_I;
            break;
        case 'l':
            long _l;
            copy(it, &it[s], &_l);
            val = (double)_l;
            break;
        case 'L':
            unsigned long _L;
            copy(it, &it[s], &_L);
            val = (double)_L;
            break;
        case 'q':
            long long _q;
            copy(it, &it[s], &_q);
            val = (double)_q;
            break;
        case 'Q':
            unsigned long long _Q;
            copy(it, &it[s], &_Q);
            val = (double)_h;
            break;
        case 'f':
            float _f;
            copy(it, &it[s], &_f);
            val = (double)_f;
            break;
        }

        index += s;
        res.append(val);
    }
    return res;
}
