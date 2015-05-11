#-------------------------------------------------
#
# Project created by QtCreator 2015-02-25T21:20:23
#
#-------------------------------------------------

QT       += core gui sql
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets




TARGET = duombaze
TEMPLATE = app

SUBDIRS += \
    qdbf

SOURCES += main.cpp\
        mainwindow.cpp \
    naujasmodelis.cpp \
    naujasklientas.cpp \
    settings.cpp \
    databasemanager.cpp \
    naujaseka.cpp \
    klientupaieska.cpp \
    ekaediting.cpp \
    myqsqlrelationaltablemodel.cpp \
    importdbf.cpp \
    report.cpp

HEADERS  += mainwindow.h \
    naujasmodelis.h \
    naujasklientas.h \
    settings.h \
    databasemanager.h \
    naujaseka.h \
    klientupaieska.h \
    ekaediting.h \
    myqsqlrelationaltablemodel.h \
    importdbf.h \
    report.h

FORMS    += mainwindow.ui \
    naujasmodelis.ui \
    naujasklientas.ui \
    naujaseka.ui \
    klientupaieska.ui \
    ekaediting.ui \
    importdbf.ui \
    report.ui

include(qdbf/qdbf_static.pri)

CONFIG += c++11

RC_ICONS += program_icon.ico

