#-------------------------------------------------
#
# Project created by QtCreator 2020-09-24T21:32:05
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 10_testPort
TEMPLATE = app


SOURCES += main.cpp\
        mywidget.cpp \
    mythread.cpp

HEADERS  += mywidget.h \
    mythread.h

FORMS    += mywidget.ui
