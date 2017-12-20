#-------------------------------------------------
#
# Project created by QtCreator 2017-12-06T06:37:57
#
#-------------------------------------------------

QT       += core gui
        QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = udp
TEMPLATE = app


SOURCES += main.cpp\
        udp.cpp \
    myudp.cpp

HEADERS  += udp.h \
    myudp.h

FORMS    += udp.ui
