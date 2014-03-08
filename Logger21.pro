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
    InPut/sensormanager.cpp \
    InPut/fileimportdialog.cpp \
    ChronoReader/chronoreaderwidget.cpp \
    ChronoReader/TimeCalc/timecalcs.cpp \
    ChronoReader/eventbox.cpp \
    UI/graphicview.cpp \
    InPut/cookiedecoder.cpp \
    InPut/yolodecoder.cpp \
    InPut/databasecontroller.cpp \
    InPut/sensor.cpp \
    InPut/sensorvalue.cpp

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
    ChronoReader/TimeCalc/timecalcs.h \
    ChronoReader/eventbox.h \
    UI/graphicview.h \
    InPut/cookiedecoder.h \
    InPut/yolodecoder.h \
    InPut/databasecontroller.h

FORMS += \
    FenPrincipale.ui \
    dialog.ui \
    InPut/fileimportdialog.ui \
    ChronoReader/chronoreaderwidget.ui \
    ChronoReader/eventbox.ui

RESOURCES += \
    res.qrc

CONFIG += linusque serialport

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

