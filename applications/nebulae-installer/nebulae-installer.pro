#-------------------------------------------------
#
# Project created by QtCreator 2011-01-20T15:41:35
#
#-------------------------------------------------

QT       += core gui

TARGET = nebulae-installer
TEMPLATE = app

LIBS+= -lparted

SOURCES += main.cpp\
        mainwindow.cpp \
    langpage.cpp \
    partitionpage.cpp \
    infomanager.cpp \
    pictureflow.cpp \
    tzcombobox.cpp \
    nightcharts.cpp \
    partition.cpp

HEADERS  += mainwindow.h \
    langpage.h \
    partitionpage.h \
    infomanager.h \
    pictureflow.h \
    common.h \
    tzcombobox.h \
    nightcharts.h \
    partition.h

OTHER_FILES += \
    countries.conf \
    langages.conf

TRANSLATIONS=nebulae-installer_fr.ts
