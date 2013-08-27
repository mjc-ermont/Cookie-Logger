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
    ChronoReader/chronoreaderwidget.cpp \
    ChronoReader/TimeCalcs/timecalcs.cpp \
    ChronoReader/eventbox.cpp \
    UI/graphicview.cpp \
    InPut/cookiedecoder.cpp

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
    ChronoReader/chronoreaderwidget.h \
    ChronoReader/TimeCalcs/timecalcs.h \
    ChronoReader/eventbox.h \
    UI/graphicview.h \
    InPut/cookiedecoder.h

FORMS += \
    FenPrincipale.ui \
    dialog.ui \
    InPut/fileimportdialog.ui \
    ChronoReader/chronoreaderwidget.ui \
    ChronoReader/eventbox.ui

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

