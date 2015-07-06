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

QMAKE_CXXFLAGS += -std=c++0x -I/usr/include/python3.4/

ICON = icone.ico

PRECOMPILED_HEADER = exprtk.hpp

RESOURCES += res.qrc

CONFIG += serialport

#QMAKE_CXXFLAGS_DEBUG += -O2 #Sinon ça fait l'erreur "too many sections"

LIBS += -lqwt -lqjson -lpython3.4m

TRANSLATIONS = notroll.ts troll.ts

SOURCES += \
    main.cpp \
    FenPrincipale.cpp \
    dialog.cpp \
    tablemgr.cpp \
    webservicesmanager.cpp \
    balaifrequenciel.cpp \
    UI/mapsview.cpp \
    UI/boardingtable.cpp \
    UI/graphicview.cpp \
    UI/histogram.cpp \
    UI/qxt/qxtspanslider.cpp \
    UI/qxt/qxtglobal.cpp \
    UI/timerangeselector.cpp \
    UI/stagesmanager.cpp \
    UI/mygraphicsview.cpp \
    input/serial.cpp \
    input/sensormanager.cpp \
    input/sensor.cpp \
    input/sensorvalue.cpp \
    input/fileimportdialog.cpp \
    input/databasecontroller.cpp \
    input/pythondecoder.cpp


HEADERS  += \
    FenPrincipale.h \
    dialog.h \
    tablemgr.h \
    defines.h \
    balaifrequenciel.h \
    webservicesmanager.h \
    exprtk.hpp \
    UI/mapsview.h \
    UI/boardingtable.h \
    UI/graphicview.h \
    UI/histogram.h \
    UI/qxt/qxtglobal.h \
    UI/qxt/qxtnamespace.h \
    UI/qxt/qxtspanslider.h \
    UI/qxt/qxtspanslider_p.h \
    UI/timerangeselector.h \
    UI/stagesmanager.h \
    UI/mygraphicsview.h \
    input/serial.h \
    input/sensormanager.h \
    input/sensor.h \
    input/sensorvalue.h \
    input/fileimportdialog.h \
    input/databasecontroller.h \
    input/pythondecoder.h

FORMS += \
    FenPrincipale.ui \
    dialog.ui \
    balaifrequenciel.ui \
    input/fileimportdialog.ui


