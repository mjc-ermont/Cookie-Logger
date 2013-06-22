#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T14:33:11
#
#-------------------------------------------------
DEFINES += QWT_DLL

QT       += core gui network sql webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Logger21
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x


SOURCES += \
    FenPrincipale.cpp \
    dialog.cpp \
    tablemgr.cpp \
    main.cpp \
    UI/mapsview.cpp \
    UI/boardingtable.cpp \
    InPut/serial.cpp \
    InPut/sensorvalue.cpp \
    InPut/sensormanager.cpp \
    InPut/sensor.cpp \
    InPut/fileimportdialog.cpp \
    ../cookie-ChronoReaderWidget/chronoreaderwidget.cpp \
    ../cookie-ChronoReaderWidget/TimeCalc/timecalcs.cpp \
    ../cookie-ChronoReaderWidget/eventbox.cpp \
    UI/graphicview.cpp

HEADERS  += \
    tablemgr.h \
    FenPrincipale.h \
    dialog.h \
    defines.h \
    UI/mapsview.h \
    UI/boardingtable.h \
    InPut/serial.h \
    InPut/sensorvalue.h \
    InPut/sensormanager.h \
    InPut/sensor.h \
    InPut/fileimportdialog.h \
    ../cookie-ChronoReaderWidget/chronoreaderwidget.h \
    ../cookie-ChronoReaderWidget/TimeCalc/timecalcs.h \
    ../cookie-ChronoReaderWidget/eventbox.h \
    UI/graphicview.h

FORMS += \
    FenPrincipale.ui \
    dialog.ui \
    InPut/fileimportdialog.ui \
    ../cookie-ChronoReaderWidget/chronoreaderwidget.ui \
    ../cookie-ChronoReaderWidget/eventbox.ui

RESOURCES += \
    res.qrc

CONFIG += linusque

lucas {
    LIBS += C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\libqwtd.a
}

robotik {
    LIBS += C:\qwt-6.0.1\lib\libqwtd.a
}

linusque {
    LIBS += /usr/lib/libqwt.so
    LIBS += -lqjson
}

