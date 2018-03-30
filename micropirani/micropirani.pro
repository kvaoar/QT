#-------------------------------------------------
#
# Project created by QtCreator 2017-12-18T23:51:15
#
#-------------------------------------------------

QT       += core gui qml quick quickwidgets opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

TARGET = micropirani
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    glgraph.cpp \
    myudp.cpp \
    ftstream.cpp \
    axis.cpp \
    parser.cpp \
    torr.cpp \
    cursorstorage.cpp \
    cursor.cpp

HEADERS += \
        mainwindow.h \
    glgraph.h \
    myudp.h \
    timevalpoint.h \
    ftstream.h \
    axislabel.h \
    axis.h \
    parser.h \
    torr.h \
    cursorstorage.h \
    cursor.h

FORMS += \
        mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    res.qrc

