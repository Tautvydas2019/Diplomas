#-------------------------------------------------
#
# Project created by QtCreator 2015-02-25T21:20:23
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = duombaze
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    naujasmodelis.cpp \
    naujasklientas.cpp \
    settings.cpp \
    databasemanager.cpp \
    naujaseka.cpp

HEADERS  += mainwindow.h \
    naujasmodelis.h \
    naujasklientas.h \
    settings.h \
    databasemanager.h \
    naujaseka.h

FORMS    += mainwindow.ui \
    naujasmodelis.ui \
    naujasklientas.ui \
    naujaseka.ui

CONFIG += c++11
