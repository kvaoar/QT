#-------------------------------------------------
#
# Project created by QtCreator 2017-12-18T23:51:15
#
#-------------------------------------------------

QT       += core gui opengl network svg
CONFIG += console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

TARGET = micropirani
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES -= QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    Axis/xaxis.cpp \
    Axis/xinterval.cpp \
    Axis/xitinterval.cpp \
    Axis/yaxis.cpp \
    Axis/yinterval.cpp \
    Cursors/cursor.cpp \
    Cursors/cursorstorage.cpp \
    Cursors/diff1.cpp \
    Cursors/diff2.cpp \
    Cursors/foundp.cpp \
    Cursors/linexpdecoder.cpp \
    Cursors/metacursor.cpp \
    Cursors/mousecur.cpp \
    Parsers/imr265.cpp \
    Parsers/mks925c.cpp \
    Parsers/sparser.cpp \
    Sensors/myudp.cpp \
    Sensors/sensorstate.cpp \
    Series/scaler.cpp \
    Series/series.cpp \
    Series/seriesstorage.cpp \
    Tables/colorlistmodel.cpp \
    Tables/combocoloritemdelegate.cpp \
    Tables/combovariantitemdelegate.cpp \
    Tables/cursortable.cpp \
    Tables/glstyleeditor.cpp \
    Tables/mmcursor.cpp \
    Tables/nettable.cpp \
    Tables/seriestable.cpp \
    Tables/timeedititemdelegate.cpp \
    Units/second.cpp \
    Units/torr.cpp \
        main.cpp \
        mainwindow.cpp \
    glgraph.cpp \

HEADERS += \
    Axis/axis.h \
    Axis/intervals.h \
    Axis/xaxis.h \
    Axis/xinterval.h \
    Axis/xitinterval.h \
    Axis/yaxis.h \
    Axis/yinterval.h \
    Cursors/cursor.h \
    Cursors/cursorstorage.h \
    Cursors/diff1.h \
    Cursors/diff2.h \
    Cursors/foundp.h \
    Cursors/linexpdecoder.h \
    Cursors/metacursor.h \
    Cursors/mousecur.h \
    Parsers/imr265.h \
    Parsers/mks925c.h \
    Parsers/sparser.h \
    Sensors/myudp.h \
    Sensors/sensorstate.h \
    Series/scaler.h \
    Series/series.h \
    Series/seriesstorage.h \
    Tables/colorlistmodel.h \
    Tables/combocoloritemdelegate.h \
    Tables/combovariantitemdelegate.h \
    Tables/cursortable.h \
    Tables/glstyleeditor.h \
    Tables/metacursortable.h \
    Tables/mmcursor.h \
    Tables/nettable.h \
    Tables/serisetable.h \
    Tables/timeedititemdelegate.h \
    Units/second.h \
    Units/torr.h \
    Units/unit.h \
        mainwindow.h \
    glgraph.h \


FORMS += \
        Tables/glstyleeditor.ui \
        mainwindow.ui

RESOURCES += \
    res.qrc
#QMAKE_POST_LINK =$(STRIP) $(TARGET)

CONFIG += mobility
CONFIG += c++14
MOBILITY = systeminfo

DISTFILES += \
    x_scale.qmodel



