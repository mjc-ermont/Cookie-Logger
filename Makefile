#############################################################################
# Makefile for building: Logger21
# Generated by qmake (2.01a) (Qt 4.8.3) on: Sat Apr 6 14:49:41 2013
# Project:  Logger21.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug CONFIG+=declarative_debug -o Makefile Logger21.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQWT_DLL -DQT_WEBKIT_LIB -DQT_SQL_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtSql -I/usr/include/qt4/QtWebKit -I/usr/include/qt4 -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu /usr/lib/libqwt.so -lqjson -lQtWebKit -lQtSql -lQtGui -lQtNetwork -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = FenPrincipale.cpp \
		dialog.cpp \
		tablemgr.cpp \
		main.cpp \
		UI/mapsview.cpp \
		UI/graphicview.cpp \
		UI/boardingtable.cpp \
		InPut/serial.cpp \
		InPut/sensorvalue.cpp \
		InPut/sensormanager.cpp \
		InPut/sensor.cpp \
		InPut/fileimportdialog.cpp \
		../ChronoReaderWidget/chronoreaderwidget.cpp \
		../ChronoReaderWidget/TimeCalcs/timecalcs.cpp \
		../ChronoReaderWidget/eventbox.cpp moc_FenPrincipale.cpp \
		moc_dialog.cpp \
		moc_mapsview.cpp \
		moc_graphicview.cpp \
		moc_serial.cpp \
		moc_fileimportdialog.cpp \
		moc_chronoreaderwidget.cpp \
		moc_eventbox.cpp \
		qrc_ressources.cpp
OBJECTS       = FenPrincipale.o \
		dialog.o \
		tablemgr.o \
		main.o \
		mapsview.o \
		graphicview.o \
		boardingtable.o \
		serial.o \
		sensorvalue.o \
		sensormanager.o \
		sensor.o \
		fileimportdialog.o \
		chronoreaderwidget.o \
		timecalcs.o \
		eventbox.o \
		moc_FenPrincipale.o \
		moc_dialog.o \
		moc_mapsview.o \
		moc_graphicview.o \
		moc_serial.o \
		moc_fileimportdialog.o \
		moc_chronoreaderwidget.o \
		moc_eventbox.o \
		qrc_ressources.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/declarative_debug.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		Logger21.pro
QMAKE_TARGET  = Logger21
DESTDIR       = 
TARGET        = Logger21

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_FenPrincipale.h ui_dialog.h ui_fileimportdialog.h ui_chronoreaderwidget.h ui_eventbox.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	{ test -n "$(DESTDIR)" && DESTDIR="$(DESTDIR)" || DESTDIR=.; } && test $$(gdb --version | sed -e 's,[^0-9]\+\([0-9]\)\.\([0-9]\).*,\1\2,;q') -gt 72 && gdb --nx --batch --quiet -ex 'set confirm off' -ex "save gdb-index $$DESTDIR" -ex quit '$(TARGET)' && test -f $(TARGET).gdb-index && objcopy --add-section '.gdb_index=$(TARGET).gdb-index' --set-section-flags '.gdb_index=readonly' '$(TARGET)' '$(TARGET)' && rm -f $(TARGET).gdb-index || true

Makefile: Logger21.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/declarative_debug.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtWebKit.prl \
		/usr/lib/i386-linux-gnu/libQtSql.prl \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtNetwork.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug CONFIG+=declarative_debug -o Makefile Logger21.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/declarative_debug.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtWebKit.prl:
/usr/lib/i386-linux-gnu/libQtSql.prl:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtNetwork.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug CONFIG+=declarative_debug -o Makefile Logger21.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/Logger211.0.0 || $(MKDIR) .tmp/Logger211.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/Logger211.0.0/ && $(COPY_FILE) --parents tablemgr.h FenPrincipale.h dialog.h defines.h UI/mapsview.h UI/graphicview.h UI/boardingtable.h InPut/serial.h InPut/sensorvalue.h InPut/sensormanager.h InPut/sensor.h InPut/fileimportdialog.h ../ChronoReaderWidget/chronoreaderwidget.h ../ChronoReaderWidget/TimeCalcs/timecalcs.h ../ChronoReaderWidget/eventbox.h .tmp/Logger211.0.0/ && $(COPY_FILE) --parents ressources.qrc .tmp/Logger211.0.0/ && $(COPY_FILE) --parents FenPrincipale.cpp dialog.cpp tablemgr.cpp main.cpp UI/mapsview.cpp UI/graphicview.cpp UI/boardingtable.cpp InPut/serial.cpp InPut/sensorvalue.cpp InPut/sensormanager.cpp InPut/sensor.cpp InPut/fileimportdialog.cpp ../ChronoReaderWidget/chronoreaderwidget.cpp ../ChronoReaderWidget/TimeCalcs/timecalcs.cpp ../ChronoReaderWidget/eventbox.cpp .tmp/Logger211.0.0/ && $(COPY_FILE) --parents FenPrincipale.ui dialog.ui InPut/fileimportdialog.ui ../ChronoReaderWidget/chronoreaderwidget.ui ../ChronoReaderWidget/eventbox.ui .tmp/Logger211.0.0/ && (cd `dirname .tmp/Logger211.0.0` && $(TAR) Logger211.0.0.tar Logger211.0.0 && $(COMPRESS) Logger211.0.0.tar) && $(MOVE) `dirname .tmp/Logger211.0.0`/Logger211.0.0.tar.gz . && $(DEL_FILE) -r .tmp/Logger211.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_FenPrincipale.cpp moc_dialog.cpp moc_mapsview.cpp moc_graphicview.cpp moc_serial.cpp moc_fileimportdialog.cpp moc_chronoreaderwidget.cpp moc_eventbox.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_FenPrincipale.cpp moc_dialog.cpp moc_mapsview.cpp moc_graphicview.cpp moc_serial.cpp moc_fileimportdialog.cpp moc_chronoreaderwidget.cpp moc_eventbox.cpp
moc_FenPrincipale.cpp: ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		FenPrincipale.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		FenPrincipale.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) FenPrincipale.h -o moc_FenPrincipale.cpp

moc_dialog.cpp: ui_dialog.h \
		InPut/serial.h \
		defines.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		dialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) dialog.h -o moc_dialog.cpp

moc_mapsview.cpp: FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		UI/mapsview.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) UI/mapsview.h -o moc_mapsview.cpp

moc_graphicview.cpp: Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		UI/mapsview.h \
		UI/graphicview.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) UI/graphicview.h -o moc_graphicview.cpp

moc_serial.cpp: defines.h \
		InPut/serial.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) InPut/serial.h -o moc_serial.cpp

moc_fileimportdialog.cpp: FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		ui_fileimportdialog.h \
		InPut/fileimportdialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) InPut/fileimportdialog.h -o moc_fileimportdialog.cpp

moc_chronoreaderwidget.cpp: ../ChronoReaderWidget/eventbox.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		../ChronoReaderWidget/chronoreaderwidget.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ../ChronoReaderWidget/chronoreaderwidget.h -o moc_chronoreaderwidget.cpp

moc_eventbox.cpp: ../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		../ChronoReaderWidget/eventbox.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ../ChronoReaderWidget/eventbox.h -o moc_eventbox.cpp

compiler_rcc_make_all: qrc_ressources.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_ressources.cpp
qrc_ressources.cpp: ressources.qrc \
		UI/maps.html
	/usr/bin/rcc -name ressources ressources.qrc -o qrc_ressources.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_FenPrincipale.h ui_dialog.h ui_fileimportdialog.h ui_chronoreaderwidget.h ui_eventbox.h
compiler_uic_clean:
	-$(DEL_FILE) ui_FenPrincipale.h ui_dialog.h ui_fileimportdialog.h ui_chronoreaderwidget.h ui_eventbox.h
ui_FenPrincipale.h: FenPrincipale.ui
	/usr/bin/uic-qt4 FenPrincipale.ui -o ui_FenPrincipale.h

ui_dialog.h: dialog.ui
	/usr/bin/uic-qt4 dialog.ui -o ui_dialog.h

ui_fileimportdialog.h: InPut/fileimportdialog.ui
	/usr/bin/uic-qt4 InPut/fileimportdialog.ui -o ui_fileimportdialog.h

ui_chronoreaderwidget.h: ../ChronoReaderWidget/chronoreaderwidget.ui
	/usr/bin/uic-qt4 ../ChronoReaderWidget/chronoreaderwidget.ui -o ui_chronoreaderwidget.h

ui_eventbox.h: ../ChronoReaderWidget/eventbox.ui
	/usr/bin/uic-qt4 ../ChronoReaderWidget/eventbox.ui -o ui_eventbox.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

FenPrincipale.o: FenPrincipale.cpp FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		InPut/fileimportdialog.h \
		ui_fileimportdialog.h \
		../ChronoReaderWidget/chronoreaderwidget.h \
		../ChronoReaderWidget/eventbox.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FenPrincipale.o FenPrincipale.cpp

dialog.o: dialog.cpp dialog.h \
		ui_dialog.h \
		InPut/serial.h \
		defines.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dialog.o dialog.cpp

tablemgr.o: tablemgr.cpp tablemgr.h \
		InPut/sensorvalue.h \
		defines.h \
		InPut/sensor.h \
		InPut/sensormanager.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tablemgr.o tablemgr.cpp

main.o: main.cpp defines.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		dialog.h \
		ui_dialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mapsview.o: UI/mapsview.cpp UI/mapsview.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mapsview.o UI/mapsview.cpp

graphicview.o: UI/graphicview.cpp UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/mapsview.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o graphicview.o UI/graphicview.cpp

boardingtable.o: UI/boardingtable.cpp UI/boardingtable.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/serial.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o boardingtable.o UI/boardingtable.cpp

serial.o: InPut/serial.cpp InPut/serial.h \
		defines.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o serial.o InPut/serial.cpp

sensorvalue.o: InPut/sensorvalue.cpp InPut/sensorvalue.h \
		defines.h \
		InPut/sensor.h \
		InPut/sensormanager.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sensorvalue.o InPut/sensorvalue.cpp

sensormanager.o: InPut/sensormanager.cpp InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sensormanager.o InPut/sensormanager.cpp

sensor.o: InPut/sensor.cpp InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		InPut/sensormanager.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sensor.o InPut/sensor.cpp

fileimportdialog.o: InPut/fileimportdialog.cpp InPut/fileimportdialog.h \
		FenPrincipale.h \
		ui_FenPrincipale.h \
		InPut/sensormanager.h \
		InPut/sensor.h \
		InPut/sensorvalue.h \
		defines.h \
		tablemgr.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		InPut/serial.h \
		UI/boardingtable.h \
		UI/graphicview.h \
		Qwt/qwt_plot.h \
		Qwt/qwt_global.h \
		Qwt/qwt_text.h \
		Qwt/qwt_plot_dict.h \
		Qwt/qwt_plot_item.h \
		Qwt/qwt_legend_itemmanager.h \
		Qwt/qwt_scale_map.h \
		Qwt/qwt_math.h \
		Qwt/qwt_interval.h \
		Qwt/qwt_plot_curve.h \
		Qwt/qwt_plot_seriesitem.h \
		Qwt/qwt_scale_div.h \
		Qwt/qwt_series_data.h \
		Qwt/qwt_point_3d.h \
		Qwt/qwt_point_polar.h \
		Qwt/qwt_curve_fitter.h \
		UI/mapsview.h \
		ui_fileimportdialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fileimportdialog.o InPut/fileimportdialog.cpp

chronoreaderwidget.o: ../ChronoReaderWidget/chronoreaderwidget.cpp ../ChronoReaderWidget/chronoreaderwidget.h \
		../ChronoReaderWidget/eventbox.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		ui_chronoreaderwidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o chronoreaderwidget.o ../ChronoReaderWidget/chronoreaderwidget.cpp

timecalcs.o: ../ChronoReaderWidget/TimeCalcs/timecalcs.cpp ../ChronoReaderWidget/TimeCalcs/timecalcs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o timecalcs.o ../ChronoReaderWidget/TimeCalcs/timecalcs.cpp

eventbox.o: ../ChronoReaderWidget/eventbox.cpp ../ChronoReaderWidget/eventbox.h \
		../ChronoReaderWidget/TimeCalcs/timecalcs.h \
		ui_eventbox.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o eventbox.o ../ChronoReaderWidget/eventbox.cpp

moc_FenPrincipale.o: moc_FenPrincipale.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_FenPrincipale.o moc_FenPrincipale.cpp

moc_dialog.o: moc_dialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_dialog.o moc_dialog.cpp

moc_mapsview.o: moc_mapsview.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mapsview.o moc_mapsview.cpp

moc_graphicview.o: moc_graphicview.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_graphicview.o moc_graphicview.cpp

moc_serial.o: moc_serial.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_serial.o moc_serial.cpp

moc_fileimportdialog.o: moc_fileimportdialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_fileimportdialog.o moc_fileimportdialog.cpp

moc_chronoreaderwidget.o: moc_chronoreaderwidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_chronoreaderwidget.o moc_chronoreaderwidget.cpp

moc_eventbox.o: moc_eventbox.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_eventbox.o moc_eventbox.cpp

qrc_ressources.o: qrc_ressources.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_ressources.o qrc_ressources.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
