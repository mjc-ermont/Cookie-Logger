#include "pythondecoder.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig)
{
    printf("Je code avec mon cul %d\n", sig);
}

pythondecoder::pythondecoder() {}



void pythondecoder::init() {

    ok = false;
    buffer = "";

    Py_Initialize();

    _import_array();
    QString argv_str = QString::fromAscii(*qApp->argv());
    wchar_t * argv = const_cast<wchar_t*>(argv_str.toStdWString().c_str());
    PySys_SetArgv(qApp->argc(), &argv);

    pModule = PyImport_ImportModule("decoder");

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "decode");
        if (pFunc && PyCallable_Check(pFunc)) {
            emit message ("Chargement du decodeur python: OK");
            qDebug() << "Decodeur python ok";
            ok = true;
        } else {
            PyErr_Print();
            emit message ("Chargement du decodeur python: ERREUR (2)");
            qDebug() << "Decodeur python err2";
        }
    } else {
        PyErr_Print();
        emit message ("Chargement du decodeur python: ERREUR (1)");
        qDebug() << "Decodeur python err1";
    }

    if (! PyEval_ThreadsInitialized()) {
        PyEval_InitThreads();
    }


     startByte = 255;
     frame_length = calcframelength();
     qDebug() << "Frame length: " << frame_length;
}

pythondecoder::~pythondecoder() {
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();
}

bool pythondecoder::decode(QByteArray frame) {
    qDebug() << QString(frame.toHex());
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();


    PyObject* pData = PyByteArray_FromStringAndSize(frame.data(),frame.length());

    pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, pData);


    PyObject* pReturnValue =  PyObject_CallObject(pFunc, pArgs);

    Py_DECREF(pArgs);

    bool success = false;

    if (pReturnValue != NULL) {
        PyObject* result = PyTuple_GetItem(pReturnValue,0);
        PyObject* data = PyTuple_GetItem(pReturnValue,1);

        qDebug() << PyTuple_Size(pReturnValue);

        if(result != NULL) {
            bool trame_decodee = PyObject_IsTrue(result);
            if(trame_decodee && data != NULL) {
                PyArrayObject* array = (PyArrayObject*)PyArray_FromAny(data, NULL, 0, 0, 0, NULL);
                if(array != NULL) {
                    int s = PyArray_Size((PyObject*)array);
                    QVector<double> frame;
                    for (int c=0; c<s;c++) {
                        PyObject* tuple = PyArray_GETITEM(array,(const char*)PyArray_GETPTR1(array,c));
                        int tsize = PyTuple_Size(tuple);
                        for (int cv=0;cv<tsize;cv++) {
                            PyObject* py_value = PyTuple_GetItem(tuple,cv);
                            double value = PyFloat_AsDouble(py_value);
                            frame.append(value);
                        }
                        Py_DECREF(tuple);
                    }
                    emit newFrame(frame);
                    success = true;
                    emit trame_increment(1);
                }
            }

            if (data != NULL) {
                Py_DECREF(data);
            }
            Py_DECREF(result);
        }
    } else {
        emit trame_erreur(1);
        emit message ("Erreur de décodage");
        PyErr_Print();
    }

    PyGILState_Release(gstate);
    return success;
}

int pythondecoder::calcframelength() {
    PyObject* pFuncCalc = PyObject_GetAttrString(pModule, "calcframelength");
    if (pFuncCalc && PyCallable_Check(pFuncCalc)) {
        emit message ("Chargement fonction calcul: OK");

        PyObject* pReturnValue =  PyObject_CallObject(pFuncCalc, NULL);
        Py_XDECREF(pFuncCalc);
        if(pReturnValue != NULL)
            return PyLong_AsLong(pReturnValue);
        else
            return 42;
    } else {
        PyErr_Print();
        emit message ("Chargement fonction calcul: ERREUR");
        qDebug() << "Decodeur python err";
        Py_XDECREF(pFuncCalc);
        return 42;
    }
}

void pythondecoder::appendData(QByteArray received) {
    if(!ok)
        return;

    buffer.append(received);
    qDebug() << "Received:" << QString(received.toHex())<< " || buffer:"<<QString(buffer.toHex());


    while(buffer.indexOf(startByte) != -1 && (buffer.length() - buffer.indexOf(startByte)) >= frame_length) {
        int firstStart = buffer.indexOf(startByte);
        QByteArray frame_to_decode = buffer.mid(firstStart, frame_length);
        bool result = decode(frame_to_decode);
        qDebug() << "result: "<< (result? " true":"false");
        if(result)
            buffer = buffer.mid(firstStart + frame_length);
        else if (buffer.indexOf(startByte,firstStart+1) != -1)
            buffer = buffer.mid(buffer.indexOf(startByte,firstStart+1));
        else
            buffer = "";

        qDebug() << "New buffer: " << QString(buffer.toHex());
    }

}
