#-------------------------------------------------
#
# Project created by QtCreator 2020-09-21T10:47:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 05_yunwei
TEMPLATE = app

ICON = icon.icns

RC_FILE = scan.rc

SOURCES += main.cpp\
        mywidget.cpp \
    mythread.cpp

HEADERS  += mywidget.h \
    mythread.h

FORMS    += mywidget.ui

RESOURCES += \
    img.qrc
