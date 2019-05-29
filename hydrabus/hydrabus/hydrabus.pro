#-------------------------------------------------
#
# Project created by QtCreator 2019-05-25T06:15:45
#
#-------------------------------------------------

QT       += core gui qml quick quickwidgets serialport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hydrabus
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

CONFIG += c++17

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    portselect.cpp \
    mcmdline.cpp \
    console.cpp \
    serialservice.cpp \
    pwm.cpp \
    adc.cpp \
    gpio.cpp \
    i2c.cpp \
    uart.cpp \
    jtag.cpp \
    spi.cpp \
    iinterface.cpp \
    dac.cpp

HEADERS += \
        mainwindow.h \
    portselect.h \
    mcmdline.h \
    console.h \
    serialservice.h \
    pwm.h \
    adc.h \
    gpio.h \
    i2c.h \
    uart.h \
    jtag.h \
    spi.h \
    iinterface.h \
    dac.h

FORMS += \
        mainwindow.ui \
    portselect.ui \
    console.ui \
    pwm.ui \
    adc.ui \
    dac.ui \
    gpio.ui \
    i2c.ui \
    uart.ui \
    jtag.ui \
    spi.ui \
    iinterface.ui \
    dac.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
