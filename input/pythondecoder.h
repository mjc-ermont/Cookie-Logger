#ifndef PYTHONDECODER_H
#define PYTHONDECODER_H

#include "defines.h"

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <QByteArray>
#include <QFile>
#include <QVector>
#include <QDebug>

#include <QApplication>


class pythondecoder : public QObject
{
    Q_OBJECT
public:
    pythondecoder();
    ~pythondecoder();

    void init();

public slots:
    void appendData(QByteArray data);
signals:
    void newFrame(QVector<double>);
    void message(QString);
    void trame_erreur(int);
    void trame_increment(int);
    void trame_corrigee(int);

private:
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;

    bool ok;
};

#endif // PYTHONDECODER_H
