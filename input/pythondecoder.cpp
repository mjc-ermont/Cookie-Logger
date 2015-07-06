#include "pythondecoder.h"

pythondecoder::pythondecoder() {}

void pythondecoder::init() {
    Py_Initialize();

    _import_array();
    QString argv_str = QString::fromAscii(*qApp->argv());
    wchar_t * argv = const_cast<wchar_t*>(argv_str.toStdWString().c_str());
    PySys_SetArgv(qApp->argc(), &argv);

    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");


    PyList_Append(path, PyByteArray_FromStringAndSize(".",1));

    qDebug() << "path:" << PyByteArray_AsString(path);

    pModule = PyImport_ImportModule("decoder");

    ok = false;
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
}

pythondecoder::~pythondecoder() {
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();
}

void pythondecoder::appendData(QByteArray received) {
    if(!ok)
        return;

    PyObject* pData = PyByteArray_FromStringAndSize(received.data(),received.length());

    received = "";
    pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, pData);

    Py_INCREF(pArgs);

    PyObject* pReturnValue =  PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pData);
    Py_DECREF(pArgs);

    if (pReturnValue != NULL) {
        PyObject* result = PyTuple_GetItem(pReturnValue,0);
        PyObject* data = PyTuple_GetItem(pReturnValue,1);

        qDebug() << PyTuple_Size(pReturnValue);

        if(result != NULL) {
            bool trame_decodee = PyObject_IsTrue(result);
            if(trame_decodee && data != NULL) {
                PyArrayObject* array = (PyArrayObject*)PyArray_FromAny(data,PyArray_DescrFromType(NPY_FLOAT64), 0,0, 0,NULL);
                if(array != NULL) {
                    QVector<double> values;
                    int s = PyArray_Size((PyObject*)array);
                    for (int i=0; i<s;i++) {
                        values.append(PyFloat_AsDouble(PyArray_GETITEM(array,(const char*)PyArray_GETPTR1(array,i))));

                    }

                    newFrame(values);
                }

                Py_DECREF(data);
            }
            Py_DECREF(result);
        }
        Py_DECREF(pReturnValue);
    } else {
        PyErr_Print();
        emit message ("Erreur de décodage");
    }

}
