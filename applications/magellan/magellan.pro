#-------------------------------------------------
#
# Project created by QtCreator 2011-04-20T14:43:01
#
#-------------------------------------------------

QT       += core gui

TARGET = magellan
TEMPLATE = app

LIBS += -lkommoditygio

SOURCES += main.cpp \
    magellan-mainwindow.cpp \
    magellan-filebrowser.cpp \
    magellan-fileinfo.cpp \
    magellan-devicemanager.cpp \
    magellan-breadcrumb.cpp \
    magellan-toolbar.cpp

HEADERS  += \
    magellan-mainwindow.h \
    magellan-filebrowser.h \
    magellan-devicemanager.h \
    magellan-breadcrumb.h \
    magellan-toolbar.h
