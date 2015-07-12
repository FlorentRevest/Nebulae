#-------------------------------------------------
#
# Project created by QtCreator 2011-02-01T20:21:10
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = nebulae-autostart
CONFIG   += console
CONFIG   -= app_bundle
LIBS += -lnebulae
INCLUDEPATH += . \
        /usr/include/nebulae

TEMPLATE = app


SOURCES += main.cpp
