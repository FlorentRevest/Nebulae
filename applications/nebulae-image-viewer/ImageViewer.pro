#-------------------------------------------------
#
# Project created by QtCreator 2011-01-18T19:36:57
#
#-------------------------------------------------

QT += core gui
TARGET = nebulae-image-viewer 
TEMPLATE = app
SOURCES += main.cpp\
    imageviewer.cpp \
    mainwindow.cpp
HEADERS  += \
    imageviewer.h \
    mainwindow.h
TRANSLATIONS = imageviewer_fr.ts imageviewer_es.ts
target.path = /usr/bin
translates.files = imageviewer_fr.qm
translates.path = /usr/share/nebulae/image-viewer/translations

INSTALLS += target translates
