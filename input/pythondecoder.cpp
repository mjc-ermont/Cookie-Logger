#include "pythondecoder.h"

pythondecoder::pythondecoder() {}

void pythondecoder::init() {
    Py_Initialize();

    _import_array();
    QString argv_str = QString::fromAscii(*qApp->argv());
    wchar_t * argv = const_cast<wchar_t*>(argv_str.toStdWString().c_str());
    PySys_SetArgv(qApp->argc(), &argv);

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

    if (! PyEval_ThreadsInitialized()) {
        PyEval_InitThreads();
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

    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();


    PyObject* pData = PyByteArray_FromStringAndSize(received.data(),received.length());

    qDebug() << "rec: "<< QString(received) << " | " << received.length();

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
                PyArrayObject* array = (PyArrayObject*)PyArray_FromAny(data, NULL, 0, 0, 0, NULL);
                if(array != NULL) {
                    int s = PyArray_Size((PyObject*)array);
                    for (int c=0; c<s;c++) {
                        PyObject* tuple = PyArray_GETITEM(array,(const char*)PyArray_GETPTR1(array,c));
                        int tsize = PyTuple_Size(tuple);
                        for (int cv=0;cv<tsize;cv++) {
                            PyObject* py_value = PyTuple_GetItem(tuple,cv);
                            double value = PyFloat_AsDouble(py_value);
                            Py_DECREF(py_value);
                            emit newValue(c,cv,value);
                            emit trame_increment(1);
                        }
                        Py_DECREF(tuple);
                    }
                    qDebug() << "chhec";
                } else {
                    qDebug() << "ez";
                }
            }

            if (data != NULL) {
                Py_DECREF(data);
            }
            Py_DECREF(result);
        }
        if(pReturnValue->ob_refcnt > 0 && pReturnValue != 0)
            Py_DECREF(pReturnValue);
            pReturnValue = 0;
    } else {
        emit trame_erreur(1);
        emit message ("Erreur de décodage");
    }

    PyGILState_Release(gstate);

}
